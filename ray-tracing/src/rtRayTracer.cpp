#include "rtRayTracer.h"

RayTracer::RayTracer(int w, int h, Camera& c, const Viewport& v, 
    const AmbientLight& ambient):
    width(w), height(h), camera(c),viewport(v), ambient_light(ambient) {

}

float RayTracer::render(ofPixels& pixels,bool parallel)
{
    auto t_start = Clock::now();
    if (parallel){
        tbb::parallel_for(0, width, [&] (int i){
            for(int j = 0; j < height; j++){
                auto color = getPixelColor(i,j);
                pixels.setColor(i, height-j-1, color);
            }
        });
    }
    else{
        for(int i = 0; i < width; i++){
            for(int j = 0; j < height; j++){
                auto color = getPixelColor(i,j);
                pixels.setColor(i, height-j-1, color);
            }
        }
    }
    
    auto elapsed_time = Clock::now() - t_start;
    auto time_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count();

    return time_in_ms / 1e+3;
}

void RayTracer::addObject(std::shared_ptr<Primitive> obj)
{
    objects.push_back(obj);
}

void RayTracer::addLight(std::shared_ptr<Light> light)
{
    lights.push_back(light);
}

ofColor RayTracer::getPixelColor(int i, int j) const
{
    float new_i = static_cast<float>(i);
    float new_j = static_cast<float>(j);

    glm::vec4 color(0,0,0,0);
    std::vector<HitRecord> hit_records;

    //performAntialiasing(new_i, new_j, hit_records);
    hit_records.push_back(traceRay(new_i, new_j));
    //HitRecord record = traceRay(new_i, new_j);
    auto n_records = hit_records.size();
    for(const auto& record: hit_records){
        if (record.hit){
            // perform shading 
            performShading(record, color);
        }
        else color[3] += 1.f;
    }
    
    color = scale_vec(255.0 / n_records , color);

    return ofColor(color[0], color[1], color[2], color[3]); 

}

HitRecord  RayTracer::traceRay(float x, float y) const
{
    auto u = viewport.toU(x, width);
    auto v = viewport.toV(y, height);
    Ray ray = camera.rayForPixel(u,v);
    HitRecord record = testHit(ray);

    return record;
}

HitRecord RayTracer::testHit(const Ray& ray) const
{
    HitRecord closest_so_far;

    for(const auto& object: objects){
        HitRecord temp = object->hit(ray);
        if(temp.t >= EPS){
            if(temp.t < closest_so_far.t || closest_so_far.t < 0.0){
                closest_so_far = temp;
            }
        }
    }

    return closest_so_far;
}

std::shared_ptr<Primitive> RayTracer::selectObject(int i, int j) const
{
    float x = static_cast<float>(i);
    float y = static_cast<float>(j);

    auto u = viewport.toU(x, width);
    auto v = viewport.toV(y, height);
    Ray ray = camera.rayForPixel(u,v);
    HitRecord closest_so_far;

    std::shared_ptr<Primitive> closest_obj = nullptr;
    for(const auto& object: objects){
        HitRecord temp = object->hit(ray);
        if(temp.hit && temp.t > EPS){
            if(closest_so_far.t < 0.0 || temp.t < closest_so_far.t){
                closest_so_far = temp;
                closest_obj = object;
            }
        }
    }

    return closest_obj;
}

void RayTracer::performAntialiasing(float x, float y, std::vector<HitRecord>& arr) const
{
    int n = 4;
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(0, 1.0);

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            auto sign = (i % 2) ? 1.f : -1.f;
            float rand_i = static_cast<float>(dis(gen));
            float rand_j = static_cast<float>(dis(gen));
            x += sign * (i + rand_i) / n;
            y += sign * (j + rand_j) / n;

            auto hit_record = traceRay(x, y);
            arr.push_back(hit_record);
        }
    }
}

glm::vec4 RayTracer::getIntensity(const HitRecord& record, const std::shared_ptr<Light>& light) const
{
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(0, 0.1);

    auto jittered_pos = add_vecs(light->position, glm::vec3(dis(gen),dis(gen),dis(gen)));

    auto l = subtract_vecs(light->position, record.p);
    float l_dist = vec_length(l);
    l = normalize(l);

    Ray ray(record.p,l);
    auto shadow_record = testHit(ray);

    if(!shadow_record.hit || l_dist < shadow_record.t){
        float angle = dot_product(record.n, l);
        return light->getIllumination(angle);
    }

    return glm::vec4(0,0,0,1.0);
}

glm::vec4 RayTracer::getReflectionColor(const HitRecord& record, const std::shared_ptr<Light>& light) const
{
    int n_reflect = 64;
    glm::vec4 color(0.0, 0.0, 0.0, 0.0);
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(0, 1.0);
    
    for (int i = 0; i < n_reflect; i++){
        auto scale = 2.f * dot_product(record.ray.d, record.n);
        auto norm_scaled = scale_vec(scale, record.n);
        auto diff = subtract_vecs(record.ray.d, norm_scaled);
        auto reflect_vec = hadamard_product(diff,glm::vec3(1/3.0, 1.f, 1/2.0));
        auto random_vec = glm::vec3(0.25 * dis(gen), 0.125 * dis(gen), 0.25 * dis(gen));

        auto reflect_dir = normalize(add_vecs(reflect_vec, random_vec));
        auto reflect_org = add_vecs(record.p, record.n);
        Ray ray(reflect_org, reflect_dir);
        auto reflect_record = testHit(ray);
        if (reflect_record.hit){
            reflect_record.n *= -1.f;
            reflect_record.p *= -1.f;

            auto intensity = getIntensity(reflect_record, light);
            intensity = scale_vec(0.8,intensity);
            auto object_color = reflect_record.obj->getColor();
            auto obj_vcolor = glm::vec4(object_color.r,object_color.g,object_color.b, object_color.a);
            obj_vcolor = hadamard_product(obj_vcolor,intensity);
            obj_vcolor = clamp(obj_vcolor,0.0, 1.0);
            color = add_vecs(color,obj_vcolor);
        }
    }

    color = scale_vec(1.0/n_reflect, color);
    color[3] = 1.f;
    return clamp(color,0.0,1.0);
}

void RayTracer::performShading(const HitRecord& record, glm::vec4& color) const
{
    int num_shadow_rays = 1;
    auto illumination = ambient_light.light;

    for(const auto& light: lights){
        auto intensity = glm::vec4(0,0,0,0);
        for (int j=0; j < num_shadow_rays; j++){
             intensity = add_vecs(intensity,getIntensity(record, light));
        }
        intensity = scale_vec(1.f/num_shadow_rays,intensity);
        intensity[3] = 1;
        illumination = add_vecs(illumination, intensity);
        //reflection
        /*
        if (record.obj->isReflectEnabled()){
            auto reflect_color = getReflectionColor(record, light);
            color = add_vecs(color, reflect_color);
        }*/
    }
    auto object_color = record.obj->getColor();
    auto obj_vcolor = glm::vec4(object_color.r,object_color.g,object_color.b, object_color.a);
    obj_vcolor = hadamard_product(obj_vcolor,illumination);
    obj_vcolor = clamp(obj_vcolor,0.0, 1.0);

    color = add_vecs(color, obj_vcolor);
    //reflection
    color = clamp(color, 0.0, 1.0);
}
#include "rtRayTracer.h"

RayTracer::RayTracer(int w, int h, const Viewport& v, 
    const AmbientLight& ambient, const ProjectMode& m):
    width(w), height(h), viewport(v), ambient_light(ambient),mode(m) {

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

    glm::vec3 color(0,0,0);
    HitRecord record = traceRay(new_i, new_j);
    if (record.hit){
        glm::vec3 intensity = ambient_light.light;
        for(const auto& light: lights){
            auto l = subtract_vecs(light->position,record.p);
            auto l_dist = vec_length(l);
            l = normalize(l);
            Ray ray(record.p,l);
            auto shadow_record = testHit(ray);
            if(!shadow_record.hit || l_dist < shadow_record.t){
                auto ks = record.obj->getSpecularCoeff();
                auto diffuse_color = light->getDiffuseColor(l,record.n,record.ray.d,ks);
                intensity = add_vecs(intensity,diffuse_color);
            }
        }
        auto object_color = record.obj->getColor();
        color = glm::vec3(object_color.r,object_color.g,object_color.b);
        color = matmul_elementwise(color,intensity);
        color = clamp(color,0.0,1.0);
    }
    color = scale_vec(255.0, color);

    return ofColor(color.r,color.g,color.b);
}

HitRecord  RayTracer::traceRay(float x, float y) const
{
    auto u = viewport.l + ((viewport.r - viewport.l) * (x + 0.5)) / static_cast<float>(width);
    auto v = viewport.b + ((viewport.t - viewport.b) * (y + 0.5)) / static_cast<float>(height);

    Ray ray;
    switch(mode){
        case PERSPECTIVE:
            ray.d = normalize(glm::vec3(u, v, 1.0));
            ray.o = glm::vec3(0.0, 0.0, -1);
            break;
        case PARALLEL:
            ray.d = glm::vec3(0.0, 0.0, 1.0);
            ray.o = glm::vec3(u, v, -1.0);
            break;
        default:
            break;
    }

    HitRecord record = testHit(ray);

    return record;
}

HitRecord RayTracer::testHit(const Ray& ray) const
{
    HitRecord closest_so_far;

    for(const auto& object: objects){
        HitRecord temp = object->hit(ray);
        if(temp.hit && temp.t > EPS){
            if(closest_so_far.t < 0.0 || temp.t < closest_so_far.t){
                closest_so_far = temp;
            }
        }
    }

    return closest_so_far;
}
#include "rtPrimitives.h"

Plane::Plane(const glm::vec3& p, const glm::vec3& n, float spec_coef,const ofFloatColor& color):
    point(p), normal(n){
        this->color = color;
        this->specular_coeff = spec_coef;
    }

HitRecord Plane::hit(const Ray& ray) const
{
    auto denom = dot_product(ray.d, normal); 
    if (abs(denom) > EPS){
        auto po = subtract_vecs(point,ray.o);
        auto t = dot_product(po,normal) / denom;
        if (t >= EPS){
            auto p = ray.at(t);
            auto alpha = denom > 0 ? -1:1;
            auto norm = scale_vec(alpha, normal);

            return HitRecord(true, t, p, norm, *this, ray);
        }
    }
    return HitRecord();
}    




// ---------------------------------------------------------------------------------------------------

Sphere::Sphere(float r, const glm::vec3& c, float spec_coef,const ofFloatColor& color):
    radius(r), center(c){
        this->color = color;
        this->specular_coeff = spec_coef;

        center_default = center;
        radius_default = radius;
    }

HitRecord Sphere::hit(const Ray& ray) const
{
    auto oc = subtract_vecs(ray.o, center);
    auto a = dot_product(ray.d, ray.d);
    auto b = 2.0 * dot_product(oc, ray.d);
    auto c = dot_product(oc, oc) - radius * radius;

    float t1, t2;
    if(solve_quadratic(a,b,c,t1,t2)){
        auto t = min_distance(t1, t2);
        auto p = ray.at(t);
        auto ip_center = subtract_vecs(p,center);
        auto norm = scale_vec(1.0/radius, ip_center);

        return HitRecord(true, t, p, norm, *this, ray);
    }

    return HitRecord();
}

void Sphere::translate(const glm::mat4& m)
{
    auto center_v4 = glm::vec4(center[0],center[1],center[2], 1);
    center_v4 = matmul(m,center_v4);

    center = glm::vec3(center_v4[0],center_v4[1],center_v4[2]);
}

void Sphere::scale(const glm::mat4& m)
{
    radius *= m[0][0];
}

void Sphere::reset()
{
    center = center_default;
    radius = radius_default;

}

// ---------------------------------------------------------------------------------------------------

Cone::Cone(const glm::vec3& c, const glm::vec3& o, float a, float spec_coef, const ofFloatColor& color):
    apex(c), center(o), angle(a){
        this->color = color;
        axis = subtract_vecs(center,apex);
        height = vec_length(axis);
        axis = normalize(axis);
        cosa = cos(angle);

        center_default = center;
        apex_default = apex;
    }

HitRecord Cone::hit(const Ray& ray) const
{
    glm::vec3 co = subtract_vecs(ray.o, apex);
    auto dv = dot_product(ray.d, axis);
    auto cv = dot_product(co,axis);
    auto a = dv * dv - cosa * cosa;
    auto b = 2.0 * (dv * cv - dot_product(ray.d, co) * cosa * cosa);
    auto c = cv * cv - dot_product(co,co)*cosa*cosa;

    float t1, t2;
    if(solve_quadratic(a,b,c,t1,t2)){
        auto t = min_distance(t1, t2);
        if(t < EPS) return HitRecord();

        auto p = ray.at(t);
        auto cp = subtract_vecs(p,apex);
        auto h = dot_product(cp,axis);

        if (h < EPS || h > height) return HitRecord();

        auto scale_factor = dot_product(axis,cp)/dot_product(cp,cp);
        cp = scale_vec(scale_factor,cp);
        auto norm = subtract_vecs(cp,axis);
        norm = normalize(norm);

        return HitRecord(true, t, p, norm, *this, ray);
    }

    return HitRecord();
}

void Cone::rotate(const glm::mat4& m)
{
    auto tmp = scale_vec(height/2.0, axis);
    auto center_p = subtract_vecs(center,tmp);
    auto shifted_a = subtract_vecs(apex,center_p);
    auto shifted_c = subtract_vecs(center,center_p);

    auto center_v4 = glm::vec4(shifted_c[0],shifted_c[1],shifted_c[2], 1);
    auto apex_v4 = glm::vec4(shifted_a[0],shifted_a[1],shifted_a[2], 1);
    center_v4 = matmul(m,center_v4);
    apex_v4 = matmul(m,apex_v4);

    center = add_vecs(center_p,glm::vec3(center_v4[0],center_v4[1],center_v4[2]));
    apex = add_vecs(center_p,glm::vec3(apex_v4[0],apex_v4[1],apex_v4[2]));

    axis = subtract_vecs(center,apex);
    height = vec_length(axis);
    axis = normalize(axis);
}

void Cone::translate(const glm::mat4& m)
{
   
    auto center_v4 = glm::vec4(center[0],center[1],center[2], 1);
    auto apex_v4 = glm::vec4(apex[0],apex[1],apex[2], 1);
    center_v4 = matmul(m,center_v4);
    apex_v4 = matmul(m,apex_v4);

    center = glm::vec3(center_v4[0],center_v4[1],center_v4[2]);
    apex = glm::vec3(apex_v4[0],apex_v4[1],apex_v4[2]);

    axis = subtract_vecs(center,apex);
    height = vec_length(axis);
    axis = normalize(axis);
}

void Cone::scale(const glm::mat4& m)
{
    auto tmp = scale_vec(height/2.0, axis);
    auto center_p = subtract_vecs(center,tmp);
    auto shifted_a = subtract_vecs(apex,center_p);
    auto shifted_c = subtract_vecs(center,center_p);

    auto center_v4 = glm::vec4(shifted_c[0],shifted_c[1],shifted_c[2], 1);
    auto apex_v4 = glm::vec4(shifted_a[0],shifted_a[1],shifted_a[2], 1);
    center_v4 = matmul(m,center_v4);
    apex_v4 = matmul(m,apex_v4);

    center = add_vecs(center_p,glm::vec3(center_v4[0],center_v4[1],center_v4[2]));
    apex = add_vecs(center_p,glm::vec3(apex_v4[0],apex_v4[1],apex_v4[2]));

    axis = subtract_vecs(center,apex);
    height = vec_length(axis);
    axis = normalize(axis);
}
void Cone::reset()
{
    center = center_default;
    apex = apex_default;
    axis = subtract_vecs(center,apex);
    height = vec_length(axis);
    axis = normalize(axis);
}


// ---------------------------------------------------------------------------------------------------
Cylinder::Cylinder(const glm::vec3& c1, const glm::vec3& c2, float r, float spec_coef, const ofFloatColor& color):
    center_top(c1), center_bottom(c2), radius(r){
        this->color = color;
        this->specular_coeff = spec_coef;
        axis = subtract_vecs(c1,c2);
        height = vec_length(axis);
        axis = normalize(axis);

        radius_default = radius;
        ct_default = center_top;
        cb_default = center_bottom;
    }

HitRecord Cylinder::hit(const Ray& ray) const
{
    auto co = subtract_vecs(ray.o,center_bottom);
    auto da = dot_product(ray.d,axis);
    auto ba = dot_product(co,axis);

    auto a = dot_product(ray.d,ray.d) - da*da;
    auto b = 2.0 * (dot_product(ray.d,co) - (da*ba));
    auto c = dot_product(co,co) - (ba*ba) - radius * radius;

    float t1, t2;
    if(solve_quadratic(a,b,c,t1,t2)){
        auto t = min_distance(t1,t2);
        if (t < EPS) return HitRecord();
        auto p = ray.at(t);
        auto cp = subtract_vecs(p,center_bottom);
        auto h = dot_product(cp,axis);
        if (h < EPS || h > height) return HitRecord();

        auto temp = scale_vec(dot_product(cp,axis),axis);
        auto norm = subtract_vecs(cp,temp);
        norm = normalize(norm);

        return HitRecord(true,t,p,norm,*this,ray);
    }
    
    return HitRecord();
}

void Cylinder::rotate(const glm::mat4& m)
{
    auto tmp = scale_vec(height/2.0, axis);
    auto center_p = add_vecs(center_bottom,tmp);
    auto shifted_ct = subtract_vecs(center_top,center_p);
    auto shifted_cb = subtract_vecs(center_bottom,center_p);

    auto cb_v4 = glm::vec4(shifted_cb[0],shifted_cb[1],shifted_cb[2], 1);
    auto ct_v4 = glm::vec4(shifted_ct[0],shifted_ct[1],shifted_ct[2], 1);
    cb_v4 = matmul(m,cb_v4);
    ct_v4 = matmul(m,ct_v4);

    center_bottom = add_vecs(center_p,glm::vec3(cb_v4[0],cb_v4[1],cb_v4[2]));
    center_top = add_vecs(center_p,glm::vec3(ct_v4[0],ct_v4[1],ct_v4[2]));

    axis = subtract_vecs(center_top,center_bottom);
    height = vec_length(axis);
    axis = normalize(axis);
}

void Cylinder::translate(const glm::mat4& m)
{
    auto cb_v4 = glm::vec4(center_bottom[0],center_bottom[1],center_bottom[2], 1);
    auto ct_v4 = glm::vec4(center_top[0],center_top[1],center_top[2], 1);
    cb_v4 = matmul(m,cb_v4);
    ct_v4 = matmul(m,ct_v4);

    center_bottom = glm::vec3(cb_v4[0],cb_v4[1],cb_v4[2]);
    center_top = glm::vec3(ct_v4[0],ct_v4[1],ct_v4[2]);

    axis = subtract_vecs(center_top,center_bottom);
    height = vec_length(axis);
    axis = normalize(axis);

}

void Cylinder::scale(const glm::mat4& m)
{
    auto tmp = scale_vec(height/2.0, axis);
    auto center_p = add_vecs(center_bottom,tmp);
    auto shifted_ct = subtract_vecs(center_top,center_p);
    auto shifted_cb = subtract_vecs(center_bottom,center_p);

    auto cb_v4 = glm::vec4(shifted_cb[0],shifted_cb[1],shifted_cb[2], 1);
    auto ct_v4 = glm::vec4(shifted_ct[0],shifted_ct[1],shifted_ct[2], 1);
    cb_v4 = matmul(m,cb_v4);
    ct_v4 = matmul(m,ct_v4);

    center_bottom = add_vecs(center_p,glm::vec3(cb_v4[0],cb_v4[1],cb_v4[2]));
    center_top = add_vecs(center_p,glm::vec3(ct_v4[0],ct_v4[1],ct_v4[2]));

    axis = subtract_vecs(center_top,center_bottom);
    height = vec_length(axis);
    radius *= m[0][0];
    axis = normalize(axis);
}

void Cylinder::reset()
{
    center_top = ct_default;
    center_bottom = cb_default;
    radius = radius_default;

    axis = subtract_vecs(center_top,center_bottom);
    height = vec_length(axis);
    axis = normalize(axis);
}
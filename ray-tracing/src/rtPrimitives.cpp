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

Sphere::Sphere(float r, const glm::vec3& c, float spec_coef,const ofFloatColor& color):
    radius(r), center(c){
        this->color = color;
        this->specular_coeff = spec_coef;

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

Cone::Cone(const glm::vec3& c, const glm::vec3& o, float a, float spec_coef, const ofFloatColor& color):
    apex(c), center(o), angle(a){
        this->color = color;
        axis = subtract_vecs(center,apex);
        height = vec_length(axis);
        axis = normalize(axis);
        cosa = cos(angle);
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

Cylinder::Cylinder(const glm::vec3& c1, const glm::vec3& c2, float r, float spec_coef, const ofFloatColor& color):
    center_top(c1), center_bottom(c2), radius(r){
        this->color = color;
        this->specular_coeff = spec_coef;
        axis = subtract_vecs(c1,c2);
        height = vec_length(axis);
        axis = normalize(axis);
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

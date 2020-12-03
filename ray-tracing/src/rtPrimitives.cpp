#include "rtPrimitives.h"

Plane::Plane(const glm::vec3& p, const glm::vec3& n, float spec_coef,const ofFloatColor& color):
    point(p), normal(n){
        this->color = color;
        this->specular_coeff = spec_coef;
        reflect = true;
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
        reflect = true;
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
        reflect = false;
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
        reflect = false;
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


// ---------------------------------------------------------------------------------------------------
Model::Model(const glm::vec3& p, const ofFloatColor& color): 
    position(p){
        this->color = color;
        reflect = false;
        use_precomputed = true;
    }
    
HitRecord Model::hit(const Ray& ray) const
{
    //auto bound_record = bounding_sphere.hit(ray);
    //if (!bound_record.hit) return HitRecord();

    auto bound_record = bbox.hit(ray);
    if (!bound_record.hit) return HitRecord();

    float min_t = INFINITY;
    bool hit = false;
    glm::vec3 n(0,0,0), p(0,0,0);

    // ref: https://github.com/0ctobyte/raytracer/blob/master/src/mesh.cpp
    for(auto& triangle: triangles){

        // Compute the intersection using Moller & Trumbore's algorithm and Cramer's rule
        // The following variables are the expanded terms from the matrix form of the system
        auto A = vertices[triangle.vertices[0]];
        auto B = vertices[triangle.vertices[1]];
        auto C = vertices[triangle.vertices[2]];

        auto e1 = subtract_vecs(B,A);
        auto e2 = subtract_vecs(C,A);
        auto h = cross_product(ray.d,e2);

        // If determinant is zero then the ray is parallel to the triangle
        auto det = dot_product(h,e1);
        if (det < EPS) continue;

        // Calculate alpha, barycentric coordinate, and make sure it is within range of [0, 1]
        auto s = subtract_vecs(ray.o, A);
        float alpha = dot_product(s,h) / det;
        if (alpha < 0.0 || alpha > 1.0) continue;

        // Calculate v, barycentric coordinate, and make sure it is within range. 
        // alpha + beta must be less than 1!
        auto q = cross_product(s,e1);
        auto beta = dot_product(ray.d,q) / det;
        if (beta < 0.0 || (alpha + beta) > 1.0) continue;

        auto t = dot_product(e2, q) / det;
        if(t > EPS && t < min_t){  // The ray intersects this triangle
            hit = true;
            min_t = t;
            p = ray.at(min_t);
            auto gamma = 1.0 - (alpha + beta);
            if (use_precomputed){
                auto v1 = scale_vec(gamma, pnormals[triangle.vertices[0]]);
                auto v2 = scale_vec(alpha, pnormals[triangle.vertices[1]]);
                auto v3 = scale_vec(beta, pnormals[triangle.vertices[2]]);
                auto tmp = add_vecs(v1,v2);
                n = normalize(add_vecs(tmp,v3));
            }
            else{
                auto v1 = scale_vec(gamma, normals[triangle.vertices[0]]);
                auto v2 = scale_vec(alpha, normals[triangle.vertices[1]]);
                auto v3 = scale_vec(beta, normals[triangle.vertices[2]]);
                auto tmp = add_vecs(v1,v2);
                n = normalize(add_vecs(tmp,v3));
            }
        }
    }

    return HitRecord(hit, min_t, p, n, *this, ray);
}

// ---------------------------------------------------------------------------------------------------

BBox::BBox(const glm::vec3& v1, const glm::vec3& v2):
    min(v1), max(v2)
{
    this->color = ofFloatColor(1,1,1,1);
}

HitRecord BBox::hit(const Ray& ray) const
{
    // ref: http://web.cse.ohio-state.edu/~parent.1/classes/782/Lectures/02_Intersections.pdf
    glm::vec3 n,p;
    auto inv_dir = glm::vec3(1.f/ray.d[0], 1.f/ray.d[1], 1.f/ray.d[2]);
    auto delta_min = hadamard_product(inv_dir, subtract_vecs(min, ray.o));
    auto delta_max = hadamard_product(inv_dir, subtract_vecs(max, ray.o));

    float tmin = delta_min.x;
    float tmax = delta_max.x;
    if(tmin > tmax) std::swap(tmin, tmax);
    
    float tmin_y = delta_min.y;
    float tmax_y = delta_max.y;
    if(tmin_y > tmax_y) std::swap(tmin_y, tmax_y);

    tmin = std::min(tmin, tmin_y);
    tmax = std::max(tmax, tmax_y);
    
    float tmin_z = delta_min.z;
    float tmax_z = delta_max.z;
    if(tmin_z > tmax_z) std::swap(tmin_z, tmax_z);

    if(tmin > tmax_z || tmin_z > tmax) return HitRecord();

    tmin = std::min(tmin, tmin_z);
    tmax = std::max(tmax, tmax_z);

    return HitRecord(true, tmin, p, n, *this, ray);
}
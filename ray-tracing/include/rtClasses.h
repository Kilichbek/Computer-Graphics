#pragma once 

#include "ofMain.h"
#include "rtPrimitives.h"
#include "rtLinearAlgebra.h"

class Primitive;
struct Triangle { unsigned vertices[3] = {0, 0, 0}; };

// Viewport class
class Viewport{
  public:
    float l,r,b,t;
    Viewport();
    Viewport(float w_extent, float h_extent):
        l(-w_extent), r(w_extent), b(-h_extent), t(h_extent)
        {}
    float toU(float x, int width) const { return l + ((r - l) * (x + 0.5)) / static_cast<float>(width); }
    float toV(float y, int height) const { return b + ((t - b) * (y + 0.5)) / static_cast<float>(height); }
  };


// Ray Class
class Ray {
 public:
    Ray(): o(0,0,0), d(0,0,0) {};
    Ray(const glm::vec3& origin, const glm::vec3& direction):
        o(origin), d(direction) {};

    // calculates the intersection point of a ray with
    // a point defined with its distance from origin
    glm::vec3 at(float t) const {
        glm::vec3 p = scale_vec(t, d);
        return add_vecs(o, p);
    }

    glm::vec3 o;
    glm::vec3 d;
};

// HitRecord Class
class HitRecord {
  public:
    bool hit;
    float t;
    glm::vec3 p, n;
    const Primitive* obj;
    Ray ray;

    HitRecord(): hit(false), t(-1.0), p(0,0,0), n(0,0,0), ray(p,n) {};
    HitRecord(bool hit, float d, const glm::vec3& hit_point,
        const glm::vec3& norm, const Primitive& object, const Ray& ray):
        t(d), p(hit_point), n(norm), obj(&object){
            this->hit = hit;
            this->ray = ray;
        }
};


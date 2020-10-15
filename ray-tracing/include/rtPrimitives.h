#pragma once

#include "rtClasses.h"
#include "ofMain.h"
//#include <cmath>

class Ray; 
class HitRecord;

// Abstract Class for any simple geometric objects

class Primitive {
 public:
    virtual HitRecord hit(const Ray& ray) const = 0;
    virtual ofFloatColor getColor() const { return color; }
    virtual float getSpecularCoeff() const { return specular_coeff;}
    virtual void rotate(const glm::mat4& m)=0;
    virtual void translate(const glm::mat4& m)=0;
    virtual void scale(const glm::mat4& m)=0;
    virtual void reset()=0;

 protected:
    ofFloatColor color;
    float specular_coeff;
};

// Plane Class

class Plane: public Primitive {
 public:
    Plane(const glm::vec3& p, const glm::vec3& n, float spec_coef,const ofFloatColor& color);
    virtual HitRecord hit(const Ray& ray) const override;
    virtual void rotate(const glm::mat4& m) override {return;};
    virtual void translate(const glm::mat4& m) override {return;};
    virtual void scale(const glm::mat4& m) override {return;};
    virtual void reset() override {return;};


 private:
    glm::vec3 point,normal;
};

// Sphere Class

class Sphere: public Primitive {
 public:
    Sphere(float r, const glm::vec3& c,float spec_coef, const ofFloatColor& color);
    virtual HitRecord hit(const Ray& ray) const override;
    virtual void rotate(const glm::mat4& m) override {return;};
    virtual void translate(const glm::mat4& m) override;
    virtual void scale(const glm::mat4& m) override;
    virtual void reset() override;

 private:
    float radius, radius_default;
    glm::vec3 center, center_default;
};

// Cone class

class Cone: public Primitive {
 public:
    Cone(const glm::vec3& c, const glm::vec3& o, float a,float spec_coef, const ofFloatColor& color);
    virtual HitRecord hit(const Ray& ray) const override;
    virtual void rotate(const glm::mat4& m) override;
    virtual void translate(const glm::mat4& m) override;
    virtual void scale(const glm::mat4& m) override;
    virtual void reset() override;

 private:
    float height, angle, cosa;
    glm::vec3 apex, center, axis, apex_default, center_default;
};

// Cylinder class

class Cylinder: public Primitive {
 public:
    Cylinder(const glm::vec3& c1, const glm::vec3& c2, float r,float spec_coef, const ofFloatColor& color);
    virtual HitRecord hit(const Ray& ray) const override;
    
    virtual void rotate(const glm::mat4& m) override;
    virtual void translate(const glm::mat4& m) override;
    virtual void scale(const glm::mat4& m) override;
    virtual void reset() override;

 private:
    float height, radius, radius_default;
    glm::vec3 center_top, center_bottom, axis,cb_default,ct_default;
};

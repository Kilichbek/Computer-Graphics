#pragma once

#include "ofMain.h"
#include "rtLinearAlgebra.h"

class AmbientLight {
  public:
    float ambient_coeff;
    ofFloatColor color;
    glm::vec4 light;
    AmbientLight(float coeff, const ofFloatColor& c);
};

class Light {
  public:
    glm::vec3 position;
    ofFloatColor color;
    float diffuse_coeff;

    Light(const glm::vec3& p, float diff_coef, const ofFloatColor& c);
    glm::vec3 getDiffuseColor(const glm::vec3& n,const glm::vec3& l,const glm::vec3& r, float spec);
    glm::vec4 getIllumination(float angle);
};
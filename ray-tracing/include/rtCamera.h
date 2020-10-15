#pragma once

#include "ofMain.h"
#include "rtClasses.h"
#include "rtLinearAlgebra.h"

enum ProjectMode { PARALLEL, PERSPECTIVE };

class Camera {
  public:
    glm::vec3 u,v,w;
    glm::vec3 cur_pos;
    
    // refer to https://learnopengl.com/Getting-started/Camera
    Camera(const glm::vec3& pos, const glm::vec3& dir, 
        const glm::vec3& right, ProjectMode m=PERSPECTIVE);
    void setTransform(const glm::vec4& matrix);
    void pan(const glm::vec3& v);
    void reset();
    void zoom(float alpha);
    void rotate(const glm::mat4& m);

    Ray rayForPixel(float x, float y) const;

  private:
    ProjectMode mode;
    glm::vec3 default_pos, dft_u, dft_w;
    float dist;
};
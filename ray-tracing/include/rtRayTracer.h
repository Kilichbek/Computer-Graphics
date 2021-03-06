#pragma once

#include <vector>
#include <chrono>
#include <random>
#include "rtClasses.h"
#include "rtCamera.h"
#include "rtLight.h"
#include "ofMain.h"
#include "tbb/parallel_for.h"


typedef std::chrono::high_resolution_clock Clock;

class RayTracer {
  public:
    RayTracer(int w, int h, Camera& c, const Viewport& v, 
      const AmbientLight& ambient);

    float render(ofPixels& pixels, bool parallel=false);
    void addObject(std::shared_ptr<Primitive> obj);
    void addLight(std::shared_ptr<Light> light);
    std::shared_ptr<Primitive> selectObject(int i, int j) const;

    Viewport viewport;
    Camera& camera;

  private:
    int height, width;
    AmbientLight ambient_light;

    std::vector<std::shared_ptr<Primitive>> objects;
    std::vector<std::shared_ptr<Light>> lights;

    HitRecord  traceRay(float x, float y) const;
    HitRecord testHit(const Ray& ray) const;
    ofColor getPixelColor(int i, int j) const;
    void performAntialiasing(float x, float y, std::vector<HitRecord>& arr) const;
    glm::vec4 getReflectionColor(const HitRecord& record,  const std::shared_ptr<Light>&) const;
    void performShading(const HitRecord& record, glm::vec4& color) const;
    glm::vec4 getIntensity(const HitRecord& record, const std::shared_ptr<Light>& light) const;
};
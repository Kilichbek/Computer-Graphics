#pragma once

#include <vector>
#include <chrono>
#include "rtClasses.h"
#include "rtLight.h"
#include "ofMain.h"
#include "tbb/parallel_for.h"


typedef std::chrono::high_resolution_clock Clock;
enum ProjectMode { PARALLEL, PERSPECTIVE };

class RayTracer {
  public:
    RayTracer(int w, int h, const Viewport& v, 
      const AmbientLight& ambient, const ProjectMode& mode=PERSPECTIVE);

    float render(ofPixels& pixels, bool parallel=false);
    void addObject(std::shared_ptr<Primitive> obj);
    void addLight(std::shared_ptr<Light> light);

    Viewport viewport;
  private:
    int height, width;
    ProjectMode mode;
    AmbientLight ambient_light;

    std::vector<std::shared_ptr<Light>> lights;
    std::vector<std::shared_ptr<Primitive>> objects;
    
    HitRecord  traceRay(float x, float y) const;
    HitRecord testHit(const Ray& ray) const;
    ofColor getPixelColor(int i, int j) const;
};
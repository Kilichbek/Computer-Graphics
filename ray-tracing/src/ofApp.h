#pragma once

#include "ofxGui.h"
#include "ofMain.h"
#include "rtRayTracer.h"
#include "rtTransformation.h"
#include "rtCamera.h"
#include "rtLight.h"
#include "tbb/parallel_for.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		bool loadObjModels(const std::string& filename, const glm::vec3& position);
		void updateMinMax(float x, float y, float z);

		int width, height;
		float min_x, max_x, min_y, max_y, min_z, max_z;
		
		ofTexture texColor;
		ofPixels colorPixels;

		ofParameter<float> render_time;
  		ofxPanel gui;

		std::unique_ptr<RayTracer> raytracer;
		std::unique_ptr<Camera> camera;

		std::shared_ptr<Primitive> sphere, plane, cone, cylinder, s1;
		std::shared_ptr<Primitive> object;
		
};

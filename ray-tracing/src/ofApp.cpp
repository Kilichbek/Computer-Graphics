#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    width = 640;
    height = 480;
    colorPixels.allocate(width, height, OF_PIXELS_RGB);

    // Initialize lights
    AmbientLight ambient_light(0.3, ofFloatColor(1,1,1));
    std::shared_ptr<Light> light1, light2, light3;
    light1 = std::make_shared<Light>(glm::vec3(0.0, 5.0, 0), 0.7,ofFloatColor(1,1,1));
    light2 = std::make_shared<Light>(glm::vec3(-0.5, 0.5, 0.0), 0.7, ofFloatColor(1,1,1));
    light3 = std::make_shared<Light>(glm::vec3(-0.5, 0.4, -0.2), 0.7, ofFloatColor(1,1,1));


    // Initialize primitives
    std::shared_ptr<Primitive> sphere, plane, cone, cylinder;
    plane = std::make_shared<Plane>(glm::vec3(0, -1.4, 0), glm::vec3(0, 1, 0),0,ofFloatColor(0.5, 0.5, 0.5));
    cone = std::make_shared<Cone>(glm::vec3(0.9, 1.3, 1.0), glm::vec3(0.9, -1, 1.0), 0.26, 1, ofFloatColor(1, 0, 0));
    cylinder = std::make_shared<Cylinder>(glm::vec3(-1.2, 1, -1.2), glm::vec3(-1.2, -1, -1.2), 0.15,1,ofFloatColor(0, 0, 1));
    sphere = std::make_shared<Sphere>(1.2, glm::vec3(-0.7, -0.5, 2),1,ofFloatColor(0, 1, 0));
    
    // Initialize Raytracer
    RayTracer raytracer(width, height, Viewport(2.0, 1.5), ambient_light);
    raytracer.addLight(light1);
    raytracer.addLight(light2);
    raytracer.addLight(light3);
    
    raytracer.addObject(plane);
    raytracer.addObject(cone);
    raytracer.addObject(cylinder);
    raytracer.addObject(sphere);


    auto time = raytracer.render(colorPixels, true);
    texColor.allocate(colorPixels);

    render_time.set("Render Time (sec): ", time, 0.0, 1.0);
    gui.setup();
    gui.add(render_time);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    texColor.draw(0, 0, width, height);
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

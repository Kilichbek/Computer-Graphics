#include "ofApp.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    width = 640;
    height = 480;
    colorPixels.allocate(width, height, OF_PIXELS_RGB);

    // Initialize lights
    AmbientLight ambient_light(0.3, ofFloatColor(1,1,1,1));
    std::shared_ptr<Light> light1, light2, light3;
    light1 = std::make_shared<Light>(glm::vec3(0.0, 5.0, 0), 0.7,ofFloatColor(1,1,1,1));
    //light2 = std::make_shared<Light>(glm::vec3(-1, 0.5, 1), 0.7, ofFloatColor(1,1,1,1));
    light3 = std::make_shared<Light>(glm::vec3(-0.5, 0.4, -0.2), 0.7, ofFloatColor(1,1,1,1));


    // Initialize primitives

    plane = std::make_shared<Plane>(glm::vec3(0, -1.4, 0), glm::vec3(0, 1, 0),0,ofFloatColor(0.5, 0.5, 0.5));
    cone = std::make_shared<Cone>(glm::vec3(0.9, 0.5, 0.0), glm::vec3(0.9, -1.4, 0.0), 0.26, 0.5, ofFloatColor(1, 0, 0));
    cylinder = std::make_shared<Cylinder>(glm::vec3(-1.2, 1, -1.2), glm::vec3(-1.2, -1, -1.2), 0.15,1,ofFloatColor(0, 0, 1));
    sphere = std::make_shared<Sphere>(1.2, glm::vec3(-0.7, -0.5, 1),0.9,ofFloatColor(0, 1, 0));

    //cone = std::make_shared<Cone>(glm::vec3(-1.0, -0.3, -1), glm::vec3(-0.5, -1, -1), 0.26, 0.5, ofFloatColor(1, 1, 0, 1));
    //sphere = std::make_shared<Sphere>(0.5, glm::vec3(2.5, -0.3, -0.2),0.9,ofFloatColor(0, 1, 0, 1));

    // Initialize Camera
    camera = std::make_unique<Camera>(glm::vec3(0, 0.5, -3), glm::vec3(0,0,1),glm::vec3(1,0,0));
    
    // Initialize Raytracer
    raytracer = std::make_unique<RayTracer>(width, height,*camera,Viewport(2.0, 1.5), ambient_light);
    raytracer->addLight(light1);
    //raytracer.addLight(light2);
    raytracer->addLight(light3);
    
    raytracer->addObject(plane);
    //raytracer->addObject(cone);
    raytracer->addObject(sphere);
    //raytracer->addObject(cylinder);



    // load models
    /*
    std::string filename = "models/teapot.obj";
    auto pst = glm::vec3(1,-1, 2);
    loadObjModels(filename, pst);
    
    filename = "models/teacup.obj";
    pst = glm::vec3(-2.8,-1, 1);
    loadObjModels(filename, pst);

    filename = "models/spoon.obj";
    pst = glm::vec3(-2.8, -1, -0.5);
    loadObjModels(filename, pst);
    */
    auto time = raytracer->render(colorPixels, true);

    texColor.allocate(colorPixels);

    render_time.set("Render Time (sec): ", time, 0.0, 100.0);
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
    switch(key){
        case OF_KEY_RIGHT:
        {
            camera->pan(glm::vec3(0.1,0,0));
            break;
        }
        case OF_KEY_LEFT:
        {
            camera->pan(glm::vec3(-0.1,0,0));
            break;
        }
        case OF_KEY_UP:
        {
            camera->pan(glm::vec3(0,0.1,0));
            break;
        }
        case OF_KEY_DOWN:
        {
            camera->pan(glm::vec3(0,-0.1,0));
            break;
        }
        case '=':
        {
            camera->zoom(1.05);
            break;
        }
        case OF_KEY_END:
        {
            camera->reset();
            break;
        }
        
        case ']':
        {
            camera->rotate(rotation_x(0.5236));
            break;
        }
        case '[':
        {
            camera->rotate(rotation_x(-0.5236));
            break;
        }
        case '.':
        {
            camera->rotate(rotation_y(0.5236));
            break;
        }
        case ',':
        {
            camera->rotate(rotation_y(-0.5236));
            break;
        }
        case 'w':
        {
            object->translate(translation(0,0,0.5));
            break; 
        }
            
        case 's':
        {
            object->translate(translation(0,0,-0.5));
            break;
        }
        case 'a':
        {
            object->translate(translation(-0.5,0,0));
            break;
        }
        case 'd':
        {
            object->translate(translation(0.5,0,0));
            break;
        }
        case 'q':
        {
            object->translate(translation(0,0.5,0));
            break;
        }
        case 'e':
        {
            object->translate(translation(0,-0.5,0));
            break;
        }
        case '1':
        {
            object->rotate(rotation_x(0.5236));
            break;
        }
        case '2':
        {
            object->rotate(rotation_x(-0.5236));
            break;
        }
        case '3':
        {
            object->rotate(rotation_y(0.5236));
            break;
        }
        case '4':
        {
            object->rotate(rotation_y(-0.5236));
            break;
        }
        case '5':
        {
            object->rotate(rotation_z(0.5236));
            break;
        }
        case '6':
        {
            object->rotate(rotation_z(-0.5236));
            break;
        }
        case '7':
        {
            object->rotate(scale(1.05,1.05,1.05));
            break;
        }
        case '8':
        {
            object->rotate(scale(0.95,0.95,0.95));
            break;
        }
        case 'r':
        {
            object->reset();
            break;
        }

        default:
            break;
    }
    auto time = raytracer->render(colorPixels, true);
    texColor.allocate(colorPixels);
    render_time.set("Render Time (sec): ", time, 0.0, 1.0);
    draw();
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
    object = raytracer->selectObject(x,y);
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
//--------------------------------------------------------------
bool ofApp::loadObjModels(const std::string& filename, const glm::vec3& position) {

  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::string warn, err;

  bool ret = tinyobj::LoadObj(&attrib, &shapes, nullptr, &warn, &err, filename.c_str());

  for (size_t s = 0; s < shapes.size(); s++) {
    
    std::shared_ptr<Model> model = std::make_shared<Model>(position, ofFloatColor(1,0,0,1));
    std::vector<glm::vec3> vert_norm(attrib.vertices.size(), glm::vec3(0.f, 0.f, 0.f));

    min_x = min_y = min_z = INFINITY;
    max_x = max_y = max_z = -INFINITY;

    size_t index_offset = 0;
    for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
      Triangle triangle;
      int fv = shapes[s].mesh.num_face_vertices[f];

      unsigned index[3] = {0, 0, 0};
      for (int v = 0; v < fv; v++) {
        const tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
        index[v] = idx.vertex_index;

        auto vertex_pos = glm::vec3(
            attrib.vertices[3*idx.vertex_index+0],
            attrib.vertices[3*idx.vertex_index+1],
            attrib.vertices[3*idx.vertex_index+2]
        );
        
        // load provided normals
        auto normal = glm::vec3(
            attrib.normals[3*idx.normal_index+0],
            attrib.normals[3*idx.normal_index+1],
            attrib.normals[3*idx.normal_index+2]
        );
        
    
        auto vertex = add_vecs(position, glm::rotateY(vertex_pos, static_cast<float>(M_PI_2)));

        model->vertices.push_back(vertex);
        model->normals.push_back(normal);
        triangle.vertices[v] = model->vertices.size() - 1;
        
        updateMinMax(vertex[0], vertex[1], vertex[2]);
      }

      index_offset += fv;
      model->triangles.push_back(triangle);

      auto A = model->vertices[triangle.vertices[0]];
      auto B = model->vertices[triangle.vertices[1]];
      auto C = model->vertices[triangle.vertices[2]];

      // Compute Normals from vertices instead of from the given values
      auto edge1 = subtract_vecs(A, B); 
      auto edge2 = subtract_vecs(A, C);

      // ref: https://www.iquilezles.org/www/articles/normals/normals.htm
      auto no = normalize(cross_product(edge1,edge2));
      vert_norm[index[0]] += no; vert_norm[index[1]] += no; vert_norm[index[2]] += no;
    }

    index_offset = 0;
    for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
      int fv = shapes[s].mesh.num_face_vertices[f];
      for (int v = 0; v < fv; v++) {
        const tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
        model->pnormals.push_back(normalize(vert_norm[idx.vertex_index]));
      }
      index_offset += fv;
    }

    
    const float radius = std::max(max_x, std::max(max_y, max_z)) - std::min(min_x, std::min(min_y, min_z));
    auto b1 = glm::vec3(min_x, min_y, min_z);
    auto b2 = glm::vec3(max_x, max_y, max_z);

    model->bounding_sphere = Sphere(radius, position, 0, ofFloatColor(0,1,0,1));
    model->bbox = BBox(b1, b2);
    
    raytracer->addObject(model);
  }

  return true;
}

void ofApp::updateMinMax(float x, float y, float z)
{
    min_x = std::min(min_x, x);
    max_x = std::max(max_x, x);

    min_y = std::min(min_y, y);
    max_y = std::max(max_y, y);

    min_z = std::min(min_z, z);
    max_z = std::max(max_z, z);
}
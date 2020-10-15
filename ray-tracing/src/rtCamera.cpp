#include "rtCamera.h"

// refer to https://learnopengl.com/Getting-started/Camera

Camera::Camera(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& right, const ProjectMode m):
    default_pos(pos), cur_pos(pos), mode(m)
{
    w = dir;
    u = right;
    v = cross_product(w,u);

    dft_w = w;
    dft_u = u;

    dist = 1;
}

void Camera::pan(const glm::vec3& v)
{
    cur_pos = add_vecs(cur_pos, v);
}

void Camera::reset()
{
    cur_pos = default_pos;
    u = dft_u;
    w = dft_w;
    v = cross_product(u,w);
    v = normalize(v);
    dist = 1;
}
/*
void Camera::rotate(const glm::mat4& m)
{
    auto w_v4 = glm::vec4(w[0],w[1],w[2], 0);

    w_v4 = matmul(m,w_v4);

    w = normalize(glm::vec3(w_v4[0],w_v4[1],w_v4[2]));
    u = normalize(cross_product(w,glm::vec3(0,-1,0)));
    v = normalize(cross_product(w,u));

}*/

void Camera::rotate(const glm::mat4& m)
{
    auto w_v4 = glm::vec4(w[0],w[1],w[2], 0);
    auto pos_v4 = glm::vec4(cur_pos[0],cur_pos[1],cur_pos[2], 1);

    w_v4 = matmul(m,w_v4);
    pos_v4 = matmul(m,pos_v4);

    cur_pos = glm::vec3(pos_v4[0],pos_v4[1],pos_v4[2]);
    w = normalize(glm::vec3(w_v4[0],w_v4[1],w_v4[2]));
    u = normalize(cross_product(w,glm::vec3(0,-1,0)));
    v = normalize(cross_product(w,u));
}

void Camera::zoom(float alpha)
{
    dist *= alpha;
}

Ray Camera::rayForPixel(float x, float y) const 
{
    Ray ray;
    auto a = scale_vec(x,u);
    auto b = scale_vec(y,v);
    auto uv = add_vecs(a,b);

    switch(mode){
        case PERSPECTIVE:
        {
            auto d = scale_vec(dist,w);
            ray.d = normalize(add_vecs(uv,d));
            ray.o = cur_pos;
            break;
        }
        case PARALLEL:
        {
            ray.d = normalize(w);
            ray.o = add_vecs(cur_pos, uv);
            break;
        }
        default:
            break;
    }

    return ray;
}
#include "rtLight.h"

AmbientLight::AmbientLight(float coeff, const ofFloatColor& c):
    ambient_coeff(coeff), color(c){
        light = glm::vec4(coeff*color.r,coeff*color.g,coeff*color.b,color.a);
    }

Light::Light(const glm::vec3& p, float diff_coef, const ofFloatColor& c):
    position(p), color(c), diffuse_coeff(diff_coef) {
    }

glm::vec3 Light::getDiffuseColor(const glm::vec3& n,const glm::vec3& l, const glm::vec3& r, float spec)
{
    auto float_color = glm::vec3(color.r,color.g,color.b);

    // Lambertian shading model
    auto lambert_angle = dot_product(n,l);
    auto lambert_sf = 0.5*diffuse_coeff * std::max(0.f,lambert_angle);
    auto lambertian = scale_vec(lambert_sf, float_color);

    // Blinn-Phong shading model
    auto v = scale_vec(-1.0, r);
    v = normalize(v);
    auto vl = add_vecs(v,l);
    auto h = normalize(vl);
    auto phong_angle = dot_product(n,h);
    auto phong_sf = 0.7* spec * pow(std::max(0.f,phong_angle),1000.0);
    auto blinn_phong = scale_vec(phong_sf, float_color);

    return add_vecs(lambertian,blinn_phong);
}

glm::vec4 Light::getIllumination(float angle)
{
    auto sf = diffuse_coeff * std::max(0.f, angle);

    return glm::vec4(sf * color.r, sf * color.g, sf * color.b, color.a);
}
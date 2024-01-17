#include "Transparent.hh"
#include <iostream>

Transparent::Transparent(const vec3& color): Material()
{
    Kd = color;
}

Transparent::Transparent(const vec3& a, const vec3& d, const vec3& s, const vec3& t, const float k, const float s2):
    Material(a, d, s, t, k, s2) {
}

Transparent::Transparent(const vec3& a, const vec3& d, const vec3& s, const vec3& t, const float k, const float s2, const float o):
    Material(a, d, s, t, k, s2, o) {
}
Transparent::~Transparent()
{
}

bool Transparent::scatter(const Ray& r_in, const HitInfo& rec, vec3& color, Ray & r_out) const  {
    float eta = 1.0/rec.mat_ptr->nut; //index de refraccio considerant que estem en el buit (nu = 1)
    vec3 n = rec.normal;
    if (dot(r_in.getDirection(), rec.normal) > DBL_EPSILON) { //el raig refractit surt de lesfera cap al buit
        n = rec.normal * vec3(-1.0);
        eta = 1.0/eta;
    }
    vec3 rout_v = glm::refract(r_in.getDirection(), n, eta);

    if (glm::length(rout_v) < 0.001) { //reflexio interna sÇha passat langle limit
        rout_v = glm::reflect(r_in.getDirection(), n);
        color = rec.mat_ptr->Ks;
    } else color = rec.mat_ptr->Kt;

    r_out = Ray(rec.p, rout_v);
    return true;
}

vec3 Transparent::getDiffuse(vec2 uv) const {
    return Kd;

}

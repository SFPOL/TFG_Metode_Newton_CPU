#include "Metal.hh"

Metal::Metal(const vec3& color): Material()
{
    Kd = color;
}

Metal::Metal(const vec3& a, const vec3& d, const vec3& s, const vec3& t, const float k, const float s2):
    Material(a, d, s, t, k, s2) {
}

Metal::Metal(const vec3& a, const vec3& d, const vec3& s, const vec3& t, const float k, const float s2, const float o):
    Material(a, d, s, t, k, s2, o) {
}
Metal::~Metal()
{
}

bool Metal::scatter(const Ray& r_in, const HitInfo& rec, vec3& color, Ray & r_out) const  {
    vec3 rout_v = glm::reflect(r_in.getDirection(), rec.normal);
    r_out =  Ray(rec.p, rout_v);
    color = Ks;
    return true;
}

vec3 Metal::getDiffuse(vec2 uv) const {
    return Kd;

}

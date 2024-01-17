#include "Lambertian.hh"

Lambertian::Lambertian(const vec3& color): Material()
{
    Kd = color;
}

Lambertian::Lambertian(const vec3& a, const vec3& d, const vec3& s, const vec3& t, const float k, const float s2):
    Material(a, d, s, t, k, s2) {
}

Lambertian::Lambertian(const vec3& a, const vec3& d, const vec3& s, const vec3& t, const float k, const float s2, const float o):
    Material(a, d, s, t, k, s2, o) {
}
Lambertian::~Lambertian()
{
}

bool Lambertian::scatter(const Ray& r_in, const HitInfo& rec, vec3& color, Ray & r_out) const  {
    vec3 target = rec.p + rec.normal + Hitable::RandomInSphere();
    r_out =  Ray(rec.p, target-rec.p);
    color = Kd;
    return true;
}

vec3 Lambertian::getDiffuse(vec2 uv) const {
    return Kd;

}

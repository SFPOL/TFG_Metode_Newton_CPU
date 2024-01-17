#pragma once

#include "Material.hh"

class Lambertian : public Material
{

public:
    Lambertian() {};
    Lambertian(const vec3& color);
    Lambertian(const vec3& a, const vec3& d, const vec3& s, const vec3& t, const float k, const float s2);
    Lambertian(const vec3& a, const vec3& d, const vec3& s, const vec3& t, const float k, const float s2, const float o);
    virtual ~Lambertian();

    virtual bool scatter(const Ray& r_in, const HitInfo& rec, vec3& color, Ray & r_out) const;
    virtual vec3 getDiffuse(vec2 uv) const;

};

#pragma once

#include "Material.hh"
#include "Texture.hh"


class MaterialTextura : public Material
{

public:
    MaterialTextura() {};
    MaterialTextura(const vec3& color);
    MaterialTextura(const vec3& a, const vec3& d, const vec3& s, const vec3& t, const float k, const float s2);
    MaterialTextura(const vec3& a, const vec3& d, const vec3& s, const vec3& t, const float k, const float s2, const float o);
    virtual ~MaterialTextura();

    virtual void read (const QJsonObject &json) override;
    virtual bool scatter(const Ray& r_in, const HitInfo& rec, vec3& color, Ray & r_out) const;
    virtual vec3 getDiffuse(vec2 uv) const;

private:
    Texture textura = Texture();
};

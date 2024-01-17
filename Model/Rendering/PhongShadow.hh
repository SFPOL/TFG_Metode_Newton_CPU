#pragma once

#include "ShadingStrategy.hh"

class PhongShadow: public ShadingStrategy
{
public:
    PhongShadow() {};
    vec3 shading(shared_ptr<Scene> scene, HitInfo& info, vec3 lookFrom, std::vector<shared_ptr<Light>> lights, vec3 globalLight) override;
    ~PhongShadow(){};
};

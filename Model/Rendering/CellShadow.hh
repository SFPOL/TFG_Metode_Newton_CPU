#pragma once

#include "ShadingStrategy.hh"

class CellShadow: public ShadingStrategy
{
public:
    CellShadow() {};
    vec3 shading(shared_ptr<Scene> scene, HitInfo& info, vec3 lookFrom, std::vector<shared_ptr<Light>> lights, vec3 globalLight) override;
    ~CellShadow(){};
};


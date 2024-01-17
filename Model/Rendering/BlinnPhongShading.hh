#pragma once

#include "ShadingStrategy.hh"

class BlinnPhongShading: public ShadingStrategy
{
public:
    BlinnPhongShading() {};
    vec3 shading(shared_ptr<Scene> scene, HitInfo& info, vec3 lookFrom, std::vector<shared_ptr<Light>> lights, vec3 globalLight) override;
    ~BlinnPhongShading(){};
};

#pragma once

#include "Model/Modelling/Scene.hh"
#include "Model/Modelling/Lights/Light.hh"
#include "Model/Modelling/Lights/PointLight.hh"


class ShadingStrategy {
 public:
    // TODO: Fase 2: Canviar el mètode per passar les llums per calcular el shading
    virtual vec3 shading(shared_ptr<Scene> scene, HitInfo& info, vec3 lookFrom, std::vector<shared_ptr<Light>> lights, vec3 globalLight) {
        return vec3(0.0, 0.0, 0.0);
    };

    // FASE 2: Calcula si el punt "point" és a l'ombra segons si el flag està activat o no
    float computeShadow(shared_ptr<Scene> scene, shared_ptr<Light> light, vec3 point, vec3 &shadow_color, bool &transparency); //TIREM UN RAIG A LA LLUM. SI HI HA HIT POSEM EL FACTOR D'OMBRA A 0
    float computeShadow(shared_ptr<Scene> scene, shared_ptr<Light> light, vec3 point);

    virtual ~ShadingStrategy() {};
};

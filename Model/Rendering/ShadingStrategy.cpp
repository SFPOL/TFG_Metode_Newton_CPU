#include "ShadingStrategy.hh"
#include <iostream>

float ShadingStrategy::computeShadow(shared_ptr<Scene> scene, shared_ptr<Light> light, vec3 point, vec3 &shadow_color, bool &transparency) {
    Ray ray = Ray(point, light->vectorL(point));
    vector<HitInfo> infos;
    if (scene->allHits(ray, 0.01, light->distanceToLight(point), infos)) {
        float factorOmbra = 1;
        for (unsigned int i = 0; i < infos.size(); i += 2) {
            if (infos.size() == 1 || glm::length(infos[i].mat_ptr->Kt) < DBL_EPSILON)
                return 0.0;
            float dist = glm::distance(infos[i].p, infos[i+1].p);
            shadow_color *= (vec3(1.0)-infos[i].mat_ptr->Kt);
            factorOmbra *= glm::max(1.0 - dist/1.2, 0.0);
            transparency = true;
        }
        return factorOmbra;
    }
    return 1.0;
}

float ShadingStrategy::computeShadow(shared_ptr<Scene> scene, shared_ptr<Light> light, vec3 point) {
    Ray ray = Ray(point, light->vectorL(point));
    HitInfo throwaway;
    if (scene->hit(ray, 0.01, light->distanceToLight(point), throwaway))
        return 0.0;
    return 1.0;
}

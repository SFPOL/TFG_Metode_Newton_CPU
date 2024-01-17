#include "ColorShadow.hh"

vec3 ColorShadow::shading(shared_ptr<Scene> scene, HitInfo& info, vec3 lookFrom, std::vector<shared_ptr<Light>> lights, vec3 globalLight){
    if(lights.empty())
    {
        return info.mat_ptr->getDiffuse(info.uv);
    }
    else
    {
        vec3 factor = vec3(0.0);
        for(std::shared_ptr<Light> l: lights) {
            if(glm::abs(computeShadow(scene, l, info.p)) < DBL_EPSILON)
                factor += l->getIa();
            else
                factor += l->getId();
        }
        return glm::min(info.mat_ptr->getDiffuse(info.uv)*factor+globalLight, vec3(1.0, 1.0, 1.0));
    }
}

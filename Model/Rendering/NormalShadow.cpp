#include "NormalShadow.hh"

vec3 NormalShadow::shading(shared_ptr<Scene> scene, HitInfo& info, vec3 lookFrom, std::vector<shared_ptr<Light>> lights, vec3 globalLight) {
    vec3 normal_color = (info.normal+vec3(1))*0.5f;
    if(lights.empty())
    {
        return normal_color;
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
        return glm::min(normal_color*factor+globalLight, vec3(1.0, 1.0, 1.0));
     }
}

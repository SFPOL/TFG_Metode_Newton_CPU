#include "DepthShadow.hh"

vec3 DepthShadow::shading(shared_ptr<Scene> scene, HitInfo& info, vec3 lookFrom, std::vector<shared_ptr<Light>> lights, vec3 globalLight) {
    vec3 vec = info.p-lookFrom;
    float d = sqrt(dot(vec, vec));
    float k = 3; // Dividing Factor
    if(lights.empty())
    {
        return vec3(d/k);
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
        return glm::min(factor*vec3(d/k), vec3(1.0, 1.0, 1.0));  // no afegim globalLight per veure millor el shading
    }
}

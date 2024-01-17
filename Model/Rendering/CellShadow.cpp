#include "CellShadow.hh"

vec3 CellShadow::shading(shared_ptr<Scene> scene, HitInfo& info, vec3 lookFrom, std::vector<shared_ptr<Light>> lights, vec3 globalLight){
    vec3 color = info.mat_ptr->getDiffuse(info.uv);
    vec3 finalCol = vec3(0.0);
    for(std::shared_ptr<Light> l: lights){
        vec3 tempColor;
        float intFactor = 0.2f;
        vec3 vec1 = l->vectorL(info.p);
        vec3 vec2 = info.normal;
        float intensity = dot(vec1, vec2);
        if (intensity > 0.95)      intFactor = 1.0f;
        else if (intensity > 0.5)  intFactor = 0.6f;
        else if (intensity > 0.25) intFactor = 0.4f;
        tempColor = globalLight+intFactor*color*computeShadow(scene, l, info.p);
        finalCol = glm::max(finalCol, tempColor);
    }
    return finalCol;
}

#include "BlinnPhongShading.hh"

vec3 BlinnPhongShading::shading(shared_ptr<Scene> scene, HitInfo& info, vec3 lookFrom, std::vector<shared_ptr<Light>> lights, vec3 globalLight) {
    vec3 i_tot = vec3(0.0, 0.0, 0.0);

    for (std::shared_ptr<Light> light: lights) {
        vec3 i1 = vec3(0.0, 0.0, 0.0);
        vec3 i2 = vec3(0.0, 0.0, 0.0);
        vec3 i3 = vec3(0.0, 0.0, 0.0);

        // Component ambient
        i1 += light->getIa() * info.mat_ptr->Ka;

        //Component difosa
        double prodEsc = dot(light->vectorL(info.p), info.normal);
        prodEsc = glm::max(prodEsc, 0.0);
        i2 += light->attenuation(info.p) * light->getId() * info.mat_ptr->getDiffuse(info.uv) * float(prodEsc);

        //Component especular
        std::shared_ptr<PointLight> p_light = dynamic_pointer_cast<PointLight>(light);
        vec3 vecV = p_light->getPos() - info.p;
        vec3 vecH = normalize(p_light->vectorL(info.p) + vecV);
        double prodEsc2 = dot(vecH, info.normal);
        prodEsc2 = pow(glm::max(prodEsc2, 0.0), info.mat_ptr->shininess);
        i3 += p_light->attenuation(info.p) * p_light->getIs() * info.mat_ptr->Ks * float(prodEsc2);

        i_tot += i1+i2+i3;
    }

    i_tot += info.mat_ptr->Ka*globalLight;

    return glm::min(i_tot, vec3(1.0, 1.0, 1.0));
}

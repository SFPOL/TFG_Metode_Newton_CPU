#include "Scene.hh"

Scene::Scene()
{
    pmin.x = -0.5f;  pmin.y = -0.5f; pmin.z = -0.5f;
    pmax.x = 0.5f;  pmax.y = 0.5f; pmax.z = 0.5f;
}


bool Scene::hit(Ray &raig, float tmin, float tmax, HitInfo& info) const {
    // TODO FASE 0 i FASE 1:
    // Heu de codificar la vostra solucio per aquest metode substituint el 'return true'
    // Una possible solucio es cridar el mètode "hit" per a tots els objectes i quedar-se amb la interseccio
    // mes propera a l'observador, en el cas que n'hi hagi més d'una.
    // Si un objecte es intersecat pel raig, el parametre  de tipus HitInfo conte
    // la informació sobre la interseccio.
    // Cada vegada que s'intersecta un objecte s'ha d'actualitzar el HitInfo del raig.
    bool ret = false;
    float t = tmax;

    HitInfo aux;
    //Hit amb el FittedPlane base
    if (this->basePlane != nullptr && this->basePlane->hit(raig, tmin, t, aux)) {
        t = aux.t;
        ret = true;
    }


    for (std::shared_ptr<Object> o: this->objects) {
        if(o->hit(raig, tmin, t, aux)){
            t = aux.t;
            ret = true;
        }
    }

    info = aux;

    //retorna el pt més proper
    return ret;
}

bool Scene::allHits(Ray &r, float tmin, float tmax, vector<HitInfo> &infos) const {
    HitInfo info_plane;
    if (this->basePlane != nullptr && this->basePlane->hit(r, tmin, tmax, info_plane))
        infos.push_back(info_plane);
    for (std::shared_ptr<Object> o: this->objects) {
        vector<HitInfo> o_infos;
        if(o->allHits(r, tmin, tmax, o_infos)) {
            unsigned int old_size = infos.size();
            infos.resize(old_size+o_infos.size());
            std::move(o_infos.begin(), o_infos.end(), infos.begin()+old_size);
        }
    }
    sort(infos.begin(), infos.end(), HitInfo::sortInfos);
    return !infos.empty();
}


void Scene::update(int nframe) {
    for (unsigned int i = 0; i< objects.size(); i++) {
        objects[i]->update(nframe);
    }
}

void Scene::setDimensions(vec3 p1, vec3 p2) {
    pmin = p1;
    pmax = p2;
}


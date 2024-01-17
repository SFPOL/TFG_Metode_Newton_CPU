#pragma once


#include "Object.hh"
#include "Model/Modelling/Animation.hh"
#include "Model/Modelling/TG/TranslateTG.hh"
#include "glm/gtx/constants.hpp"

class FittedPlane: public Object {
public:
    FittedPlane();
    FittedPlane(vec3 normal, vec3 point, vec3 pmin, vec3 pmax, float v);
    FittedPlane(vec3 normal, float d, float v);
    FittedPlane(float data);
    virtual ~FittedPlane() {}
    virtual bool hit(Ray &r, float tmin, float tmax, HitInfo &info) const override;

    virtual void aplicaTG(shared_ptr<TG> tg) override;

    virtual void read (const QJsonObject &json) override;
    virtual void write(QJsonObject &json) const override;
    virtual void print(int indentation) const override;

    vec3 normal;
    vec3 point;
    vec3 pmin;
    vec3 pmax;
private:

};

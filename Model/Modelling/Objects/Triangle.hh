#pragma once

#include "Object.hh"
#include "Plane.hh"
#include "Model/Modelling/Animation.hh"
#include "Model/Modelling/TG/TranslateTG.hh"

class Triangle: public Object {
public:
    Triangle();

    Triangle(vec3 a, vec3 b, vec3 c, float data);
    Triangle(float data);
    virtual ~Triangle() {}
    virtual bool hit(Ray& r, float tmin, float tmax, HitInfo& info) const override;
    virtual void aplicaTG(shared_ptr<TG> tg) override {}

    virtual void read (const QJsonObject &json) override;
    virtual void write(QJsonObject &json) const override {}
    virtual void print(int indentation) const override {}

private:
    vec3 a;
    vec3 b;
    vec3 c;
    vec3 normal;
    shared_ptr<Plane> pla;

    static float calculArea(vec3 v) { return sqrt(dot(v,v))/2; }
};

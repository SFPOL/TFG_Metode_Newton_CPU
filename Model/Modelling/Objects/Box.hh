#pragma once

#include "Object.hh"
#include "Model/Modelling/Animation.hh"
#include "Model/Modelling/TG/TranslateTG.hh"
#include "Model/Modelling/TG/ScaleTG.hh"

class Box:  public Object {
public:
    Box();

    //Crea una box a partir dels dos vèrtex extrem
    Box(vec3 v0, vec3 v1, float data);

    //Crea una box amb v0 en (0,0,0) i aresta long 1
    Box(float data);
    virtual ~Box() {}
    virtual bool hit(Ray& r, float tmin, float tmax, HitInfo& info) const override;
    virtual void aplicaTG(shared_ptr<TG> tg) override;

    virtual void read (const QJsonObject &json) override;
    virtual void write(QJsonObject &json) const override;
    virtual void print(int indentation) const override;

    vec3 getVertex0()  {return v0;};
    vec3 getVertex1()  {return v1;}

private:
    vec3 v0;
    vec3 v1;


};

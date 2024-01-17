/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#pragma once

#include "Object.hh"
#include "Model/Modelling/Animation.hh"
#include "Model/Modelling/TG/TranslateTG.hh"
#include "Model/Modelling/TG/ScaleTG.hh"

class Sphere: public Object  {
public:
    Sphere();

    Sphere(vec3 cen, float r, float data);
    //Crea una esfera unitaria centrada al punt (0,0,0) i de radi 1
    Sphere(float data);
    virtual ~Sphere() {}
    virtual bool hit(Ray& r, float tmin, float tmax, HitInfo& info) const override;
    virtual void aplicaTG(shared_ptr<TG> tg) override;

    virtual void read (const QJsonObject &json) override;
    virtual void write(QJsonObject &json) const override;
    virtual void print(int indentation) const override;


    vec3  getCenter() { return center;};
    float getRadius() { return radius;}

private:
    // Centre de l'esfera
    vec3 center;
    // Radi de l'esfera
    float radius;
    // Numero PI
    const float PI = 3.141592653;

};






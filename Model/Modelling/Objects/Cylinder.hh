/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#pragma once

#include "Object.hh"
#include "Model/Modelling/Animation.hh"
#include "Model/Modelling/TG/TranslateTG.hh"

class Cylinder: public Object  {
public:
    Cylinder();

    Cylinder(vec3 cen, float r, float h, vec3 ax, float data);
    //Crea un cilindre amb:
    // · center = [0,-1,0]
    // · radius = 1
    // · h = 2
    // · axis = [0,1,0]
    Cylinder(float data);
    virtual ~Cylinder() {}
    virtual bool hit(Ray& r, float tmin, float tmax, HitInfo& info) const override;
    virtual void aplicaTG(shared_ptr<TG> tg) override;

    virtual void read (const QJsonObject &json) override;
    virtual void write(QJsonObject &json) const override;
    virtual void print(int indentation) const override;


    vec3  getCenter() { return center;};
    float getRadius() { return radius;};
    float getHeight() { return height;};
    vec3  getAxis()   { return axis;};

private:
    // Centre de la base del cilindre
    vec3 center;
    // Radi del cilindre
    float radius;
    // Altura del cilindre
    float height;
    // Orientaió del cilindre
    vec3 axis;
    // Numero PI
    const float PI = 3.141592653;
};






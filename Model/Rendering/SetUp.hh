#pragma once

#include "DataInOut/Serializable.hh"

#include <QFile>

#include "glm/glm.hpp"
#include "Model/Rendering/Camera.hh"
#include "Model/Modelling/Lights/LightFactory.hh"

#include "Model/Rendering/ShadingFactory.hh"
#include "Model/Rendering/ShadingStrategy.hh"
#include "Model/Rendering/ColorShading.hh"

class SetUp : public Serializable
{
public:
    SetUp();

    QString                         getOutputFile();
    shared_ptr<Camera>              getCamera();
    vec3                            getGlobalLight();
    std::vector<shared_ptr<Light>>  getLights();
    shared_ptr<ShadingStrategy>     getShadingStrategy() {return shade;}
    bool                            getBackground();
    vec3                            getTopBackground();
    vec3                            getDownBackground();
    int                             getMAXDEPTH();
    int                             getSamples();
    bool                            getReflections() {return reflections;}
    bool                            getRefractions() {return refractions;}
    bool                            getShadows() {return shadows;}
    bool                            getTextures() {return textures;}


    void setOutpuFile(QString name);
    void setCamera(shared_ptr<Camera> cam);
    void setGlobalLight(vec3 globalLight);
    void setLights(std::vector<shared_ptr<Light>> lights);
    void setShadingStrategy(shared_ptr<ShadingStrategy> s) {shade = s;}
    void setBackground(bool back);
    void setTopBackground(vec3 color);
    void setDownBackground(vec3 color);
    void setSamples(int s);
    void setMaxDepth(int d);
    void setReflections(bool b);
    void setRefractions(bool b);
    void setShadows(bool b);
    void setTextures(bool b);

    virtual void read (const QJsonObject &json);
    virtual void write (QJsonObject &json) const;
    virtual void print (int indentation) const;

    bool load( QString nameFile);
    bool save( QString nameFile) const;

    virtual ~SetUp() {};


private:
    QString outputFile;
    // Camera
    shared_ptr<Camera> camera;

    // Shading utilitzat
    shared_ptr<ShadingStrategy> shade;

    // FASE 2: Afegir llums a l'escena
    // Vector de llums de l'escena
    std::vector<shared_ptr<Light>> lights;
    // Llum global
    vec3 globalLight;

    // Conté si s'ha de tenir en compte el background o no en els rajos secundaris en
    // cas de no fer hit amb l'escena
    bool background = false;
    // colors top i bottom de  fons
    vec3 topBackground;
    vec3 downBackground;

    // màxim nombre de rajos recursius incloent el primari
    int   MAXDEPTH;

    // number of samples per pixels
    int   numSamples;

    // flags per activar funcionalitats del RayColor
    // FASE 3: cal usar-los allà
     bool reflections;
     bool refractions;
     bool shadows;
     bool textures;

     // FASE 3: Guarda si en les iteracions recursives de rayColor() en cas de no haver-hi hit
     // s'utilitza el color de background o la llum ambient global
     // bool backgroundInRecurvise = false;
};


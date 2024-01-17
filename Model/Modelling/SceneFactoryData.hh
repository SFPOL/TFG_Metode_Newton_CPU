#pragma once

#include "Model/Modelling/Materials/ColorMapStatic.hh"
#include "Model/Modelling/Materials/Material.hh"
#include "Model/Modelling/SceneFactory.hh"
#include "Model/Modelling/Materials/MaterialFactory.hh"
#include "DataInOut/VisualMapping.hh"

class SceneFactoryData : public SceneFactory
{
    // dades: vector de parells <AttributeMapping, vector de dades d'aquella propietat>
    vector<pair<QString, vector<vec3>>> dades;
    shared_ptr<VisualMapping> mapping;

public:
    SceneFactoryData() {};
    SceneFactoryData(shared_ptr<VisualMapping> mapping);
    virtual shared_ptr<Scene>  createScene(QString nomFitxer) override;
    virtual shared_ptr<Scene>  createScene() override;

    virtual void read (const QJsonObject &json) override;
    virtual void write (QJsonObject &json) const override;
    virtual void print (int indentation) const override;

    bool load (QString nameFile);
    bool save (QString nameFile) const;

    vec3 getPuntBase(ObjectFactory::OBJECT_TYPES gyzmo, vec2 puntReal);

    void readData(const QJsonObject &json);
    void writeData (QJsonObject &json, int i) const ;

    shared_ptr<Scene>    visualMaps();
    shared_ptr<Object>   objectMaps(int i, int j);
    shared_ptr<Material> materialMaps(int i, int j);
};



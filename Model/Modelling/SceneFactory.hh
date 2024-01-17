#pragma once

#include <string.h>
#include <QString>

#include "DataInOut/Serializable.hh"
#include "Model/Modelling/Scene.hh"
#include "Model/Modelling/Objects/ObjectFactory.hh"


class SceneFactory: public Serializable
{
public:
    typedef enum
    {
           VIRTUALWORLD,
           REALDATA,
           TEMPORALVW
    } SCENE_TYPES;

    SceneFactory() {};
    virtual ~SceneFactory() {};

    virtual shared_ptr<Scene> createScene()=0;
    virtual shared_ptr<Scene> createScene(QString nomFitxer)=0;

    static SCENE_TYPES getSceneFactoryType( QString name);
    QString            getNameType(SCENE_TYPES t) const;

protected:
    shared_ptr<Scene> scene;
    SCENE_TYPES currentType;

};



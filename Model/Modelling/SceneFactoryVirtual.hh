#pragma once

#include "Model/Modelling/SceneFactory.hh"

class SceneFactoryVirtual : public SceneFactory
{

public:
    SceneFactoryVirtual();

    virtual shared_ptr<Scene>  createScene (QString nomFitxer) override;
    virtual shared_ptr<Scene>  createScene() override;

    virtual void read(const QJsonObject &json) override;
    virtual void write(QJsonObject &json) const override;
    virtual void print(int indentation) const override;

    bool load(QString nameFile);
    bool save(QString nameFile) const;

private:
    void OneSphere(shared_ptr<Scene> s);
};



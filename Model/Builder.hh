#pragma once

#include <QtWidgets>
#include "Controller.hh"

class Builder : public QObject {
    Q_OBJECT

public:
    Builder(){};

public slots:
    void newSphere();
    void newPC();
    void newBox();
    void newTriangle();
    void newCylinder();
    void newMesh();
    void newSimulatedScene();
    void newVirtualScene();
    void newDataScene();
    void loadSettings();
    void activaColorShading();
    void activaNormalShader();
    void activaDepthShader();
    void activaCell_Shading();
    void activaBlinn_Phong();
    void activaPhongShader();

signals:
     void settingsChanged();
};


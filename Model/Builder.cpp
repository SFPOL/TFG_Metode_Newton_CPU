#include "Builder.hh"


void Builder::newSphere()
{
    vec3 position(0.0, 0.0, 0.0);
    float radius = 0.5;
    QMessageBox msgBox;
    if (Controller::getInstance()->createScene(position, radius))
            msgBox.setText("One Sphere is created.");
    else msgBox.setText("Not enough memory. Error.");
    msgBox.exec();
}

void Builder::newPC()
{
    QMessageBox msgBox;
    if (Controller::getInstance()->createScenePC())
            msgBox.setText("One PC is created.");
    else msgBox.setText("Not enough memory. Error.");
    msgBox.exec();
}

void Builder::newBox()
{
    vec3 v0(0.0, 0.0, 0.0);
    vec3 v1(1.0, 1.0, -1.0);
    QMessageBox msgBox;
    if (Controller::getInstance()->createScene(v0, v1))
            msgBox.setText("One Box is created.");
    else msgBox.setText("Not enough memory. Error.");
    msgBox.exec();
}

void Builder::newTriangle()
{
    vec3 a(0.0, 0.0, 0.0);
    vec3 b(1.0, 0.0, 0.0);
    vec3 c(0.0, 1.0, 0.0);
    QMessageBox msgBox;
    if (Controller::getInstance()->createScene(a, b, c))
        msgBox.setText("One Triangle is created.");
    else msgBox.setText("Not enough memory. Error.");
    msgBox.exec();
}

void Builder::newCylinder()
{
    QMessageBox msgBox;
    if (Controller::getInstance()->createSceneCylinder())
            msgBox.setText("One Cylinder is created.");
    else msgBox.setText("Not enough memory. Error.");
    msgBox.exec();
}

void Builder::newMesh()
{
    QMessageBox msgBox;
    QString fileName = "://resources/cube.obj";
    if (Controller::getInstance()->createSceneMesh(fileName))
        msgBox.setText("One Mesh is created.");
    else msgBox.setText("Not enough memory. Error.");
    msgBox.exec();
}

void Builder::newSimulatedScene()
{
   QMessageBox msgBox;
   if (Controller::getInstance()->createScene())
           msgBox.setText("A simulated Scene is created.");
   else msgBox.setText("Not enough memory. Error.");
   msgBox.exec();
}


void Builder::newVirtualScene()
{
    QString fileName = QFileDialog::getOpenFileName();
    if (!fileName.isNull()) {
        QMessageBox msgBox;
        if(Controller::getInstance()->createScene(SceneFactory::SCENE_TYPES::VIRTUALWORLD, fileName))
            msgBox.setText("Virtual Scene well loaded.");
        else msgBox.setText("Scene NOT loaded. Error reading data.");
        msgBox.exec();
    }
}


void Builder::newDataScene()
{
    QString fileName = QFileDialog::getOpenFileName();
    if (!fileName.isNull()) {
        QMessageBox msgBox;
        if (Controller::getInstance()->createScene(SceneFactory::SCENE_TYPES::REALDATA, fileName)) {
                msgBox.setText("Data Scene well loaded.");
        } else msgBox.setText("Scene NOT loaded. Error reading data.");
        msgBox.exec();
    }
}

void Builder::loadSettings() {
    QString fileName = QFileDialog::getOpenFileName();
    if (!fileName.isNull()) {
        QMessageBox msgBox;
        if (Controller::getInstance()->createSettings(fileName)) {
                msgBox.setText("Visual Settings well loaded.");

        } else msgBox.setText("Settings NOT loaded. Error reading data.");
        msgBox.exec();
        emit settingsChanged();
    }
}

void Builder::activaColorShading() {
    QMessageBox msgBox;
    if (Controller::getInstance()->createShading(ShadingFactory::SHADING_TYPES::COLOR)) {
        msgBox.setText("Color Shading created.");
    } else msgBox.setText("Error creating shading");
    msgBox.exec();
}

void Builder::activaNormalShader() {
    QMessageBox msgBox;
    if (Controller::getInstance()->createShading(ShadingFactory::SHADING_TYPES::NORMAL)) {
        msgBox.setText("Normal Shading created.");
    } else msgBox.setText("Error creating shading");
    msgBox.exec();
}

void Builder::activaDepthShader() {
    QMessageBox msgBox;
    if (Controller::getInstance()->createShading(ShadingFactory::SHADING_TYPES::DEPTH)) {
        msgBox.setText("Depth Shading created.");
    } else msgBox.setText("Error creating shading");
    msgBox.exec();
}

void Builder::activaCell_Shading() {
    QMessageBox msgBox;
    if (Controller::getInstance()->createShading(ShadingFactory::SHADING_TYPES::CELL)) {
        msgBox.setText("Cell Shading created.");
    } else msgBox.setText("Error creating shading");
    msgBox.exec();
}

void Builder::activaBlinn_Phong() {
    QMessageBox msgBox;
    if (Controller::getInstance()->createShading(ShadingFactory::SHADING_TYPES::BLINNPHONG)) {
        msgBox.setText("Blinn Phong Shading created.");
    } else msgBox.setText("Error creating shading");
    msgBox.exec();
}

void Builder::activaPhongShader() {
    QMessageBox msgBox;
    if (Controller::getInstance()->createShading(ShadingFactory::SHADING_TYPES::PHONG)) {
        msgBox.setText("Phong Shading created.");
    } else msgBox.setText("Error creating shading");
    msgBox.exec();
}



#include "CameraMenu.hh"

CameraMenu::CameraMenu()
{


    ui.setupUi(&dialog);


    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(modify()));
}

void CameraMenu::cameraMenuShow() {


    auto camera = Controller::getInstance()->getSetUp()->getCamera();
    vec3 lookAt = camera->getLookAt();
    vec3 lookFrom = camera->getLookFrom();
    float vfov = camera->getFOV();
    ui.valLookAtX->setValue(lookAt.x);
    ui.valLookAtY->setValue(lookAt.y);
    ui.valLookAtZ->setValue(lookAt.z);
    ui.valLookAtX_2->setValue(lookAt.x);
    ui.valLookAtY_2->setValue(lookAt.y);
    ui.valLookAtZ_2->setValue(lookAt.z);

    ui.valX->setValue(lookFrom.x);
    ui.valY->setValue(lookFrom.y);
    ui.valZ->setValue(lookFrom.z);
    ui.valX_2->setValue(lookFrom.x);
    ui.valY_2->setValue(lookFrom.y);
    ui.valZ_2->setValue(lookFrom.z);
    ui.valVFov->setValue(vfov);


    dialog.show();
    dialog.exec();
}

void CameraMenu::close() {
}

void CameraMenu::modify() {
    auto camera = Controller::getInstance()->getSetUp()->getCamera();
    vec3 lookAt;
    vec3 lookFrom;
    float vfov;

    lookFrom.x = ui.valX->value();
    lookFrom.y = ui.valY->value();
    lookFrom.z = ui.valZ->value();

    lookAt.x = ui.valLookAtX->value();
    lookAt.y = ui.valLookAtY->value();
    lookAt.z = ui.valLookAtZ->value();

    vfov = ui.valVFov->value();

    camera->changeAttributeMappings(lookFrom, lookAt, vfov);

    Controller::getInstance()->getSetUp()->setCamera(camera);

}

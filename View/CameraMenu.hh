#pragma once

#include <QtWidgets>

#include "ui_camera.h"
#include "Controller.hh"

class CameraMenu : public QObject {
    Q_OBJECT
    Ui::dialogCamera ui;
    QDialog dialog;
public:
    CameraMenu();
public slots:
    void cameraMenuShow();
    void close();
    void modify();
};



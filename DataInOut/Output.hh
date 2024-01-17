#pragma once

#include <QImage>
#include <QFileDialog>
#include <QMessageBox>
class Output : public QObject {
    Q_OBJECT

   QImage image;
public:
    Output();
    void saveAnimation(std::vector<QImage> frames);

public slots:
    void setImage(QImage image);
    void saveImage();

};


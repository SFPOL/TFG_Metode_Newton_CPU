#include "Output.hh"

Output::Output()
{

}

void Output::setImage(QImage im) {
    image = im;
}

void Output::saveImage() {

    QMessageBox msgBox;
    QString path = QFileDialog::getSaveFileName(NULL, "Save as", "untitled.png", "PNG File(*.png);;");

    if (path.isNull()) msgBox.setText("Path not found.");

    if(image.save(path, "png"))
        msgBox.setText("Frame saved!");
    else msgBox.setText("Error saving image.");
    msgBox.exec();
}
void Output::saveAnimation(std::vector<QImage> frames) {

    QMessageBox msgBox;
    QString path = QFileDialog::getSaveFileName(NULL, "Save as", "frame", "PNG File(*.png);;");

    if (path.isNull()) msgBox.setText("Path not found.");


    QString resultName;
    for (unsigned long i=0; i<frames.size(); i++ ) {
        resultName = path.remove(".png");
        resultName = resultName +  QString::number(i) + ".png";
        frames[i].save(resultName, "png");

    }
    msgBox.setText("Animation saved!");
    msgBox.exec();

}



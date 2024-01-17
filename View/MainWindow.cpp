#include "MainWindow.hh"
#include <chrono>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Main)
{

    ui->setupUi(this);
    ui->ColorTop->setStyleSheet("QPushButton {background-color: #7fb2ff; color: blue;}");
    ui->ColorBottom->setStyleSheet("QPushButton {background-color: #ffffff; color: blue;}");
    outputFile = new Output();
    builder = new Builder();
    cameraMenu = new CameraMenu();

    ui->verticalLayout->insertWidget(0, &screen);

    // Connect with other Windows or Canvas
    QObject::connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    QObject::connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(aboutMenu()));

    QObject::connect(ui->buttonCamera, SIGNAL(clicked()), cameraMenu, SLOT(cameraMenuShow()));
    QObject::connect(ui->buttonTrace, SIGNAL(clicked()), this, SLOT(trace()));

    // Change Background Colors
    QObject::connect(ui->ColorTop, SIGNAL(clicked()), this, SLOT(setColorTop()));
    QObject::connect(ui->ColorBottom, SIGNAL(clicked()), this, SLOT(setColorBottom()));


    // Connect to Data Loaders and Generators
    // TO DO: Fase 1: Ampliar els menús per obtenir nous tipus d'objectes
    QObject::connect(ui->actionNew_Sphere, SIGNAL(triggered()), builder, SLOT(newPC()));
    QObject::connect(ui->actionNew_Cylinder, SIGNAL(triggered()), builder, SLOT(newCylinder()));
    QObject::connect(ui->actionNew_Box, SIGNAL(triggered()), builder, SLOT(newBox()));
    QObject::connect(ui->actionNew_Triangle, SIGNAL(triggered()), builder, SLOT(newTriangle()));
    QObject::connect(ui->actionNew_Mesh, SIGNAL(triggered()), builder, SLOT(newMesh()));
    QObject::connect(ui->actionGenerate_Scene, SIGNAL(triggered()), builder, SLOT(newSimulatedScene()));
    QObject::connect(ui->actionOpen_Virtual_Scene, SIGNAL(triggered()), builder, SLOT(newVirtualScene()));
    QObject::connect(ui->actionOpen_Data, SIGNAL(triggered()), builder, SLOT(newDataScene()));
    QObject::connect(ui->actionTracing_Settings, SIGNAL(triggered()), builder, SLOT(loadSettings()));

    // Connect to Image Savers
    QObject::connect(ui->actionSave,SIGNAL(triggered()), outputFile, SLOT(saveImage()));
    QObject::connect(ui->actionStart_and_Save_Animation, SIGNAL(triggered()), this, SLOT(runAnimation()));


    // Connect with different shaders
    // TO DO Fase 0: Connectar els menús ja existents amb els nous shadings
    QObject::connect(ui->actionColor_Shading, SIGNAL(triggered()), builder, SLOT(activaColorShading()));
    QObject::connect(ui->actionDepth_Shading, SIGNAL(triggered()), builder, SLOT(activaDepthShader()));
    QObject::connect(ui->actionNormal_Shading, SIGNAL(triggered()), builder, SLOT(activaNormalShader()));

    // TO DO Fase 2: connectar els shadings d'il·luminació Phong, Bling-Phong i d'altres

    QObject::connect(ui->actionCell_Shading, SIGNAL(triggered()), builder, SLOT(activaCell_Shading()));
    QObject::connect(ui->actionPhong_Shading, SIGNAL(triggered()), builder, SLOT(activaPhongShader()));
    QObject::connect(ui->actionBlinn_Phong, SIGNAL(triggered()), builder, SLOT(activaBlinn_Phong()));
    //    QObject::connect(ui->actionGooch_Shading, SIGNAL(triggered()), this, SLOT(activaGooch_Shading()));

    QObject::connect(builder, SIGNAL(settingsChanged()), this, SLOT(refreshWindow()));
}



MainWindow::~MainWindow()
{
    delete builder;
    delete outputFile;
    delete cameraMenu;
    delete ui;
}



void MainWindow::aboutMenu() {
    QDialog dialog;
    Ui::dialogAbout ui;
    ui.setupUi(&dialog);

    dialog.show();
    dialog.exec();
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}


void MainWindow::on_valWidth_valueChanged(int arg1)
{
    Q_UNUSED(arg1);

    auto camera = Controller::getInstance()->getSetUp()->getCamera();
    camera->viewportX = ui->valWidth->value();

}

void MainWindow::on_valHeight_valueChanged(int arg1)
{
    Q_UNUSED(arg1);

    auto camera = Controller::getInstance()->getSetUp()->getCamera();
    camera->viewportY = ui->valHeight->value();

}

void MainWindow::on_valMaxDepth_valueChanged(int arg1)
{
    Q_UNUSED(arg1);

    auto visSetUp = Controller::getInstance()->getSetUp();
    visSetUp->setMaxDepth(ui->valMaxDepth->value());

}

void MainWindow::on_valSamples_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    auto visSetUp = Controller::getInstance()->getSetUp();
    visSetUp->setSamples(ui->valSamples->value());
}

void MainWindow::on_valReflections_stateChanged()
{
    auto visSetUp = Controller::getInstance()->getSetUp();
    visSetUp->setReflections(ui->valReflections->isChecked());
}

void MainWindow::on_valRefractions_stateChanged()
{
    auto visSetUp = Controller::getInstance()->getSetUp();
    visSetUp->setRefractions(ui->valRefractions->isChecked());
}

void MainWindow::on_valTextures_stateChanged()
{
    auto visSetUp = Controller::getInstance()->getSetUp();
    visSetUp->setTextures(ui->valTextures->isChecked());
}

void MainWindow::on_valShadows_stateChanged()
{
    auto visSetUp = Controller::getInstance()->getSetUp();
    visSetUp->setShadows(ui->valShadows->isChecked());
}

void MainWindow::on_backGround_stateChanged()
{
    auto visSetUp = Controller::getInstance()->getSetUp();
    visSetUp->setBackground(ui->backGround->isChecked());
}


void MainWindow::refreshWindow() {
    auto visSetUp = Controller::getInstance()->getSetUp();

    ui->valReflections->setChecked(((bool)(visSetUp->getReflections())));
    ui->valRefractions->setChecked((bool)(visSetUp->getRefractions()));
    ui->valShadows->setChecked((bool)(visSetUp->getShadows()));
    ui->valTextures->setChecked((bool)(visSetUp->getTextures()));
    ui->valHeight->setValue((int)((visSetUp->getCamera())->viewportY));
    ui->valWidth->setValue((int)((visSetUp->getCamera())->viewportX));
    ui->valSamples->setValue((int) (visSetUp->getSamples()));
    ui->valMaxDepth->setValue((int) (visSetUp->getMAXDEPTH()));
    ui->backGround->setChecked(((bool)(visSetUp->getBackground())));
}

QImage MainWindow::getImage() { return image;}


void MainWindow::trace() {

    int width, height;

    // Camera parameters from the Controller
    auto camera = Controller::getInstance()->getSetUp()->getCamera();
    width = camera->viewportX;
    height = camera->viewportY;
    auto start = std::chrono::high_resolution_clock::now();

    image = QImage(width, height, QImage::Format_RGB888);

    Controller::getInstance()->rendering(&image);

    screen.setPixmap(QPixmap::fromImage(image));
    outputFile->setImage(image);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cerr << "Execution time:" << duration.count() << " microseconds" << std::endl;
}

void MainWindow::runAnimation() {

    int width, height;
    vector<QImage> frames;

    // Camera parameters from the Controller
    auto camera = Controller::getInstance()->getSetUp()->getCamera();
    width = camera->viewportX;
    height = camera->viewportY;

    // TODO: Canviar per incloure animacions diferents
    // Ara es crea aqui una escena amb una esfera que té animacions,
    // Caldria crear l'escena des de fitxer (opcionalment)
    // es crida el render i es guarden
    // tantes imatges com a frames s'han calculat (exemple amb MAXFRAMES = 5 a Animation.hh)

    Controller::getInstance()->createScene(MAXFRAMES);
    for (int i=0; i<MAXFRAMES; i++) {
        QImage *image = new QImage(width, height, QImage::Format_RGB888);
        Controller::getInstance()->update(i);
        Controller::getInstance()->rendering(image);
        frames.push_back(*image);
    }

    outputFile->saveAnimation(frames);
    frames.clear();
}


void MainWindow::setColorTop() {

    auto visSetUp = Controller::getInstance()->getSetUp();
    vec3 col = visSetUp->getTopBackground();
    QColor base = QColor(255*col[0], 255*col[1], 255*col[2]);
    QColor color = QColorDialog::getColor(base, this, "Pick a color TOP",  QColorDialog::DontUseNativeDialog);

    visSetUp->setTopBackground(vec3(color.red()/255.0, color.green()/255.0, color.blue()/255.0));

    QString colorButton = "QPushButton {background-color: "+color.name()+"; color: blue;}";
    ui->ColorTop->setStyleSheet(colorButton);

}

void MainWindow::setColorBottom() {
    auto visSetUp = Controller::getInstance()->getSetUp();
    vec3 col = visSetUp->getDownBackground();
    QColor base = QColor(255*col[0], 255*col[1], 255*col[2]);
    QColor color = QColorDialog::getColor(base, this, "Pick a color BOTTOM",  QColorDialog::DontUseNativeDialog);
    visSetUp->setDownBackground(vec3(color.red()/255.0, color.green()/255.0, color.blue()/255.0));

    QString colorButton = "QPushButton {background-color: "+color.name()+"; color: blue;}";
    ui->ColorBottom->setStyleSheet(colorButton);

}




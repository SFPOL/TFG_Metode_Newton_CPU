#include "Controller.hh"

Controller::Controller()
{
    scene = make_shared<Scene>();
    // Valors per defecte de visualSetup
    visualSetup = make_shared<SetUp>();
}


bool Controller::createScene(SceneFactory::SCENE_TYPES currentType, QString name) {
    shared_ptr<SceneFactory> sf = nullptr;
    switch (currentType)  {
    case SceneFactory::SCENE_TYPES::VIRTUALWORLD:
        sf = make_shared<SceneFactoryVirtual>();
        break;
    case SceneFactory::SCENE_TYPES::REALDATA:
        sf = make_shared<SceneFactoryData>();
        break;
    case SceneFactory::SCENE_TYPES::TEMPORALVW:
        // TO DO:  Afegir les factories de escenes temporals amb les animacions
        return false;
      break;
    default:
        break;
    }
    scene = sf->createScene(name);
    return (scene != nullptr);
}

bool Controller::createScene(vec3 position, float radius) {
    // Create a single Sphere
    scene = make_shared<Scene>();
    auto sphere = make_shared<Sphere>(position, radius, 1.0);
    sphere->setMaterial(make_shared<Lambertian>(vec3(0.5, 0.2, 0.7)));
    scene->objects.push_back(sphere);
    return true;
}

bool Controller::createSceneCylinder() {
    // Create a single Cylinder
    scene = make_shared<Scene>();
    auto cylinder = make_shared<Cylinder>(1.0);
    cylinder->setMaterial(make_shared<Lambertian>(vec3(0.5, 0.2, 0.7)));
    scene->objects.push_back(cylinder);
    return true;
}

bool Controller::createScenePC() {
    // Create a single Cylinder
    scene = make_shared<Scene>();
    auto pc = make_shared<PC>("/home/sfpol/Baixades/Gmail/1_0_0.ply");
    pc->setMaterial(make_shared<Lambertian>(vec3(0.5, 0.2, 0.7)));
    scene->objects.push_back(pc);
    return true;
}

bool Controller::createScene(vec3 a, vec3 b, vec3 c) {
    // Create a single Triangle
    scene = make_shared<Scene>();
    auto triangle = make_shared<Triangle>(a, b, c, 1.0);
    triangle->setMaterial(make_shared<Lambertian>(vec3(0.5, 0.2, 0.7)));
    scene->objects.push_back(triangle);
    return true;

}

bool Controller::createScene() {
    //TODO: Pots posar codi per a fer una escena amb multiples esferes

    return true;
}


bool Controller::createScene(vec3 v0, vec3 v1) {
    //Crea a single Box
    scene = make_shared<Scene>();
    auto box = make_shared<Box>(v0, v1, 1.0);
    box->setMaterial(make_shared<Lambertian>(vec3(0.5, 0.2, 0.7)));
    scene->objects.push_back(box);
    return true;
}

bool Controller::createSceneMesh(const QString &fileName) {
    //Crea a single Mesh
    scene = make_shared<Scene>();
    auto mesh = make_shared<Mesh>(fileName);
    mesh->setMaterial(make_shared<Lambertian>(vec3(0.5, 0.2, 0.7)));
    scene->objects.push_back(mesh);
    return true;
}

bool Controller::createScene(int nFrames) {

    // TO DO Fase 3 opcional: Codi exemple amb animacions però que es pot canviar
    // pel que creguis convenient

    auto sphere = make_shared<Sphere>(vec3(0, 0, -1), 0.5, 1.0);
    sphere->setMaterial(make_shared<Lambertian>(vec3(0.5, 0.2, 0.7)));

    shared_ptr<Animation> anim = make_shared<Animation>();
    anim->transf =  make_shared<TranslateTG>(vec3(0.2));
    sphere->addAnimation(anim);

    return true;
}
bool Controller::createSettings(QString name) {
    // Create Settings and set them to the scene
    visualSetup = make_shared<SetUp>();
    if (visualSetup->load(name) ) {
        visualSetup->print(0);
        return (true);
    }
    return false;
}

void Controller::rendering(QImage *image) {
    RayTracer *tracer = new RayTracer(image);
    tracer->run();
    delete tracer;
}


bool Controller::createShading(ShadingFactory::SHADING_TYPES t) {
    visualSetup->setShadingStrategy(ShadingFactory::getInstance().createShading(t));
    return visualSetup->getShadingStrategy()!=nullptr;

}

void Controller::update(int i) {
    scene->update(i);
}

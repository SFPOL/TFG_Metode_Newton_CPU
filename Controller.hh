#pragma once

#include <QString>

#include "Model/Modelling/SceneFactory.hh"
#include "Model/Modelling/SceneFactoryVirtual.hh"
#include "Model/Modelling/SceneFactoryData.hh"
#include "Model/Rendering/ShadingFactory.hh"

#include "Model/Rendering/SetUp.hh"
#include "Model/Rendering/RayTracer.hh"
#include "Model/Modelling/Objects/Sphere.hh"

using namespace std;

class Controller
{
private:
    shared_ptr<Scene>  scene;
    shared_ptr<SetUp>  visualSetup;

    static Controller* // Singleton

    // static pointer which will points to the instance of this class
    instancePtr;

    // Default constructor
    Controller();

public:

    // deleting copy constructor
    Controller(const Controller& obj)
        = delete;

    static Controller* getInstance() {
        // If there is no instance of class
        // then we can create an instance.
        if (instancePtr == NULL) {
          // We can access private members
          // within the class.
          instancePtr = new Controller();

          // returning the instance pointer
          return instancePtr;
        } else {
          // if instancePtr != NULL that means
          // the class already have an instance.
          // So, we are returning that instance
          // and not creating new one.
          return instancePtr;
        }
      }


    shared_ptr<Scene>  getScene() {return scene; }
    shared_ptr<SetUp>  getSetUp() {return visualSetup; }

    void setScene (shared_ptr<Scene> s) {  scene = s;}
    void setSetUp (shared_ptr<SetUp> v) {  visualSetup = v;}

    bool createScene(SceneFactory::SCENE_TYPES currentType, QString name);
    bool createScene(vec3 position, float radius);
    bool createScene(vec3 v0, vec3 v1);
    bool createScene(vec3 a, vec3 b, vec3 c);
    bool createSceneCylinder();
    bool createScenePC();
    bool createSceneMesh(const QString &fileName);
    bool createScene();
    bool createScene(int nFrames);

    bool createSettings(QString name);
    bool createShading(ShadingFactory::SHADING_TYPES t);

    void rendering(QImage *image);
    void update(int i);
};

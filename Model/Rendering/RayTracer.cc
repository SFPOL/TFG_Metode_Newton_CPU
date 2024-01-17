#include "RayTracer.hh"
#include "glm/gtc/random.hpp"


RayTracer::RayTracer(QImage *i):
    image(i) {

    setup = Controller::getInstance()->getSetUp();
    scene = Controller::getInstance()->getScene();
}


void RayTracer::run() {

    init();
    int numSamples = setup->getSamples();
    auto camera = setup->getCamera();
    int  width = camera->viewportX;
    int  height = camera->viewportY;

    for (int y = height-1; y >= 0; y--) {
        //std::cerr << "\rScanlines remaining: " << y << ' ' << std::flush;  // Progrés del càlcul
        for (int x = 0; x < width; x++) {

            vec3 color(0, 0, 0);

            for (int i=0; i<numSamples; i++){
                // Desviació del raig
                float du = glm::linearRand(-0.5f, 0.5f);
                float dv = glm::linearRand(-0.5f, 0.5f);

                float u = (float(x) + du) / float(width);
                float v = (float(height - y + dv)) / float(height);

                Ray r = camera->getRay(u, v);

                color += this->RayPixel(r, 0);
            }

            color *= float(1.0f/numSamples);
            color = glm::clamp(color, 0.0, 1.0);

            // TODO FASE 2: Gamma correction
            color = sqrt(color);

            color *= 255;
            setPixel(x, y, color);
        }
    }
}


void RayTracer::setPixel(int x, int y, vec3 color) {

    if (color.r < 0) color.r = 0;
    if (color.g < 0) color.g = 0;
    if (color.b < 0) color.b = 0;

    if (color.r > 255) color.r = 255;
    if (color.g > 255) color.g = 255;
    if (color.b > 255) color.b  = 255;

    image->setPixelColor(x, y, QColor(color.r, color.g, color.b));

}

/* Mètode RayPixel
** TODO: Cal modificar la funcio RayPixel es la funcio recursiva del RayTracing. Cal retocar-lo per:
** FASE 0 per a fer el degradat del fons
** FASES 0 i 1 per a cridar a la intersecció amb l'escena i posar el color de l'objecte (via color, depth o normal)
** FASE 2 per incloure els shading més realistes (Blinn-Phong  i ombres)
** FASE 2 per a tractar reflexions i transparències
**
*/

// Funcio recursiva que calcula el color.
vec3 RayTracer::RayPixel(Ray &ray, int depth) {

    vec3 color = vec3(0);
    vec3 unit_direction;
    HitInfo info;

    //Intersecció amb l'escena (0.01 per evitar acne)
    if (scene->hit(ray, 0.01, std::numeric_limits<float>::infinity(), info)) {
        Ray r_out;
        if (depth < setup->getMAXDEPTH() && info.mat_ptr->scatter(ray, info, color, r_out)) {
            color *= RayPixel(r_out, depth+1);
        }
        color += (vec3(1.0) - info.mat_ptr->Kt) *  setup->getShadingStrategy()->shading(scene, info, setup->getCamera()->getLookFrom(), setup->getLights(), setup->getGlobalLight());
    } else if (/*depth == 0 && */setup->getBackground()) {
        vec3 ray2 = normalize(ray.getDirection());

        //color = 0.5f * (ray2+vec3(1.0f));

        float t = 0.5f*(ray2.y+1);
        //Degradat blau a partir de variables de la interfície
        vec3 col0 = setup->getDownBackground();
        vec3 col1 = setup->getTopBackground();
        color = (1-t)*col0 + t*col1;

    } else if (depth > 0) {
        color += setup->getGlobalLight();
    }
    return color;
}


void RayTracer::init() {
    auto s = setup->getShadingStrategy();
    auto s_out = ShadingFactory::getInstance().switchShading(s, setup->getShadows());
    if (s_out!=nullptr) setup->setShadingStrategy(s_out);
}


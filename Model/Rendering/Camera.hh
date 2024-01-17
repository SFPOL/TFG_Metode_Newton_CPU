#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #define _USE_MATH_DEFINES
    #include <cmath>
#endif

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Model/Modelling/Ray.hh"
#include "DataInOut/Serializable.hh"

using namespace glm;


// Classe que codifica les propietats físiques de la càmera
class Camera : public Serializable {
public:
    Camera();
    Camera(vec3 lookfrom,
                vec3 lookat,
                vec3   vup,
                double vfov, // vertical field-of-view in degrees
                double aspect_ratio, double pixelsX, double aperture, bool defocus_blur);

    virtual ~Camera() {};
    void computeAtributes(vec3 lookfrom,
                           vec3 lookat,
                           vec3   vup,
                           double vfov, // vertical field-of-view in degrees
                           double aspect_ratio, double pixelsX,
                           bool defocus_blur, double lensRadius);

    Ray   getRay(float s, float t);

    vec3  getLookFrom() {return origin; }
    vec3  getLookAt() { return vrp; }
    float getAspectRatio() { return aspectRatio; }
    vec3  getVUP() { return v;}
    float getFOV()  {return vfov; }
    float getAperture() { return lens_radius * 2.0;}
    bool  getDefocusBlur() { return defocus_blur;}
    Ray   getBlurRay(float s, float t);

    void changeAttributeMappings(vec3 lookfrom,
                          vec3 lookat,
                          double vfov);

    static vec3 random_in_unit_disk() {
        vec3 p;
        do {
            p = 2.0f*vec3(double(rand())/RAND_MAX,double(rand())/RAND_MAX,0) - vec3(1,1,0);
        } while (dot(p,p) >= 1.0);
        return p;
    }

    virtual void read (const QJsonObject &json);
    virtual void write (QJsonObject &json) const;
    virtual void print(int indentation) const;

    // Viewport: mides del frame buffer
    int viewportX;
    int viewportY;

private:
    vec3 origin;
    vec3 vrp;
    float vfov;
    vec3 vup;
    // Window: lower_left_corner, horizontal, vertical
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    // Camera system coordinates: u, v, w
    vec3 u, v, w;
    bool defocus_blur;
    float lens_radius;
    float aspectRatio;
};




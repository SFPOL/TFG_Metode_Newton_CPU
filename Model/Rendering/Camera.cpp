#include "Camera.hh"

Camera::Camera() {
    viewportX = 500;
    viewportY = 250;
    vfov = 30.0;
    auto vup = vec3(0,1,0);
    auto lookat = vec3(0,0,0);
    auto lookfrom = vec3(2,2,2);
    defocus_blur = false;
    computeAtributes(lookfrom, lookat, vup, vfov, viewportX/viewportY, viewportX, false, 1.0);
}


Camera::Camera(
            vec3 lookfrom,
            vec3 lookat,
            vec3   vup,
            double vfov, // vertical field-of-view in degrees
            double aspect_ratio, double pixelsX, double aperture, bool defocus_blur)
{
    computeAtributes(lookfrom, lookat, vup, vfov, aspect_ratio, pixelsX, defocus_blur, aperture/2.0);
}

void Camera::changeAttributeMappings(vec3 lookfrom,
                              vec3 lookat,
                              double vfov) {
    computeAtributes(lookfrom, lookat, vup, vfov, viewportX/viewportY, viewportX, false, 1.0);
}

void Camera::computeAtributes(vec3 lookfrom,
                               vec3 lookat,
                               vec3   vup,
                               double vfov, // vertical field-of-view in degrees
                               double aspect_ratio, double pixelsX,
                               bool defocus_blur, double lensRadius)
{
    this->vfov = vfov;
    this->vup = vup;
    auto theta = (vfov)*M_PI/180.0;
    auto h = tan(theta/2);
    float window_height = 2.0 * h;
    float window_width = aspect_ratio * window_height;
    aspectRatio = aspect_ratio;
    lens_radius = lensRadius;

    w = normalize(lookfrom - lookat);
    u = normalize(cross(vup, w));
    v = cross(w, u);

    origin = lookfrom;
    vrp = lookat;

    horizontal = window_width * u;
    vertical = window_height * v;
    float half_width = window_width/2.0;
    float half_height = window_height/2.0;
    lower_left_corner = origin  - (half_width)*u -(half_height)*v - w;

    viewportX = pixelsX;
    viewportY = pixelsX/aspect_ratio;

    this->defocus_blur = defocus_blur;
    if (this->defocus_blur) {
        float focus_dist = glm::length(origin - lookat);
        lower_left_corner = origin  - half_width*focus_dist*u -half_height*focus_dist*v - focus_dist*w;
        horizontal = window_width*focus_dist*u;
        vertical = window_height*focus_dist*v;
    }
}

Ray Camera::getRay(float s, float t) {
    if (!defocus_blur)
        return Ray(origin, lower_left_corner + s*horizontal + t*vertical - origin);
    //Si s'ha de fer defocus blur, retornem el raig blur
    return getBlurRay(s, t);
}

Ray Camera::getBlurRay(float s, float t) {
    vec3 n_origin = origin;
    vec3 random_in_disk = lens_radius*random_in_unit_disk();
    n_origin = n_origin + u*random_in_disk.x + v*random_in_disk.y;
    return Ray(n_origin, lower_left_corner + s*horizontal + t*vertical - n_origin);
}


//! [0]
void Camera::read (const QJsonObject &json)
{
    vec3 vup;
    double vfov = 180.0;
    double aspectRatio = 1.0;
    if (json.contains("lookFrom") && json["lookFrom"].isArray()) {
        QJsonArray auxVec = json["lookFrom"].toArray();
        origin[0] = auxVec[0].toDouble();
        origin[1] = auxVec[1].toDouble();
        origin[2] = auxVec[2].toDouble();
    }
    if (json.contains("lookAt") && json["lookAt"].isArray()) {
        QJsonArray auxVec = json["lookAt"].toArray();
        vrp[0] = auxVec[0].toDouble();
        vrp[1] = auxVec[1].toDouble();
        vrp[2] = auxVec[2].toDouble();
    }
    if (json.contains("vup") && json["vup"].isArray()) {
        QJsonArray auxVec = json["vup"].toArray();
        vup[0] = auxVec[0].toDouble();
        vup[1] = auxVec[1].toDouble();
        vup[2] = auxVec[2].toDouble();
    }
    if (json.contains("vfov") && json["vfov"].isDouble())
        vfov = json["vfov"].toDouble();
    if (json.contains("aspectRatio") && json["aspectRatio"].isDouble())
        aspectRatio = json["aspectRatio"].toDouble();

    if (json.contains("pixelsX") && json["pixelsX"].isDouble())
        viewportX = (int)(json["pixelsX"].toDouble());

    if (json.contains("defocusBlur") && json["vfov"].isObject()) {
        QJsonObject object = json["defocusBlur"].toObject();
        if (object.contains("enabled") && object["enabled"].isBool()) {
             defocus_blur = object["enabled"].toBool();
            if (defocus_blur) {
                if (json.contains("aperture") && json["aperture"].isDouble())
                    lens_radius = json["aperture"].toDouble()/2.0;
            }
        }
    }
    computeAtributes(origin, vrp, vup, vfov, aspectRatio, viewportX, defocus_blur, lens_radius);
}
//! [0]

//! [1]
void Camera::write(QJsonObject &json) const
{
    QJsonArray auxArray;
    auxArray.append(origin[0]);auxArray.append(origin[1]);auxArray.append(origin[2]);
    json["lookFrom"] = auxArray;

    QJsonArray auxArray2;
    auxArray2.append(vrp[0]);auxArray2.append(vrp[1]);auxArray2.append(vrp[2]);
    json["lookAt"] = auxArray2;

    QJsonArray auxArray3;
    auxArray3.append(v[0]);auxArray3.append(v[1]);auxArray3.append(v[2]);
    json["vup"] = auxArray3;
    json["vfov"] = vfov;
    json["aspectRatio"] = aspectRatio;
    json["pixelsX"] = viewportX;

    QJsonObject obj;
    obj["enabled"] = defocus_blur;
    obj["aperture"] = lens_radius* 2.0;
    json["defocusBlur"] = obj;
}
//! [1]

void Camera::print(int indentation) const
{
    const QString indent(indentation * 2, ' ');

    QTextStream(stdout) << "Camera" << "\n";
    QTextStream(stdout) << indent << "lookFrom:\t" << origin[0] << ", "<< origin[1] << ", "<< origin[2] << "\n";
    QTextStream(stdout) << indent << "lookAt: \t" << vrp[0] << ", "<< vrp[1] << ", "<< vrp[2] << "\n";
    QTextStream(stdout) << indent << "vup: \t \t" << v[0] << ", "<< v[1] << ", "<< v[2] << "\n";

    QTextStream(stdout) << indent << "vfov: \t \t" << vfov << "\n";

    QTextStream(stdout) << indent << "aspectRatio:\t" << aspectRatio << "\n";
    QTextStream(stdout) << indent << "pixelsX:\t" << viewportX << "\n";
    QTextStream(stdout) << indent << "pixelsY:\t" << viewportY << "\n";

    QTextStream(stdout) << indent << "defocusBlur:\t"  << "\n";
    QTextStream(stdout) << indent+4 << "enabled:\t" << defocus_blur << "\n";
    QTextStream(stdout) << indent+4 << "aperture:\t" << lens_radius*2.0 << "\n";

}


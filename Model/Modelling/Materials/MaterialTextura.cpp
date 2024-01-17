#include "MaterialTextura.hh"

MaterialTextura::MaterialTextura(const vec3& color): Material()
{
    Kd = color;
}

MaterialTextura::MaterialTextura(const vec3& a, const vec3& d, const vec3& s, const vec3& t, const float k, const float s2):
    Material(a, d, s, t, k, s2) {
}

MaterialTextura::MaterialTextura(const vec3& a, const vec3& d, const vec3& s, const vec3& t, const float k, const float s2, const float o):
    Material(a, d, s, t, k, s2, o) {
}
MaterialTextura::~MaterialTextura()
{
}

bool MaterialTextura::scatter(const Ray& r_in, const HitInfo& rec, vec3& color, Ray & r_out) const  {
    return false;
}

void MaterialTextura::read (const QJsonObject &json){
    Material::read(json);
    if (json.contains("textureFile") && json["textureFile"].isString()){
        textura.setTexture(json["textureFile"].toString());
    }
}

vec3 MaterialTextura::getDiffuse(vec2 uv) const {
    return textura.getColorPixel(uv);

}

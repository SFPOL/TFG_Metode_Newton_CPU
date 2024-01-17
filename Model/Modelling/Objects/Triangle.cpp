#include "Triangle.hh"

Triangle::Triangle() {
    a = vec3(-1,0,0);
    b = vec3(1,0,0);
    c = vec3(0,1,0);
    normal = cross(b-a,c-a);
    pla = make_shared<Plane>(normal, a, 1.0);
}

Triangle::Triangle(vec3 a, vec3 b, vec3 c, float data) :Object(data) {
    this->a = a;
    this->b = b;
    this->c = c;
    normal = cross(b-a,c-a);
    pla = make_shared<Plane>(normal, a, 1.0);
}

Triangle::Triangle(float data) :Object(data) {
    a = vec3(-1,0,0);
    b = vec3(1,0,0);
    c = vec3(0,1,0);
    normal = cross(b-a,c-a);
    pla = make_shared<Plane>(normal, a, 1.0);
}

bool Triangle::hit(Ray &raig, float tmin, float tmax, HitInfo& info) const {
    HitInfo aux;
    if (pla->hit(raig, tmin, tmax, aux)) {
        float area_abc = calculArea(normal);
        float area_cap = calculArea(cross(a-c,aux.p-c));
        float area_abp = calculArea(cross(b-a,aux.p-a));
        float area_bcp = calculArea(cross(c-b,aux.p-b));
        float u = area_cap/area_abc;
        float v = area_abp/area_abc;
        float w = area_bcp/area_abc;

        if (glm::abs(u+v+w - 1) < 0.0001 && Hitable::strictlyBetween0And1(u) && Hitable::strictlyBetween0And1(v) && Hitable::strictlyBetween0And1(w)) {
            info = aux;
            info.mat_ptr = material.get();
            return true;
        }
    }
    return false;
}

void Triangle::read (const QJsonObject &json) {
    Object::read(json);

    if (json.contains("p1") && json["p1"].isArray()) {
        QJsonArray auxVec = json["p1"].toArray();
        a[0] = auxVec[0].toDouble();
        a[1] = auxVec[1].toDouble();
        a[2] = auxVec[2].toDouble();
    }
    if (json.contains("p2") && json["p2"].isArray()) {
        QJsonArray auxVec = json["p2"].toArray();
        b[0] = auxVec[0].toDouble();
        b[1] = auxVec[1].toDouble();
        b[2] = auxVec[2].toDouble();
    }
    if (json.contains("p3") && json["p3"].isArray()) {
        QJsonArray auxVec = json["p3"].toArray();
        c[0] = auxVec[0].toDouble();
        c[1] = auxVec[1].toDouble();
        c[2] = auxVec[2].toDouble();
    }
    /*
    return;
    if (json.contains("p3") && json["p3"].isArray()) {
        QJsonArray auxVec = json["p3"].toArray();
        c[0] = auxVec[0].toDouble();
        c[1] = auxVec[1].toDouble();
        c[2] = auxVec[2].toDouble();
    }
    */
}

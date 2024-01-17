#include "FittedPlane.hh"
#include <iostream>

//Constructor per defecte
FittedPlane::FittedPlane()
{
    this->normal = vec3(0.0, 1.0, 0.0);
    this->point = vec3(0.0);
    this->pmin = vec3(-5.0, 0.0, -5.0);
    this->pmax = vec3(-5.0, 0.0, -5.0);
}

//Constructor amb atributs
FittedPlane::FittedPlane(vec3 normal, vec3 pass_point, vec3 pmin, vec3 pmax, float v) : Object(v){
    this->normal = normalize(normal);
    this->point = pass_point;
    this->pmin = pmin;
    this->pmax = pmax;
}

//Constructor amb D de pla
FittedPlane::FittedPlane(vec3 normal, float d, float v) : Object(v) {
    normal  = normalize(normal);
    this->normal = normal;
    if (abs(normal.z)>DBL_EPSILON)
        this->point = vec3(0.0, 0.0, -d/normal.z);
    else if (abs(normal.y)>DBL_EPSILON)
        this->point = vec3(0.0, -d/normal.y, 0.0);
    else
        this->point = vec3(-d/normal.x, 0.0, 0.0);
}

//Constructor nomes amb data
FittedPlane::FittedPlane(float data) :Object(data) {
    this->normal = vec3(0.0, 1.0, 0.0);
    this->point = vec3(0.0);
    this->pmin = vec3(-5.0, 0.0, -5.0);
    this->pmax = vec3(-5.0, 0.0, -5.0);
}

bool FittedPlane::hit(Ray &raig, float tmin, float tmax, HitInfo &info) const{
    // Comprovem interseccio entre el pla i el raig

    // Comprovem si el normal al pla i el raig son ortogonals.
    // En aquest cas son paralels i no hi ha interseccio

    if(abs(dot(raig.getDirection(), normal))<DBL_EPSILON){
        return false;
    }

    // En els altres casos hi haurà interseccio (si estem en el rang de min/max).
    // Cal omplir la informació del hit.

    // PLA: Ax+By+Cz+D=0
    // on A,B,C = normal

    // 1) Calculem la D = -Ax-By-Cz
    double d = -normal[0]*point[0] - normal[1]*point[1] - normal[2]*point[2];

    // 2) Imposem que la recta p+tv compleixi l'eq del pla
    // A(p1 + tv1) + ... + D = 0

    // Aillem la t
    vec3 rp = raig.getOrigin();
    vec3 vp = raig.getDirection();

    double temp =  -normal[0]*rp[0] - normal[1]*rp[1] - normal[2]*rp[2] - d;

    temp/= normal[0]*vp[0] + normal[1]*vp[1] + normal[2]*vp[2];

    if (temp > tmax || temp < tmin) {
            return false;
    }

    //Ara també ha de complir estar dins de pmin pmax al ser pla afitat
    vec3 punt = raig.pointAtParameter(temp);

    if (this->pmin.x - 0.0001 > punt.x || this->pmax.x + 0.0001 < punt.x ||
        this->pmin.y - 0.0001 > punt.y || this->pmax.y + 0.0001 < punt.y ||
        this->pmin.z - 0.0001 > punt.z || this->pmax.z + 0.0001 < punt.z) {
        return false;
    }

    // Omplim el camp de info:
    info.t = temp;
    info.p = punt;

    // La normal a un pla es la mateixa per tots els punts
    info.normal = normal;
    info.mat_ptr = material.get();
    float h = this->pmax.x - this->pmin.x;
    float w = this->pmax.z - this->pmin.z;
    vec3 ppmin = punt - this->pmin;
    info.uv = vec2(ppmin.x/h, ppmin.z/w);
    return true;
}


void FittedPlane::aplicaTG(shared_ptr<TG> t) {
    if (dynamic_pointer_cast<shared_ptr<TranslateTG>>(t)) {

        // Nomes movem el punt de pas
        vec4 newp(this->point, 1.0);
        newp = t->getTG() * newp;
        this->point.x = newp.x;
        this->point.y = newp.y;
        this->point.z = newp.z;
    }
}

void FittedPlane::read (const QJsonObject &json)
{


    Object::read(json);

    if (json.contains("point") && json["point"].isArray()) {
        QJsonArray auxVec = json["point"].toArray();
        point[0] = auxVec[0].toDouble();
        point[1] = auxVec[1].toDouble();
        point[2] = auxVec[2].toDouble();
    }
    if (json.contains("normal") && json["normal"].isArray()) {
        QJsonArray auxVec = json["normal"].toArray();
        normal[0] = auxVec[0].toDouble();
        normal[1] = auxVec[1].toDouble();
        normal[2] = auxVec[2].toDouble();
    }

    //calculem la y de pmin i pmax usant lequacio del pla, aillem D primer
    float d = -normal[0]*point[0] - normal[1]*point[1] - normal[2]*point[2];

    if (json.contains("pmin") && json["pmin"].isArray()) {
        QJsonArray auxVec = json["pmin"].toArray();
        pmin[0] = auxVec[0].toDouble();
        pmin[1] = (-normal[0]*pmin[0] - normal[2]*pmin[2] - d)/normal[1];
        pmin[2] = auxVec[1].toDouble();
    }
    if (json.contains("pmax") && json["pmax"].isArray()) {
        QJsonArray auxVec = json["pmax"].toArray();
        pmax[0] = auxVec[0].toDouble();
        pmax[1] = (-normal[0]*pmax[0] - normal[2]*pmax[2] - d)/normal[1];
        pmax[2] = auxVec[1].toDouble();
    }
}


//! [1]
void FittedPlane::write(QJsonObject &json) const
{
    Object::write(json);

    QJsonArray auxArray;
    auxArray.append(normal[0]);auxArray.append(normal[1]);auxArray.append(normal[2]);
    json["normal"] = auxArray;

    QJsonArray auxArray2;
    auxArray2.append(point[0]);auxArray2.append(point[1]);auxArray2.append(point[2]);
    json["normal"] = auxArray2;
}
//! [1]

void FittedPlane::print(int indentation) const
{
    Object::print(indentation);
    const QString indent(indentation * 2, ' ');

    QTextStream(stdout) << indent << "point:\t" << point[0] << ", "<< point[1] << ", "<< point[2] << "\n";
    QTextStream(stdout) << indent << "normal:\t" << normal[0] << ", "<< normal[1] << ", "<< normal[2] << "\n";

}

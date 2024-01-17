#include "Cylinder.hh"

Cylinder::Cylinder() {
    center = vec3(0.0,0.0,0.0);
    radius = 1.0;
}

Cylinder::Cylinder(vec3 cen, float r, float h, vec3 ax, float data) :Object(data) {
    center = cen;
    radius = r;
    height = h;
    axis = normalize(ax);
}

Cylinder::Cylinder(float data) :Object(data) {
    center = vec3(0,-1,0);
    radius = 1.0f;
    height = 2.0f;
    axis = vec3(0,1,0);
}

bool Cylinder::hit(Ray &raig, float tmin, float tmax, HitInfo& info) const {
    vec3 pc = raig.getOrigin() - center;
    vec3 v = raig.getDirection();
    float tParet = tmax, tTapa = tmax, temp, sign, h;

    // Càlculs previs
    float aXpc = dot(axis,pc), aXv = dot(axis,v);
    float r2 = radius*radius;
    float con = int(aXv<0), Ncon = 1 - con;

    // Mirem si dona a la PARET del cilindre
    float a = dot(v,v) - aXv*aXv;
    float b = dot(pc,v) - aXpc*aXv;
    float c = dot(pc,pc) - aXpc*aXpc - r2;
    float disc = b*b - a*c;
    if (disc > 0) {
        temp = (-b - sqrt(disc))/a;
        h = aXpc + temp * aXv;
        if (temp < tmax && temp > tmin && 0 <= h && h <= height) {
            tParet = temp;
        }else{
            temp = (-b + sqrt(disc)) / a;
            h = aXpc + temp * aXv;
            if (temp < tmax && temp > tmin && 0 <= h && h <= height) {
                tParet = temp;
            }
        }
    }

    // Mirem si li dona a les TAPES del cilindre
    if(aXv!=0){
        temp = (con*height-aXpc)/aXv;
        vec3 pP = pc+temp*v;
        if(temp < tmax && temp > tmin && dot(pP,pP) - dot(axis,pP)*dot(axis,pP)<r2){
            tTapa = temp;
            sign = con-Ncon;
        }else{
            temp = (Ncon*height-aXpc)/aXv;
            vec3 pP = pc+temp*v;
            if(temp < tmax && temp > tmin && dot(pP,pP) - dot(axis,pP)*dot(axis,pP)<r2){
                tTapa = temp;
                sign = Ncon-con;
            }
        }
    }

    // Si no li ha donat ni a la paret ni a la tapa, això signfica que
    // no li ha donat al cilindre
    if(tParet == tmax && tTapa == tmax){
        return false;
    }

    //Ara anem a veure si li dona abans a la tapa o a la paret
    if(tTapa<tParet){
        // Li ha donat a la TAPA
        info.t = tTapa;
        info.p = raig.pointAtParameter(info.t);
        info.normal = sign*axis;
        info.mat_ptr = material.get();
        // TODO Fase 3: Cal calcular les coordenades de textura

        return true;
    }else{
        // Li ha donat a la PARET
        info.t = tParet;
        info.p = raig.pointAtParameter(info.t);
        info.normal = (info.p - center - h*axis) / radius;
        info.mat_ptr = material.get();
        // TODO Fase 3: Cal calcular les coordenades de textura

        return true;
    }
    return false;
}


void Cylinder::aplicaTG(shared_ptr<TG> t) {
    if (dynamic_pointer_cast<TranslateTG>(t)) {
        // Per ara només es fan les translacions
        vec4 c(center, 1.0);
        c = t->getTG() * c;
        center.x = c.x; center.y = c.y; center.z = c.z;
    }
    //TODO: Cal ampliar-lo per a acceptar Escalats

}

void Cylinder::read (const QJsonObject &json)
{
    Object::read(json);

    if (json.contains("radius") && json["radius"].isDouble()) {
        radius = json["radius"].toDouble();
    }
    if (json.contains("h") && json["h"].isDouble()) {
        height = json["h"].toDouble();
    }
    if (json.contains("center") && json["center"].isArray()) {
        QJsonArray auxVec = json["center"].toArray();
        center[0] = auxVec[0].toDouble();
        center[1] = auxVec[1].toDouble();
        center[2] = auxVec[2].toDouble();
    }
    if (json.contains("axis") && json["axis"].isArray()) {
        QJsonArray auxVec = json["axis"].toArray();
        axis[0] = auxVec[0].toDouble();
        axis[1] = auxVec[1].toDouble();
        axis[2] = auxVec[2].toDouble();
        axis = normalize(axis);
    }
}


//! [1]
void Cylinder::write(QJsonObject &json) const
{
    Object::write(json);

    QJsonArray auxArrayCenter;
    auxArrayCenter.append(center[0]);auxArrayCenter.append(center[1]);auxArrayCenter.append(center[2]);
    json["center"] = auxArrayCenter;

    json["radius"] = radius;

    QJsonArray auxArrayAxis;
    auxArrayAxis.append(axis[0]);auxArrayAxis.append(axis[1]);auxArrayAxis.append(axis[2]);
    json["axis"] = auxArrayAxis;

    json["h"] = height;
}
//! [1]

void Cylinder::print(int indentation) const
{
    Object::print(indentation);

    const QString indent(indentation * 2, ' ');

    QTextStream(stdout) << indent << "center:\t" << center[0] << ", "<< center[1] << ", "<< center[2] << "\n";
    QTextStream(stdout) << indent << "radius:\t" << radius<< "\n";
    QTextStream(stdout) << indent << "height:\t" << height<< "\n";
    QTextStream(stdout) << indent << "axis:\t" << axis[0] << ", "<< axis[1] << ", "<< axis[2] << "\n";
}


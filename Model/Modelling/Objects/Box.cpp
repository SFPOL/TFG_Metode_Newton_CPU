#include "Box.hh"

Box::Box() {
    v0 = vec3(-0.5, 0.0,-0.5);
    v1 = vec3( 0.5, 1.0, 0.5);
}

Box::Box(vec3 v, vec3 u, float data):Object(data) {
    v0 =  v;
    v1 = u;
}

Box::Box(float data):Object(data) {
    v0 = vec3(0.0,0.0,0.0);
    v1 = vec3(1.0,1.0,1.0);
}

bool Box::hit(Ray &ray, float tmin, float tmax, HitInfo& info) const {
    vec3 d = ray.getDirection();
    vec3 p = ray.getOrigin();

    const float epsilon = 0.0001;

    float t_min_x, t_min_y, t_min_z, t_max_x,  t_max_y, t_max_z, t_enter, t_exit;
    t_min_x = (v0.x - p.x) / d.x;
    t_min_y = (v0.y - p.y) / d.y;
    t_min_z = (v0.z - p.z) / d.z;
    t_max_x = (v1.x - p.x) / d.x;
    t_max_y = (v1.y - p.y) / d.y;
    t_max_z = (v1.z - p.z) / d.z;

    float t_enter_x = glm::min(t_min_x, t_max_x);
    float t_enter_y = glm::min(t_min_y, t_max_y);
    float t_enter_z = glm::min(t_min_z, t_max_z);
    float t_exit_x = glm::max(t_min_x, t_max_x);
    float t_exit_y = glm::max(t_min_y, t_max_y);
    float t_exit_z = glm::max(t_min_z, t_max_z);

    t_enter = glm::max((glm::max(t_enter_x, t_enter_y)), t_enter_z);
    t_exit = glm::min((glm::min(t_exit_x, t_exit_y)), t_exit_z);

    if (t_enter >= t_exit) return false; // No hit

    // Store info
    if (t_enter < tmin || t_enter > tmax) {
        return false;
    }

    info.t = t_enter;
    info.p = ray.pointAtParameter(info.t);
    vec3 normal = vec3(0.0);

    // Calculate normal
    for (int i = 0; i < 3; i++) {
        if (glm::abs(info.p[i] - v0[i]) < epsilon) {
             normal[i] = -1.0;
        }
        else if (glm::abs(info.p[i] - v1[i]) < epsilon) {
            normal[i] = 1.0;
        }
    }

    // Normalize the normal vector
    info.normal = glm::normalize(normal);
    info.mat_ptr = material.get();

    return true;
}


void Box::aplicaTG(shared_ptr<TG> t) {
    if (dynamic_pointer_cast<TranslateTG>(t)) {
        vec4 vx0(v0, 1.0);
        vx0 = t->getTG() * vx0;
        v0.x = vx0.x; v0.y = vx0.y; v0.z = vx0.z;

        vec4 vx1(v1, 1.0);
        vx1 = t->getTG() * vx1;
        v1.x = vx1.x; v1.y = vx1.y; v1.z = vx1.z;
    }
    else if (dynamic_pointer_cast<ScaleTG>(t)) {
        vec4 vx1(v1, 1.0);
        glm::mat4 mat = t->getTG();
        vx1 = mat * vx1;
        v1.x = vx1.x; v1.y = vx1.y; v1.z = vx1.z;
    }
}

void Box::read (const QJsonObject &json)
{
    Object::read(json);

    if (json.contains("vertex0") && json["vertex0"].isArray()) {
        QJsonArray auxVec = json["vertex0"].toArray();
        v0[0] = auxVec[0].toDouble();
        v0[1] = auxVec[1].toDouble();
        v0[2] = auxVec[2].toDouble();
    }
    if (json.contains("vertex1") && json["vertex1"].isArray()) {
        QJsonArray auxVec = json["vertex1"].toArray();
        v1[0] = auxVec[0].toDouble();
        v1[1] = auxVec[1].toDouble();
        v1[2] = auxVec[2].toDouble();
    }

}


//! [1]
void Box::write(QJsonObject &json) const
{
    Object::write(json);

    QJsonArray auxArray;
    auxArray.append(v0[0]);auxArray.append(v0[1]);auxArray.append(v0[2]);
    json["vertex0"] = auxArray;

    auxArray.removeAt(0);
    auxArray.removeAt(1);
    auxArray.removeAt(2);

    auxArray.append(v1[0]);auxArray.append(v1[1]);auxArray.append(v1[2]);
    json["vertex1"] = auxArray;

}
//! [1]

void Box::print(int indentation) const
{
    Object::print(indentation);

    const QString indent(indentation * 2, ' ');

    QTextStream(stdout) << indent << "vertex0:\t" << v0[0] << ", "<< v0[1] << ", "<< v0[2] << "\n";
    QTextStream(stdout) << indent << "vertex1:\t" << v1[0] << ", "<< v1[1] << ", "<< v1[2] << "\n";
}

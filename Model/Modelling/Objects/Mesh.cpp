#include <QVector>
#include <QVector3D>
#include <cmath>
#include <vector>


#include "Mesh.hh"

Mesh::Mesh(): Object()
{}

Mesh::Mesh(const QString &fileName): Object()
{
    nom = fileName;
    load(fileName);
}

Mesh::Mesh(const QString &fileName, float data): Object(data)
{
    nom = fileName;
    load(fileName);
}

Mesh::~Mesh() {
    if (cares.size() > 0) cares.clear();
    if (vertexs.size() > 0) vertexs.clear();

}

void Mesh::makeTriangles() {
    // TO DO Fase 1: A implementar
    for(Face cara : cares){
        int idx1 = cara.idxVertices[0];
        int idx2 = cara.idxVertices[1];
        int idx3 = cara.idxVertices[2];

        vec3 vertex1 = vec3(vertexs[idx1][0], vertexs[idx1][1], vertexs[idx1][2]);
        vec3 vertex2 = vec3(vertexs[idx2][0], vertexs[idx2][1], vertexs[idx2][2]);
        vec3 vertex3 = vec3(vertexs[idx3][0], vertexs[idx3][1], vertexs[idx3][2]);


        triangles.push_back(make_shared<Triangle>(vertex1, vertex2, vertex3, 1.0));

    }

    return;
}

void Mesh::boundingSphere(){
    vec3 center = vec3(0.0, 0.0, 0.0);
    vec3 sum = vec3(0.0, 0.0, 0.0);
    int comp = 0;

    for(auto vertex : vertexs){
        vec3 vertex3dim = vec3(vertex[0], vertex[1], vertex[2]);
        sum += vertex3dim;
        comp++;
    }
    center=vec3(sum[0]/comp, sum[1]/comp, sum[2]/comp);

    float distMax = -1;
    for(auto vertex : vertexs){
        float dist = sqrt(pow(center[0]-vertex[0], 2) + pow(center[1]-vertex[1], 2) + pow(center[2]-vertex[2], 2));
        if (distMax < dist){
               distMax =  dist;
        }
    }
    minSphere = make_shared<Sphere>(center, distMax, 1.0);

    return;

}

void Mesh::boundingBox(){
    float minX = vertexs[0][0];
    float minY = vertexs[0][1];
    float minZ = vertexs[0][2];

    float maxX = vertexs[0][0];
    float maxY = vertexs[0][1];
    float maxZ = vertexs[0][2];

    for(vec4 vertex : vertexs){
        if(minX > vertex[0]){
            minX = vertex[0];
        }
        if(minY > vertex[1]){
            minY = vertex[1];
        }
        if(minZ > vertex[2]){
            minZ = vertex[2];
        }
        if(maxX < vertex[0]){
            maxX = vertex[0];
        }
        if(maxY > vertex[1]){
            maxY = vertex[1];
        }
        if(maxZ > vertex[2]){
            maxZ = vertex[2];
        }
    }
    vec3 vertMin = vec3(minX, minY, minZ);
    vec3 vertMax = vec3(maxX, maxY, maxZ);

    minBox = make_shared<Box>(vertMin, vertMax, 1.0);

    return;

}


bool Mesh::hit(Ray &raig, float tmin, float tmax, HitInfo& info) const {
    // TODO Fase 1: A implementar

    bool ret = false;
    //HitInfo infoAux =;

    if(this->minBox->hit(raig, tmin, tmax, info) || this->minSphere->hit(raig, tmin, tmax, info)){
        float t = tmax;

        for (std::shared_ptr<Triangle> triangle: triangles) {
            if(triangle->hit(raig, tmin, t, info)){
                t = info.t;
                ret = true;
            }
        }
    }

    if(ret) {
        info.mat_ptr = material.get();
    }

    return ret;

}


void Mesh::aplicaTG(shared_ptr<TG> t) {
    // TO DO: Fase 1
}

void Mesh::load (QString fileName) {
    bool entraBool = false;
    QFile file(fileName);
    if(file.exists()) {
        if(file.open(QFile::ReadOnly | QFile::Text)) {
            while(!file.atEnd()) {
                QString line = file.readLine().trimmed();
                QStringList lineParts = line.split(QRegularExpression("\\s+"));
                if(lineParts.count() > 0) {
                    // if it’s a comment
                    if(lineParts.at(0).compare("#", Qt::CaseInsensitive) == 0)
                    {
                        // qDebug() << line.remove(0, 1).trimmed();
                    }

                    // if it’s a vertex position (v)
                    else if(lineParts.at(0).compare("v", Qt::CaseInsensitive) == 0)
                    {
                        vertexs.push_back(vec4(lineParts.at(1).toFloat(),
                                               lineParts.at(2).toFloat(),
                                               lineParts.at(3).toFloat(), 1.0f));
                    }

                    // if it’s a normal (vn)
                    else if(lineParts.at(0).compare("vn", Qt::CaseInsensitive) == 0)
                    {

                    }
                    // if it’s a texture (vt)
                    else if(lineParts.at(0).compare("vt", Qt::CaseInsensitive) == 0)
                    {

                    }

                    // if it’s face data (f)
                    // there’s an assumption here that faces are all triangles
                    else if(lineParts.at(0).compare("f", Qt::CaseInsensitive) == 0)
                    {
                        Face *face = new Face();

                        // get points from v array
                        face->idxVertices.push_back(lineParts.at(1).split("/").at(0).toInt() - 1);
                        face->idxVertices.push_back(lineParts.at(2).split("/").at(0).toInt() - 1);
                        face->idxVertices.push_back(lineParts.at(3).split("/").at(0).toInt() - 1);

                        cares.push_back(*face);
                        entraBool = true;
                    }
                }
            }
            file.close();
            if(entraBool){
                makeTriangles();

            }
            boundingBox();
            boundingSphere();

        } else {
            qWarning("Boundary object file can not be opened.");
        }
    } else  qWarning("Boundary object file not found.");
}

void Mesh::read (const QJsonObject &json)
{
    Object::read(json);
    if (json.contains("objFileName") && json["objFileName"].isString()) {
        nom = json["objFileName"].toString();
        load(nom);
    }
}


//! [1]
void Mesh::write(QJsonObject &json) const
{
    Object::write(json);
    json["objFileName"] = nom;
}
//! [1]

void Mesh::print(int indentation) const
{
    Object::print(indentation);
    const QString indent(indentation * 2, ' ');
    QTextStream(stdout) << indent << "objFileName:\t" << nom << "\n";
}




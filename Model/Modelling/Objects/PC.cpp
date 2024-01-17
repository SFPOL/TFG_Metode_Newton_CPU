#include <QVector>
#include <QVector3D>
#include <cmath>
#include <vector>

#include "PC.hh"

PC::PC(): Object()
{
    this->nrOfVertices = 0;
    float size = 1;
    for (int z = -size; z <= size; z++) {
        for (int y = -size; y <= size; y++) {
            for (int x = -size; x <= size; x++) {
                int parity = (z + y + x) % 4;
                if (parity != 0) {
                    Point point;
                    point.position = vec3(x, y, z);
                    point.color = vec3(x, y, z);
                    point.normal = normalize(vec3(x, y, z));
                    this->vertices.push_back(point);
                    this->nrOfVertices++;
                }
            }
        }
    }
    vec3 vertMin = vec3(-size);
    vec3 vertMax = vec3(size);

    this->minBox = make_shared<Box>(vertMin, vertMax, 1.0);
    this->minBox->setMaterial(make_shared<Lambertian>(vec3(0.5, 0.2, 0.7)));

}

PC::PC(const QString &fileName): Object()
{
    //nom = fileName;
    load(fileName);
}

PC::PC(const QString &fileName, float data): Object(data)
{
    //nom = fileName;
    load(fileName);
}

PC::~PC() {
    //if (cares.size() > 0) cares.clear();
    //if (vertexs.size() > 0) vertexs.clear();

}

/*void PC::makeTriangles() {
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

void PC::boundingSphere(){
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

void PC::boundingBox(){
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

}*/


bool PC::hit(Ray &raig, float tmin, float tmax, HitInfo& info) const {

    if(this->minBox->hit(raig, tmin, tmax, info)){
        vec3 q = raig.getOrigin();
        vec3 v = raig.getDirection();
        vec3 vnor = normalize(v);

        int npoints = 0;
        std::vector<Point> verticesCone;
        for (Point pi: vertices) {
            vec3 piq = pi.position-q;
            if(dot(piq,vnor) > length(piq) * epsilon){
                verticesCone.push_back(pi);
                npoints++;
            }
        }
        if(npoints==0){
            return false;
        }


        float t = tmin;
        vec3 x = raig.pointAtParameter(t);
        int niter = 0;
        while(true){
            niter++;
            Point p;
            this->weightedAvgGauss(x, verticesCone, p);
            vec3 n = p.normal;
            if (glm::abs(dot(n, x-p.position))<tol){
                info.t = t; //dot(p.position-q,v)/dot(v,v);
                info.p = x; //q+info.t*v;
                info.normal = n;
                info.mat_ptr = material.get();
                std::cout << niter << endl;
                return true;
            }
            else {
                t = dot(p.position-q,n)/dot(v,n);
                if(t<tmin || t>tmax){
                    return false;
                }else{
                    x = raig.pointAtParameter(t);
                }
            }
        }
    }
    return false;

}

bool PC::weightedAvg(vec3 p, std::vector<Point> verticesCone, Point &point) const{
    vec3 position = vec3(0);
    vec3 color = vec3(0);
    vec3 normal = vec3(0);
    vec3 labels = vec3(0);

    float d = 0;

    for (Point pi: verticesCone) {
        // This if is for non-octree implementation and it's provisional
        // When doing octree, we will only accept the point on the leaf.
        vec3 vec = pi.position - p;
        float wnorm = 1/(length(vec)+0.001);
        position += wnorm*pi.position;
        color += wnorm*pi.color;
        normal += wnorm*pi.normal;
        labels += wnorm*pi.labels;
        d += wnorm;
    }
    point.position = (1/d)*position;
    point.color = (1/d)*color;
    point.normal = normalize(normal);
    point.labels = (1/d)*labels;
    return true;
}


bool PC::weightedAvgGauss(vec3 p, std::vector<Point> verticesCone, Point &point) const{
    vec3 position = vec3(0);
    vec3 color = vec3(0);
    vec3 normal = vec3(0);
    vec3 labels = vec3(0);

    float d = 0;

    for (Point pi: verticesCone) {
        // This if is for non-octree implementation and it's provisional
        // When doing octree, we will only accept the point on the leaf.
        vec3 vec = pi.position - p;
        float wnorm = exp(-pow(length(vec),2));
        position += wnorm*pi.position;
        color += wnorm*pi.color;
        normal += wnorm*pi.normal;
        labels += wnorm*pi.labels;
        d += wnorm;
    }
    point.position = (1/d)*position;
    point.color = (1/d)*color;
    point.normal = normalize(normal);
    point.labels = (1/d)*labels;
    return true;
}


void PC::aplicaTG(shared_ptr<TG> t) {
    // TO DO: Fase 1
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
}

void PC::loadFromBigEndianPLYFile(string filePath) {
        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open())
        {
            std::cerr << "Error: Could not open PLY file: " << filePath << std::endl;
            return;
        }

        int numVertices = -1;
        vec3 minVertex;
        vec3 maxVertex;

        std::unordered_map<double, int> lCounts; // Diccionario para contar los valores de "l"
        std::unordered_map<double, int> pCounts; // Diccionario para contar los valores de "p"

        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string type;
            iss >> type;

            if (type == "element")
            {
                std::string elementType;
                int count;
                iss >> elementType >> count;
                if (elementType == "data_visual")
                {
                    numVertices = count;
                }
            }
            else if (type == "end_header")
            {
                if (numVertices == -1)
                {
                    std::cerr << "Error: Invalid PLY file format." << std::endl;
                    return;
                }

                vertices.resize(numVertices);
                file.seekg(0, std::ios_base::cur);
                for (int i = 0; i < numVertices; i++)
                {
                    file.seekg(0, std::ios_base::cur);

                    double l, x, y, z, p;
                    file.read(reinterpret_cast<char*>(&l), sizeof(double));
                    file.read(reinterpret_cast<char*>(&x), sizeof(double));
                    file.read(reinterpret_cast<char*>(&y), sizeof(double));
                    file.read(reinterpret_cast<char*>(&z), sizeof(double));
                    file.read(reinterpret_cast<char*>(&p), sizeof(double));

                    if (!isBigEndian())
                    {
                        swapBytes(reinterpret_cast<char*>(&l), sizeof(double));
                        swapBytes(reinterpret_cast<char*>(&x), sizeof(double));
                        swapBytes(reinterpret_cast<char*>(&y), sizeof(double));
                        swapBytes(reinterpret_cast<char*>(&z), sizeof(double));
                        swapBytes(reinterpret_cast<char*>(&p), sizeof(double));
                    }

                    glm::vec3 vertex(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));

                    if (i == 0)
                    {
                        minVertex = maxVertex = vertex;
                    }
                    else
                    {
                        minVertex = glm::min(minVertex, vertex);
                        maxVertex = glm::max(maxVertex, vertex);
                    }

                    Point point;
                    point.position = vertex;
                    point.labels = glm::vec3(l, p, l==p);
                    if (p != l)
                    {
                        //point.color = getColor(0);
                    }
                    else
                    {
                        //point.color = getColor(1);
                    }
                    point.color = getColor(l);//glm::vec3(1.f, 0.f, 0.f);
                    point.normal = getColor(p);//glm::vec3(1.f, 0.f, 0.f);
                    std::cout << point.position << ", " << point.normal << ", " << point.color << ", " << point.labels << std::endl;
                    vertices[i] = point;

                    //file.seekg(sizeof(double), std::ios_base::cur);

                    // Contar los valores distintos de "l"
                    if (lCounts.find(l) != lCounts.end())
                    {
                        lCounts[l]++;
                    }
                    else
                    {
                        lCounts[l] = 1;
                    }

                    // Contar los valores distintos de "p"
                    if (pCounts.find(p) != pCounts.end())
                    {
                        pCounts[p]++;
                    }
                    else
                    {
                        pCounts[p] = 1;
                    }
                }
            }
        }

        file.close();
        this->minBox = make_shared<Box>(minVertex, maxVertex, 1.0);
        this->minBox->setMaterial(make_shared<Lambertian>(vec3(0.5, 0.2, 0.7)));
        return;
}


bool PC::isBigEndian()
{
    const uint32_t one = 1;
    return *reinterpret_cast<const char*>(&one) == 0;
}


void PC::swapBytes(char* data, int size)
{
    for (int i = 0; i < size / 2; ++i)
    {
        std::swap(data[i], data[size - i - 1]);
    }
}
void PC::load(QString file_Path) {
    std::string filePath = file_Path.toStdString();
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open PLY file: " << filePath << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line);
    if (line != "ply")
    {
        std::cerr << "Error: Not a valid PLY file: " << filePath << std::endl;
        return;
    }

    std::getline(file, line);
    file.close();

    if (line == "format ascii 1.0")
    {
        loadFromASCIIPLYFile(filePath);
        return;
    }
    else if (line == "format binary_big_endian 1.0")
    {
        loadFromBigEndianPLYFile(filePath);
        return;
    }
    else
    {
        std::cerr << "Error: Unsupported PLY format: " << line << std::endl;
        return;
    }
}


void PC::loadFromASCIIPLYFile(string filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open PLY file: " << filePath << std::endl;
        return;
    }

    // Variables para almacenar el número de vertices y la posición actual en el archivo
    int numVertices = -1;
    vec3 minVertex;
    vec3 maxVertex;
    int currentVertexIndex = 0;

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "element")
        {
            // Leer el número de vertices del archivo
            std::string elementType;
            int count;
            iss >> elementType >> count;
            if (elementType == "vertex")
            {
                numVertices = count;
            }
        }
        else if (type == "end_header")
        {
            // Después de leer el header, se deben haber encontrado el número de vertices
            if (numVertices == -1)
            {
                std::cerr << "Error: Invalid PLY file format." << std::endl;
                return;
            }

            // Leer los vertices del archivo
            vertices.resize(numVertices);
            for (int i = 0; i < numVertices; i++)
            {
                glm::vec3 vertex;
                file >> vertex.x >> vertex.y >> vertex.z;

                // Actualizar las coordenadas mínimas y máximas
                if (i == 0)
                {
                    minVertex = maxVertex = vertex;
                }
                else
                {
                    minVertex = glm::min(minVertex, vertex);
                    maxVertex = glm::max(maxVertex, vertex);
                }

                // Agregar el vértice al vector
                Point p;
                p.position = vertex;
                p.color = glm::vec3(1.f, 0.f, 0.f);
                vertices[i] = p;

                // Incrementar el índice actual del vértice
                currentVertexIndex++;
            }
        }
    }
    file.close();
    this->minBox = make_shared<Box>(minVertex, maxVertex, 1.0);
    this->minBox->setMaterial(make_shared<Lambertian>(vec3(0.5, 0.2, 0.7)));
    return;
}

glm::vec3 PC::getColor(int index) {
    glm::vec3 colors[8] = {
        //glm::vec3(1.0, 0.0, 0.0), // Rojo
        glm::vec3(0.0, 1.0, 0.0), // Verde
        glm::vec3(0.0, 0.0, 1.0), // Azul
        glm::vec3(1.0, 1.0, 0.0), // Amarillo
        glm::vec3(1.0, 0.0, 1.0), // Magenta
        glm::vec3(0.0, 1.0, 1.0), // Cyan
        glm::vec3(0.5, 0.5, 0.5), // Gris
        glm::vec3(0.0, 0.0, 0.0) // Negro
    };
    return colors[index];
}


void PC::read (const QJsonObject &json)
{
    /*Object::read(json);
    if (json.contains("objFileName") && json["objFileName"].isString()) {
        nom = json["objFileName"].toString();
        load(nom);
    }*/
}


//! [1]
void PC::write(QJsonObject &json) const
{
    /*Object::write(json);
    json["objFileName"] = nom;*/
}

void PC::print(int indentation) const
{
    /*Object::print(indentation);
    const QString indent(indentation * 2, ' ');
    QTextStream(stdout) << indent << "objFileName:\t" << nom << "\n";*/
}




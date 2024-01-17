#pragma once

#include <vector>
#include <string>
#include <limits>

#include <QString>
#include <QFile>
#include <QRegularExpression>


#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <glm/glm.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

#include "Object.hh"
#include "Face.hh"
#include "Triangle.hh"
#include "Sphere.hh"
#include "Box.hh"
#include "Point.hh"


using namespace std;

class PC : public Object
{
public:
    PC();
    PC(const QString &fileName);
    PC(const QString &fileName, float data);
    virtual bool hit( Ray& r, float tmin, float tmax, HitInfo& info) const override;


    virtual void aplicaTG(shared_ptr<TG> tg) override;

    virtual void read (const QJsonObject &json) override;
    virtual void write(QJsonObject &json) const override;
    virtual void print(int indentation) const override;

    virtual ~PC();
private:

    std::vector<Point> vertices;
    unsigned nrOfVertices;
    shared_ptr<Box> minBox; // minima Box contenidora
    float epsilon = 0.99994;
    float tol = 0.1;

    void load(QString filename);
    bool weightedAvg(vec3 p, std::vector<Point> verticesCone, Point &point) const;
    bool weightedAvgGauss(vec3 p, std::vector<Point> verticesCone, Point &point) const;
    bool isBigEndian();
    void swapBytes(char* data, int size);
    void loadFromBigEndianPLYFile(string filePath);
    void loadFromASCIIPLYFile(string filePath);
    glm::vec3 getColor(int index);
    /*void makeTriangles();
    void boundingBox();
    void boundingSphere();*/
};


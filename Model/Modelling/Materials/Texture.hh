//
// Created by anna on 20/01/2021.
//

#pragma once

#include <QImage>
#include <QColor>

#include <string>
#include <iostream>
#include "glm/glm.hpp"

using namespace glm;

using namespace std;

class Texture
{
private:
    QImage image;
    bool success;

public:
    Texture();
    Texture(QString nomfitxer);
    virtual ~Texture();

    void setTexture(QString nomfitxer);

    vec3 getColorPixel(vec2 uv) const;

};


#ifndef TRANSLATETG_H
#define TRANSLATETG_H

#include "TG.hh"

class TranslateTG : public TG
{
public:
    glm::vec3 traslation;
    TranslateTG(glm::vec3 trasl);
    virtual ~TranslateTG();
};

#endif // TRANSLATETG_H

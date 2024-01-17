#ifndef SCALETG_H
#define SCALETG_H

#include "TG.hh"

class ScaleTG : public TG
{
public:
    glm::vec3 scaling;
    ScaleTG(glm::vec3 scal);
    virtual ~ScaleTG();
};

#endif // SCALETG_H

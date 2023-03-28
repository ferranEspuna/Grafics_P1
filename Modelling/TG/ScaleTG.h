#ifndef SCALETG_H
#define SCALETG_H


#include "TG.h"

class ScaleTG : public TG
{
public:
    glm::vec3 scaling;
    ScaleTG(float value);
    virtual ~ScaleTG();
};

#endif // SCALETG_H

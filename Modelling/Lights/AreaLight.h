#ifndef AREALIGHT_H
#define AREALIGHT_H

#include <iostream>
#include "Light.h"
#include <math.h>

#include "glm/gtx/norm.hpp"

class AreaLight: public Light {
public:
    AreaLight() {};
    /*
     * Constructor de la classe AreaLight.
     * param posicio: posició de la llum.
     * param Ia: component ambient de la llum.
     * param Id: component difosa de la llum.
     * param Is: component especular de la llum.
     * param a: coeficient a de l'atenuacio.
     * param b: coeficient b de l'atenuacio.
     * param c: coeficient c de l'atenuacio.
     * */
    AreaLight(vec3 posicio, vec3 Ia, vec3 Id, vec3 Is, float a, float b, float c);
    virtual ~AreaLight() {}
    vec3 getPos();
    void setPos(vec3 p2);
    float getArea();
    vec3 getRandomPoint();
    void resetPoint();
    virtual vec3 vectorL(vec3 point) override;
    virtual float attenuation(vec3 point) override;
    virtual float distanceToLight(vec3 point) override;

    virtual void read (const QJsonObject &json) override;
    virtual void write(QJsonObject &json) const override;
    virtual void print(int indentation) const override;
private:
    //Podem veure el center com el centre real i pos com el led concret que mirem en determinats moments.
    vec3 pos;
    vec3 center;
    vec3 width;
    vec3 height;
    float a;
    float b;
    float c;
};

#endif // AREALIGHT_H

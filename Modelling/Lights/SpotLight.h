#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Light.h"

#include "glm/gtx/norm.hpp"

class SpotLight: public Light {
public:
    SpotLight() {};
    /*
     * Constructor de la classe Spotight.
     * param posicio: posició de la llum.
     * param Ia: component ambient de la llum.
     * param Id: component difosa de la llum.
     * param Is: component especular de la llum.
     * param a: coeficient a de l'atenuacio.
     * param b: coeficient b de l'atenuacio.
     * param c: coeficient c de l'atenuacio.
     * */
    SpotLight(vec3 posicio, vec3 Ia, vec3 Id, vec3 Is, float a, float b, float c);
    virtual ~SpotLight() {}
    vec3 getPos();
    virtual vec3 vectorL(vec3 point) override;
    virtual float attenuation(vec3 point) override;
    virtual float distanceToLight(vec3 point) override;

    virtual void read (const QJsonObject &json) override;
    virtual void write(QJsonObject &json) const override;
    virtual void print(int indentation) const override;
private:
    vec3 pos;
    vec3 dir;
    float angle;
    float a;
    float b;
    float c;
};

#endif // SPOTLIGHT_H

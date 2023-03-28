#ifndef CYLINDER_H
#define CYLINDER_H

#include "Object.h"
#include "Modelling/Animation.h"
#include "Modelling/TG/TranslateTG.h"
#include "Modelling/TG/ScaleTG.h"

class Cylinder: public Object
{

public:
    Cylinder();
    Cylinder(vec3 center, vec3 axis, float radius, float height);
    virtual ~Cylinder();
    virtual bool closestHit(Ray& r, HitInfo& info) override;
    virtual bool hasHit(const Ray& r) override;
    virtual void aplicaTG(shared_ptr<TG> tg) override;

    virtual void read (const QJsonObject &json) override;
    virtual void write(QJsonObject &json) const override;
    virtual void print(int indentation) const override;


    vec3  getCenter() { return center;}
    float getRadius() { return radius;}
private:
    vec3 center;
    vec3 axis;
    float radius;
    float height;
    // Numero PI
    const float PI = 3.141592653;
    bool updateHitInfo(Ray &raig, HitInfo& info, float t);
};

#endif // CYLINDER_H







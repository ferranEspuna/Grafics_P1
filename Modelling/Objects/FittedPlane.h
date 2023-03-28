#pragma once

#include "Plane.h"
#include "Modelling/Animation.h"
#include "Modelling/TG/TranslateTG.h"
#include "glm/gtx/constants.hpp"

class FittedPlane: public Plane{
public:
    FittedPlane() {};
    FittedPlane(vec3 normal, vec3 pass_point, vec2 pmin, vec2 pmax, bool puntoCentro, float v);
    FittedPlane(vec3 normal, vec3 pass_point, vec2 pmin, vec2 pmax, bool puntoCentro);

    FittedPlane(vec3 normal, float d, vec2 pmin, vec2 pmax, bool puntoCentro, float v);

    virtual ~FittedPlane(){}
    virtual bool closestHit(Ray &r, HitInfo &info) override;
    virtual bool hasHit(const Ray &r) override;
    virtual void aplicaTG(shared_ptr<TG> tg) override;


    virtual void read (const QJsonObject &json) override;
    virtual void write(QJsonObject &json) const override;
    virtual void print(int indentation) const override;

    vec2 pmin;
    vec2 pmax;
    bool puntoCentro;

private:
    // Per poder aplicar textures
    void getParametricPoint(HitInfo &info, float maxX, float minX, float maxZ, float minZ);
};

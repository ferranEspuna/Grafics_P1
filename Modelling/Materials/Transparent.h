#pragma once

#include "Material.h"
class Transparent : public Material
{

public:
    Transparent() {};
    Transparent(const vec3& color);
    Transparent(const vec3& a, const vec3& d, const vec3& s, const float k);
    Transparent(const vec3& a, const vec3& d, const vec3& s, const float k, const float o);
    virtual ~Transparent();
    virtual vec3 getAttenuation(const Ray& r_in, const HitInfo& rec) const override;
    virtual bool getOneScatteredRay(const Ray& r_in, const HitInfo& rec, Ray& r_out) const override;
    virtual bool getMultipleScatteredRays(const Ray& r_in, const HitInfo& rec, std::vector<Ray>& r_out) const override;
    float refractionRatio(vec3 incident_ray, vec3 &surface_normal) const;
    bool isTotalInternalReflection(vec3 surface_normal, vec3 refracted_ray) const;
    void read (const QJsonObject &json) override;
    vec3 Kt;
    float dmax;
    int id;
    float refractionIndex = 1.000293; //AIR
private:
    const int MULTIPLE_SCATTER_SAMPLES = 5;
    //const float refractionIndex = 1.333; //WATER
    //const float refractionIndex = 1.000293; //AIR
    //const float refractionIndex = 1.52; //GLASS
    //const float refractionIndex = 1.46; //OLIVE OIL
};

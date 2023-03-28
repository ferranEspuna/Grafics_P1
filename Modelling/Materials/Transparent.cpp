#include "Transparent.h"

Transparent::Transparent(const vec3& color): Material()
{
    Kd = color;
}

Transparent::Transparent(const vec3& a, const vec3& d, const vec3& s, const float k):
    Material(a, d, s, k) {
}

Transparent::Transparent(const vec3& a, const vec3& d, const vec3& s, const float k, const float o):
    Material(a, d, s, k, o) {
}
Transparent::~Transparent()
{
}

vec3 Transparent::getAttenuation(const Ray& r_in, const HitInfo& rec) const  {
    vec3 incident_ray = glm::normalize(r_in.getDirection());
    vec3 surface_normal = glm::normalize(rec.normal);
    float refraction_ratio = refractionRatio(incident_ray, surface_normal);
    vec3 refracted_ray = glm::refract(incident_ray, surface_normal, refraction_ratio);

    if (isTotalInternalReflection(surface_normal, refracted_ray)) {
        return Ks; //internal reflection
    } else {
        return Kt; //refraction
    }
}

bool Transparent::getOneScatteredRay(const Ray& r_in, const HitInfo& rec, Ray& r_out) const{
    vec3 incident_ray = glm::normalize(r_in.getDirection());
    vec3 surface_normal = glm::normalize(rec.normal);
    float refraction_ratio = refractionRatio(incident_ray, surface_normal);

    vec3 refracted_ray = glm::refract(incident_ray, surface_normal, refraction_ratio);

    if (isTotalInternalReflection(surface_normal, refracted_ray)) {
        vec3 reflected_ray = glm::reflect(incident_ray, surface_normal);
        r_out = Ray(rec.p, reflected_ray);
    } else {
        r_out = Ray(rec.p, refracted_ray);
    }
    return true;
}

bool Transparent::getMultipleScatteredRays(const Ray& r_in, const HitInfo& rec,  std::vector<Ray>& r_out) const  {
    vec3 incident_ray = glm::normalize(r_in.getDirection());
    vec3 surface_normal = glm::normalize(rec.normal);
    float refraction_ratio = refractionRatio(incident_ray, surface_normal);
    vec3 refracted_ray = glm::refract(incident_ray, surface_normal, refraction_ratio);

    if (isTotalInternalReflection(surface_normal, refracted_ray)) {
        vec3 reflected_ray = glm::reflect(incident_ray, surface_normal);
        r_out.push_back(Ray(rec.p, reflected_ray));
    } else {
        r_out.push_back(Ray(rec.p, refracted_ray));
    }
    return true;
}


float Transparent::refractionRatio(vec3 incident_ray, vec3 &surface_normal) const {
    float cosinus_of_incident_with_normal = glm::dot(incident_ray, surface_normal);
    float refraction_ratio;
    if(cosinus_of_incident_with_normal > 0){ //ray exiting transparent material
        surface_normal = -surface_normal;
        refraction_ratio = refractionIndex;
    } else { //ray entering transparent material
        refraction_ratio = 1/refractionIndex;
    }
    return refraction_ratio;
}


bool Transparent::isTotalInternalReflection(vec3 surface_normal, vec3 refracted_ray) const {
    float cosinus_refracted_with_normal = glm::dot(surface_normal, refracted_ray);
    return cosinus_refracted_with_normal > 0;
}

void Transparent::read (const QJsonObject &json){
    Material::read(json);
    if (json.contains("kt") && json["kt"].isArray()) {
        QJsonArray auxVec = json["kt"].toArray();
        Kt[0] = auxVec[0].toDouble();
        Kt[1] = auxVec[1].toDouble();
        Kt[2] = auxVec[2].toDouble();
    }
    if (json.contains("dmax") && json["dmax"].isDouble()) {
        dmax =json["dmax"].toDouble();
    }
    if (json.contains("id") && json["id"].isDouble()) {
        id =(int)json["id"].toDouble();
    }
    if (json.contains("nut") && json["nut"].isDouble())
        refractionIndex = json["nut"].toDouble();
}

#include "Metal.h"

Metal::Metal(const vec3& color): Material()
{
    Kd = color;
}

Metal::Metal(const vec3& a, const vec3& d, const vec3& s, const float k):
    Material(a, d, s, k) {
}

Metal::Metal(const vec3& a, const vec3& d, const vec3& s, const float k, const float o):
    Material(a, d, s, k, o) {
}
Metal::~Metal()
{
}

vec3 Metal::getAttenuation(const Ray& r_in, const HitInfo& rec) const  {
    return Ks;
}

bool Metal::getOneScatteredRay(const Ray& r_in, const HitInfo& rec, Ray& r_out) const{
    vec3 target = glm::reflect(r_in.getDirection(), rec.normal);
    r_out =  Ray(rec.p, target);
    return true;
}
bool Metal::getMultipleScatteredRays(const Ray& r_in, const HitInfo& rec,  std::vector<Ray>& r_out) const  {
    vec3 target = glm::reflect(r_in.getDirection(), rec.normal);
    r_out.push_back(Ray(rec.p, target));
    return true;
}

//
// Created by anna on 20/01/2021.
//
#include "Texture.h"

Texture::~Texture() {

}
vec3 Texture::getColorPixel(vec2 uv) const {
    if (success) {
        QColor c(image.pixel((uv.x)*image.width(), (uv.y)*image.height() ));
        return vec3(c.redF(), c.greenF(), c.blueF());
    }
    return vec3(0.0f);
}

void Texture::read (const QJsonObject &json){
    Material::read(json);
    if (json.contains("textureFile") && json["textureFile"].isString()){
        image = QImage(json["textureFile"].toString());
        success = true;
    } else {
        success = false;
    }
}

vec3 Texture::getDiffuse(vec2 point) const {
    return(this->getColorPixel(point));
}

vec3 Texture::getAttenuation(const Ray& r_in, const HitInfo& rec) const  {
    return Kd;
}

bool Texture::getOneScatteredRay(const Ray& r_in, const HitInfo& rec, Ray& r_out) const{
    return false;
}
bool Texture::getMultipleScatteredRays(const Ray& r_in, const HitInfo& rec,  std::vector<Ray>& r_out) const  {
    return false;
}


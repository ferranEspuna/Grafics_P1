//
// Created by anna on 20/01/2021.
//

#pragma once

#include <QImage>
#include <QColor>

#include <string>
#include <iostream>
#include "glm/glm.hpp"
#include "Material.h"

using namespace glm;

using namespace std;

class Texture : public Material
{
private:
    QImage image;
    bool success;

public:
    Texture() {};
    virtual ~Texture();
    void read (const QJsonObject &json) override;
    vec3 getDiffuse(vec2 point) const override;

    vec3 getColorPixel(vec2 uv) const;

    virtual vec3 getAttenuation(const Ray& r_in, const HitInfo& rec) const override;
    virtual bool getOneScatteredRay(const Ray& r_in, const HitInfo& rec, Ray& r_out) const override;
    virtual bool getMultipleScatteredRays(const Ray& r_in, const HitInfo& rec, std::vector<Ray>& r_out) const override;

};


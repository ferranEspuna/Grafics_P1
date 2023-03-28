#include "SpotLight.h"
#include <math.h>
#include <iostream>

SpotLight::SpotLight(vec3 posicio, vec3 Ia, vec3 Id, vec3 Is, float a, float b, float c): Light(Ia, Id, Is) {
    this->pos = posicio;
    this->a = a;
    this->b = b;
    this->c = c;
}

vec3 SpotLight::getPos() {
    return this->pos;
}

vec3 SpotLight::vectorL(vec3 point) {
    return normalize(pos - point);
}

float SpotLight::attenuation(vec3 point) {
    if (abs(a)<DBL_EPSILON && abs(b)<DBL_EPSILON  && abs(c)<DBL_EPSILON) {
        //Si tots els coeficients son 0 considerem que no hi ha atenuacio
        return 1.0f;
    }
    vec3 vecDir = normalize(point-pos);
    float valor = dot(dir,vecDir);
    float base = cos(angle);

    if(valor<base){
        return 0.0f;
    }else{
        //Calculem ponderant per la distància a la llum i la distància al centre del focus.
        float d = distance(point, pos);
        return pow((valor/base),0.25f)/(c*d*d + b*d + a);
    }

}

float SpotLight::distanceToLight(vec3 point) {
    return distance(point, pos);
}

void SpotLight::read (const QJsonObject &json)
{
    Light::read(json);

    if (json.contains("position") && json["position"].isArray()) {
        QJsonArray auxVec = json["position"].toArray();
        pos[0] = auxVec[0].toDouble();
        pos[1] = auxVec[1].toDouble();
        pos[2] = auxVec[2].toDouble();
    }
    if (json.contains("direction") && json["direction"].isArray()) {
        QJsonArray auxVec = json["direction"].toArray();
        dir[0] = auxVec[0].toDouble();
        dir[1] = auxVec[1].toDouble();
        dir[2] = auxVec[2].toDouble();
    }
    if (json.contains("a") && json["a"].isDouble())
        a = json["a"].toDouble();
    if (json.contains("b") && json["b"].isDouble())
        b = json["b"].toDouble();
    if (json.contains("c") && json["c"].isDouble())
        c = json["c"].toDouble();
    if (json.contains("angle") && json["angle"].isDouble())
        angle = json["angle"].toDouble();
}

//! [1]
void SpotLight::write(QJsonObject &json) const
{
    Light::write(json);
    QJsonArray auxArray;
    auxArray.append(pos[0]);auxArray.append(pos[1]);auxArray.append(pos[2]);
    json["position"] = auxArray;

    json["a"] = a;
    json["b"] = b;
    json["c"] = c;
}
//! [1]

void SpotLight::print(int indentation) const
{
    const QString indent(indentation * 2, ' ');

    QTextStream(stdout) << "Point Light" << "\n";
    QTextStream(stdout) << indent.toInt()+2 << "position:\t" << pos[0] << ", "<< pos[1] << ", "<< pos[2] << "\n";
    QTextStream(stdout) << indent.toInt()+2 << "a:\t" << a << "\n";
    QTextStream(stdout) << indent.toInt()+2 << "b:\t" << b << "\n";
    QTextStream(stdout) << indent.toInt()+2 << "c:\t" << c << "\n";

}

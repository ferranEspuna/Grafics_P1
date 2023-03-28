#include "AreaLight.h"

AreaLight::AreaLight(vec3 posicio, vec3 Ia, vec3 Id, vec3 Is, float a, float b, float c): Light(Ia, Id, Is) {
    this->pos = posicio;
    this->a = a;
    this->b = b;
    this->c = c;
}

vec3 AreaLight::getPos() {
    return this->pos;
}

vec3 AreaLight::vectorL(vec3 point) {
    return normalize(pos - point);
}

float AreaLight::attenuation(vec3 point) {
    if (abs(a)<DBL_EPSILON && abs(b)<DBL_EPSILON  && abs(c)<DBL_EPSILON) {
        //Si tots els coeficients son 0 considerem que no hi ha atenuacio
        return 1.0f;
    }
    //Calculem la distancia entre el punt i la posicio de la llum
    float d = distance(point, pos);
    return 1.0f/(c*d*d + b*d + a);
}

void AreaLight::setPos(vec3 p2){
    pos = p2;
}

float AreaLight::distanceToLight(vec3 point) {
    return distance(point, pos);
}

float AreaLight::getArea(){
    return length(height)*length(width);
}

vec3 AreaLight::getRandomPoint(){
    //Generem dos randoms entre -0.5 i 0.5
    float aa = ((float) rand() / (RAND_MAX))-0.5;
    float bb = ((float) rand() / (RAND_MAX))-0.5;
    return center + aa*width+bb*height;
}

void AreaLight::resetPoint(){
    pos[0] = center[0];
    pos[1] = center[1];
    pos[2] = center[2];
}

void AreaLight::read (const QJsonObject &json)
{
    Light::read(json);

    if (json.contains("position") && json["position"].isArray()) {
        QJsonArray auxVec = json["position"].toArray();
        pos[0] = auxVec[0].toDouble();
        pos[1] = auxVec[1].toDouble();
        pos[2] = auxVec[2].toDouble();
        center[0] = auxVec[0].toDouble();
        center[1] = auxVec[1].toDouble();
        center[2] = auxVec[2].toDouble();
    }
    if (json.contains("width") && json["width"].isArray()) {
        QJsonArray auxVec = json["width"].toArray();
        width[0] = auxVec[0].toDouble();
        width[1] = auxVec[1].toDouble();
        width[2] = auxVec[2].toDouble();
    }
    if (json.contains("height") && json["height"].isArray()) {
        QJsonArray auxVec = json["height"].toArray();
        height[0] = auxVec[0].toDouble();
        height[1] = auxVec[1].toDouble();
        height[2] = auxVec[2].toDouble();
    }
    if (json.contains("a") && json["a"].isDouble())
        a = json["a"].toDouble();
    if (json.contains("b") && json["b"].isDouble())
        b = json["b"].toDouble();
    if (json.contains("c") && json["c"].isDouble())
        c = json["c"].toDouble();
}

//! [1]
void AreaLight::write(QJsonObject &json) const
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

void AreaLight::print(int indentation) const
{
    const QString indent(indentation * 2, ' ');

    QTextStream(stdout) << "Point Light" << "\n";
    QTextStream(stdout) << indent.toInt()+2 << "position:\t" << pos[0] << ", "<< pos[1] << ", "<< pos[2] << "\n";
    QTextStream(stdout) << indent.toInt()+2 << "a:\t" << a << "\n";
    QTextStream(stdout) << indent.toInt()+2 << "b:\t" << b << "\n";
    QTextStream(stdout) << indent.toInt()+2 << "c:\t" << c << "\n";

}

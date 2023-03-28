#include "FittedPlane.h"

#include <iostream>


FittedPlane::FittedPlane(vec3 normal, vec3 pass_point, vec2 pmin, vec2 pmax, bool puntoCentro, float v) : Plane(normal,pass_point,v){

    this->pmin = pmin;
    this->pmax = pmax;
    this->puntoCentro = puntoCentro;
}

FittedPlane::FittedPlane(vec3 normal, vec3 pass_point, vec2 pmin, vec2 pmax, bool puntoCentro) : Plane(normal,pass_point){

    this->pmin = pmin;
    this->pmax = pmax;
    this->puntoCentro = puntoCentro;
}

FittedPlane::FittedPlane(vec3 normal, float d, vec2 pmin, vec2 pmax, bool puntoCentro, float v) : Plane(normal,d,v){

    this->pmin = pmin;
    this->pmax = pmax;
    this->puntoCentro = puntoCentro;

}

bool FittedPlane::closestHit(Ray &r, HitInfo &info) {

    bool result = Plane::closestHit(r,info);

    if(result){
        float maxX, maxZ, minX, minZ;
        vec3 pointItersect = info.p;

        if(puntoCentro){
            maxX = point[0]+pmax[0];
            maxZ = point[2]+pmax[1];
            minX = point[0]+pmin[0];
            minZ = point[2]+pmin[1];
        }else{
            maxX = pmax[0];
            maxZ = pmax[1];
            minX = pmin[0];
            minZ = pmin[1];
        }
        //En cas de que els punts d'intersecció es siguin fora dels limits maxims o minims, retornem false.
        if(maxX < pointItersect[0] || pointItersect[0] < minX || maxZ < pointItersect[2] || pointItersect[2] < minZ){
            return false;
        }
        getParametricPoint(info, maxX, minX, maxZ, minZ); //per poder aplicar textures
    }
    return  result;
}


bool FittedPlane::hasHit(const Ray &r) {
    // Comprovem interseccio entre el pla i el raig

    // Comprovem si el normal al pla i el raig son ortogonals.
    // En aquest cas son paralels i no hi ha interseccio

    if(dot(r.getDirection(), normal) == 0){
        return false;
    }


    // En els altres casos hi haurà interseccio (si estem en el rang de min/max).
    // Cal omplir la info.

    // PLA: Ax+By+Cz+D=0
    // on A,B,C = normal

    // 1) Calculem la D = -Ax-By-Cz
    float d = -normal[0]*point[0] - normal[1]*point[1] - normal[2]*point[2];

    // 2) Imposem que la recta p+tv compleixi l'eq del pla
    // A(p1 + tv1) + ... + D = 0
    // Aillem la t

    vec3 rp = r.getOrigin();
    vec3 vp = r.getDirection();
    float temp =  -normal[0]*rp[0] - normal[1]*rp[1] - normal[2]*rp[2] - d;

    temp/= normal[0]*vp[0] + normal[1]*vp[1] + normal[2]*vp[2];

    float x = rp[0] + temp*vp[0];
    float z = rp[2] + temp*vp[2];
    float maxX, maxZ, minX, minZ;

    if(puntoCentro){
        maxX = point[0]+pmax[0];
        maxZ = point[2]+pmax[1];
        minX = point[0]+pmin[0];
        minZ = point[2]+pmin[1];
    }else{
        maxX = pmax[0];
        maxZ = pmax[1];
        minX = pmin[0];
        minZ = pmin[1];
    }
    //En cas de que els punts d'intersecció es siguin fora dels limits maxims o minims, retornem false.
    if(maxX < x || x < minX || maxZ < z || z < minZ){
        return false;
    }

    // Retornem false si no estem en el rang demanat
    if (temp > r.getTmax() || temp < r.getTmin()) {
        return false;
    }

    return true;
}


void FittedPlane::aplicaTG(shared_ptr<TG> tg){
    Plane::aplicaTG(tg);
}

void FittedPlane::read(const QJsonObject &json){
    Plane::read(json);

    if (json.contains("pmin") && json["pmin"].isArray()) {
        QJsonArray auxVec = json["pmin"].toArray();
        pmin[0] = auxVec[0].toDouble();
        pmin[1] = auxVec[1].toDouble();
    }
    if (json.contains("pmax") && json["pmax"].isArray()) {
        QJsonArray auxVec = json["pmax"].toArray();
        pmax[0] = auxVec[0].toDouble();
        pmax[1] = auxVec[1].toDouble();
    }
    if (json.contains("puntoCentro") && json["puntoCentro"].isBool()) {
        bool auxBool = json["puntoCentro"].toBool();
        puntoCentro = auxBool;
    }
}

void FittedPlane::write(QJsonObject &json) const{
    Plane::write(json);


    QJsonArray auxArray3;
    auxArray3.append(pmin[0]);auxArray3.append(pmin[1]);
    json["pmin"] = auxArray3;

    QJsonArray auxArray4;
    auxArray4.append(pmax[0]);auxArray4.append(pmax[1]);
    json["pmax"] = auxArray4;

    json["puntoCentro"] = puntoCentro;
}

void FittedPlane::print(int indentation) const{
    Plane::print(indentation);

    const QString indent(indentation * 2, ' ');
    QTextStream(stdout) << indent << "pmin:\t" << pmin[0] << ", "<< pmin[1] << "\n";
    QTextStream(stdout) << indent << "pmax:\t" << pmax[0] << ", "<< pmax[1] <<"\n";
    QTextStream(stdout) << indent << "puntoCentro:\t" << puntoCentro << "\n";

}

void FittedPlane::getParametricPoint(HitInfo &info, float maxX, float minX, float maxZ, float minZ) {
    info.uv.x = (info.p.x - minX)/(std::abs(maxX - minX));
    info.uv.y = (info.p.z - minZ)/(std::abs(maxZ - minZ));
}


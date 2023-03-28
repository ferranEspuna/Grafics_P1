#include "Box.h"
#include <algorithm>
#include <iostream>


Box::Box(){
    this->minPoint = vec3(-0.5,-0.5,-0.5);
    this->maxPoint = vec3(0.5,0.5,0.5);
    calculaSizeAndCenter();
    calculaCares();
}

Box::Box(vec3 minimumPoint, vec3 maximumPoint){
    this->minPoint = minimumPoint;
    this->maxPoint = maximumPoint;
    calculaSizeAndCenter();
    calculaCares();
}

Box::Box(vec3 minimumPoint, vec3 maximumPoint, ivec3 position){
    this->minPoint = minimumPoint;
    this->maxPoint = maximumPoint;
    this->position = position;
    calculaSizeAndCenter();
    calculaCares();
}



bool Box::closestHit(Ray &raig, HitInfo &info){

    //TODO COMENTAR I DEJAR UNO DE LOS DOS.
    Ray infiniteRay = raig;
    //Creem un raig infinit per tenir intersecciió amb tots els plans sempre..
    infiniteRay.setTmin(-std::numeric_limits<float>::infinity());
    //Creem un array d'intervals.
    vec2 intervals[3];
    vec3 normals[6];
    //Omplim els intervals segonns la informació del closest hit de cada cara del cub.
    int cont = 0;
    for(int i=0; i<=5; i++){
        if(i > 2){
            cont = 1;
        }
        if(cares[i].closestHit(infiniteRay,info)){
            intervals[i%3][cont] = info.t;

        }else{
            if(i > 2){
                intervals[i%3][cont] = -INFINITY;
            }else{
                intervals[i%3][cont] = INFINITY;
            }
        }
        normals[i] = cares[i].normal;


    }
    //Creem la variable interval final.
    vec2 intervalFinal = vec2(infiniteRay.getTmin(),infiniteRay.getTmax());
    vec3 normalFinal = normals[0];
    int times = 0;
    for(int i=0; i<=2; i++){
        //En cas que els intervals estiguin girats, els posem correctament. Tot depen des d'on llencem el raig.
        if(intervals[i][1] < intervals[i][0]){
            float temp = intervals[i][0];
            intervals[i][0] = intervals[i][1];
            intervals[i][1] = temp;
            vec3 tempNormal = normals[i];
            normals[i] = normals[i+3];
            normals[i+3] = tempNormal;
        }
        //Si no hi ha intersecció, res.
        if(intervalFinal[1] < intervals[i][0] || intervalFinal[0] > intervals[i][1]){

        }
        //Si hi ha intersecció, actualitzem els valors de l'intervval final i sumem 1 al comptador d'interseccions.
        else
        {
            if(intervals[i][0] < intervalFinal[0]){
                normalFinal = normals[i];
            }
            intervalFinal[0] = std::max(intervals[i][0], intervalFinal[0]);
            intervalFinal[1] = std::min(intervals[i][1], intervalFinal[1]);
            times++;
        }
    }
    //Si hi ha hagut mes de dos interseccions vol dir que estem dins del cub.

    if(times > 2){
        info.normal = normalFinal;
        info.t = intervalFinal[0];
        info.p = raig.pointAtParameter(info.t);
        info.mat_ptr = material.get();
        return true;
    }
    return false;
}

bool Box::hasHit (const Ray &raig){
    //Reptim el metode de closest hit pero sense guardar la informació.
    Ray infiniteRay = raig;
    HitInfo info;

    infiniteRay.setTmin(-std::numeric_limits<float>::infinity());
    vec2 intervals[3];
    int cont = 0;
    for(int i=0; i<=5; i++){
        if(i > 2){
            cont = 1;
        }
        if(cares[i].closestHit(infiniteRay,info)){
            intervals[i%3][cont] = info.t;
        }else{
            if(i > 2){
                intervals[i%3][cont] = -INFINITY;
            }else{
                intervals[i%3][cont] = INFINITY;
            }
        }


    }
    vec2 intervalFinal = vec2(infiniteRay.getTmin(),infiniteRay.getTmax());
    int times = 0;
    for(int i=0; i<=2; i++){

        if(intervals[i][1] < intervals[i][0]){
            float temp = intervals[i][0];
            intervals[i][0] = intervals[i][1];
            intervals[i][1] = temp;
        }
        //No hay interseccion
        if(intervalFinal[1] < intervals[i][0] || intervalFinal[0] > intervals[i][1]){

        }
        else
        {
            intervalFinal[0] = std::max(intervals[i][0], intervalFinal[0]);
            intervalFinal[1] = std::min(intervals[i][1], intervalFinal[1]);
            times++;
        }
    }
    if(times > 2){
        return true;
    }
    return false;
}

void Box::aplicaTG(shared_ptr<TG> t) {


    this->maxPoint = vec3(t->getTG() * vec4(this->maxPoint, 1.0));
    this->minPoint = vec3(t->getTG() * vec4(this->minPoint, 1.0));
    calculaSizeAndCenter();
}

void Box::calculaSizeAndCenter(){
    sizes = maxPoint-minPoint;
    center = vec3((maxPoint[0]+minPoint[0])*0.5,(maxPoint[1]+minPoint[1])*0.5,(maxPoint[2]+minPoint[2])*0.5);
    calculaCares();
}

void Box::calculaCares(){
    cares[0] = Plane(vec3(1,0,0), maxPoint);
    cares[1] = Plane(vec3(0,1,0), maxPoint);
    cares[2] = Plane(vec3(0,0,1), maxPoint);
    cares[3] = Plane(vec3(-1,0,0), minPoint);
    cares[4] = Plane(vec3(0,-1,0), minPoint);
    cares[5] = Plane(vec3(0,0,-1), minPoint);
    for(int i=0;i<6;i++){
        cares[i].setMaterial(material);
    }

}

bool Box::pointIn(vec3 point){
    if(minPoint[0] > point[0] || point[0] > maxPoint[0] || minPoint[1] > point[1] || point[1]
            > maxPoint[1] || minPoint[2] > point[2] || point[2] > maxPoint[2]){
        return false;
    }else{
        return true;
    }
}


void Box::read (const QJsonObject &json)
{
    Object::read(json);


    if (json.contains("maxPoint") && json["maxPoint"].isArray()) {
        QJsonArray auxVec = json["maxPoint"].toArray();
        maxPoint[0] = auxVec[0].toDouble();
        maxPoint[1] = auxVec[1].toDouble();
        maxPoint[2] = auxVec[2].toDouble();
    }
    if (json.contains("minPoint") && json["minPoint"].isArray()) {
        QJsonArray auxVec = json["minPoint"].toArray();
        minPoint[0] = auxVec[0].toDouble();
        minPoint[1] = auxVec[1].toDouble();
        minPoint[2] = auxVec[2].toDouble();
    }

    calculaSizeAndCenter();
}


//! [1]
void Box::write(QJsonObject &json) const
{
    Object::write(json);


}
//! [1]

void Box::print(int indentation) const
{
    Object::print(indentation);
    const QString indent(indentation * 2, ' ');


}

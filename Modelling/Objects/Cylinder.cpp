#include "Cylinder.h"
#include <vector>

Cylinder::Cylinder()
{
    center = vec3(0,0,0);
    axis = vec3(0,1,0);
    radius = 0.1;
    height = 4.0;
}

Cylinder::Cylinder(vec3 center, vec3 axis, float radius, float height){
    this->center = center;
    this->radius = radius;
    this->axis = axis;
    this->height = height;
}

bool Cylinder::updateHitInfo(Ray &raig, HitInfo& info, float t){

    if(t <= raig.getTmax() && t >= raig.getTmin() && t < info.t){
        info.t = t;
        info.p = raig.pointAtParameter(t);
        return true;
    }
    return false;
}


bool Cylinder::closestHit(Ray &raig, HitInfo& info) {

    float temp;
    bool hit = false;

    float a = pow(raig.getDirection()[0],2)+pow(raig.getDirection()[2],2);
    float b = 2*(raig.getDirection()[0]*(raig.getOrigin()[0]-center[0])+raig.getDirection()[2]*(raig.getOrigin()[2]-center[2]));
    float c = pow(raig.getOrigin()[0]-center[0],2)+pow(raig.getOrigin()[2]-center[2],2)-pow(radius,2);
    float r = sqrt(pow(b,2)-4*a*c);
    info.mat_ptr = material.get();


    info.t = numeric_limits<float>::infinity();

    //En el cas en que no intersequi la solució és complexa (nan).
    if(!isnan(r)){

        if(abs(raig.pointAtParameter((-b+r)/(2*a))[1] - center[1]) < height/2){

            temp = (-b+r)/(2*a);
            if(updateHitInfo(raig, info, temp)){
                info.normal = vec3(info.p[0]-center[0],0,info.p[2]-center[2])/radius;
                hit = true;
            }
        }

        if(abs(raig.pointAtParameter((-b-r)/(2*a))[1] - center[1]) < height/2){
            temp = (-b-r)/(2*a);
            if(updateHitInfo(raig, info, temp)){
                info.normal = vec3(info.p[0]-center[0],0,info.p[2]-center[2])/radius;
                hit = true;
            }
        }

    }

    //Comprovem si tallem les tapes, i si ho fem abans que el cilindre principal
    temp = ((center[1]-height/2)-raig.getOrigin()[1])/(raig.getDirection()[1]);
    if(pow(raig.pointAtParameter(temp)[0]-center[0],2)+ pow(raig.pointAtParameter(temp)[2]-center[2],2) <=pow(radius, 2)){
        if(updateHitInfo(raig, info, temp)){
            info.normal = vec3(0.,-1.,0.);
            hit = true;
        }
    }

    temp = ((center[1]+height/2)-raig.getOrigin()[1])/(raig.getDirection()[1]);
    if(pow(raig.pointAtParameter(temp)[0]-center[0],2)+pow(raig.pointAtParameter(temp)[2]-center[2],2) <= pow(radius, 2)){
        if(updateHitInfo(raig, info, temp)){
            info.normal = vec3(0.,1.,0.);
            hit = true;
        }
    }

    return hit;
}

bool Cylinder::hasHit (const Ray& raig){

    float a = pow(raig.getDirection()[0],2)+pow(raig.getDirection()[2],2);
    float b = 2*(raig.getDirection()[0]*(raig.getOrigin()[0]-center[0])+raig.getDirection()[2]*(raig.getOrigin()[2]-center[2]));
    float c = pow(raig.getOrigin()[0]-center[0],2)+pow(raig.getOrigin()[2]-center[2],2)-pow(radius,2);
    float r = sqrt(pow(b,2)-4*a*c);
    float temp;

    if(!isnan(r)){

        //En el cas en que no intersequi la solució és complexa (nan).
        if(abs(raig.pointAtParameter((-b+r)/(2*a))[1] - center[1]) < height/2 && temp <= raig.getTmax() && temp >= raig.getTmin()){
            return true;
        }

        if(abs(raig.pointAtParameter((-b-r)/(2*a))[1] - center[1]) < height/2 && temp <= raig.getTmax() && temp >= raig.getTmin()){
            return true;
        }
    }

    //Comprovem si tallem les tapes
    temp = ((center[1]-height/2)-raig.getOrigin()[1])/(raig.getDirection()[1]);
    if(pow(raig.pointAtParameter(temp)[0]-center[0],2)+pow(raig.pointAtParameter(temp)[2]-center[2],2) <=pow(radius, 2)){
        if(temp <= raig.getTmax() && temp >= raig.getTmin()){
            return true;
        }

    }

    temp = ((center[1]+height/2)-raig.getOrigin()[1])/(raig.getDirection()[1]);
    if(pow(raig.pointAtParameter(temp)[0]-center[0],2)+pow(raig.pointAtParameter(temp)[2]-center[2],2) <= pow(radius, 2)){
        if(temp <= raig.getTmax() && temp >= raig.getTmin()){
            return true;
        }
    }

    return false;

}

void Cylinder::aplicaTG(shared_ptr<TG> t) {
    if (dynamic_pointer_cast<TranslateTG>(t)) {
        vec4 c(center, 1.0);
        c = t->getTG() * c;
        center.x = c.x; center.y = c.y; center.z = c.z;
    }else if (dynamic_pointer_cast<ScaleTG>(t)) {
        mat4 scaling_matrix = t->getTG();
        float value = scaling_matrix[0][0];
        height = height*value;
    }
}


void Cylinder::read (const QJsonObject &json)
{
    Object::read(json);

    if (json.contains("radius") && json["radius"].isDouble()) {
        radius = json["radius"].toDouble();
    }
    if (json.contains("h") && json["h"].isDouble()) {
        height = json["h"].toDouble();
    }
    if (json.contains("center") && json["center"].isArray()) {
        QJsonArray auxVec = json["center"].toArray();
        center[0] = auxVec[0].toDouble();
        center[1] = auxVec[1].toDouble();
        center[2] = auxVec[2].toDouble();
    }
    if (json.contains("axis") && json["axis"].isArray()) {
        QJsonArray auxVec = json["axis"].toArray();
        axis[0] = auxVec[0].toDouble();
        axis[1] = auxVec[1].toDouble();
        axis[2] = auxVec[2].toDouble();
    }

}

void Cylinder::write(QJsonObject &json) const
{
    Object::write(json);

    QJsonArray auxArray;
    QJsonArray auxArray2;
    auxArray.append(center[0]);auxArray.append(center[1]);auxArray.append(center[2]);
    auxArray2.append(axis[0]);auxArray.append(axis[1]);auxArray.append(axis[2]);
    json["center"] = auxArray;
    json["axis"] = auxArray2;
    json["radius"] = radius;
    json["h"] = height;
}

void Cylinder::print(int indentation) const
{
    Object::print(indentation);

    const QString indent(indentation * 2, ' ');

    QTextStream(stdout) << indent << "center:\t" << center[0] << ", "<< center[1] << ", "<< center[2] << "\n";
    QTextStream(stdout) << indent << "axis:\t" << axis[0] << ", "<< axis[1] << ", "<< axis[2] << "\n";
    QTextStream(stdout) << indent << "radius:\t" << radius<< "\n";
    QTextStream(stdout) << indent << "height:\t" << height<< "\n";
}


Cylinder::~Cylinder()
{ }

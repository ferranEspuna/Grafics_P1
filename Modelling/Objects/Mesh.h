#pragma once

#include <vector>
#include <string>
#include <limits>
#include "Box.h"
#include <QString>
#include <QFile>
#include <QRegularExpression>
#include <map>
#include <tuple>
#include <set>

#include <iostream>
#include <stdlib.h>
#include <cstring>

#include "Object.h"
#include "Factories/ObjectFactory.h"
#include "Cara.h"

using namespace std;

class Mesh : public Object
{
public:
    Mesh() {};
    Mesh(const QString &fileName);
    Mesh(const QString &fileName, float data);
    virtual bool closestHit( Ray& r, HitInfo& info) override;
    virtual bool hasHit(const Ray& r) override;

    virtual void aplicaTG(shared_ptr<TG> tg) override;

    virtual void read (const QJsonObject &json) override;
    virtual void write(QJsonObject &json) const override;
    virtual void print(int indentation) const override;


   virtual ~Mesh();

private:


    QString nom;
    vector<Cara> cares; // cares de l'objecte
    vector<vec4> vertexs; // vertexs de l'objecte sense repetits
    Box generalBounding; //Bounding box general
    vec3 maxP; //maxims x,y,z de la mesh
    vec3 minP; //minims x,y,z de la mesh
    int nBox; //Numero de bounding box que es vol fer per dimensió.
    bool bounding; //Dicta si es farà bounding box o no
    map<int,Box> boxes; //diccionari de bounding boxes
    map<int,vector<Triangle>> trianglesBoxes; //dicionari de triangles
    void load(QString filename);
    void construeix_triangles();
    void construeix_triangles_amb_box(int nBoxes);

    vector<Triangle> triangles;
};


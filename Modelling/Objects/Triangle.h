#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "Object.h"

class Triangle: public Object{

    public:

        Triangle();
        Triangle(vec3 p1, vec3 p2, vec3 p3);
        virtual ~Triangle() {}
        virtual bool closestHit(Ray& r, HitInfo& info) override;
        virtual bool hasHit(const Ray& r) override;
        virtual void aplicaTG(shared_ptr<TG> tg) override;

        virtual void read (const QJsonObject &json) override;
        virtual void write(QJsonObject &json) const override;
        virtual void print(int indentation) const override;
        vec3  getVertex(int i);
        vec3 getNormal();

        vec3 coordinates(vec3 from, vec3 vec);

    private:
        //Punts del triangle
        vec3 vertices[3];
        //Vector normal al triangle (el precalculem)
        vec3 normal;

        //Per a crear o modificar el triangle, caldrà cridar aquest mètode
        void calculaNormal();

        //Matriu per a canviar el vector del raig a coordenades basades en el triangle
        mat3 coordinate_change;

        //Mentre aquest valor no canvii, podem utilitzar la coordinate_change sense canviar-la
        vec3 lookFrom;
    };


#endif // TRIANGLE_H

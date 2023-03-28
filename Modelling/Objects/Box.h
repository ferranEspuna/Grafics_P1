#ifndef BOX_H
#define BOX_H
#include "Object.h"
#include "FittedPlane.h"
#include "Modelling/Animation.h"
#include "Modelling/TG/TranslateTG.h"
#include "glm/gtx/constants.hpp"

class Box: public Object{
    public:
        Box();
        Box(vec3 minimumPoint, vec3 maximumPoint);
        Box(vec3 minimumPoint, vec3 maximumPoint, ivec3 position);
        virtual ~Box() {}
        virtual bool closestHit(Ray& r, HitInfo& info) override;
        virtual bool hasHit(const Ray& r) override;
        virtual void aplicaTG(shared_ptr<TG> tg) override;

        virtual void read (const QJsonObject &json) override;
        virtual void write(QJsonObject &json) const override;
        virtual void print(int indentation) const override;

        void calculaSizeAndCenter();
        void calculaCares();
        bool pointIn(vec3 point);
        //Position of the box in to the box matrix
        ivec3 position;
    private:
        //Center of box
        vec3 center;
        //Maximum and minimum point (x,y,z)
        vec3 maxPoint;

        vec3 minPoint;
        Plane cares[6];
        //Sizes of the box
        vec3 sizes;



};




#endif // BOX_H


#pragma once


#include <vector>
#include "Ray.h"
#include "Hitable.h"
#include "Animation.h"
#include "Modelling/Objects/FittedPlane.h"
#include "Modelling/Objects/Object.h"
#include "Modelling/Objects/Sphere.h"
#include "Modelling/Materials/Material.h"
#include "Modelling/Lights/Light.h"
#include "Modelling/Lights/PointLight.h"
#include "Modelling/Lights/SpotLight.h"
#include "Modelling/Lights/AreaLight.h"
#include <algorithm>
#include "math.h"
#include <iostream>

class Scene: public Hitable
{
public:
    typedef enum
    {
           VIRTUALWORLD,
           REALDATA,
           TEMPORALVW,
           TEMPORALDATA
    } DATA_TYPES;

    typedef enum
    {
        MATERIAL,
        COLORMAP
    } MATERIAL_TYPES;



    QString name;

    Scene();
    Scene(shared_ptr<FittedPlane> fp);
    virtual ~Scene() {};

    // Funcio que calcula la interseccio del raig r amb l'escena. Guarda la informacio
    // del punt d'interseccio més proper a t_min del Raig, punt que està entre t_min i t_max del Raig.
    // Retorna cert si existeix la interseccio, fals, en cas contrari
    virtual bool closestHit(Ray& raig, HitInfo& info);

    // Funcio que detecta si el raig intersecta amb algun objecte de l'escena
    virtual bool hasHit(const Ray& raig);

    // OPCIONAL: Mètode que retorna totes les interseccions que es troben al llarg del raig
    //    virtual bool allHits(const Ray& r, vector<shared_ptr<HitInfo> infos) const = 0;


    // Funcio recursiva que calcula el color. Inicialment es
    // es crida a cada pixel. Tambe es crida en calcular les ombres o les reflexions.
    vec3 RayColor (vec3 lookFrom, Ray &ray, int depth, int maxDepth, int multiple_maxDepth);
    vec3 getBackgroundColor (vec3 ray2);

    void update(int nframe);

    void setDimensions(vec3 p1, vec3 p2);

    // TODO FASE 2:
    // Incloure bases a l'escena: FittedPlane
    // void setBasePlane(shared_ptr<FittedPlane> plane);

    // AMPLIACIO: Posible objecte que no sigui un fitted plane: una esfera
    // void setBaseSphere(shared_ptr<Sphere> sphere);


    void setBackground(bool background);

    void setMAXDEPTH(int depth);

    void setTopBackground(vec3 color);

    void setDownBackground(vec3 color);

    // dimensions de l'escena virtual: capsa mínima contenidora
    vec3 pmin, pmax;

    //Objecte base
    shared_ptr<Object> baseObj;

    // Vector d'objectes continguts a l'escena
    std::vector<shared_ptr<Object>> objects;
    // FASE 2: Afegir llums a l'escena i la il.luminació global
    // Implementar els mètodes següents
    // Recordar cridar-los per omplir les dades des de fitxer

    void setGlobalLight(vec3 light);
    void setLights(std::vector<shared_ptr<Light>> lights);

private:
    // FASE 1: Càlcul de la il.luminació en un punt (Blinn-Phong i ombres)
    vec3 shading(HitInfo& info, Ray& r);

    //Llum ambient global
    vec3 globalLight;
    std::vector<shared_ptr<Light>> lights;

    // FASE 2: Calcula si el punt "point" és a l'ombra
    vec3 computeShadow(shared_ptr<Light> light, vec3 point);

    float getLocalAmbientOcclusion(vec3 point,vec3 v1, vec3 v2, vec3 v3);
    float getGlobalAmbientOcclusion(vec3 point,vec3 v1, vec3 v2, vec3 v3);
    vec3 getPerpendicular(vec3 vector);
    //Guarda si en les iteracions recursives de rayColor() en cas de no haver-hi hit
    // s'utilitza el color
    //de background o la llum ambient global
    bool backgroundInRecurvise = false;

    //Color de dalt del background de la imatge
    vec3 colorTop;
    //Color de baix del background de la imatge
    vec3 colorDown;
    // per controlar la recursivitat
    int MAXDEPTH;

};


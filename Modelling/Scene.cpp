#include "Scene.h"
#include <map>
const int NRAOG = 20; //Número de rajos en ambient occlusion global.
const int NRAOL = 20; //Número rajos en ambient occlusion local.
const float IGNDIST = 100.0; //Distància a partir de la que ignorem objectes en ambientocclusion.
const bool AO = false; //Indica si ambient occlusion està activat.


Scene::Scene()
{
    pmin.x = -0.5f;  pmin.y = -0.5f; pmin.z = -0.5f;
    pmax.x = 0.5f;  pmax.y = 0.5f; pmax.z = 0.5f;
}

// TODO: Fase ***
// Constructora a utilitzar quan s'inicialitza una escena amb un pla base
Scene::Scene(shared_ptr<FittedPlane> fp)
{
    baseObj = fp;
    pmin.x = fp->pmin.x;  pmin.y = 0.f; pmin.z = fp->pmin.y;
    pmax.x = fp->pmax.x;  pmax.y = 0.f; pmax.z = fp->pmax.y;
}

/*
** TODO: FASE 1:
** Metode que testeja la interseccio contra tots els objectes de l'escena i retorna
** la que està més aprop del punt origen del raig
** Si un objecte es intersecat pel raig, el parametre  de tipus HitInfo conte
** la informació sobre la interseccio.
** El metode retorna true si algun objecte es intersecat o false en cas contrari.
**
*/

bool Scene::closestHit(Ray &raig, HitInfo& info) {
    // TODO FASE 0 i FASE 1:
    // Heu de codificar la vostra solucio per aquest metode substituint el 'return true'
    // Una possible solucio es cridar el mètode "hit" per a tots els objectes i quedar-se amb la interseccio
    // mes propera a l'observador, en el cas que n'hi hagi més d'una.
    // Cada vegada que s'intersecta un objecte s'ha d'actualitzar el HitInfo del raig.

    float closestT = raig.getTmax();

    //De moment, no tenim cap hit
    bool hit = false;
    //Per tant no hi ha informació del hit.
    //La tindrem en una variable temportal.
    HitInfo infoTemp;

    //Recorrem tots els objectes
    for(auto& o : this->objects){

        //Si el raig interseca amb l'objecte:
        if(o->closestHit(raig, infoTemp)){

            //En tal cas, retornarem true, però cal mirar si hi ha un hit més proper
            hit = true;

            //mirem el valor de t per veure si és el hit més proper de moment
            if(infoTemp.t < closestT){

                //En tal cas actualitzem el valor de t i info
                closestT = infoTemp.t;
                info = infoTemp;
            }
        }
    }



    return hit;
}

/*
** FET: FASE 2:
** Metode que testeja la inte1rseccio contra tots els objectes de l'escena i retorna
** true si algun objecte es intersecat o false en cas contrari.
**
*/
bool Scene::hasHit (const Ray& raig) {

    //Recorrem tots els objectes de l'escena
    for(auto& o : this->objects){

        //Si algun d'ells interseca amb el raig, retornem true
        if(o->hasHit(raig)){
            return true;
        }
    }

    //Si hem recorregut tots els objectes i cap interseca, retornem false
    return false;
}

// OPCIONAL:
// FASE 3: Mètode que retorna totes les interseccions que es troben al llarg del raig
//    virtual bool allHits(const Ray& r, vector<shared_ptr<HitInfo> infos) const { }



vec3 Scene::RayColor (vec3 lookFrom, Ray &ray, int depth, int maxDepth, int multiple_maxDepth) {

    vec3 color;
    vec3 ray2;
    vec3 direction = ray.getDirection();

    if (!(direction.x < 0.0001 && direction.y < 0.0001 && direction.z < 0.0001)){
        ray2 = normalize(direction);
    } else{
        ray2 = direction;
    }

    HitInfo info;


    if(this->closestHit(ray, info)){

        color += shading(info, ray);

        if(maxDepth - depth < multiple_maxDepth){
            //recursivitat amb diversos raigs
            vector<Ray> rays_out;

            if(info.mat_ptr->getMultipleScatteredRays(ray, info, rays_out)){
                for(auto& rout : rays_out){
                    color += info.mat_ptr->getAttenuation(ray, info) * RayColor(info.p, rout, depth - 1, maxDepth, multiple_maxDepth) / (float) rays_out.size();
                }
            }

        }else if(depth > 0){
            //recursivitat amb un raig
            Ray rout;
            if(info.mat_ptr->getOneScatteredRay(ray, info, rout)){
                color += info.mat_ptr->getAttenuation(ray, info) * RayColor(info.p, rout, depth - 1, maxDepth, multiple_maxDepth);
            }
        }

    }else{
        color = getBackgroundColor(ray2);
    }
    return color;
}

vec3 Scene::getBackgroundColor(vec3 ray2){
    //degradado de arriba a abajo de colorTop a colorDown
    float normalized_y = (ray2[1]+1.0)/2.0;
    return (normalized_y * colorTop + (1-normalized_y) * colorDown);
}

void Scene::update(int nframe) {
    for (unsigned int i = 0; i< objects.size(); i++) {
        objects[i]->update(nframe);
    }
}

void Scene::setDimensions(vec3 p1, vec3 p2) {
    pmin = p1;
    pmax = p2;
}

void Scene::setBackground(bool background) {
    backgroundInRecurvise = background;
}

void Scene::setMAXDEPTH(int depth) {
    MAXDEPTH = depth;
}

void Scene::setDownBackground(vec3 color) {
    colorDown = color;
}

void Scene::setTopBackground(vec3 color) {
    colorTop = color;
}

/*
** Mètode encarregat de realitzar el shading del model de BlinFong.
** Crida el mètode de shadow que és l'encarregar de generar les sombres.
** Crida als mètodes d'ambient occlusión per generar llumb ambient més realista.
*/
vec3 Scene::shading(HitInfo& info, Ray& r) {

    //Càlculs d'ambient occlusion.
    float localRatio;
    float globalRatio;
    if(AO){
        vec3 normal = normalize(info.normal);
        vec3 wec1 = getPerpendicular(info.normal);
        vec3 wec2 = cross(normal,wec1);
        localRatio = getLocalAmbientOcclusion(info.p,wec1,wec2,normal);
        globalRatio = getGlobalAmbientOcclusion(info.p,wec1,wec2,normal);
    }else{
        localRatio = 1.0;
        globalRatio = 1.0;
    }

    //Les tres llums que farem servir.
    vec3 ambLight = globalLight*info.mat_ptr->Ka*globalRatio;
    vec3 difLight;
    vec3 specLight;

    for(auto light : lights){
        shared_ptr<PointLight> lightP = dynamic_pointer_cast<PointLight>(light);

        //PointLight
        if (lightP != nullptr) {
            vec3 lightPos = lightP->getPos();
            vec3 L = lightP->vectorL(info.p);
            vec3 V = -r.getDirection();
            float attenuation = lightP->attenuation(info.p);

            vec3 shadow = computeShadow(light,info.p);

            //Llum difusa
            float lightFactor = dot(L,info.normal);
            if(lightFactor<0){lightFactor=0;}
            vec3 Id = lightP->getId();
            vec3 Kd = info.mat_ptr->getDiffuse(info.uv);

            difLight += Id*Kd*lightFactor*shadow*attenuation;


            //Llum especular
            vec3 Is = lightP->getIs();
            vec3 Ks = info.mat_ptr->Ks;


            if ((abs(Kd.x-Kd.y) < 0.2) && (Kd.z > Kd.x) && (Kd.z > Kd.y)){
                Ks + vec3(0.1, 0.1, 0.6);
            }
            //Model de Phong
            /*
            vec3 R = normalize((2*lightFactor*info.normal-L));
            lightFactor = dot(normalize(V),R);
            */

            //Model de Blinn-Phong
            vec3 H = normalize(L+V);
            lightFactor = dot(info.normal,H);

            if(lightFactor<0){lightFactor=0;}
            lightFactor = pow(lightFactor,info.mat_ptr->shininess);
            specLight += Ks*Is*lightFactor*shadow*attenuation;

            //Llum ambient
            vec3 Ia = lightP->getIa();
            vec3 Ka = info.mat_ptr->Ka;
            ambLight+=Ia*Ka*localRatio;
        }

        //SpotLight
        shared_ptr<SpotLight> lightS = dynamic_pointer_cast<SpotLight>(light);
        if (lightS != nullptr) {
            vec3 lightPos = lightS->getPos();
            vec3 L = lightS->vectorL(info.p);
            vec3 V = -r.getDirection();
            float attenuation = lightS->attenuation(info.p);

            vec3 shadow = computeShadow(light,info.p);

            //Llum difusa
            float lightFactor = dot(L,info.normal);
            if(lightFactor<0){lightFactor=0;}
            vec3 Id = lightS->getId();
            vec3 Kd = info.mat_ptr->getDiffuse(info.uv);
            difLight += Id*Kd*lightFactor*shadow*attenuation;

            //Llum especular
            vec3 Is = lightS->getIs();
            vec3 Ks = info.mat_ptr->Ks;

            //Model de Phong
            /*
            vec3 R = normalize((2*lightFactor*info.normal-L));
            lightFactor = dot(normalize(V),R);
            */

            //Model de Blinn-Phong
            vec3 H = normalize(L+V);
            lightFactor = dot(info.normal,H);

            if(lightFactor<0){lightFactor=0;}
            lightFactor = pow(lightFactor,info.mat_ptr->shininess);
            specLight += Ks*Is*lightFactor*shadow*attenuation;

            //Llum ambient
            vec3 Ia = lightS->getIa();
            vec3 Ka = info.mat_ptr->Ka;
            ambLight+=Ia*Ka*localRatio;
        }

        //AreaLight
        shared_ptr<AreaLight> lightA = dynamic_pointer_cast<AreaLight>(light);
        if(lightA!=nullptr){
            vec3 lightPos = lightA->getPos();
            vec3 L = lightA->vectorL(info.p);
            vec3 V = -r.getDirection();
            float attenuation = lightA->attenuation(info.p);

            //Aquí és on entra en joc que sigui d'àrea.
            int numMostresPerPixel2 = 5;
            float area = lightA->getArea()* numMostresPerPixel2 ;
            int reps = (int) area;
            vec3 shadow = vec3(0.,0.,0);
            for(int i = 0; i<reps; i++){
                lightA->setPos(lightA->getRandomPoint());
                shadow = shadow + computeShadow(light,info.p);
            }
            //Fem la mitjana.
            shadow = shadow/(float)reps;

            //Reset del pos;
            lightA->resetPoint();

            //Llum difusa
            float lightFactor = dot(L,info.normal);
            if(lightFactor<0){lightFactor=0;}
            vec3 Id = lightA->getId();
            vec3 Kd = info.mat_ptr->getDiffuse(info.uv);
            difLight += Id*Kd*lightFactor*shadow*attenuation;

            //Llum especular
            vec3 Is = lightA->getIs();
            vec3 Ks = info.mat_ptr->Ks;

            //Model de Phong
            /*
            vec3 R = normalize((2*lightFactor*info.normal-L));
            lightFactor = dot(normalize(V),R);
            */

            //Model de Blinn-Phong
            vec3 H = normalize(L+V);
            lightFactor = dot(info.normal,H);

            if(lightFactor<0){lightFactor=0;}
            lightFactor = pow(lightFactor,info.mat_ptr->shininess);
            specLight += Ks*Is*lightFactor*shadow*attenuation;

            //Llum ambient
            vec3 Ia = lightA->getIa();
            vec3 Ka = info.mat_ptr->Ka;
            ambLight+=Ia*Ka*localRatio;
        }

    }
    return ambLight+difLight+specLight;
}

/*
** Aquest mètode s'encarrega de gestionar les diverses ombres presents.
** Té en compte la distància que es travessa d'un objecte i el color d'aquest (Kt)
** Si hi ha intersecció de dos objectes és com si cohexistíssin en l'espai (mon real no passa)
*/
vec3 Scene::computeShadow(shared_ptr<Light> light, vec3 point) {
    vec3 dir = light->vectorL(point);
    float eps= 0.0005;
    vec3 shadow = vec3(1.0,1.0,1.0); //On anirem guardant l'acumulació de les shadows.

    float dist;
    float dmax;
    vec3 kt;
    HitInfo info;
    vec3 enterPoint;
    std::map<int, vec3> id_points;
    std::map<int, vec3> id_kts;
    Ray r = Ray(point, dir, eps, light->distanceToLight(point));


    //Anirem revisant tots els xocs que hi hagi fins la llum.
    while(closestHit(r,info)){
        Transparent* objecte = dynamic_cast<Transparent*>(info.mat_ptr);

        //Primer xoc mirem que sigui transparent.
        if(objecte){
             //Transparent* objecte = static_cast<Transparent*>(info.mat_ptr);

             //Mirem si ja hem entrat a l'objecte, és a dir, que estem sortint.
             if(id_points.find(objecte->id) != id_points.end()){
                 //Calculem la distància recorreguda.
                 enterPoint = id_points[objecte->id];
                 dist = distance(enterPoint,info.p);
                 dmax = objecte->dmax;
                 kt = objecte->Kt;
                 //Ho apliquem al shadow.
                 shadow*=((float)(1.0-glm::min(dist/dmax,1.0f)))*kt;

                 //Traiem les coses del diccionaris.
                 id_kts.erase(objecte->id);
                 id_points.erase(objecte->id);

             //Si encara no hem entrat a l'objecte.
             }else{
                 id_points[objecte->id] = info.p;
                 id_kts[objecte->id] = objecte->Kt;
             }

        //Actualitzem la informació del raig per continuar buscant xocs.
        r = Ray(info.p, dir, eps, light->distanceToLight(point));

        //Objecte opac.
        }else{
            return vec3(0.0,0.0,0.0);
        }
    }

    //Hi haura vores on haguem entrat i no sortit.
    for (auto it = id_points.begin(); it != id_points.end(); ++it){
      int key = it->first;
      shadow*=id_kts[key];
    }

    //Retornem el shadow calculat.
    return shadow;
}

void Scene::setGlobalLight(vec3 light){
    this->globalLight = light;
}

void Scene::setLights(std::vector<shared_ptr<Light>> lights){
    this->lights = lights;
}


/*
** Calcula l'oclusió global (outdoor). Es llencen rajos aleatoris i es miren quins no tenen interseccions (arriben al cel),
** FInalment és fa una proporció entre aquests i els totals obtenint quanta llum ambient arriba.
*/
float Scene::getGlobalAmbientOcclusion(vec3 point,vec3 v1, vec3 v2, vec3 v3){
    int skyRay = 0;
    for(int i = 0; i< NRAOG; i++){
        float aa = 2*((float) rand() / (RAND_MAX))-1.0;
        float bb = 2*((float) rand() / (RAND_MAX))-1.0;
        float cc = ((float) rand() / (RAND_MAX));  //Aquest no pot ser negatiu ja que és el de la normal.
        vec3 randomDir = normalize(aa*v1+bb*v2+cc*v3);
        skyRay += (int)(!hasHit(Ray(point, randomDir)));
    return (float)skyRay/(float)NRAOG;
    }
}


/*
** Calcula l'oclusió local. Tenint en compte les interseccions fins a la constant IGNDIST.
** D'aquesta forma podem ponderar la oclusió per lo prop que està la intersecció.
** En espais outdors podem utilitzar directament l'oclusió global per totes les llums.
** En ambients interiors haurien de fixar la distància màxima com la mida de l'escena interior.
*/
float Scene::getLocalAmbientOcclusion(vec3 point,vec3 v1, vec3 v2, vec3 v3){
    HitInfo infoTemp; //Per guardar els hits per trobar la distància.
    float freeRay = 0;
    for(int i = 0; i<NRAOL; i++){
        float aa = 2*((float) rand() / (RAND_MAX))-1.0;
        float bb = 2*((float) rand() / (RAND_MAX))-1.0;
        float cc = ((float) rand() / (RAND_MAX));  //Aquest no pot ser negatiu ja que és el de la normal.
        vec3 randomDir = normalize(aa*v1+bb*v2+cc*v3); //Generem un vector tal que la seva component per la normal és + (evitar anar dins de l'objecte)
        Ray r = Ray(point, randomDir,0.001);
        bool hit = this->closestHit(r,infoTemp);
        //Mires si hi ha hit i si n'hi ha mirem a quina distància per saber l'oclusió.
        if(!hit){
            freeRay+=1;
        }else{
            float rec = distance(infoTemp.p,point);
            if(rec>IGNDIST){
                freeRay+=1.0;
            }else{
                freeRay+=rec/IGNDIST;
            }
        }
    }
    return (float)freeRay/(float)NRAOL;
}


/*
** Retorna un vector perpendicular al vector proporcionat.
** El vector de tornada es troba normalitzat.
*/
vec3 Scene::getPerpendicular(vec3 vector){
    if(vector[0] == 0){
        return vec3(1,0,0);
    }else{
        return normalize(vec3((-vector[1]-vector[2])/vector[0],1,1));
    }
}




#include "SceneFactoryData.h"

SceneFactoryData::SceneFactoryData(shared_ptr<InfoMapping> mr):SceneFactory()
{
    mapping = mr;
}


shared_ptr<Scene> SceneFactoryData::createScene(Serializable::SaveFormat saveFormat, QString nameFile) {
    scene= make_shared<Scene>();
    load(saveFormat, nameFile);
    print(0);
    return buildVirtualScene();
}


shared_ptr<Scene> SceneFactoryData::createScene() {
    auto scene= make_shared<Scene>();
    //  TODO: Només si voleu provar manualment algunes dades posades
    //  hardcoded aqui

    return scene;
}


//! [3]
bool SceneFactoryData::load(SceneFactory::SaveFormat saveFormat, QString nameFile)
{
    QFile loadFile(saveFormat == Json
        ? QString(nameFile.append(".json"))
        : QString(nameFile.append(".dat")));
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open the virtual scene file.");
        return false;
    }

    QByteArray saveData = loadFile.readAll();
    QJsonParseError error;
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData, &error));


    if (loadDoc.isNull()) {
        qWarning("Parse error in json data scene file.");
        return false;
    }
    QJsonObject object = loadDoc.object();
    read(object);

    QTextStream(stdout) << "Loaded data scene using "
                        << (saveFormat != Json ? "CBOR" : "JSON") << "...\n";
    return true;
}
//! [3]

//! [4]
bool SceneFactoryData::save(Serializable::SaveFormat saveFormat, QString nameFile) const
{
    QFile saveFile(saveFormat == Json
                   ? QString(nameFile.append(".json"))
                   : QString(nameFile.append(".dat")));

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open data file.");
        return false;
    }

    QJsonObject setUpObject;
    write(setUpObject);
    saveFile.write(QJsonDocument(setUpObject).toJson());

    return true;
}

//! [0]
void SceneFactoryData::read(const QJsonObject &json)
{
    if (json.contains("scene") && json["scene"].isString())
        scene->name = json["scene"].toString();
    if (json.contains("typeScene") && json["typeScene"].isString())
        currentType = getSceneFactoryType(json["typeScene"].toString());

    if (json.contains("base") && json["base"].isObject()) {
        QJsonObject jbase = json["base"].toObject();
        shared_ptr<Object> o;
        if (jbase.contains("type") && jbase["type"].isString()) {
            QString objStr = jbase["type"].toString().toUpper();

            o = ObjectFactory::getInstance().createObject(ObjectFactory::getInstance().getObjectType(objStr));
            o->read(jbase);
            scene->baseObj = o;
            sceneType = objStr;
            set_base_object_y(jbase);
        }
    }
    mapping = make_shared<InfoMapping>();
    mapping->read(json);
    doubleValue = mapping->valorDoble;

}
//! [0]

void SceneFactoryData:: write(QJsonObject &json) const
{
   json["scene"] = scene->name;
   json["typeScene"] = SceneFactory::getNameType(currentType);

   QJsonObject jbase;
   // TO DO Fase 1: Cal escriure l'objecte base a fitxer: Descomenta les següents línies
   // scene->baseObj->write(jbase);
   // auto value = ObjectFactory::getInstance().getIndexType(scene->baseObj);
   // jbase["type"]  = ObjectFactory::getInstance().getNameType(value);

   json["base"] = jbase;

    mapping->write(json);
}

void SceneFactoryData::print(int indentation) const
{
    const QString indent(indentation * 2, ' ');
    QTextStream(stdout) << indent << "scene:\t" << scene->name << "\n";
    QTextStream(stdout) << indent << "typeScene:\t" << SceneFactory::getNameType(currentType) << "\n";
    QTextStream(stdout) << indent << "base:\t\n";
    mapping->print(indentation+2);
}

shared_ptr<Scene> SceneFactoryData::buildVirtualScene() {
    // TO DO: A partir de les dades carregades, cal construir l'escena virtual amb tot colocat al seu lloc
    // i a la seva mida

    //Afegim objecte base a la llista d'objectes
    scene->objects.push_back(scene->baseObj);

    for (unsigned int i=0; i<mapping->props.size(); i++) {
        // A props[i].first es te la informació de la propietat per fer el mapping de cada valor
         shared_ptr<PropertyInfo> propinfo = mapping->props[i].first;
         // a props[i].second es tenen les dades
         std::vector<glm::detail::tvec4<float>> data = mapping->props[i].second;


        //Escalamos los datos segon la informacion que tenemos en data y en propinfo.
         scaleRealDataToVirtual(&data, propinfo);


         // Per cada valor, es dona d'alta un objecte a l'escena
         for (unsigned int j=0; j<data.size(); j++) {
             int cont = 1;
             if(doubleValue){
                 cont = 2;
             }
             //Obtenim el tipus d'objecte.
             QString s = ObjectFactory::getInstance().getNameType(propinfo->gyzmo);

             for(int k=0; k<cont; k++){

                 //Creem l'objecte.
                 shared_ptr<Object> o = ObjectFactory::getInstance().createObject(s, mapping->props[i].second[j][2],
                         propinfo->gyzmo);
                 //Obtenim el material.
                 o->setMaterial(mapeigMaterial(propinfo, propinfo->colorMapType,
                                               MaterialFactory::getInstance().getIndexType(propinfo->material),
                                               mapping->props[i].second[j][2]));
                 /***
                  * PRINTS TODO QUITAR
                  * data[j][0] ==> COORDENADA X VIRTUAL
                  * data[j][1] ==> COORDENADA Z VIRTUAL
                  * data[j][2] ==> RADI (VALOR) VIRTUAL
                  * */
                 cout << "printing object data" << endl;
                 cout << data[j][0]<<endl;
                 cout << data[j][1]<<endl;
                 cout << data[j][2]<<endl;
                 cout << "end object data" << endl;


                 //Conseguimos las coordenadas que queremos segun el tipo de objecto base.
                 vec3 coord = getPuntBase(ObjectFactory::getInstance().getObjectType(sceneType),vec2(data[j][0],data[j][1]));
                 if(k == 1){
                     coord[0] = coord[0]+0.3;
                     o->inverseKd();
                 }
                 //Hacemos la transformación del objeto.
                 o = transformaObjecte(s,vec3(data[j][0],data[j][1],data[j][k+2]),coord,o);

                 // Afegir objecte a l'escena
                 scene->objects.push_back(o);


             }
         }
    }
    return scene;
}

shared_ptr<Object> SceneFactoryData::transformaObjecte(QString s, glm::detail::tvec3<float> data, vec3 coord, shared_ptr<Object> o){
    //1. FEM ELS ESCALATS.
    auto shared_ptr_scaling = make_shared<ScaleTG>(data[2]);
    o->aplicaTG(shared_ptr_scaling);
    //2. FEM LES TRANSLACIONS DEPENENT DE L'OBJECTE.

    //Si tenim una esfera com a objecte base, no toquem les coordenades
    if(sceneType == "SPHERE"){}
    //En cas contrari, movem les coordenades per a que estiguin per sobre del pla i poguem fer correctament les translacions.
    else{

        if(s == "SPHERE")
        {

           coord[1] = coord[1]+data[2]*0.5;
        }
        else if(s == "CYLINDER"){
           coord[1] = coord[1]+data[2]*2;
           // OBSERVACION: base_object_y+data[j][2]*2 es por dos porque el cilindro por defecto tiene altura 4 y 4/2 = 2
        }
    }
    //Creem el vector de translació
    glm::vec3 translation_vector = glm::vec3(coord[0], coord[1], coord[2]);
    //Apliquem la translació
    auto shared_ptr_translation = make_shared<TranslateTG>(translation_vector);
    o->aplicaTG(shared_ptr_translation);
    return o;
}

//le pasas un punto que quieres situar en el objeto base y el objeto base (?)
vec3 SceneFactoryData::getPuntBase(ObjectFactory::OBJECT_TYPES basetipo, vec2 puntReal) {
    if (basetipo == ObjectFactory::FITTEDPLANE || basetipo == ObjectFactory::PLANE) {
        return vec3(puntReal.x, base_object_y, puntReal.y);
    } else if(basetipo == ObjectFactory::SPHERE) {
        float lon = -puntReal.x * glm::pi<float>() / 180;
        float lat = puntReal.y * glm::pi<float>() / 180;
        float r = base_object_y;
        vec3 centre = centreBaseObject;
        return vec3(r * cos(lat) * cos(lon) + centre.x, r * sin(lat) + centre.y, r * cos(lat) * sin(lon) + centre.z);
    }
    return vec3(0);
}


// TODO: Fase 2
// Cal usar-lo
// shared_ptr<TranslateTG> SceneFactoryData::translacioGizmo(shared_ptr<PropertyInfo> propinfo, vec3 puntMonReal) {
// shared_ptr<ScaleTG> SceneFactoryData::escalatGizmo(shared_ptr<PropertyInfo> propinfo, float valorMonReal) {

// }

shared_ptr<Material> SceneFactoryData::mapeigMaterial(shared_ptr<PropertyInfo> propinfo, ColorMapStatic::COLOR_MAP_TYPES tCM, MaterialFactory::MATERIAL_TYPES tMat, double valorMonReal) {

    // TO DO Fase 2: Cal mapejar el color difus al color de la paleta o ColorMap segons el valorMonReal i
    // els valors minims i maxims de les propietats
    // Tens els valors minims i maxim de cada propietat a l'estructura de setup d'aquesta classe

    auto cm = make_shared<ColorMapStatic>(tCM);
    int idx = (int)(255.0*(valorMonReal-propinfo->minValueReal)/(propinfo->maxValueReal-propinfo->minValueReal));
    return MaterialFactory::getInstance().createMaterial(propinfo->material->Ka,
                                                         cm->getColor(idx),
                                                         propinfo->material->Ks,
                                                         propinfo->material->shininess,
                                                         propinfo->material->opacity, tMat);
}
void SceneFactoryData::scaleRealDataToVirtual(std::vector<glm::detail::tvec4<float>> *real_data, std::shared_ptr<PropertyInfo> propinfo){
    for (unsigned int i=0; i<real_data[0].size(); i++) {
        //En cas de que tinguem un pla com objecte base, escalem també las coordenadas del pla, si no, com que les coordenades ja ens arriben com longitud i latitud, les deixem
        //tal i com estan i tot dependrà de l'objecte base.
        if(sceneType == "FITTEDPLANE" || sceneType == "PLANE"){
            real_data[0][i][0] = scaleValueToRange(real_data[0][i][0], mapping->Rxmax, mapping->Rxmin, mapping->Vxmax, mapping->Vxmin);
            real_data[0][i][1] = scaleValueToRange(real_data[0][i][1], mapping->Rzmax, mapping->Rzmin, mapping->Vzmax, mapping->Vzmin);
        }
        //Lo multiplicamos por 0.5 para que nos quede un mejor escalado (Se puede cambiar)
        float virtualRangeSize = abs(mapping->Vymax - mapping->Vymin);
        //Factor de truncación //Se puede cambiar.
        float sizeMargin = 0.1;
        real_data[0][i][2] = scaleValueToRange(real_data[0][i][2], propinfo->maxValueReal, propinfo->minValueReal, virtualRangeSize*0.5, sizeMargin);
        //En cas de que tinguem un 2n valor, també l'escalem de la mateixa manera.
        if(real_data[0][i][3] != -1){
        real_data[0][i][3] = scaleValueToRange(real_data[0][i][3], propinfo->maxValueReal, propinfo->minValueReal, virtualRangeSize*0.5, sizeMargin);
        }
    }
}

float SceneFactoryData::scaleValueToRange(float realValue, float realMax, float realMin, float virtualMax, float virtualMin){
    float realRange = realMax - realMin;
    float virtualRange = virtualMax - virtualMin;
    return (((realValue - realMin)*virtualRange) / realRange) + virtualMin;
}

void SceneFactoryData::set_base_object_y(QJsonObject jbase){
    cout <<"hi there"<<endl;

    if (sceneType == "FITTEDPLANE" || sceneType == "PLANE"){
        if (jbase.contains("point")) {
            base_object_y = jbase["point"].toArray()[1].toDouble();
        }
    }
    else if(sceneType == "SPHERE"){
        if (jbase.contains("radius")) {
            base_object_y = jbase["radius"].toDouble();
        }
        if (jbase.contains("center")) {
            QJsonArray auxVec = jbase["center"].toArray();
            centreBaseObject[0] = auxVec[0].toDouble();
            centreBaseObject[1] = auxVec[1].toDouble();
            centreBaseObject[2] = auxVec[2].toDouble();

        }else{
            centreBaseObject = vec3(0,0,0);
        }
    }
}




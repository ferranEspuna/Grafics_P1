#include <QVector>
#include <QVector3D>

#include "Mesh.h"

Mesh::Mesh(const QString &fileName): Object()
{
    nom = fileName;
    load(fileName);
}

Mesh::Mesh(const QString &fileName, float data): Object(data)
{
    nom = fileName;
    load(fileName);
}

Mesh::~Mesh() {
    if (cares.size() > 0) cares.clear();
    if (vertexs.size() > 0) vertexs.clear();
    if (triangles.size() > 0) triangles.clear();

}

void Mesh::construeix_triangles() {

    for(auto& c: cares){

        Triangle t = Triangle(
                    vec3(vertexs[c.idxVertices[0]]),
                    vec3(vertexs[c.idxVertices[1]]),
                    vec3(vertexs[c.idxVertices[2]]));


        t.setMaterial(material);
        triangles.push_back(t);

        if (c.idxVertices.size() == 4){

            Triangle t = Triangle(
                        vec3(vertexs[c.idxVertices[1]]),
                        vec3(vertexs[c.idxVertices[2]]),
                        vec3(vertexs[c.idxVertices[3]]));


            t.setMaterial(material);
            triangles.push_back(t);

        }

    }
}
void Mesh::construeix_triangles_amb_box(int nBoxes) {
    //Creem les mesures que ha de tenir cada box per x, y ,z.
    float xboxes = (maxP[0] - minP[0])/nBoxes;
    float yboxes = (maxP[1] - minP[1])/nBoxes;
    float zboxes = (maxP[2] - minP[2])/nBoxes;
    //Iniciem un comptador per numerar les box.
    int cont = 0;
    //Fem un triple bucle on:
        //les i miren les x
        //les j miren les y
        //les k miren les z
    for(int i=0; i<nBoxes; i++){
        for(int j=0; j<nBoxes; j++){
            for(int k=0; k<nBoxes; k++){
                //Creem el nou punt maxim i minim de la box segons els valors que ens proporcionen i,j,k i les mesures establertes.
                vec3 newMaxP = vec3(maxP[0]-i*xboxes+0.001,maxP[1]-j*yboxes+0.001,maxP[2]-k*zboxes+0.001);
                vec3 newMinP = vec3(maxP[0]-(i+1)*xboxes-0.001,maxP[1]-(j+1)*yboxes-0.001,maxP[2]-(k+1)*zboxes-0.001);
                //Construim la posició de la noster box per a poder mapejar-la posteriorment.
                ivec3 position = ivec3(i,j,k);
                vector<Triangle> vect;
                //Insertem la nova box en el nostre diccionari de boxes amb un numero com a clau associat a comptador.
                boxes.insert({cont,Box(newMinP,newMaxP,position)});
                //Insertem un vector al diccionari de triangles associats a cada box ja que te el mateix numero de clau.
                trianglesBoxes.insert({cont,vect});
                cont++;
            }
        }
    }

    for(auto& c: cares){

        //Es creen els triangles.
        Triangle t = Triangle(
                    vec3(vertexs[c.idxVertices[0]]),
                    vec3(vertexs[c.idxVertices[1]]),
                    vec3(vertexs[c.idxVertices[2]]));


        t.setMaterial(material);
        triangles.push_back(t);

        //Variables per a poder omplir les box intermitges segons la posició de les box en els vertex del triangle.
        ivec3 minBoxesPosition = ivec3(nBoxes,nBoxes,nBoxes);
        ivec3 maxBoxesPosition = ivec3(0,0,0);
        //Per cada box:
        for(int i=0;i<pow(nBoxes,3);i++){
            //Variable booleana que ens ajuda a no repetir un mateix triangle en un vector, ja que un triangle pot tenir 2 punts en un mateix box.
            bool added = false;
            //Per cada vertex
            for(int j=0; j<3; j++){
                //Comprobem que el triangle no ha estat afegit ja en aquesta box.
                if(added == false){
                    //En cas de que el vertex j es trobi a la box i
                    if(boxes.at(i).pointIn(vec3(vertexs[c.idxVertices[j]]))){
                        //Afegim el triangle a la llista de triangles associada a la box i.
                        trianglesBoxes.at(i).push_back(t);
                        //Marquem que ja hem afegit aquest triangle en aquesta box.
                        added = true;
                        //Actualitzem els index màxims i minims de mapeig de boxes.
                        if(minBoxesPosition[j] > boxes.at(i).position[j]){
                            minBoxesPosition[j] = boxes.at(i).position[j];
                        }
                        if(maxBoxesPosition[j] < boxes.at(i).position[j]){
                            maxBoxesPosition[j] = boxes.at(i).position[j];
                        }
                    }
                }
            }
        }
        //Un cop ja hem comprobar totes les caixes per cada un dels vertex.
        //Posem els triangles a les boxes intermitges entre les que hem afegit, ja que pot ser que
        //part del triangle es trobi en alters boxes que no siguin les dels seus vertex.
        for(int i=minBoxesPosition[0]; i<= maxBoxesPosition[0]; i++){
            for(int j=minBoxesPosition[1]; j<=maxBoxesPosition[1]; j++){
                for(int k=minBoxesPosition[2]; k<=maxBoxesPosition[2]; k++){
                    //Fem servir les posicions associades segons com hem construir l'ordre de la llista al principi del mètode.
                    int pos = i*pow(nBoxes,2)+j*nBoxes+k;
                    trianglesBoxes.at(pos).push_back(t);
                }
            }
        }

        //Repetició del procediment per als quadrilaters.
        if (c.idxVertices.size() == 4){

            Triangle t = Triangle(
                        vec3(vertexs[c.idxVertices[1]]),
                        vec3(vertexs[c.idxVertices[2]]),
                        vec3(vertexs[c.idxVertices[3]]));


            t.setMaterial(material);
            triangles.push_back(t);

            ivec3 minBoxesPosition = ivec3(nBoxes,nBoxes,nBoxes);
            ivec3 maxBoxesPosition = ivec3(0,0,0);
            for(int i=0;i<pow(nBoxes,3);i++){
                bool added = false;
                for(int j=0; j<3; j++){
                    if(added == false){
                        if(boxes.at(i).pointIn(vec3(vertexs[c.idxVertices[j+1]]))){
                            trianglesBoxes.at(i).push_back(t);
                            added = true;
                            if(minBoxesPosition[j] > boxes.at(i).position[j]){
                                minBoxesPosition[j] = boxes.at(i).position[j];
                            }
                            if(maxBoxesPosition[j] < boxes.at(i).position[j]){
                                maxBoxesPosition[j] = boxes.at(i).position[j];
                            }
                        }
                    }

                }
            }
            for(int i=minBoxesPosition[0]; i<= maxBoxesPosition[0]; i++){
                for(int j=minBoxesPosition[1]; j<=maxBoxesPosition[1]; j++){
                    for(int k=minBoxesPosition[2]; k<=maxBoxesPosition[2]; k++){
                        int pos = i*pow(nBoxes,2)+j*nBoxes+k;
                        trianglesBoxes.at(pos).push_back(t);
                    }
                }
            }

        }

    }
}

bool Mesh::closestHit(Ray &raig, HitInfo& info) {
    float closestT = raig.getTmax();

    //De moment, no tenim cap hit
    bool hit = false;
    //Per tant no hi ha informació del hit.
    //La tindrem en una variable temportal.
    HitInfo infoTemp;
    //Fem el bounding general per saber si el raig apunta cap a l'objecte.
    bool isInObject = generalBounding.hasHit(raig);
    if(isInObject){
        //En cas que tinguem el bounding multiple activat
        if(bounding){
            //Recorrem totes les caixes.
            for(int i=0; i<int(boxes.size()); i++){
                //En cas que xoquem amb la caixa.
                if(boxes.at(i).hasHit(raig)){
                    //Recorrem tots els triangles associats en aquesta.
                    for(auto t: trianglesBoxes.at(i)){
                        //Si el raig interseca amb l'objecte:
                        if(t.closestHit(raig, infoTemp)){

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
               }
            }
        }
        //En cas que no tinguem el bounding multiple activat, recorrem tots els triangles.
        else{
            //Recorrem els triangles
            for(auto& t: triangles){

                //Si el raig interseca amb l'objecte:
                if(t.closestHit(raig, infoTemp)){

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
        }



    }




    //Si en algun moment hem xocat, retornem true
    return hit;

}



bool Mesh::hasHit(const Ray& raig) {

    //Recorrem tots els objectes de l'escena
    for(auto& t : triangles){

        //Si algun d'ells interseca amb el raig, retornem true
        if(t.hasHit(raig)){
            return true;
        }
    }

    //Si hem recorregut tots els triangles i cap interseca, retornem false
    return false;

}

void Mesh::aplicaTG(shared_ptr<TG> t) {
    // TO DO: Fase 1
}

void Mesh::load (QString fileName) {
    QFile file(fileName);
    if(file.exists()) {
        minP = vec3(std::numeric_limits<float>::infinity(),std::numeric_limits<float>::infinity(),std::numeric_limits<float>::infinity());
        maxP = vec3(-std::numeric_limits<float>::infinity(),-std::numeric_limits<float>::infinity(),-std::numeric_limits<float>::infinity());
        if(file.open(QFile::ReadOnly | QFile::Text)) {
            while(!file.atEnd()) {
                QString line = file.readLine().trimmed();
                QStringList lineParts = line.split(QRegularExpression("\\s+"));
                if(lineParts.count() > 0) {
                    // if it’s a comment
                    if(lineParts.at(0).compare("#", Qt::CaseInsensitive) == 0)
                    {
                        // qDebug() << line.remove(0, 1).trimmed();
                    }

                    // if it’s a vertex position (v)
                    else if(lineParts.at(0).compare("v", Qt::CaseInsensitive) == 0)
                    {
                        float x = lineParts.at(1).toFloat();
                        float y = lineParts.at(2).toFloat();
                        float z = lineParts.at(3).toFloat();
                        vertexs.push_back(vec4(x,
                                               y,
                                               z, 1.0f));
                        //Preparing variables for general bounding.
                        if(minP[0]>x){
                            minP[0] = x;
                        }
                        if(maxP[0]<x){
                            maxP[0] = x;
                        }
                        if(minP[1]>y){
                            minP[1] = y;
                        }
                        if(maxP[1]<y){
                            maxP[1] = y;
                        }
                        if(minP[2]>z){
                            minP[2] = z;
                        }
                        if(maxP[2]<z){
                            maxP[2] = z;
                        }
                    }

                    // if it’s a normal (vn)
                    else if(lineParts.at(0).compare("vn", Qt::CaseInsensitive) == 0)
                    {

                    }
                    // if it’s a texture (vt)
                    else if(lineParts.at(0).compare("vt", Qt::CaseInsensitive) == 0)
                    {

                    }

                    // if it’s face data (f)
                    // there’s an assumption here that faces are all triangles
                    else if(lineParts.at(0).compare("f", Qt::CaseInsensitive) == 0)
                    {
                        Cara *cara = new Cara();

                        // get points from v array
                        cara->idxVertices.push_back(lineParts.at(1).split("/").at(0).toInt() - 1);
                        cara->idxVertices.push_back(lineParts.at(2).split("/").at(0).toInt() - 1);
                        cara->idxVertices.push_back(lineParts.at(3).split("/").at(0).toInt() - 1);

                        cares.push_back(*cara);
                    }
                }
            }
            file.close();
            //Creating generalBunding.
            generalBounding = Box(minP, maxP);
        } else {
            qWarning("Boundary object file can not be opened.");
        }
    } else  qWarning("Boundary object file not found.");
}

void Mesh::read (const QJsonObject &json)
{
    Object::read(json);
    if (json.contains("objFileName") && json["objFileName"].isString()) {
        nom = json["objFileName"].toString();
        load(nom);
    }
    if (json.contains("nBox") && json["nBox"].isDouble()) {
        nBox = json["nBox"].toInt();
        if(nBox > 1){
            bounding = true;
            construeix_triangles_amb_box(nBox);
        }
        else{
            construeix_triangles();
        }

    }
    else{
        construeix_triangles();
    }

}




//! [1]
void Mesh::write(QJsonObject &json) const
{
    Object::write(json);
    json["objFileName"] = nom;
}
//! [1]

void Mesh::print(int indentation) const
{
    Object::print(indentation);
    const QString indent(indentation * 2, ' ');
    QTextStream(stdout) << indent << "objFileName:\t" << nom << "\n";
}




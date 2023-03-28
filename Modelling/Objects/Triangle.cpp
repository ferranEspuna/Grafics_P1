#include "Triangle.h"

Triangle::Triangle(){
    vertices[0] = vec3(-1.f,-1.f,0.f);
    vertices[1] = vec3(1.f,-1.f,0.f);
    vertices[2] = vec3(0.f,2.f,0.f);
    calculaNormal();
}

Triangle::Triangle(vec3 p1, vec3 p2, vec3 p3){
    vertices[0] = p1;
    vertices[1] = p2;
    vertices[2] = p3;

    calculaNormal();
}

void Triangle::calculaNormal(){
    normal = normalize(cross(vertices[1] - vertices[0], vertices[2] - vertices[0]));
}

vec3 Triangle::getNormal(){
    return normal;
}


//Passem el vector vec a una base formada pels vectors que uneixen el punt de vista amb els vertices del triangle
//TODO descomentar les linies del metode per optimitzar-ho (no cal calcular la matriu cada vegada)
vec3 Triangle::coordinates(vec3 from, vec3 vec){

    //Si ens el mirem des d'un punt de vista nou, hem de fer tots els calculs (hauria de passar només una vegada per imatge renderitzada)
    if(from != this->lookFrom){

        //Actualitzem from
        this->lookFrom = from;

        vec3 v1 = vertices[0] - from;
        vec3 v2 = vertices[1] - from;
        vec3 v3 = vertices[2] - from;

        //TODO: i si la matriu no és invertible?
        coordinate_change = inverse(mat3(v1, v2, v3));

    }

    //Retornem vec canviat de base
    return coordinate_change * vec;
    //return inverse(canvi) * vec;


}
bool Triangle::hasHit (const Ray& r){


    vec3 c = coordinates(r.getOrigin(), r.getDirection());

    //El vector ha de ser al primer octant de la base v1, v2, v3 del metode anterior
    if(c.x < 0 || c.y < 0 || c.z < 0){
        return false;
    }

    //En aquesta base, el pla del triangle és x + y + z = 1. Calculem el valor de t per la interseccio.
    float t = 1 / (c.x + c.y + c.z);

    //Ha d'estar entre els valors mínim i màxim del raig
    return t >= r.getTmin() && t <= r.getTmax();

}

bool Triangle::closestHit(Ray& r, HitInfo& info) {

    vec3 c = coordinates(r.getOrigin(), r.getDirection());

    //El vector ha de ser al primer octant de la base v1, v2, v3 del metode anterior
    if(c.x < 0 || c.y < 0 || c.z < 0){
        return false;
    }

    //En aquesta base, el pla del triangle és x + y + z = 1. Calculem el valor de t per la interseccio.
    float t = 1 / (c.x + c.y + c.z);

    //Ha d'estar entre els valors mínim i màxim del raig
    if(t >= r.getTmin() && t <= r.getTmax()){

        info.t = t;
        info.p = r.pointAtParameter(info.t);

        //Corregim la normal si està al reves (ha de ser des d'on estem mirant)
        if(dot(r.getDirection(), normal) > 0){
            info.normal = -normal;
        }else{
            info.normal = normal;
        }

        info.mat_ptr = material.get();

        return true;
    }
    return false;
}

vec3 Triangle::getVertex(int i){
    //Retornem el vèrtex demanat
    return vertices[i];
}

void Triangle::aplicaTG(shared_ptr<TG> tg) {
    for(int i = 0; i < 3; i++){
        vertices[i] = vec3(tg->getTG() * vec4(vertices[i], 1.0));
    }
    calculaNormal();
}

void Triangle::read (const QJsonObject &json){

    Object::read(json);

    //Recorrem els punts del triangle
    for(int i = 0; i < 3; i++){

        //Nom del punt al json
        string name = "p" + to_string(i + 1);
        QString name2 = QString::fromStdString(name);

        //Si hi és la propietat:
        if (json.contains(name2) && json[name2].isArray()) {

            QJsonArray auxVec = json[name2].toArray();
            //Recorrem les components del punt i les assignem
            for(int j = 0; j < 3; j++){
                vertices[i][j] = auxVec[j].toDouble();
            }

        }
    }

    calculaNormal();
}

void Triangle::write(QJsonObject &json) const {

    Object::write(json);

    //Recorrem els punts del triangle
    for(int i = 0; i < 3; i++){

        //Array on hi posarem cada component del punt p_i
        QJsonArray auxVec;

        //Recorrem les components del punt i posem al array
        for(int j = 0; j < 3; j++){
            auxVec.append(vertices[i][j]);
        }

        //Nom del punt al json
        string name = "p" + to_string(i + 1);
        QString name2 = QString::fromStdString(name);

        //Posem el punt al json
        json[name2] = auxVec;

    }

}


void Triangle::print(int indentation) const {

    Object::print(indentation);

    const QString indent(indentation * 2, ' ');

    //Recorrem els punts
    for(int i = 0; i < 3; i++){
        //Escrivim les 3 components de cada punt
        QTextStream(stdout) << indent << "p" << i + 1 << ":\t" << vertices[i][0] << ", "<< vertices[i][1] << ", "<< vertices[i][2] << "\n";
    }

}


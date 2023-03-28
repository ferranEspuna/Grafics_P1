#include "PolynomialEq.h"
#include <iostream>

PolynomialEq::PolynomialEq(){


    //termes dels quadrats de x, y i z
    ivec3 xsquare = ivec3(2, 0, 0);
    terms.push_back(xsquare);
    coefficients.push_back(1.0f);

    ivec3 ysquare = ivec3(0, 2, 0);
    terms.push_back(ysquare);
    coefficients.push_back(1.0f);

    ivec3 zsquare = ivec3(0, 0, 2);
    terms.push_back(zsquare);
    coefficients.push_back(-1.0f);

    //terme connstant -1
    ivec3 one = ivec3(0, 0, 0);
    terms.push_back(one);
    coefficients.push_back(-1.0f);

    calculate_grad();

}


PolynomialEq::PolynomialEq(vector<ivec3> terms, vector<float> coefficients){
    this->terms = terms;
    this->coefficients = coefficients;
    calculate_grad();


}

//Ens calculem el gradient del polinomi
//Seran 3 polinomis: df/dx, df/dy i df/dz
void PolynomialEq::calculate_grad(){

    ivec3 current_term;
    ivec3 current_term_prime;

    //recorrem les 3 dimensions
    for(int dim_idx = 0; dim_idx < 3; dim_idx++){

        //Afegim la nova dimensió al gradient
        terms_grad.push_back(vector<ivec3>());
        coefficients_grad.push_back(vector<float>());

        //recorrem els termes del polinomi i derivem respecte la variable que toca
        for(int term_idx = 0; term_idx < terms.size(); term_idx++){

            current_term = terms.at(term_idx);

            //mirem l'exponent al terme de la variable
            int p = current_term[dim_idx];

            //Si és 0, el terme desapareix a la derivada resp. aquesta variable
            if(p > 0){

                //Multipliquem el terme per l'exponent que toqui
                coefficients_grad.at(dim_idx).push_back( ((float) p) * coefficients.at(term_idx));

                //Fem una còpia del terme al terme derivat
                current_term_prime = ivec3(0, 0, 0);
                for(int i = 0; i < 3; i++){
                    current_term_prime[i] = current_term[i];
                }

                //Restem 1 a l'exponent que toqui
                current_term_prime[dim_idx] = p - 1;

                //Afegim el terme a la df/dxi que toqui
                terms_grad.at(dim_idx).push_back(current_term_prime);

            }
        }
    }
}

//Mètode molt limitat, per polinomis de grau baix va prou bé
bool PolynomialEq::closestHit(Ray &raig, HitInfo& info) {

    float t = raig.getTmin();
    float f_init = evaluate_f(raig.pointAtParameter(t));

    for(int i = 0; i < 250; i++){

        vec3 p = raig.pointAtParameter(t);
        vec3 n = evaluate_grad(p);
        float f_p = evaluate_f(p);

        //hem convergit
        if(abs(f_p) < 0.0001){
            info.t = t;
            info.p = p;
            info.normal = normalize(n);
            info.mat_ptr = material.get();
            return true;
        }

        float der = dot(n, raig.getDirection());

        //si no, mirem si ens hem passat de tmax
        if(t > raig.getTmax()){
            return false;
        }

        float t_next;

        //si derivada 0, ens movem una mica
        if (abs(der) < 0.01) {
           t_next = t + 0.1;
        }else{

            //si no, apliquem newton rhapson
            t_next = t - f_p / der;

            //si sortim per darrera, provem un valor d'iteració inicial més endavant
            if(t_next < raig.getTmin()){
                t_next = t + 0.2 * der;
            }
        }
        t = t_next;
    }

    return false;
}

bool PolynomialEq::hasHit(const Ray& raig) {

    HitInfo inf;
    Ray r2 = raig;
    return closestHit(r2, inf);

}

//TODO
void PolynomialEq::aplicaTG(shared_ptr<TG> t) {

}

//TODO
void PolynomialEq::read(const QJsonObject &json){

    Object::read(json);

    if (json.contains("monomis")) {

        terms.clear();
        coefficients.clear();
        terms_grad.clear();
        coefficients_grad.clear();

        QJsonArray mon = json["monomis"].toArray();
        for(auto o: mon){

            QString s = o.toString();
            int i = 0;
            float sign = 1.f;

            if(s.at(0) == '-'){
                sign = -1.f;
                i++;
            }

            bool afterPoint = false;
            float c = 0;
            ivec3 term(0,0,0);
            int prec = -1;

            if(!s.at(i).isDigit()){
                c = 1.f;
            }

            while(true){


                if(i >= s.length()){break;}

                QChar ch = s.at(i);
                if(ch == '.'){afterPoint = true;}
                else if(ch.isDigit()){
                    int d = ch.digitValue();
                    if(!afterPoint){
                        c *= 10;
                        c += (float) d;
                    }
                    else{
                        c += d * pow(10.0f, (float) prec);
                        prec -= 1;
                    }
                }else{
                    break;
                }
                i++;
            }

            int index = 0;

            while(true){

                if(i >= s.length()){break;}

                QChar ch = s.at(i);
                if(ch.isDigit()){
                    int d = ch.digitValue();
                    term[index] = d;
                }else{
                    if(ch == 'x'){
                        index = 0;
                    }if(ch == 'y'){
                        index = 1;
                    }if(ch == 'z'){
                        index = 2;
                    }

                    if(i+1 == s.length()){
                        term[index] += 1;
                    }

                    else if(!s.at(i+1).isDigit()){
                        term[index] += 1;
                    }
                }
                i++;
            }

            terms.push_back(term);
            coefficients.push_back(c*sign);


        }
        calculate_grad();

    }
}

//TODO
void PolynomialEq::write(QJsonObject &json) const
{
}

//TODO
void PolynomialEq::print(int indentation) const
{

}

float PolynomialEq::evaluate(vector<ivec3> t, vector<float> c, vec3 p){

    float totalSum = 0;
    for(int i = 0; i < c.size(); i++){

        double totalProd = c.at(i);
        for(int dim = 0; dim < 3; dim++){
            for(int j = 0; j < t.at(i)[dim]; j++){
                totalProd *= p[dim];
            }
        }

        totalSum += totalProd;
    }

    return totalSum;

}

float PolynomialEq::evaluate_f(vec3 p){
    return evaluate(terms, coefficients, p);
}

vec3 PolynomialEq::evaluate_grad(vec3 p){

    vec3 ret(0, 0, 0);

    for(int i = 0; i < 3; i++){
        ret[i] = evaluate(terms_grad[i], coefficients_grad[i], p);

    }

    return ret;

}

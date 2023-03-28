/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#ifndef PolynomialEqH
#define PolynomialEqH

#include "Object.h"
#include "Modelling/Animation.h"
#include "Modelling/TG/TranslateTG.h"
#include "Modelling/TG/ScaleTG.h"

class PolynomialEq: public Object {
public:
    PolynomialEq();
    PolynomialEq(vector<ivec3> terms, vector<float> coefficients);
    virtual ~PolynomialEq() {}
    virtual bool closestHit(Ray& r, HitInfo& info) override;
    virtual bool hasHit(const Ray& r) override;
    virtual void aplicaTG(shared_ptr<TG> tg) override;

    virtual void read (const QJsonObject &json) override;
    virtual void write(QJsonObject &json) const override;
    virtual void print(int indentation) const override;

private:

    int degree;

    vector<ivec3> terms;
    vector<float> coefficients;

    vector<vector<ivec3>> terms_grad;
    vector<vector<float>> coefficients_grad;

    float evaluate(vector<ivec3> t, vector<float> c, vec3 p);
    float evaluate_f(vec3 p);
    vec3 evaluate_grad(vec3 p);
    void calculate_grad();



};


#endif




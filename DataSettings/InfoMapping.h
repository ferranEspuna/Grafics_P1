#pragma once


#include <QFile>
#include <QTextStream>
#include <iostream>
#include <sstream>
#include <QObject>
#include <QString>
#include <QStringList>

#include <vector>
#include "glm/glm.hpp"
#include "Modelling/Scene.h"
#include "Factories/ObjectFactory.h"
#include "Modelling/Materials/ColorMapStatic.h"
#include "Modelling/Materials/Texture.h"
#include "Factories/MaterialFactory.h"
#include "PropertyInfo.h"

/* InfoMapping
 * Classe que guarda les dimensions del mon real i del mon virtual per
 * i les informació de les dades
 * per a cada propietat guarda les seves caracteristiques a propInfo i
 * el vector de les dades mostrejades d'aquella propietat
 * per a cada dada mostrejada es te (coordenadaX, coordenadaZ, valor)
 * con les coordenades es suposen mostrejades en un mapa
*/

class InfoMapping: public Serializable {
public:
    InfoMapping() {};

    virtual void read(const QJsonObject &json) override;
    virtual void write(QJsonObject &json) const override;
    virtual void print(int indentation) const override;

    virtual ~InfoMapping() {};

    float               Rxmin, Rxmax, Rzmin, Rzmax;
    float               Vxmin, Vxmax, Vymin, Vymax, Vzmin, Vzmax;

    // vector de parells <PropertyInfo, vector de  mostres d'aquella propietat>
    vector<pair<shared_ptr<PropertyInfo>, vector<vec4>>> props;
    bool valorDoble;


};


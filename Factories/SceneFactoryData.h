#pragma once

#include "Modelling/Materials/ColorMapStatic.h"
#include "Modelling/Materials/Material.h"
#include "SceneFactory.h"
#include "Factories/MaterialFactory.h"
#include "DataSettings/InfoMapping.h"

class SceneFactoryData : public SceneFactory
{
    shared_ptr<InfoMapping> mapping;

public:
    SceneFactoryData() {};
    SceneFactoryData(shared_ptr<InfoMapping> mapping);
    virtual shared_ptr<Scene>  createScene(Serializable::SaveFormat saveFormat, QString nomFitxer) override;
    virtual shared_ptr<Scene>  createScene() override;

    virtual void read(const QJsonObject &json) override;
    virtual void write(QJsonObject &json) const override;
    virtual void print(int indentation) const override;

    bool load(Serializable::SaveFormat saveFormat, QString nameFile);
    bool save(Serializable::SaveFormat saveFormat, QString nameFile) const;

    bool doubleValue;
    QString sceneType;
    float base_object_y = 0;
    vec3 centreBaseObject;
    void set_base_object_y(QJsonObject jbase);

    vec3 getPuntBase(ObjectFactory::OBJECT_TYPES gyzmo, vec2 puntReal);
    shared_ptr<Object> transformaObjecte(QString s, glm::detail::tvec3<float> data, vec3 coord, shared_ptr<Object> o);
    // TODO: Fase 2: Translada del Gizmo
    // shared_ptr<TranslateTG> translacioGizmo(shared_ptr<PropertyInfo>  propinfo, vec3 puntMonReal);
    // TODO: Fase 2: Escala del Gizmo
    // shared_ptr<ScaleTG>     escalatGizmo(shared_ptr<PropertyInfo> propinfo, float valorMonReal);
    shared_ptr<Material>    mapeigMaterial(shared_ptr<PropertyInfo> propinfo, ColorMapStatic::COLOR_MAP_TYPES tCM, MaterialFactory::MATERIAL_TYPES tMat, double valorMonReal);

    shared_ptr<Scene> buildVirtualScene();
    void scaleRealDataToVirtual(std::vector<glm::detail::tvec4<float>> *real_data, std::shared_ptr<PropertyInfo> propinfo);
    float scaleValueToRange(float realValue, float realMax, float realMin, float virtualMax, float virtualMin);
};



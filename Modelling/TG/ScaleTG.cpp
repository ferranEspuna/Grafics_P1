#include "ScaleTG.h"

ScaleTG::ScaleTG(float value): scaling(value)
{
    matTG = glm::scale(glm::mat4(1.0f), glm::vec3(value));
}

ScaleTG::~ScaleTG()
{

}

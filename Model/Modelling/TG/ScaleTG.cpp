#include "ScaleTG.hh"

ScaleTG::ScaleTG(glm::vec3 scal): scaling(scal)
{
    matTG = glm::scale(glm::mat4(1.0f), scaling);
}

ScaleTG::~ScaleTG()
{

}

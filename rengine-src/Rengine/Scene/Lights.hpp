#pragma once
#include "Rengine/Core/core.hpp"
#include <glm/glm.hpp>

namespace Rengin
{
class Lights
{
public:
    glm::vec3 LightPos,LightIntensity,LookAtPoint;
public:
    Lights(/* args */);
    ~Lights();
};


} // namespace Rengin

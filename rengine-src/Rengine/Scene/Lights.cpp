#include "repch.hpp"
#include "Lights.hpp"

namespace Rengin
{

Lights::Lights(/* args */)
{
    LightPos = glm::vec3(0,80,80);
    // LightIntensity = glm::vec3(1,5000,5000);
    LightIntensity = glm::vec3(5000,5000,5000);
}

Lights::~Lights()
{
}

} // namespace Rengin

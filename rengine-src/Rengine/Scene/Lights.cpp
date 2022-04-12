#include "repch.hpp"
#include "Lights.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Rengin
{

Lights::Lights()
{
    LightPos = glm::vec3(0,80,80);
    LightIntensity = glm::vec3(5000,5000,5000);
    LookAtPoint = glm::vec3(0.0,0.0,0.0);
    Direction = LookAtPoint - LightPos;
    WorldUp = glm::vec3(0.0,1.0,0.0);
    Bound = glm::vec3(600.f,600.f,600.f);
    UpdateView();
    UpdateProjection();
}

Lights::~Lights()
{
}


void Lights::UpdateProjection()
{
    m_Projection = glm::ortho(-Bound.x / 2,Bound.x / 2,-Bound.y / 2,Bound.y / 2,-Bound.z / 2,Bound.z / 2);
}

void Lights::UpdateView()
{
    m_ViewMatrix = glm::lookAt(LightPos,LookAtPoint,WorldUp);
}

} // namespace Rengin

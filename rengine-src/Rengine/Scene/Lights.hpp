#pragma once
#include "Rengine/Core/core.hpp"
#include <glm/glm.hpp>
#include "Rengine/Renderer/Buffer.hpp"

namespace Rengin
{
class Lights
{
public:
    glm::vec3 LightPos,LightIntensity,LookAtPoint,WorldUp,Bound,Direction;
    glm::mat4 m_Projection,m_ViewMatrix;
    uint32_t LightTriNum;
public:
    Lights(/* args */);
    ~Lights();

    const glm::mat4 &GetViewMatrix() const { return m_ViewMatrix; }
    const glm::mat4 &GetProjectionMatrix() const { return m_Projection; }
    glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

private:
    void UpdateProjection();
    void UpdateView();

};

} // namespace Rengin

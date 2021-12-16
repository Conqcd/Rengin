#pragma once
#include "Rengine/core.hpp"
#include <glm/glm.hpp>

namespace Rengin
{
class RE_API OrthoGraphicsCamera
{
private:
    glm::mat4 m_projection_matrix;
    glm::mat4 m_view_matrix;
    glm::mat4 m_view_projection_matrix;

    glm::vec3 m_position;
    float m_rotation = 0.0f;
private:
    void ReCalculateView();
public:
    OrthoGraphicsCamera(float left,float right,float bottom,float top);
    ~OrthoGraphicsCamera();

    //Set
    void SetPosition(const glm::vec3& position);
    void SetRotation(const float& rotation);
    
    //Get
    const glm::vec3& GetPosition()const;
    float GetRotation()const;
    
    const glm::mat4& GetViewMatrix()const;
    const glm::mat4& GetProjectionMatrix()const;
    const glm::mat4& GetViewProjectionMatrix()const;
};


    
} // namespace Rengin

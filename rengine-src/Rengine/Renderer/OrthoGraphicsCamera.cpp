#include "repch.hpp"
#include "OrthoGraphicsCamera.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Rengin
{

void OrthoGraphicsCamera::SetPosition(const glm::vec3& position)
{
    m_position = position;
    ReCalculateView();
}

void OrthoGraphicsCamera::SetRotation(const float& rotation)
{
    m_rotation = rotation;
    ReCalculateView();
}

void OrthoGraphicsCamera::SetProjection(float left,float right,float bottom,float top)
{
    m_projection_matrix = glm::ortho(left,right,bottom,top,-1.0f,1.0f);
    m_view_projection_matrix = m_projection_matrix * m_view_matrix;
}

OrthoGraphicsCamera::OrthoGraphicsCamera(float left,float right,float bottom,float top)
    : m_projection_matrix(glm::ortho(left,right,bottom,top,-1.0f,1.0f))
{
    ReCalculateView();
}

OrthoGraphicsCamera::~OrthoGraphicsCamera()
{
}

void OrthoGraphicsCamera::ReCalculateView()
{
    glm::mat4 transform = glm::translate(glm::mat4(1.0f),m_position) * glm::rotate(glm::mat4(1.0f),glm::radians(m_rotation),glm::vec3(0,0,1));
    m_view_matrix = glm::inverse(transform);
    m_view_projection_matrix = m_projection_matrix * m_view_matrix;
}

const glm::vec3& OrthoGraphicsCamera::GetPosition()const
{
    return m_position;
}

float OrthoGraphicsCamera::GetRotation()const
{
    return m_rotation;
}

const glm::mat4& OrthoGraphicsCamera::GetViewMatrix()const
{
    return m_view_matrix;
}

const glm::mat4& OrthoGraphicsCamera::GetProjectionMatrix()const
{
    return m_projection_matrix;
}

const glm::mat4& OrthoGraphicsCamera::GetViewProjectionMatrix()const
{
    return m_view_projection_matrix;
}

} // namespace Rengin

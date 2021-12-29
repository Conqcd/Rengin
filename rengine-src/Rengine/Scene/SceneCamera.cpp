#include "repch.hpp"
#include "SceneCamera.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Rengin
{

SceneCamera::SceneCamera(/* args */)
{
    ReCalculateProjection();
}

void SceneCamera::SetOrthographics(float size,float nearClip,float farClip)
{
    m_OrthographicsSize = size;
    m_OrthographicsNear = nearClip;
    m_OrthographicsFar = farClip;
    ReCalculateProjection();
}

void SceneCamera::SetViewportSize(uint32_t width,uint32_t height)
{
    m_AspectRatio = static_cast<float>(width) / height;
    ReCalculateProjection();
}

void SceneCamera::ReCalculateProjection()
{
    float orthoLeft = -m_OrthographicsSize * m_AspectRatio * 0.5f;
    float orthoRight = m_OrthographicsSize * m_AspectRatio * 0.5f;
    float orthoBottom = -m_OrthographicsSize * 0.5f;
    float orthoTop = m_OrthographicsSize * 0.5f;

    m_Projection = glm::ortho(orthoLeft,orthoRight,
    orthoBottom,orthoTop,m_OrthographicsNear,m_OrthographicsFar);
}

} // namespace Rengin

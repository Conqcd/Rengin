#pragma once
#include "Rengine/Core/core.hpp"
#include "Rengine/Renderer/Camera.hpp"

namespace Rengin
{
class RE_API SceneCamera : public Camera
{
private:
    float m_OrthographicsSize = 10.0f;
    float m_OrthographicsNear = -1.0f;
    float m_OrthographicsFar = 1.0f;
    float m_AspectRatio = 0.0f;
private:
    void ReCalculateProjection();
public:
    SceneCamera(/* args */);
    virtual ~SceneCamera() = default;

    void SetOrthographics(float size,float nearClip,float farClip);
    void SetViewportSize(uint32_t width,uint32_t height);

    float GetOrthographicsSize()const {return m_OrthographicsSize;}
    void SetOrthographicsSize(float orthographicsSize){ m_OrthographicsSize = orthographicsSize;}
};


} // namespace Rengin

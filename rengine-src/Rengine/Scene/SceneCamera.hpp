#pragma once
#include "Rengine/Core/core.hpp"
#include "Rengine/Renderer/Camera.hpp"

namespace Rengin
{
class RE_API SceneCamera : public Camera
{
public:
    enum class ProjectionType {Perspective = 0,Orthographics = 1};
private:
    ProjectionType m_ProjectionType = ProjectionType::Orthographics;
    
    float m_OrthographicsSize = 10.0f;
    float m_OrthographicsNear = -1.0f;
    float m_OrthographicsFar = 1.0f;

    float m_PerspectiveFOV = glm::radians(45.f);
    float m_PerspectiveNear = 0.01f;
    float m_PerspectiveFar = 1000.0f;

    float m_AspectRatio = 0.0f;
private:
    void ReCalculateProjection();
public:
    SceneCamera(/* args */);
    virtual ~SceneCamera() = default;

    void SetOrthographics(float size,float nearClip,float farClip);
    void SetPerspective(float fov,float nearClip,float farClip);

    void SetViewportSize(uint32_t width,uint32_t height);


    ProjectionType GetProjectionType()const {return m_ProjectionType;}
    void SetProjectionType(ProjectionType type) { m_ProjectionType = type; ReCalculateProjection();}
    
    float GetOrthographicsSize()const {return m_OrthographicsSize;}
    void SetOrthographicsSize(float orthographicsSize){ m_OrthographicsSize = orthographicsSize;}

    float GetOrthographicsNearClip()const {return m_OrthographicsNear;}
    void SetOrthographicsNearClip(float _near) { m_OrthographicsNear = _near; ReCalculateProjection();}

    float GetOrthographicsFarClip()const {return m_OrthographicsFar;}
    void SetOrthographicsFarClip(float _far) { m_OrthographicsFar = _far; ReCalculateProjection();}
    
    float GetPerspectiveFOV()const {return m_PerspectiveFOV;}
    void SetPerspectiveFOV(float fov){ m_PerspectiveFOV = fov;}

    float GetPerspectiveNearClip()const {return m_PerspectiveNear;}
    void SetPerspectiveNearClip(float _near) { m_PerspectiveNear = _near; ReCalculateProjection();}

    float GetPerspectiveFarClip()const {return m_PerspectiveFar;}
    void SetPerspectiveFarClip(float _far) { m_PerspectiveFar = _far; ReCalculateProjection();}
};


} // namespace Rengin

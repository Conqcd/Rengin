#pragma once
#include "Rengine/Core/core.hpp"
#include "OrthoGraphicsCamera.hpp"
#include "Rengine/Core/TimeStep.hpp"
#include "Rengine/Event/ApplicationEvent.hpp"
#include "Rengine/Event/MouseEvent.hpp"
#include <glm/glm.hpp>

namespace Rengin
{

class RE_API OrthoGraphicsCameraController
{
public:
    explicit OrthoGraphicsCameraController(float aspectRatio,bool rotation = false);

    inline const OrthoGraphicsCamera& getCamera()const {return m_camera;}
    inline OrthoGraphicsCamera& getCamera() {return m_camera;}
    void OnUpdate(TimeStep ts);
    void OnEvent(Event& ev);

    void setZoomLevel(float level);
    inline float getZoomLevel() const {return m_zoomLevel;}
private:
    bool OnMouseScrolled(MouseScrolledEvent& ev);
    bool OnWindowResized(WindowResizeEvent& ev);
private:
    float m_aspectRatio;
    float m_zoomLevel = 1.0f;
    bool m_is_Rotation;

    glm::vec3 m_position = {0.0f,0.0f,0.0f};
    float m_rotation = 0.0f;

    float m_moveSpeed = 1.0f;
    float m_rotateSpeed = 30.0f;
    OrthoGraphicsCamera m_camera;
};



} // namespace Rengin

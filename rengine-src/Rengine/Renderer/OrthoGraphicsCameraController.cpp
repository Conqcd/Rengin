#include "repch.hpp"
#include "OrthoGraphicsCameraController.hpp"
#include "Rengine/Core/KeyCodes.hpp"
#include "Rengine/Event/Event.hpp"
#include "Rengine/Core/Input.hpp"

namespace Rengin
{
    
OrthoGraphicsCameraController::OrthoGraphicsCameraController(float aspectRatio,bool rotation)
    :m_aspectRatio(aspectRatio),m_is_Rotation(rotation),m_camera(-m_aspectRatio * m_zoomLevel,m_aspectRatio * m_zoomLevel,-m_zoomLevel, m_zoomLevel)
{
}

void OrthoGraphicsCameraController::OnUpdate(TimeStep ts)
{
    RE_PROFILE_FUNCTION();
    
    if(Input::isKeyPressed(RE_KEY_A))
        m_position.x += m_moveSpeed * ts;
    else if(Input::isKeyPressed(RE_KEY_D))
        m_position.x -= m_moveSpeed * ts;

    if(Input::isKeyPressed(RE_KEY_W))
        m_position.y -= m_moveSpeed * ts;
    else if(Input::isKeyPressed(RE_KEY_S))
        m_position.y += m_moveSpeed * ts;

    m_camera.SetPosition(m_position);
    if(m_is_Rotation)
    {
        if(Input::isKeyPressed(RE_KEY_Q))
            m_rotation += m_rotateSpeed * ts;
        else if(Input::isKeyPressed(RE_KEY_E))
            m_rotation -= m_rotateSpeed * ts;
        m_camera.SetRotation(m_rotation);
    }
}

void OrthoGraphicsCameraController::OnEvent(Event& ev)
{
    RE_PROFILE_FUNCTION();
    
    EventDispatcher dispatcher(ev);
    dispatcher.Dispatch<MouseScrolledEvent>(RE_BIND_FUNC_EVENT_1(OrthoGraphicsCameraController::OnMouseScrolled));
    dispatcher.Dispatch<WindowResizeEvent>(RE_BIND_FUNC_EVENT_1(OrthoGraphicsCameraController::OnWindowResized));
}

void OrthoGraphicsCameraController::OnResize(float width,float height)
{
    m_aspectRatio = static_cast<float>(width)/static_cast<float>(height);
    CalculateView();
}

void OrthoGraphicsCameraController::setZoomLevel(float level)
{
    m_zoomLevel = level;
    CalculateView();
}

void OrthoGraphicsCameraController::CalculateView()
{
    m_camera.SetProjection(-m_aspectRatio * m_zoomLevel,m_aspectRatio * m_zoomLevel,-m_zoomLevel, m_zoomLevel);
}

bool OrthoGraphicsCameraController::OnMouseScrolled(MouseScrolledEvent& ev)
{
    RE_PROFILE_FUNCTION();
    
    m_zoomLevel -= ev.getYoffset() * 0.25f;
    m_zoomLevel = std::max(m_zoomLevel,0.25f);
    
    CalculateView();
    return false;
}

bool OrthoGraphicsCameraController::OnWindowResized(WindowResizeEvent& ev)
{
    RE_PROFILE_FUNCTION();
    
    OnResize(static_cast<float>(ev.getWidth()),static_cast<float>(ev.getHeight()));
    return false;
}

} // namespace Rengin


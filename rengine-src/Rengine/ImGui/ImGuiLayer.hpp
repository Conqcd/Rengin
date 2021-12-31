#pragma once
#include "Rengine/Core/core.hpp"
#include "Rengine/Core/Layer.hpp"
#include "Rengine/Event/Event.hpp"
#include "Rengine/Event/KeyEvent.hpp"
#include "Rengine/Event/MouseEvent.hpp"
#include "Rengine/Event/ApplicationEvent.hpp"

namespace Rengin
{

class RE_API ImGuiLayer :public Layer
{
private:
    float m_time = 0.0f;
    bool m_BlockImGuiEvents = true;

public:
    ImGuiLayer(/* args */);
    ~ImGuiLayer();

    void SetDarkThemeColors();
    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnEvent(Event& ev)override;

    virtual void OnImGuiRender() override;

    void Begin();
    void End();

    void SetBlockEvents(bool blockEvent) {m_BlockImGuiEvents = blockEvent;}
};

} // namespace Rengin

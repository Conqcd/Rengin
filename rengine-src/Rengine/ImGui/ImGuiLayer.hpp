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
public:
    ImGuiLayer(/* args */);
    ~ImGuiLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnImGuiRender() override;

    void Begin();
    void End();

};

} // namespace Rengin

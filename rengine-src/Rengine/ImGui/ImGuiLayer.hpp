#pragma once
#include "../../repch.hpp"
#include "../core.hpp"
#include "../Layer.hpp"
#include "../Event/Event.hpp"
#include "../Event/KeyEvent.hpp"
#include "../Event/MouseEvent.hpp"
#include "../Event/ApplicationEvent.hpp"

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

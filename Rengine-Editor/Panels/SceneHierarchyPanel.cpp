#include <rengine.hpp>
#include "SceneHierarchyPanel.hpp"
#include "imgui.h"

namespace Rengin
{

SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
{
    SetContext(context);
}

void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
{
    m_Context = context;
}

void SceneHierarchyPanel::OnImGuiRender()
{
    ImGui::Begin("Scene Hierarchy");

    m_Context->m_registry.each([](auto entity)
    {
        auto& tc = m_Context->m_registry.get<TagComponent>(entity);

        ImGui::Text("%s",tc.Tag);
    });

    ImGui::End();
}

} // namespace Rengin

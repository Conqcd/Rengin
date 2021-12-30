#include <rengine.hpp>
#include "SceneHierarchyPanel.hpp"
#include "imgui.h"
#include "Rengine/Scene/Component.hpp"

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

    m_Context->m_registry.each([&](auto entityID)
    {
        Entity entity{entityID,m_Context.get()};
        
        DrawEntityNode(entity);
    });

    ImGui::End();
}

void SceneHierarchyPanel::DrawEntityNode(Entity entity)
{
    auto& tag = entity.GetComponent<TagComponent>().Tag;

    ImGuiTreeNodeFlags flags = (m_SelectionContext == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow; 
    bool opened = ImGui::TreeNodeEx((void*)(uint32_t)entity,flags,tag.c_str());

    if (ImGui::IsItemClicked())
    {
        m_SelectionContext = entity;
    }

    if (opened)
    {
        ImGui::TreePop();
    }
    
}

} // namespace Rengin

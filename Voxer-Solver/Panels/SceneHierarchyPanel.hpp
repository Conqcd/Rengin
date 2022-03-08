#pragma once
#include "Rengine/Core/core.hpp"
#include "Rengine/Scene/Scene.hpp"
#include "Rengine/Scene/Entity.hpp"
#include <vector>
namespace Rengin
{
class SceneHierarchyPanel
{
private:
    Ref<Scene> m_Context;
    Entity m_VolomeEntity;
    std::vector<int> m_PickedPixels;
public:
    SceneHierarchyPanel(const Ref<Scene>& scene);
    SceneHierarchyPanel() = default;

    void SetContext(const Ref<Scene>& scene);

    void OnImGuiRender();
    Entity GetSelectedEntity() const {return m_VolomeEntity;}
private:
    void DrawEntityNode(Entity entity);
    void DrawComponents(Entity entity);

    friend class EditorLayer;
};


} // namespace Rengin

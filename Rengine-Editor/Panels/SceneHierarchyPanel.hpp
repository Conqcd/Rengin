#pragma once

#include "Rengine/Core/core.hpp"
#include "Rengine/Scene/Scene.hpp"

namespace Rengin
{
class SceneHierarchyPanel
{
private:
    Ref<Scene> m_Context;
public:
    SceneHierarchyPanel(const Ref<Scene>& scene);
    SceneHierarchyPanel() = default;

    void SetContext(const Ref<Scene>& scene);

    void OnImGuiRender();


};


} // namespace Rengin

#pragma once
#include "Rengine/Core/core.hpp"
#include "Rengine/Scene/Scene.hpp"
#include "Rengine/Scene/Entity.hpp"
#include "Rengine/Utils/ExternalExe.hpp"
#include <vector>
namespace Rengin
{
class SceneHierarchyPanel
{
private:
    Ref<Scene> m_Context;
    Ref<ExternalExe> m_ExternalProcess = ExternalExe::Create();
    Entity m_VolomeEntity,m_SelectionContext;
    std::vector<int> m_PickedPixels;
public:
    SceneHierarchyPanel(const Ref<Scene>& scene);
    SceneHierarchyPanel() = default;

    void SetContext(const Ref<Scene>& scene);

    void OnImGuiRender();
    Entity GetSelectedEntity() const {return m_SelectionContext;}
private:
    void DrawEntityNode(Entity entity);
    void DrawComponents(Entity entity);
    void AddAttribute(std::vector<int>& v,std::vector<float>& target,float x,float y,float z,int width,int height,int depth);
    void ClearAttribute(std::vector<int>& v,std::vector<float>& target,float x,float y,float z,int width,int height,int depth);
    void SaveIntFile(Ref<Texture3D> model, Ref<Texture3D> force,Ref<Texture3D> constraint,int width,int height,int depth);

    friend class EditorLayer;
};


} // namespace Rengin

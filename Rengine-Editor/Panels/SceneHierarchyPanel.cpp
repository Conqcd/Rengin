#include <rengine.hpp>
#include "SceneHierarchyPanel.hpp"
#include "imgui.h"
#include "Rengine/Scene/Component.hpp"
#include <glm/gtc/type_ptr.hpp>

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

    if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
        m_SelectionContext = {};
    ImGui::End();

    ImGui::Begin("Proterties");
    if (m_SelectionContext)
    {
        DrawComponents(m_SelectionContext);
    }
    
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

void SceneHierarchyPanel::DrawComponents(Entity entity)
{
    if(entity.HasComponent<TagComponent>())
    {
        auto& tag = entity.GetComponent<TagComponent>().Tag;

        char buffer[256];
        memset(buffer,0,sizeof(buffer));
        strcpy_s(buffer,sizeof(buffer),tag.c_str());
        if(ImGui::InputText("Tag",buffer,sizeof(buffer)))
        {
            tag = std::string(buffer);
        }
    }
    
    if(entity.HasComponent<TransformComponent>())
    {
        if(ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(),ImGuiTreeNodeFlags_DefaultOpen,"Transform"))
        {
            auto& transform = entity.GetComponent<TransformComponent>().Transform;

            ImGui::DragFloat3("Position",glm::value_ptr(transform[3]),0.5f);
            ImGui::TreePop();
        }
    }

    if(entity.HasComponent<CameraComponent>())
    {
        if(ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(),ImGuiTreeNodeFlags_DefaultOpen,"Camera"))
        {
            auto& cameraComponent = entity.GetComponent<CameraComponent>();
            auto& camera = cameraComponent.Camera;
            
            ImGui::Checkbox("Primary",&cameraComponent.Primary);
            
            const char* prjectionTypeStrings[] = {"Perspective","Orthographics"};
            const char* currentProjectionTypeString = prjectionTypeStrings[(int)camera.GetProjectionType()];

            if(ImGui::BeginCombo("Projection",currentProjectionTypeString))
            {
                for (int i = 0; i < 2; i++)
                {
                    bool isSelected = currentProjectionTypeString == prjectionTypeStrings[i];
                    if(ImGui::Selectable(prjectionTypeStrings[i],isSelected))
                    {
                        currentProjectionTypeString = prjectionTypeStrings[i];
                        camera.SetProjectionType(static_cast<SceneCamera::ProjectionType>(i));
                    }

                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
            {
                float perspectiveFOV = glm::degrees(camera.GetPerspectiveFOV());
                if(ImGui::DragFloat("FOV",&perspectiveFOV))
                    camera.SetPerspectiveFOV(glm::radians(perspectiveFOV));

                float perspectiveNearClip = camera.GetPerspectiveNearClip();
                if(ImGui::DragFloat("Near",&perspectiveNearClip))
                    camera.SetPerspectiveNearClip(perspectiveNearClip);

                float perspectiveFarClip = camera.GetPerspectiveFarClip();
                if(ImGui::DragFloat("Far",&perspectiveFarClip))
                    camera.SetPerspectiveFarClip(perspectiveFarClip);
            }
            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographics)
            {
                float orthoSize = camera.GetOrthographicsSize();
                if(ImGui::DragFloat("Size",&orthoSize))
                    camera.SetOrthographicsSize(orthoSize);

                float orthoNearClip = camera.GetOrthographicsNearClip();
                if(ImGui::DragFloat("Near",&orthoNearClip))
                    camera.SetOrthographicsNearClip(orthoNearClip);

                float orthoFarClip = camera.GetOrthographicsFarClip();
                if(ImGui::DragFloat("Far",&orthoFarClip))
                    camera.SetOrthographicsFarClip(orthoFarClip);

                ImGui::Checkbox("Fixed Aspect Ratio",&cameraComponent.FixedAspectRatio);
            }
            
            ImGui::TreePop();
        }
    }


    if (entity.HasComponent<SpriteRendererComponent>())
    {
        if(ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(),ImGuiTreeNodeFlags_DefaultOpen,"Sprite Renderer"))
        {
            auto& src = entity.GetComponent<SpriteRendererComponent>();
            ImGui::ColorEdit4("Color",glm::value_ptr(src.Color));

            ImGui::TreePop();
        }
    }
    
}

} // namespace Rengin

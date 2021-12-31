#include <rengine.hpp>
#include "SceneHierarchyPanel.hpp"
#include <imgui.h>
#include <imgui_internal.h>
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

    //Right Click On Right Space
    if (ImGui::BeginPopupContextWindow(0,1,false))
    {
        if(ImGui::MenuItem("Create Empty Entity"))
            m_Context->CreateEntity("Empty Entity");
        ImGui::EndPopup();
    }
    

    ImGui::End();

    ImGui::Begin("Proterties");

    if (m_SelectionContext)
    {
        DrawComponents(m_SelectionContext);

        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("AddComponent");
        
        if (ImGui::BeginPopup("AddComponent"))
        {
            if (ImGui::MenuItem("Camera"))
            {
                m_SelectionContext.AddComponent<CameraComponent>();
                ImGui::CloseCurrentPopup();
            }
         
            if (ImGui::MenuItem("Sprite Render"))
            {
                m_SelectionContext.AddComponent<SpriteRendererComponent>();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
        
    }
    
    ImGui::End();
}

void SceneHierarchyPanel::DrawEntityNode(Entity entity)
{
    auto& tag = entity.GetComponent<TagComponent>().Tag;

    ImGuiTreeNodeFlags flags = (m_SelectionContext == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    bool opened = ImGui::TreeNodeEx((void*)(uint32_t)entity,flags,tag.c_str());

    if (ImGui::IsItemClicked())
        m_SelectionContext = entity;
    
    bool entityDeleted = false;
    if (ImGui::BeginPopupContextItem())
    {
        if(ImGui::MenuItem("Delete Entity"))
            entityDeleted = true;
        ImGui::EndPopup();
    }

    if (opened)
        ImGui::TreePop();

    if(entityDeleted)
    {
        m_Context->DestroyEntity(entity);
        if (m_SelectionContext == entity)
            m_SelectionContext = {};
    }
}

static void DrawVec3Control(const std::string& label,glm::vec3& values,float resetValue = 0.f,float columnWidth = 100.f)
{
    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0,columnWidth);
    ImGui::Text(label.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3,ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,ImVec2{0,0});

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

    ImVec2 buttonSize = {lineHeight + 3.0f , lineHeight};

    ImGui::PushStyleColor(ImGuiCol_Button,ImVec4{0.8f,0.1f,0.15f,1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4{0.9f,0.2f,0.2f,1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4{0.8f,0.1f,0.15f,1.0f});
    if (ImGui::Button("X",buttonSize))
        values.x = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X",&values.x,0.1f,0.0f,0.0f,"%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button,ImVec4{0.2f,0.7f,0.2f,1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4{0.3f,0.8f,0.3f,1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4{0.2f,0.7f,0.2f,1.0f});
    if (ImGui::Button("Y",buttonSize))
        values.y = resetValue;
    ImGui::PopStyleColor(3);
    
    ImGui::SameLine();
    ImGui::DragFloat("##Y",&values.y,0.1f,0.0f,0.0f,"%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button,ImVec4{0.1f,0.25f,0.8f,1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4{0.2f,0.35f,0.9f,1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4{0.1f,0.25f,0.8f,1.0f});
    if (ImGui::Button("Z",buttonSize))
        values.z = resetValue;
    ImGui::PopStyleColor(3);
    
    ImGui::SameLine();
    ImGui::DragFloat("##Z",&values.z,0.1f,0.0f,0.0f,"%.2f");
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    ImGui::Columns(1);
    ImGui::PopID();
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

    const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;
    
    if(entity.HasComponent<TransformComponent>())
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,ImVec2{4,4});
        bool open = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(),treeNodeFlags,"Transform");
        ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
        if (ImGui::Button("+",ImVec2{20,20}))
        {
            ImGui::OpenPopup("Component Setting");
        }
        ImGui::PopStyleVar();

        bool removeComponent = false;
        if (ImGui::BeginPopup("Component Setting"))
        {
            if (ImGui::MenuItem("Remove Component"))
                removeComponent = true;
            ImGui::EndPopup();
        }
        
        
        if(open)
        {
            auto tc = entity.GetComponent<TransformComponent>();

            DrawVec3Control("Translation",tc.Translation);
            glm::vec3 rotation = glm::degrees(tc.Rotation);
            DrawVec3Control("Rotation",rotation);
            tc.Rotation = glm::radians(rotation);
            DrawVec3Control("Scale",tc.Scale,1.0f);
            ImGui::TreePop();
        }

        if (removeComponent)
            entity.RemoveComponent<TransformComponent>();
        
    }

    if(entity.HasComponent<CameraComponent>())
    {
        if(ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(),treeNodeFlags,"Camera"))
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
        if(ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(),treeNodeFlags,"Sprite Renderer"))
        {
            auto& src = entity.GetComponent<SpriteRendererComponent>();
            ImGui::ColorEdit4("Color",glm::value_ptr(src.Color));

            ImGui::TreePop();
        }
    }
    
}

} // namespace Rengin

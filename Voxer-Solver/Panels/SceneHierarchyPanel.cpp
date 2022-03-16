#include <rengine.hpp>
#include <cstdio>
#include <thread>
#include "SceneHierarchyPanel.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include "Rengine/Scene/Component.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "Rengine/Utils/ExternalExe.hpp"

namespace Rengin
{

SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
{
    SetContext(context);
}

void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
{
    m_Context = context;
    m_SelectionContext = {};
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
        
    }
    
    ImGui::End();
}

void SceneHierarchyPanel::DrawEntityNode(Entity entity)
{
    auto& tag = entity.GetComponent<TagComponent>().Tag;

    ImGuiTreeNodeFlags flags = (m_SelectionContext == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

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
    ImGuiIO& io = ImGui::GetIO();
    auto boldFont = io.Fonts->Fonts[0];

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
    ImGui::PushFont(boldFont);
    if (ImGui::Button("X",buttonSize))
        values.x = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X",&values.x,0.1f,-180.0f,180.0f,"%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button,ImVec4{0.2f,0.7f,0.2f,1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4{0.3f,0.8f,0.3f,1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4{0.2f,0.7f,0.2f,1.0f});
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Y",buttonSize))
        values.y = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);
    
    ImGui::SameLine();
    ImGui::DragFloat("##Y",&values.y,0.1f,-180.0f,180.0f,"%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button,ImVec4{0.1f,0.25f,0.8f,1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4{0.2f,0.35f,0.9f,1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4{0.1f,0.25f,0.8f,1.0f});
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Z",buttonSize))
        values.z = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);
    
    ImGui::SameLine();
    ImGui::DragFloat("##Z",&values.z,0.1f,-180.0f,180.0f,"%.2f");
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    ImGui::Columns(1);
    ImGui::PopID();
}

template<typename T,typename UIFunction>
static void DrawComponent(const std::string& name,Entity entity,UIFunction function)
{
    const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth 
        | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
    if(entity.HasComponent<T>())
    {
        auto& component = entity.GetComponent<T>();
        ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,ImVec2{4,4});
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImGui::Separator();
        bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(),treeNodeFlags,name.c_str());
        ImGui::PopStyleVar();
        ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
        if (ImGui::Button("+",ImVec2{lineHeight,lineHeight}))
        {
            ImGui::OpenPopup("Component Setting");
        }

        bool removeComponent = false;
        if (ImGui::BeginPopup("Component Setting"))
        {
            if (ImGui::MenuItem("Remove Component"))
                removeComponent = true;
            ImGui::EndPopup();
        }
        
        if(open)
        {
            function(component);

            ImGui::TreePop();
        }

        if (removeComponent)
            entity.RemoveComponent<T>();
        
    }
}

void SceneHierarchyPanel::AddAttribute(std::vector<int>& v,std::vector<float>& target,float x, float y, float z,int width,int height,int depth)
{
    int size1 = v.size(),size2 = target.size();
    for (int i = 0; i < v.size(); i += 3)
    {
        if(v[i] == -1)  continue;
        int id = v[i] + v[i + 1] * width + v[i + 2] * width * height;
        target[id * 3] += x;
        target[id * 3 + 1] += y;
        target[id * 3 + 2] += z;
    }
    v.clear();
}

void SceneHierarchyPanel::ClearAttribute(std::vector<int>& v,std::vector<float>& target,float x, float y, float z,int width,int height,int depth)
{
    for (int i = 0; i < v.size(); i += 3)
    {
        if(v[i] == -1)  continue;
        int id = v[i] + v[i + 1] * width + v[i + 2] * width * height;
        target[id * 3] = x;
        target[id * 3 + 1] = y;
        target[id * 3 + 2] = z;
    }
    v.clear();
}

void SceneHierarchyPanel::SaveIntFile(Ref<Texture3D> model, Ref<Texture3D> force,
                Ref<Texture3D> constraint, int width, int height,int depth)
{
    std::string path = "./temp/";

    // material
    auto pp = path + "material.txt";
    FILE *f = fopen((path + "material.txt").c_str(), "w");
    RE_CORE_ASSERT(f, "Cant Open the file");
    fprintf(f,"0 25480000000 0.3 1\n");
    fprintf(f,"1 25480000000 0.3 1\n");
    fprintf(f,"2 25480000000 0.48 1\n");
    fclose(f);

    // model
    f = fopen((path + "model.txt").c_str(), "w");
    RE_CORE_ASSERT(f, "Cant Open the file");

    auto tex = model->getTexture();
	int idx = 0;
    int nums = 0;
    for (int k = 0; k < depth; k++)
	{
		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
                if(tex[idx] > 0)
                    fprintf(f,"%d %d %d %d %d\n",idx,tex[idx] - 1,i,j,k),nums++;
                idx++;
			}
		}
    }

    fclose(f);

    // Bound Condition
    auto forc = force->getTexture();
    auto cons = constraint->getTexture();
    f = fopen((path + "constraint.txt").c_str(), "w");
    RE_CORE_ASSERT(f, "Cant Open the file");
	idx = 0;
    int cnt = 0;
	for (int k = 0; k < depth; k++)
	{
		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
                int id1 = k * height * width + j * width  + i,id2,id3;
                id1 *= 3;
                id2 = id1 + 1;
                id3 = id2 + 1;
                if(forc[id1] || forc[id2] || forc[id3] || cons[id1] || cons[id2] || cons[id3])
                {
                    fprintf(f, "SELECT_NODE_3D %d %d %d %d %d %d %d %d %d\n", i, j, k,forc[id1],forc[id2],forc[id3],cons[id1],cons[id2],cons[id3]);
                }
			}
	    }
    }
    fclose(f);

    // Command
    f = fopen((path + "vo.txt").c_str(), "w");
    RE_CORE_ASSERT(f, "Cant Open the file");
    fprintf(f,"SET_SCRIPT_VERSION 2\nSET_VOXEL_SIZE %f %f %f %f\nLOAD_MATERIALS material.txt\nLOAD_MODEL %d %d %d %d model.txt\nSET_TOLERANCE 1e-9\nSET_MAX_ITER 2000\nSET_ALGORITHM_FEA 1 1\nSELECTION_OF_NODES\nLOAD_CONSTRAINTS constraint.txt\nSELECT_NODE_3D\nPRESERVE_NODE_3D\nCOMPUTE_SED\nSOLVE\nPRINT_DISPLACEMENTS displacements.txt\nFINISH",0.3,0.3,0.3,1.0,width,height,depth,nums);
    fclose(f);
    auto wpro = ExternalExe::Create();
    wpro->CreateProcess("");
    wpro->WaitProcess();
    wpro->Terminate();
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

    ImGui::SameLine();
    ImGui::PushItemWidth(-1);


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

    ImGui::PopItemWidth();

    DrawComponent<TransformComponent>("Transform",entity,[](auto& component)
    {
        DrawVec3Control("Translation",component.Translation);
        glm::vec3 rotation = glm::degrees(component.Rotation);
        DrawVec3Control("Rotation",rotation);
        component.Rotation = glm::radians(rotation);
        DrawVec3Control("Scale",component.Scale,1.0f);
    });

    DrawComponent<CameraComponent>("Camera",entity,[](auto& component)
    {
        auto& camera = component.Camera;

        ImGui::Checkbox("Primary",&component.Primary);

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
            if(ImGui::DragFloat("FOV",&perspectiveFOV,0.1,30.0f,90.0f,"%.2f"))
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

            ImGui::Checkbox("Fixed Aspect Ratio",&component.FixedAspectRatio);
        }
    });


    DrawComponent<SpriteRendererComponent>("Sprite Renderer",entity,[](auto& component)
    {
        ImGui::ColorEdit4("Color",glm::value_ptr(component.Color));
    });
    
    DrawComponent<Texture3DComponent>("Texture3D",entity,[](auto& component)
    {
        ImGui::Text(component.Path.c_str());
        ImGui::Text("width: %d, height: %d, depth: %d",component.width,component.height,component.depth);
        ImGui::DragFloat("threshold",&component.Threshold,0.001,0.0f,1.0f,"%.2f");
    });

    DrawComponent<ForceComponent>("Force",entity,[this](auto& component)
    {
        ImGui::Text("Forces");
        char buffer[25];
        memset(buffer,0,sizeof(buffer));
        static float fx = 0,fy = 0,fz = 0;
        sprintf(buffer,"%f",fx);
        if(ImGui::InputText("Fx",buffer,sizeof(buffer)))
        {
            fx = atof(buffer);
        }
        sprintf(buffer,"%f",fy);
        if(ImGui::InputText("Fy",buffer,sizeof(buffer)))
        {
            fy = atof(buffer);
        }
        sprintf(buffer,"%f",fz);
        if(ImGui::InputText("Fz",buffer,sizeof(buffer)))
        {
            fz = atof(buffer);
        }
        if(ImGui::Button("Add Force"))
        {
            auto& texComF = m_VolomeEntity.GetComponent<ForceComponent>();
            AddAttribute(m_PickedPixels,texComF.Texture->getTexture(),fx,fy,fz,texComF.Texture->getWidth(),texComF.Texture->getHeight(),texComF.Texture->getDepth());
            texComF.Texture->setData(texComF.Texture->getTexture().data(),texComF.Texture->getTexture().size());
            fx = fy = fz = 0;
        }
        if(ImGui::Button("Clear Force"))
        {
            auto& texComF = m_VolomeEntity.GetComponent<ForceComponent>();
            fx = fy = fz = 0;
            ClearAttribute(m_PickedPixels,texComF.Texture->getTexture(),fx,fy,fz,texComF.Texture->getWidth(),texComF.Texture->getHeight(),texComF.Texture->getDepth());
            texComF.Texture->setData(texComF.Texture->getTexture().data(),texComF.Texture->getTexture().size());
        }
    });

    DrawComponent<ConstraintComponent>("Constraint",entity,[this](auto& component)
    {
        ImGui::Text("Constraint");
        if(ImGui::Button("Add x"))
        {
            auto &texComC = m_VolomeEntity.GetComponent<ConstraintComponent>();
            AddAttribute(m_PickedPixels,texComC.Texture->getTexture(),1,0,0,texComC.Texture->getWidth(),texComC.Texture->getHeight(),texComC.Texture->getDepth());
            texComC.Texture->setData(texComC.Texture->getTexture().data(),texComC.Texture->getTexture().size());
        }
        ImGui::SameLine();
        if(ImGui::Button("Add y"))
        {
            auto &texComC = m_VolomeEntity.GetComponent<ConstraintComponent>();
            AddAttribute(m_PickedPixels,texComC.Texture->getTexture(),0,1,0,texComC.Texture->getWidth(),texComC.Texture->getHeight(),texComC.Texture->getDepth());
            texComC.Texture->setData(texComC.Texture->getTexture().data(),texComC.Texture->getTexture().size());
        }
        ImGui::SameLine();
        if(ImGui::Button("Add z"))
        {
            auto &texComC = m_VolomeEntity.GetComponent<ConstraintComponent>();
            AddAttribute(m_PickedPixels,texComC.Texture->getTexture(),0,0,1,texComC.Texture->getWidth(),texComC.Texture->getHeight(),texComC.Texture->getDepth());
            texComC.Texture->setData(texComC.Texture->getTexture().data(),texComC.Texture->getTexture().size());
        }
        ImGui::SameLine();
        if(ImGui::Button("Add Constraint"))
        {
            auto &texComC = m_VolomeEntity.GetComponent<ConstraintComponent>();
            AddAttribute(m_PickedPixels,texComC.Texture->getTexture(),1,1,1,texComC.Texture->getWidth(),texComC.Texture->getHeight(),texComC.Texture->getDepth());
            texComC.Texture->setData(texComC.Texture->getTexture().data(),texComC.Texture->getTexture().size());
        }
        if(ImGui::Button("Clear Constraint"))
        {
            auto &texComC = m_VolomeEntity.GetComponent<ConstraintComponent>();
            ClearAttribute(m_PickedPixels,texComC.Texture->getTexture(),0,0,0,texComC.Texture->getWidth(),texComC.Texture->getHeight(),texComC.Texture->getDepth());
            texComC.Texture->setData(texComC.Texture->getTexture().data(),texComC.Texture->getTexture().size());
        }
    });
    DrawComponent<SolveComponent>("Solve!", entity, [this](auto &component) {
            if (ImGui::Button("Solve")) {
                auto& texCom = m_VolomeEntity.GetComponent<Texture3DComponent>();
                auto& texComC = m_VolomeEntity.GetComponent<ConstraintComponent>();
                auto& texComF = m_VolomeEntity.GetComponent<ForceComponent>();
                // std::thread t(&SceneHierarchyPanel::SaveIntFile,this,texCom.Texture,texComF.Texture,texComC.Texture,texCom.width,texCom.height,texCom.depth);
                // SaveIntFile(texCom.Texture,texComF.Texture,texComC.Texture,texCom.width,texCom.height,texCom.depth);
                // t.detach();
                auto wpro = ExternalExe::Create();
                wpro->CreateProcess("");
                wpro->WaitProcess();
                // wpro->Terminate();
            }
        });
    DrawComponent<ColorTransferFunctionComponent>(
        "ColorTransferFunction", entity, [](auto &component) {
          auto &transferFun = component.Color;
          ImVec2 canvas_p0 = ImGui::GetCursorScreenPos(); // ImDrawList API uses
          ImVec2 canvas_sz = ImGui::GetContentRegionAvail(); // Resize canvas to
          glm::vec2 clip_board{canvas_sz.x, 10};
          canvas_sz.y = clip_board.y;
          ImVec2 canvas_p1 =
              ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
          ImGuiIO &io = ImGui::GetIO();
          ImDrawList *draw_list = ImGui::GetWindowDrawList();
          draw_list->AddRectFilled(canvas_p0, canvas_p1,
                                   IM_COL32(50, 50, 150, 255));
          draw_list->AddRect(canvas_p0, canvas_p1,
                             IM_COL32(255, 255, 255, 255));

          ImGui::InvisibleButton("Color TransferFunction", canvas_sz,
                                 ImGuiButtonFlags_MouseButtonLeft |
                                     ImGuiButtonFlags_MouseButtonRight);
          const bool is_hovered = ImGui::IsItemHovered(); // Hovered
          const bool is_active = ImGui::IsItemActive();   // Held
          const ImVec2 origin(canvas_p0.x, canvas_p0.y); // Lock scrolled origin
          const glm::vec2 mouse_pos_in_canvas(io.MousePos.x - origin.x,
                                              io.MousePos.y - origin.y);

          static int old_key = -1;
          if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            old_key = mouse_pos_in_canvas.x;
            transferFun.Insert(mouse_pos_in_canvas.x / clip_board.x);
          }
          if (is_active && old_key!= -1 &&
              ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0.0)) {
            if (old_key != mouse_pos_in_canvas.x)
            {
              transferFun.Erase(old_key / clip_board.x);
              old_key = mouse_pos_in_canvas.x;
            }
            transferFun.Insert(mouse_pos_in_canvas.x / clip_board.x);
          }
          draw_list->PushClipRect(canvas_p0, canvas_p1, true);

          for (auto it = transferFun.begin(); it != transferFun.end(); it++) {
            draw_list->AddCircleFilled(
                ImVec2(origin.x + it->first * clip_board.x,origin.y + 5.0f), 5.0f,
                IM_COL32(255, 0, 0, 255));
          }
          draw_list->PopClipRect();

          if (ImGui::Button("clear Color", {clip_board.x / 2, 20})) {
            transferFun.Reset();
          }
          ImGui::SameLine();
          if (ImGui::Button("delete Color", {clip_board.x / 2, 20})) {
            if (old_key != -1) {
              transferFun.Erase(old_key / clip_board.x);
              old_key = -1;
            }
          }
        });

    DrawComponent<OpacityTransferFunctionComponent>(
        "OpacityTransferFunction", entity, [](auto &component) {
          auto &transferFun = component.Opacity;
          ImVec2 canvas_p0 = ImGui::GetCursorScreenPos(); // ImDrawList API uses
          ImVec2 canvas_sz = ImGui::GetContentRegionAvail(); // Resize canvas to
          glm::vec2 clip_board{canvas_sz.x, 100.0f};
          canvas_sz.y = clip_board.y;
          ImVec2 canvas_p1 =
              ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
          ImGuiIO &io = ImGui::GetIO();
          ImDrawList *draw_list = ImGui::GetWindowDrawList();
          draw_list->AddRectFilled(canvas_p0, canvas_p1,
                                   IM_COL32(50, 50, 150, 255));
          draw_list->AddRect(canvas_p0, canvas_p1,
                             IM_COL32(255, 255, 255, 255));

          ImGui::InvisibleButton("Opacity TransferFunction", canvas_sz,
                                 ImGuiButtonFlags_MouseButtonLeft |
                                     ImGuiButtonFlags_MouseButtonRight);
          const bool is_hovered = ImGui::IsItemHovered(); // Hovered
          const bool is_active = ImGui::IsItemActive();   // Held
          const ImVec2 origin(canvas_p0.x, canvas_p0.y); // Lock scrolled origin
          const glm::vec2 mouse_pos_in_canvas(io.MousePos.x - origin.x,
                                              io.MousePos.y - origin.y);

          static int old_key = -1;
          if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            old_key = mouse_pos_in_canvas.x;
            transferFun.Insert(mouse_pos_in_canvas.x / clip_board.x,1.0f - mouse_pos_in_canvas.y / clip_board.y);

          }
          if (is_active && old_key!= -1 &&
              ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0.0)) {
            if (old_key != mouse_pos_in_canvas.x)
            {
              transferFun.Erase(old_key / clip_board.x);
              old_key = mouse_pos_in_canvas.x;
            }
            transferFun.Insert(mouse_pos_in_canvas.x / clip_board.x,1.0f - mouse_pos_in_canvas.y / clip_board.y);
          }
          draw_list->PushClipRect(canvas_p0, canvas_p1, true);

          for (auto it = transferFun.begin(); it != transferFun.end(); it++) {
            draw_list->AddCircleFilled(
                ImVec2(origin.x + it->first * clip_board.x, origin.y + (1.0f - it->second) * clip_board.y), 5.0f,
                IM_COL32(255, 0, 0, 255));

            auto temp = it;
            temp++;

            if (temp != transferFun.end())
              draw_list->AddLine(
                  ImVec2(origin.x + it->first * clip_board.x, origin.y + (1.0f - it->second) * clip_board.y),
                  ImVec2(origin.x + temp->first * clip_board.x, origin.y + (1.0f - temp->second) * clip_board.y),
                  IM_COL32(200, 200, 200, 40));
          }
          draw_list->PopClipRect();

          if (ImGui::Button("clear Opacity", {clip_board.x / 2, 20})) {
            transferFun.Reset();
          }
          ImGui::SameLine();
          if (ImGui::Button("delete Opacity", {clip_board.x / 2, 20})) {
            if (old_key != -1) {
              transferFun.Erase(old_key / clip_board.x);
              old_key = -1;
            }
          }
        });
}

} // namespace Rengin

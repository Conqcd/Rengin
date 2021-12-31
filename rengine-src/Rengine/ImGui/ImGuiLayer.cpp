#include "repch.hpp"
#include "ImGuiLayer.hpp"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_opengl3_loader.h"

#include "Rengine/Core/application.hpp"

// #include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Rengin
{
    
ImGuiLayer::ImGuiLayer(/* args */):Layer("ImguiLayer")
{
}

ImGuiLayer::~ImGuiLayer()
{
}

void ImGuiLayer::SetDarkThemeColors()
{
    auto& colors = ImGui::GetStyle().Colors;

    colors[ImGuiCol_WindowBg] = ImVec4{0.1f,0.105f,0.11f,1.0f};

    //Headers
    colors[ImGuiCol_Header] = ImVec4{0.2f,0.205f,0.21f,1.0f};
    colors[ImGuiCol_HeaderHovered] = ImVec4{0.3f,0.305f,0.31f,1.0f};
    colors[ImGuiCol_HeaderActive] = ImVec4{0.15f,0.1505f,0.151f,1.0f};

    //Buttons
    colors[ImGuiCol_Button] = ImVec4{0.2f,0.205f,0.21f,1.0f};
    colors[ImGuiCol_ButtonHovered] = ImVec4{0.3f,0.305f,0.31f,1.0f};
    colors[ImGuiCol_ButtonActive] = ImVec4{0.15f,0.1505f,0.151f,1.0f};

    //Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{0.2f,0.205f,0.21f,1.0f};
    colors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f,0.305f,0.31f,1.0f};
    colors[ImGuiCol_FrameBgActive] = ImVec4{0.15f,0.1505f,0.151f,1.0f};

    //Tabs
    colors[ImGuiCol_Tab] = ImVec4{0.15f,0.1505f,0.151f,1.0f};
    colors[ImGuiCol_TabHovered] = ImVec4{0.38f,0.3805f,0.381f,1.0f};
    colors[ImGuiCol_TabActive] = ImVec4{0.28f,0.2805f,0.281f,1.0f};
    colors[ImGuiCol_TabUnfocused] = ImVec4{0.15f,0.1505f,0.151f,1.0f};
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f,0.205f,0.21f,1.0f};

    //Title
    colors[ImGuiCol_TitleBg] = ImVec4{0.15f,0.1505f,0.151f,1.0f};
    colors[ImGuiCol_TitleBgActive] = ImVec4{0.15f,0.1505f,0.151f,1.0f};
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f,0.1505f,0.151f,1.0f};
    
}

void ImGuiLayer::OnAttach()
{
    RE_PROFILE_FUNCTION();
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    io.Fonts->AddFontFromFileTTF("../../../Rengine-Editor/assets/fonts/opensans/static/OpenSans/OpenSans-Bold.ttf",18.0f);
    io.FontDefault = io.Fonts->AddFontFromFileTTF("../../../Rengine-Editor/assets/fonts/opensans/static/OpenSans/OpenSans-Regular.ttf",18.0f);

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();

    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    SetDarkThemeColors();

    auto& app = Application::getApplication();
    auto* window = static_cast<GLFWwindow*>(app.getWindow().getNativeWindow());

    ImGui_ImplGlfw_InitForOpenGL(window,true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::OnDetach()
{
    RE_PROFILE_FUNCTION();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::OnEvent(Event& ev)
{
    if(m_BlockImGuiEvents)
    {
         ImGuiIO& io = ImGui::GetIO();
         ev.m_handle |= ev.isSameCategory(EventCategoryMouse) & io.WantCaptureMouse;
         ev.m_handle |= ev.isSameCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
    }
}

void ImGuiLayer::Begin()
{
    RE_PROFILE_FUNCTION();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::End()
{
    RE_PROFILE_FUNCTION();

    ImGuiIO& io = ImGui::GetIO();

    auto& app = Application::getApplication();

    io.DisplaySize = ImVec2(static_cast<float>(app.getWindow().getWidth()),static_cast<float>(app.getWindow().getHeight()));

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();

        ImGui::UpdatePlatformWindows();

        ImGui::RenderPlatformWindowsDefault();

        glfwMakeContextCurrent(backup_current_context);
    }
}


void ImGuiLayer::OnImGuiRender()
{
    RE_PROFILE_FUNCTION();

    // static bool show = true;
    // ImGui::ShowDemoWindow(&show);
}

} // namespace Rengin

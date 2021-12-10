#include "ImGuiLayer.hpp"
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_opengl3_loader.h"
#include "../application.hpp"

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

void ImGuiLayer::OnAttach()
{
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

    //temp
    io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
    io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
    io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
    io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
    io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
    io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
    io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
    io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
    io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
    io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
    io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
    io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

    ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::OnDetach()
{

}

void ImGuiLayer::OnUpdate()
{
    ImGuiIO& io = ImGui::GetIO();

    auto & app =Application::getApplication();
    io.DisplaySize = ImVec2(app.getWindow().getWidth(),app.getWindow().getHeight());

    float time = static_cast<float>(glfwGetTime());
    io.DeltaTime = m_time > 0.0f ? (time - m_time) : (1.0f / 60.0f);
    m_time = time;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
    
    static bool show = true;
    ImGui::ShowDemoWindow(&show);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::OnEvent(Event& ev)
{
    EventDispatcher dispatcher(ev);
    dispatcher.Dispatch<MouseButtonPressEvent>(RE_BIND_FUNC_EVENT_1(ImGuiLayer::OnMouseButtonPressEvent));
    dispatcher.Dispatch<MouseButtonReleaseEvent>(RE_BIND_FUNC_EVENT_1(ImGuiLayer::OnMouseButtonReleaseEvent));
    dispatcher.Dispatch<MouseMovedEvent>(RE_BIND_FUNC_EVENT_1(ImGuiLayer::OnMouseMovedEvent));
    dispatcher.Dispatch<MouseScrolledEvent>(RE_BIND_FUNC_EVENT_1(ImGuiLayer::OnMouseScrolledEvent));
    dispatcher.Dispatch<KeyReleaseEvent>(RE_BIND_FUNC_EVENT_1(ImGuiLayer::OnKeyReleaseEvent));
    dispatcher.Dispatch<KeyTypeEvent>(RE_BIND_FUNC_EVENT_1(ImGuiLayer::OnKeyTypeEvent));
    dispatcher.Dispatch<KeyPressEvent>(RE_BIND_FUNC_EVENT_1(ImGuiLayer::OnKeyPressEvent));
    dispatcher.Dispatch<WindowResizeEvent>(RE_BIND_FUNC_EVENT_1(ImGuiLayer::OnWindowResizeEvent));
}

bool ImGuiLayer::OnMouseButtonPressEvent(MouseButtonPressEvent& ev)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MouseDown[ev.getButton()] = true;
    return false;
}

bool ImGuiLayer::OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& ev)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MouseDown[ev.getButton()] = false;
    return false;
}

bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& ev)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2(ev.getX(),ev.getY());
    return false;
}

bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& ev)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MouseWheelH += ev.getYoffset();
    io.MouseWheel += ev.getXoffset();
    return false;
}

bool ImGuiLayer::OnKeyTypeEvent(KeyTypeEvent& ev)
{
    ImGuiIO& io = ImGui::GetIO();
    int key = ev.getKeyValue();

    if(key > 0 && key < 0x10000)
        io.AddInputCharacter(static_cast<unsigned int>(key));
    return false;
}

bool ImGuiLayer::OnKeyReleaseEvent(KeyReleaseEvent& ev)
{
    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[ev.getKeyValue()] = true;

    io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL | GLFW_KEY_RIGHT_CONTROL];
    io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT | GLFW_KEY_RIGHT_ALT];
    io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER | GLFW_KEY_RIGHT_SUPER];
    io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT | GLFW_KEY_RIGHT_SHIFT];
    return false;
}

bool ImGuiLayer::OnKeyPressEvent(KeyPressEvent& ev)
{
    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[ev.getKeyValue()] = false;
    return false;
}

bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& ev)
{
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(ev.getWidth(),ev.getHeight());
    io.DisplayFramebufferScale = ImVec2(1.f,1.f);
    glViewport(0,0,ev.getWidth(),ev.getHeight());
    return false;
}

} // namespace Rengin

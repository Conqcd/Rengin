#include "repch.hpp"
#include "WindowsWindow.hpp"
#include "Rengine/Core/log.hpp"
#include "Rengine/Event/ApplicationEvent.hpp"
#include "Rengine/Event/Event.hpp"
#include "Rengine/Event/KeyEvent.hpp"
#include "Rengine/Event/MouseEvent.hpp"
#include "Rengine/Platform/OpenGL/OpenGLContext.hpp"

#include <GLFW/glfw3.h>

namespace Rengin
{

static bool hasInitGLFWWindows = false;

static uint8_t s_GLFWWindowCount = 0;

static void GLFWErrorCallback(int error_code, const char* description)
{
    RE_CORE_ERROR("GLFW Error {0} : {1}",error_code,description);
}

Scope<Window> Window::WindowCreate(const WindowProps& props)
{
    return CreateScope<WindowsWindow>(props);
}

WindowsWindow::WindowsWindow(const WindowProps& props)
{
    RE_PROFILE_FUNCTION();
    Init(props);
}

WindowsWindow::~WindowsWindow()
{
    RE_PROFILE_FUNCTION();
    Shutdown();
}

void WindowsWindow::OnUpdate()
{
    RE_PROFILE_FUNCTION();
    glfwPollEvents();
    m_context->SwapBuffer();
}

void WindowsWindow::setEventCallBack(const EventCallBackFunc& fn)
{
    m_data.m_callback = fn;
}

void WindowsWindow::setVSync(bool enable)
{
    RE_PROFILE_FUNCTION();
    if(enable)
        glfwSwapInterval(1);
    else 
        glfwSwapInterval(0);
    m_data.VSync = enable;
}

bool WindowsWindow::isVSync()const
{
    return m_data.VSync;
}

void WindowsWindow::Init(const WindowProps& props)
{
    RE_PROFILE_FUNCTION();
    m_data.m_width = props.width;
    m_data.m_height = props.height;
    m_data.m_title = props.title;

    RE_CORE_INFO("Creating Window {0} {1} {2}",m_data.m_title,m_data.m_width,m_data.m_height);

    if(s_GLFWWindowCount == 0)
    {
        int ok = glfwInit();

        RE_CORE_ASSERT(ok,"Can`t intialize glfw!");

        glfwSetErrorCallback(GLFWErrorCallback);
        hasInitGLFWWindows = true;
    }

    {
        RE_PROFILE_SCOPE("glfwCreateWindow");
        m_win = glfwCreateWindow(static_cast<int>(m_data.m_width),static_cast<int>(m_data.m_height),m_data.m_title.c_str(),nullptr,nullptr);

        s_GLFWWindowCount++;
    }
    
    m_context = new OpenGLContext(m_win);
    m_context->Init();
    
    glfwSetWindowUserPointer(m_win,&m_data);
    setVSync(true);

    glfwSetWindowSizeCallback(m_win,[](GLFWwindow* window, int width, int height)
    {
        auto& data = *static_cast<WinData*>(glfwGetWindowUserPointer(window));

        data.m_width = width;
        data.m_height = height;

        WindowResizeEvent ev(width,height);

        RE_CORE_WARN("change window size {0} {1}",width,height);
        data.m_callback(ev);
    });

    glfwSetWindowCloseCallback(m_win,[](GLFWwindow* window)
    {
        auto& data = *static_cast<WinData*>(glfwGetWindowUserPointer(window));

        WindowCloseEvent ev;
        data.m_callback(ev);
    });

    glfwSetKeyCallback(m_win,[](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        auto& data = *static_cast<WinData*>(glfwGetWindowUserPointer(window));
        switch (action)
        {
        case GLFW_PRESS:
            {
                KeyPressEvent ev(key,0);
                data.m_callback(ev);
                break;
            }
        case GLFW_RELEASE:
            {
                KeyReleaseEvent ev(key);
                data.m_callback(ev);
                break;
            }
        case GLFW_REPEAT:
            {
                KeyPressEvent ev(key,1);
                data.m_callback(ev);
                break;
            }
        default:
            break;
        }
    });
    glfwSetCharCallback(m_win,[](GLFWwindow* window, unsigned int codepoint)
    {
        auto& data = *static_cast<WinData*>(glfwGetWindowUserPointer(window));
        KeyTypeEvent ev(codepoint);
        data.m_callback(ev);
    });
    glfwSetMouseButtonCallback(m_win,[](GLFWwindow* window, int button, int action, int mods)
    {
        auto& data = *static_cast<WinData*>(glfwGetWindowUserPointer(window));
        switch (action)
        {
        case GLFW_PRESS:
            {
                MouseButtonPressEvent ev(button);
                data.m_callback(ev);
                break;
            }
        case GLFW_RELEASE:
            {
                MouseButtonReleaseEvent ev(button);
                data.m_callback(ev);
                break;
            }
        default:
            break;
        }
    });

    glfwSetScrollCallback(m_win,[](GLFWwindow* window, double xoffset, double yoffset)
    {
        auto& data = *static_cast<WinData*>(glfwGetWindowUserPointer(window));
        MouseScrolledEvent ev(static_cast<float>(xoffset),static_cast<float>(yoffset));
        data.m_callback(ev);
    });

    glfwSetCursorPosCallback(m_win,[](GLFWwindow* window, double xpos, double ypos)
    {
        auto& data = *static_cast<WinData*>(glfwGetWindowUserPointer(window));
        MouseMovedEvent ev(static_cast<float>(xpos),static_cast<float>(ypos));
        data.m_callback(ev);
    });
}

void WindowsWindow::Shutdown()
{
    RE_PROFILE_FUNCTION();
    glfwDestroyWindow(m_win);
    
    s_GLFWWindowCount--;
    if(s_GLFWWindowCount == 0)
    {
        glfwTerminate();
    }
}
}
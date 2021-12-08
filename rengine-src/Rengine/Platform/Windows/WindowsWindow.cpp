#include "../../../repch.hpp"
#include "WindowsWindow.hpp"
#include "../../log.hpp"

namespace Rengin
{

static bool hasInitGLFWWindows = false;


Window* Window::WindowCreate(const WindowProps& props)
{
    return new WindowsWindow(props);
}

WindowsWindow::WindowsWindow(const WindowProps& props)
{
    Init(props);
}

WindowsWindow::~WindowsWindow()
{
    Shutdown();
}

void WindowsWindow::OnUpdate()
{
    glfwPollEvents();
    glfwSwapBuffers(m_win);
}

void WindowsWindow::setEventCallBack(EventCallBackFunc& fn)
{

}

void WindowsWindow::setVSync(bool enable)
{
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
    m_data.m_width = props.width;
    m_data.m_height = props.height;
    m_data.m_title = props.title;

    RE_CORE_INFO("Creating Window {0} {1} {2}",m_data.m_title,m_data.m_width,m_data.m_height);

    if(!hasInitGLFWWindows)
    {
        int ok = glfwInit();

        RE_CORE_ASSERT(ok,"Can`t intialize glfw!");
        hasInitGLFWWindows = true;
    }

    m_win = glfwCreateWindow(static_cast<int>(m_data.m_width),static_cast<int>(m_data.m_height),m_data.m_title.c_str(),nullptr,nullptr);
    glfwMakeContextCurrent(m_win);
    glfwSetWindowUserPointer(m_win,&m_data);
    setVSync(true);

}

void WindowsWindow::Shutdown()
{
    glfwDestroyWindow(m_win);
}
}
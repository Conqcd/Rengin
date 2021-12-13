#include "repch.hpp"
#include "OpenGLContext.hpp"
#include "Rengine/core.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Rengin
{
OpenGLContext::OpenGLContext(GLFWwindow* win):m_win(win)
{
    RE_CORE_ASSERT(m_win,"window handle is null!");
}

OpenGLContext::~OpenGLContext()
{
}

void OpenGLContext::Init()
{
    glfwMakeContextCurrent(m_win);
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    RE_CORE_ASSERT(status,"failed to intialize Glad");
}

void OpenGLContext::SwapBuffer()
{
    glfwSwapBuffers(m_win);
}

} // namespace Rengin

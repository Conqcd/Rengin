#pragma once
#include "Rengine/Renderer/GraphicsContext.hpp"

struct GLFWwindow;

namespace Rengin
{

class OpenGLContext :public GraphicsContext
{
private:
    GLFWwindow* m_win;
public:
    OpenGLContext(GLFWwindow* win);
    ~OpenGLContext();
    
    virtual void Init() override;
    virtual void SwapBuffer() override;
};


    
} // namespace Rengin

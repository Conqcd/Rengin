#include "repch.hpp"
#include "OpenGLRendererAPI.hpp"
#include "glm/glm.hpp"
#include <glad/glad.h>

namespace Rengin
{

void OpenGLRendererAPI::Init()
{
    RE_PROFILE_FUNCTION();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
}

void OpenGLRendererAPI::EnableAlpha()
{
    glEnable(GL_BLEND);
}

void OpenGLRendererAPI::DisableAlpha()
{
    glDisable(GL_BLEND);
}

void OpenGLRendererAPI::SetViewPort(uint32_t x,uint32_t y,uint32_t width,uint32_t height)
{
    glViewport(x,y,width,height);
}

void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
{
    glClearColor(color.r,color.g,color.b,color.a);
}

void OpenGLRendererAPI::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray,uint32_t count)
{
    vertexArray->Bind();
    uint32_t indexCount = count == 0? vertexArray->getIndexBuffer()->GetCount() : count;
    glDrawElements(GL_TRIANGLES,indexCount,GL_UNSIGNED_INT,nullptr);
}

void OpenGLRendererAPI::DrawLines(const Ref<VertexArray>& vertexArray,uint32_t count)
{
    vertexArray->Bind();
    glDrawArrays(GL_LINES,0,count);
}
void OpenGLRendererAPI::SetLineThickness(float thickness)
{
    glLineWidth(thickness);
}

} // namespace Rengin

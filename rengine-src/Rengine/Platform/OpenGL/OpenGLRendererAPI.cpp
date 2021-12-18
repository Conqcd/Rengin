#include "repch.hpp"
#include "OpenGLRendererAPI.hpp"
#include "glm/glm.hpp"
#include <glad/glad.h>

namespace Rengin
{

void OpenGLRendererAPI::Init()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
{
    glClearColor(color.r,color.g,color.b,color.a);
}

void OpenGLRendererAPI::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
{
    glDrawElements(GL_TRIANGLES,vertexArray->getIndexBuffer()->GetCount(),GL_UNSIGNED_INT,nullptr);
}

} // namespace Rengin

#include "repch.hpp"
#include "Renderer.hpp"
#include "Rengine/Platform/OpenGL/OpenGLShader.hpp"
#include <glm/glm.hpp>
#include "Renderer2D.hpp"
#include "Renderer3D.hpp"

namespace Rengin
{

Renderer::SceneData* Renderer::m_scene_data = new Renderer::SceneData();


void Renderer::Init()
{
    RE_PROFILE_FUNCTION();
    RenderCommand::Init();
    Renderer2D::Init();
    Renderer3D::Init();
}

void Renderer::OnWindowResized(uint32_t width ,uint32_t height)
{
    RenderCommand::SetViewPort(0,0,width,height);
}

void Renderer::BeginScene(OrthoGraphicsCamera& camera)
{
    m_scene_data->ViewProjectionMat = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene()
{

}

void Renderer::Shutdown()
{

}

void Renderer::Submit(const Ref<Shader>& shader,const Ref<VertexArray>& vertexArray,const glm::mat4& transform)
{
    shader->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(shader)->UpLoadUniformMat4("u_ViewProjection",m_scene_data->ViewProjectionMat);
    std::dynamic_pointer_cast<OpenGLShader>(shader)->UpLoadUniformMat4("u_Transform",transform);
    vertexArray->Bind();
    RenderCommand::DrawIndex(vertexArray);
}

} // namespace Rengin

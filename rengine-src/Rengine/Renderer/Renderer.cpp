#include "repch.hpp"
#include "Renderer.hpp"
#include "Rengine/Platform/OpenGL/OpenGLShader.hpp"
#include <glm/glm.hpp>

namespace Rengin
{

Renderer::SceneData* Renderer::m_scene_data = new Renderer::SceneData();

void Renderer::BeginScene(OrthoGraphicsCamera& camera)
{
    m_scene_data->ViewProjectionMat = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene()
{

}

void Renderer::Submit(const std::shared_ptr<Shader>& shader,const std::shared_ptr<VertexArray>& vertexArray,const glm::mat4& transform)
{
    shader->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(shader)->UpLoadUniformMat4("u_ViewProjection",m_scene_data->ViewProjectionMat);
    std::dynamic_pointer_cast<OpenGLShader>(shader)->UpLoadUniformMat4("u_Transform",transform);
    vertexArray->Bind();
    RenderCommand::DrawIndex(vertexArray);
}

} // namespace Rengin

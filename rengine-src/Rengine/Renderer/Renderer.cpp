#include "repch.hpp"
#include "Renderer.hpp"

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

void Renderer::Submit(const std::shared_ptr<Shader>& shader,const std::shared_ptr<VertexArray>& vertexArray)
{
    shader->Bind();
    shader->UpLoadUniformMat4("u_ViewProjection",m_scene_data->ViewProjectionMat);
    vertexArray->Bind();
    RenderCommand::DrawIndex(vertexArray);
}

} // namespace Rengin

#include "repch.hpp"
#include "Renderer.hpp"

namespace Rengin
{
    
void Renderer::BeginScene()
{

}

void Renderer::EndScene()
{

}

void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
{
    vertexArray->Bind();
    RenderCommand::DrawIndex(vertexArray);
}

} // namespace Rengin

#include "repch.hpp"
#include "RendererObject.hpp"
#include "Rengine/Renderer/RenderCommand.hpp"

namespace Rengin
{

void RendererObject::AddObj(const ObjManager& obj)
{
    ObjLists.push_back(obj);
}

void RendererObject::DrawObject(uint32_t id,Ref<Shader> shader,const EditorCamera& camera)
{
    shader->Bind();

    shader->SetUniformMat4("u_Transform", glm::mat4(1.0));
    shader->SetUniformMat4("u_View", camera.GetViewMatrix());
    shader->SetUniformMat4("u_Projection", camera.getProjection());
    RenderCommand::DrawIndex(ObjLists[id].GetVertexArray());
}

} // namespace Rengin

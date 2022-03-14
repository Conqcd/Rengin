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
    shader->SetUniformInt("u_Entity",id);
    for (size_t i = 0; i < ObjLists[id].GetVertexArraySize(); i++)
    {
        ObjLists[id].BindTexture(i);
        shader->SetUniformInt("u_texture", i % 32);
        RenderCommand::DrawIndex(ObjLists[id].GetVertexArray(i));
    }
}

} // namespace Rengin

#include "repch.hpp"
#include "RendererObject.hpp"
#include "Rengine/Renderer/RenderCommand.hpp"

namespace Rengin
{

RendererObject::RendererObject()
{
    LightPos = glm::vec3(0,80,80);
    // LightIntensity = glm::vec3(1,5000,5000);
    LightIntensity = glm::vec3(5000,5000,5000);
}

void RendererObject::AddObj(const ObjManager& obj)
{
    ObjLists.push_back(obj);
}

void RendererObject::DrawObject(uint32_t id,Ref<Shader> shader,const EditorCamera& camera)
{
    shader->Bind();

    shader->SetUniformMat4("u_View", camera.GetViewMatrix());
    shader->SetUniformMat4("u_Projection", camera.getProjection());
    shader->SetUniformInt("u_Entity",id);
    shader->SetUniformFloat3("u_CameraPos", camera.GetPosition());
    shader->SetUniformFloat3("u_LightIntensity", LightIntensity);
    shader->SetUniformFloat3("u_LightPos", LightPos);
    for (size_t i = 0; i < ObjLists[id].GetVertexArraySize(); i++)
    {
        ObjLists[id].BindTexture(i);
        shader->SetUniformInt("u_texture", i % 32);
        shader->SetUniformMat4("u_Transform", ObjLists[id].GetTransform());
        shader->SetUniformFloat3("u_Ka", ObjLists[id].GetMaterial(i).Ka);
        shader->SetUniformFloat3("u_Ks", ObjLists[id].GetMaterial(i).Ks);
        shader->SetUniformFloat3("u_Kd", ObjLists[id].GetMaterial(i).Kd);
        shader->SetUniformFloat("u_Ns", ObjLists[id].GetMaterial(i).Ns);
        shader->SetUniformFloat("u_Ni", ObjLists[id].GetMaterial(i).Ni);
        RenderCommand::DrawIndex(ObjLists[id].GetVertexArray(i));
    }
}

} // namespace Rengin

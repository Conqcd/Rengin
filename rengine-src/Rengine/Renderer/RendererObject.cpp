#include "repch.hpp"
#include "RendererObject.hpp"
#include "Rengine/Renderer/RenderCommand.hpp"

namespace Rengin
{

RendererObject::RendererObject()
{
}

void RendererObject::AddObj(const ObjManager& obj)
{
    ObjLists.push_back(obj);
}

void RendererObject::AddMethod(const std::string &name, Ref<RenderMethod> method)
{
    if(m_Methods.find(name) != m_Methods.end())   return;
    m_Methods[name] = method;
}

void RendererObject::DrawObject(const std::string& methodName,const std::vector<int>& ids,const EditorCamera& camera)
{
    m_Methods[methodName]->Render(ids,ObjLists,camera,lights);
    // shader->Bind();

    // shader->SetUniformMat4("u_View", camera.GetViewMatrix());
    // shader->SetUniformMat4("u_Projection", camera.getProjection());
    // shader->SetUniformInt("u_Entity",id);
    // shader->SetUniformFloat3("u_CameraPos", camera.GetPosition());
    // shader->SetUniformFloat3("u_LightIntensity", LightIntensity);
    // shader->SetUniformFloat3("u_LightPos", LightPos);
    // for (size_t i = 0; i < ObjLists[id].GetVertexArraySize(); i++)
    // {
    //     ObjLists[id].BindTexture(i);
    //     shader->SetUniformInt("u_texture", i % 32);
    //     shader->SetUniformMat4("u_Transform", ObjLists[id].GetTransform());
    //     shader->SetUniformFloat3("u_Ka", ObjLists[id].GetMaterial(i).Ka);
    //     shader->SetUniformFloat3("u_Ks", ObjLists[id].GetMaterial(i).Ks);
    //     shader->SetUniformFloat3("u_Kd", ObjLists[id].GetMaterial(i).Kd);
    //     shader->SetUniformFloat("u_Ns", ObjLists[id].GetMaterial(i).Ns);
    //     shader->SetUniformFloat("u_Ni", ObjLists[id].GetMaterial(i).Ni);
    //     RenderCommand::DrawIndex(ObjLists[id].GetVertexArray(i));
    // }
}

} // namespace Rengin

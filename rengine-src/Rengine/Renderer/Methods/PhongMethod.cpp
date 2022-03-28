#include "repch.hpp"
#include "PhongMethod.hpp"
#include "Rengine/Renderer/RenderCommand.hpp"

namespace Rengin
{
 
PhongMethod::PhongMethod(/* args */)
{

}

PhongMethod::~PhongMethod()
{

}

void PhongMethod::Render(const std::vector<int>& ids,const std::vector<Ref<ObjManager>>& ObjLists,const EditorCamera& camera,const Lights& lights)
{
    m_BaseShader->Bind();
    m_BaseShader->SetUniformFloat3("u_LightPos", lights.LightPos);
    m_BaseShader->SetUniformMat4("u_View", camera.GetViewMatrix());
    m_BaseShader->SetUniformMat4("u_Projection", camera.getProjection());
    m_BaseShader->SetUniformFloat3("u_CameraPos", camera.GetPosition());
    m_BaseShader->SetUniformFloat3("u_LightIntensity", lights.LightIntensity);
    for (int i = 0; i < ids.size(); i++)
    {
        m_BaseShader->SetUniformInt("u_Entity", ids[i]);
        for (int j = 0; j < ObjLists[ids[i]]->GetVertexArraySize(); j++)
        {
            ObjLists[ids[i]]->BindTexture(j,j);
            m_BaseShader->SetUniformInt("u_texture", j % 32);
            m_BaseShader->SetUniformMat4("u_Transform",ObjLists[ids[i]]->GetTransform());
            m_BaseShader->SetUniformFloat3("u_Ka", ObjLists[ids[i]]->GetMaterial(j).Ka);
            m_BaseShader->SetUniformFloat3("u_Ks", ObjLists[ids[i]]->GetMaterial(j).Ks);
            m_BaseShader->SetUniformFloat3("u_Kd", ObjLists[ids[i]]->GetMaterial(j).Kd);
            m_BaseShader->SetUniformFloat("u_Ns", ObjLists[ids[i]]->GetMaterial(j).Ns);
            m_BaseShader->SetUniformFloat("u_Ni", ObjLists[ids[i]]->GetMaterial(j).Ni);
            RenderCommand::DrawIndex(ObjLists[ids[i]]->GetVertexArray(j));
        }
    }
}

template <typename... Args>
void PhongMethod::AddResource(const Args&... resource) {
    static_assert(false);
}

template <> 
void PhongMethod::AddResource<Ref<Shader>>(const Ref<Shader>& base) {
    m_BaseShader = base;
}
} // namespace Rengin

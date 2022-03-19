#include "repch.hpp"
#include "ShadowMapMethod.hpp"
#include "Rengine/Renderer/RenderCommand.hpp"

namespace Rengin
{

ShadowMapMethod::ShadowMapMethod()
{

}

ShadowMapMethod::~ShadowMapMethod()
{

}

void ShadowMapMethod::Render(const std::vector<int>& ids,const std::vector<ObjManager>& ObjLists,const EditorCamera& camera,const Lights& lights)
{
    m_ShadowMap->Bind();
    m_ShadowShader->Bind();
    auto LightVP = lights.GetViewProjection();
    for (int i = 0; i < ids.size(); i++)
        for (int j = 0; j < ObjLists[ids[i]].GetVertexArraySize(); j++) {
            m_BaseShader->SetUniformMat4("u_LightMVP", LightVP * ObjLists[ids[i]].GetTransform());
            RenderCommand::DrawIndex(ObjLists[ids[i]].GetVertexArray(j));
        }

    //          Phong
    m_MainFrame->Bind();
    m_BaseShader->Bind();
    m_BaseShader->SetUniformFloat3("u_LightPos", lights.LightPos);
    m_BaseShader->SetUniformMat4("u_View", camera.GetViewMatrix());
    m_BaseShader->SetUniformMat4("u_Projection", camera.getProjection());
    m_BaseShader->SetUniformFloat3("u_CameraPos", camera.GetPosition());
    m_BaseShader->SetUniformFloat3("u_LightIntensity", lights.LightIntensity);
    for (int i = 0; i < ids.size(); i++)
    {
        m_BaseShader->SetUniformInt("u_Entity", ids[i]);
        for (int j = 0; j < ObjLists[ids[i]].GetVertexArraySize(); j++)
        {
            ObjLists[ids[i]].BindTexture(j);
            m_BaseShader->SetUniformInt("u_texture", j % 32);

            m_ShadowMap->BindTexture(0,(j + 1) % 32);
            m_BaseShader->SetUniformInt("u_ShadowMap", (j + 1) % 32);

            m_BaseShader->SetUniformMat4("u_LightMVP", LightVP * ObjLists[ids[i]].GetTransform());
            m_BaseShader->SetUniformMat4("u_Transform",ObjLists[ids[i]].GetTransform());
            m_BaseShader->SetUniformFloat3("u_Ka", ObjLists[ids[i]].GetMaterial(j).Ka);
            m_BaseShader->SetUniformFloat3("u_Ks", ObjLists[ids[i]].GetMaterial(j).Ks);
            m_BaseShader->SetUniformFloat3("u_Kd", ObjLists[ids[i]].GetMaterial(j).Kd);
            m_BaseShader->SetUniformFloat("u_Ns", ObjLists[ids[i]].GetMaterial(j).Ns);
            m_BaseShader->SetUniformFloat("u_Ni", ObjLists[ids[i]].GetMaterial(j).Ni);
            RenderCommand::DrawIndex(ObjLists[ids[i]].GetVertexArray(j));
        }
    }
}

template <typename... Args>
void ShadowMapMethod::AddResource(const Args&... resource) {
    static_assert(false);
}

template <>
void ShadowMapMethod::AddResource<Ref<Shader>,Ref<Shader>>(const Ref<Shader>& base,const Ref<Shader>& shadow) {
    m_BaseShader = base;
    m_ShadowShader = shadow;
}

template <>
void ShadowMapMethod::AddResource<Ref<FrameBuffer>,Ref<FrameBuffer>>(const Ref<FrameBuffer>& Shadow,const Ref<FrameBuffer>& Main) {
    m_ShadowMap = Shadow;
    m_MainFrame = Main;
}

} // namespace Rengin

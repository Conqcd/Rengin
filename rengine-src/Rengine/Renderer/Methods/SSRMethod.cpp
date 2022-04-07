#include "repch.hpp"
#include "SSRMethod.hpp"
#include "Rengine/Renderer/RenderCommand.hpp"
#include <glad/glad.h>
#include <cstdio>

namespace Rengin
{

SSRMethod::SSRMethod()
{

}

SSRMethod::~SSRMethod()
{

}

void SSRMethod::Render(const std::vector<int>& ids,const std::vector<Ref<ObjManager>>& ObjLists,const EditorCamera& camera,const Lights& lights)
{
    m_ShadowMap->Bind();
    RenderCommand::SetClearColor({0.0f,0.0f,0.0f,0.0f});
    RenderCommand::Clear();
    RenderCommand::DisableAlpha();
    m_ShadowShader->Bind();
    auto LightVP = lights.GetViewProjection();
    for (int i = 0; i < ids.size(); i++)
        for (int j = 0; j < ObjLists[ids[i]]->GetVertexArraySize(); j++) {
            m_ShadowShader->SetUniformMat4("u_LightMVP", LightVP * ObjLists[ids[i]]->GetTransform());
            RenderCommand::DrawIndex(ObjLists[ids[i]]->GetVertexArray(j));
        }

    //          Phong
    m_MainFrame->Bind();
    RenderCommand::EnableAlpha();
    m_BaseShader->Bind();
    m_BaseShader->SetUniformFloat3("u_LightPos", lights.LightPos);
    m_BaseShader->SetUniformMat4("u_View", camera.GetViewMatrix());
    m_BaseShader->SetUniformMat4("u_Projection", camera.getProjection());
    m_BaseShader->SetUniformFloat3("u_CameraPos", camera.GetPosition());
    m_BaseShader->SetUniformFloat3("u_LightIntensity", lights.LightIntensity);
    int tidx = 0;

    m_ShadowMap->BindTexture(0,31);
    m_BaseShader->SetUniformInt("u_ShadowMap", 31);
    m_BaseShader->SetUniformInt("u_RenderType", 2);

    tidx = 0;
    for (int i = 0; i < ids.size(); i++)
    {
        m_BaseShader->SetUniformInt("u_Entity", ids[i]);
        m_BaseShader->SetUniformMat4("u_LightMVP",LightVP * ObjLists[ids[i]]->GetTransform());
        m_BaseShader->SetUniformMat4("u_Transform",ObjLists[ids[i]]->GetTransform());
        for (int j = 0; j < ObjLists[ids[i]]->GetVertexArraySize(); j++)
        {
            m_BaseShader->SetUniformInt("u_TextureID", tidx);
            ObjLists[ids[i]]->BindTexture(j, tidx);
            m_BaseShader->SetUniformInt("u_texture", tidx++);
            tidx %= 31;
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
void SSRMethod::AddResource(const Args&... resource) {
    static_assert(false);
}

template <>
void SSRMethod::AddResource<Ref<Shader>,Ref<Shader>>(const Ref<Shader>& base,const Ref<Shader>& shadow,const Ref<Shader>& GShader) {
    m_BaseShader = base;
    m_ShadowShader = shadow;
    m_GBufferShader = GShader;
}

template <>
void SSRMethod::AddResource<Ref<FrameBuffer>,Ref<FrameBuffer>>(const Ref<FrameBuffer>& GBuffer,const Ref<FrameBuffer>& Shadow,const Ref<FrameBuffer>& Main) {
    m_ShadowMap = Shadow;
    m_GBuffer = GBuffer;
    m_MainFrame = Main;
}

} // namespace Rengin

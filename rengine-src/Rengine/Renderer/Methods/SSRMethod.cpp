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
    
    int tidx = 0;
    // GBuffer
    m_GBuffer->Bind();
    RenderCommand::SetClearColor({0.0f,0.0f,0.0f,0.0f});
    RenderCommand::Clear();
    float value = 1000;
    m_GBuffer->ClearAttachment(1,&value);
    m_GBufferShader->Bind();
    m_GBufferShader->SetUniformMat4("u_LightVP", LightVP);
    m_GBufferShader->SetUniformMat4("u_View", camera.GetViewMatrix());
    m_GBufferShader->SetUniformMat4("u_Projection", camera.getProjection());
    m_ShadowMap->BindTexture(0,31);
    m_GBufferShader->SetUniformInt("u_ShadowMap", 31);
    tidx = 0; 

    for (int i = 0; i < ids.size(); i++)
    {
        m_GBufferShader->SetUniformMat4("u_Transform",ObjLists[ids[i]]->GetTransform());
        for (int j = 0; j < ObjLists[ids[i]]->GetVertexArraySize(); j++) {
            ObjLists[ids[i]]->BindTexture(j, tidx);
            m_GBufferShader->SetUniformInt("u_Kd", tidx++);
            RenderCommand::DrawIndex(ObjLists[ids[i]]->GetVertexArray(j));
            tidx %= 31;
        }
    }

    //          SSR
    m_MainFrame->Bind();
    // RenderCommand::SetClearColor({0.0f,0.0f,0.0f,0.0f});
    // RenderCommand::Clear();
    
    // float values[3] = {0,0,0};
    // RenderCommand::Clear(2);
    // RenderCommand::Clear();
    // m_MainFrame->ClearAttachment(2,values);
    RenderCommand::EnableAlpha();
    m_BaseShader->Bind();
    m_BaseShader->SetUniformMat4("u_View", camera.GetViewMatrix());
    m_BaseShader->SetUniformMat4("u_Projection", camera.getProjection());
    m_BaseShader->SetUniformFloat3("u_LightDir", lights.Direction);
    m_BaseShader->SetUniformFloat3("u_LightPos", lights.LightPos);
    m_BaseShader->SetUniformFloat3("u_CameraPos", camera.GetPosition());
    m_BaseShader->SetUniformFloat3("u_LightRadiance", lights.LightIntensity);
    m_BaseShader->SetUniformFloat2("u_WindowSize", camera.GetViewportSize());
    m_BaseShader->SetUniformFloat("u_NearZ", camera.GetNear());
    m_GBuffer->BindTexture(0,0);
    m_BaseShader->SetUniformInt("u_GDiffuse", 0);
    m_GBuffer->BindTexture(1,1);
    m_BaseShader->SetUniformInt("u_GDepth", 1);
    m_GBuffer->BindTexture(2,2);
    m_BaseShader->SetUniformInt("u_GNormalWorld", 2);
    m_GBuffer->BindTexture(3,3);
    m_BaseShader->SetUniformInt("u_GShadow", 3);
    m_GBuffer->BindTexture(4,4);
    m_BaseShader->SetUniformInt("u_GPosWorld", 4);
    float KS[3] = {0,0,0.8};
    for (int i = 0; i < ids.size(); i++)
    {
        m_BaseShader->SetUniformInt("u_Entity", ids[i]);
        m_BaseShader->SetUniformMat4("u_Transform",ObjLists[ids[i]]->GetTransform());
        m_BaseShader->SetUniformFloat("u_Ks",KS[i]);
        for (int j = 0; j < ObjLists[ids[i]]->GetVertexArraySize(); j++)
        {
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

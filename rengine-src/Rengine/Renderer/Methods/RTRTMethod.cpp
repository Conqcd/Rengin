#include "repch.hpp"
#include "RTRTMethod.hpp"
#include "Rengine/Renderer/RenderCommand.hpp"
#include <glad/glad.h>
#include <cstdio>

namespace Rengin
{

RTRTMethod::RTRTMethod()
{

}

RTRTMethod::~RTRTMethod()
{

}

void RTRTMethod::Render(const std::vector<int>& ids,const std::vector<Ref<ObjManager>>& ObjLists,const EditorCamera& camera,const Lights& lights)
{
    //          RTRT
    m_MainFrame->Bind();
    
    m_BaseShader->Bind();
    m_BaseShader->SetUniformMat4("u_View", camera.GetViewMatrix());
    m_BaseShader->SetUniformMat4("u_Projection", camera.getProjection());
    m_BaseShader->SetUniformFloat3("u_LightDir", lights.Direction);
    m_BaseShader->SetUniformFloat3("u_LightPos", lights.LightPos);
    m_BaseShader->SetUniformFloat3("u_CameraPos", camera.GetPosition());
    m_BaseShader->SetUniformFloat3("u_LightRadiance", lights.LightIntensity);
    m_BaseShader->SetUniformInt("u_LightNums", lights.LightTriNum);
    lights.LightBuffer->Bind(2);
    for (int i = 0; i < ids.size(); i++)
    {
        m_BaseShader->SetUniformInt("u_Entity", ids[i]);
        m_BaseShader->SetUniformMat4("u_Transform",ObjLists[ids[i]]->GetTransform());
        for (int j = 0; j < ObjLists[ids[i]]->GetVertexArraySize(); j++)
        {
            m_BaseShader->SetUniformFloat3("u_Ks",ObjLists[ids[i]]->GetMaterial(j).Ks);
            m_BaseShader->SetUniformFloat3("u_Kd",ObjLists[ids[i]]->GetMaterial(j).Kd);
            m_BaseShader->SetUniformFloat("u_Ns",ObjLists[ids[i]]->GetMaterial(j).Ns);
            RenderCommand::DrawIndex(ObjLists[ids[i]]->GetVertexArray(j));
        }
    }
}

template <typename... Args>
void RTRTMethod::AddResource(const Args&... resource) {
    static_assert(false);
}

template <>
void RTRTMethod::AddResource<Ref<Shader>>(const Ref<Shader>& base) {
    m_BaseShader = base;
    // m_GBufferShader = GShader;
}

template <>
void RTRTMethod::AddResource<Ref<FrameBuffer>>(const Ref<FrameBuffer>& Main) {
    // m_GBuffer = GBuffer;
    m_MainFrame = Main;
}

} // namespace Rengin

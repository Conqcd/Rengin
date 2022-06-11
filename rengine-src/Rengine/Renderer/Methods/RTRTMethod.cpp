#include "repch.hpp"
#include "RTRTMethod.hpp"
#include "Rengine/Renderer/RenderCommand.hpp"
#include <glad/glad.h>
#include <cstdio>

namespace Rengin
{

RTRTMethod::RTRTMethod()
{
    float SquareVertices[5 * 4] = {
        -1.0f,-1.0f,0.0f,0.0f,0.0f,
        1.0f,-1.0f,0.0f,1.0f,0.0f,
        1.0f,1.0f,0.0f,1.0f,1.0f,
        -1.0f,1.0f,0.0f,0.0f,1.0f
    };
    unsigned int indices[6] = {
        0,1,2,2,3,0
    };
    
    m_ScreenVertex = VertexArray::Create();
    auto VertexBuffer = VertexBuffer::Create(SquareVertices, sizeof(SquareVertices));
    BufferLayout layout = {
        {ShadeDataType::Float3 , "a_position"},
        {ShadeDataType::Float2 , "a_texCoords"}
    };
    VertexBuffer->SetLayout(layout);
    m_ScreenVertex->AddVertexBuffer(VertexBuffer);
    auto IndexBuffer = IndexBuffer::Create(indices,6);
    m_ScreenVertex->SetIndexBuffer(IndexBuffer);
}

RTRTMethod::~RTRTMethod()
{

}

void RTRTMethod::Render(const std::vector<int>& ids,const std::vector<Ref<ObjManager>>& ObjLists,const EditorCamera& camera,const Lights& lights)
{
    int TriNums = 0;
    for (int i = 0; i < ids.size(); i++)
        TriNums += ObjLists[ids[i]]->GetTriangleNums();

    //          RTRT
    m_GBuffer->Bind();
    RenderCommand::SetClearColor({0.0f,0.0f,0.0f,0.0f});
    RenderCommand::Clear();
    float value = 1000;
    m_GBuffer->ClearAttachment(0,&value);
    static float timeseed = 0;
    m_BaseShader->Bind();
    m_BaseShader->SetUniformMat4("u_View", camera.GetViewMatrix());
    m_BaseShader->SetUniformMat4("u_Projection", camera.getProjection());
    m_BaseShader->SetUniformFloat3("u_LightDir", lights.Direction);
    m_BaseShader->SetUniformFloat3("u_LightPos", lights.LightPos);
    m_BaseShader->SetUniformFloat3("u_CameraPos", camera.GetPosition());
    m_BaseShader->SetUniformFloat3("u_LightRadiance", lights.LightIntensity);
    m_BaseShader->SetUniformInt("u_LightNums", lights.LightTriNum);
    m_BaseShader->SetUniformInt("u_trianglenums", TriNums);
    m_BaseShader->SetUniformInt("u_TimeSeed", timeseed);
    m_BaseShader->SetUniformInt("u_Bounce", 1);
    timeseed += 0.5;
    // lights.LightBuffer->Bind(2);
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
    m_MainFrame->Bind();

    m_DeNoiseShader->Bind();
    m_GBuffer->BindTexture(0,0);
    m_DeNoiseShader->SetUniformInt("u_Screen", 0);
    m_DeNoiseShader->SetUniformFloat2("u_WindowSize", camera.GetViewportSize());
    m_DeNoiseShader->SetUniformInt("u_FilterSize", 7);
    m_DeNoiseShader->SetUniformFloat("u_sigmap", 10.0);
    m_DeNoiseShader->SetUniformFloat("u_sigmac", 10.0);
    RenderCommand::DrawIndex(m_ScreenVertex);
}

template <typename... Args>
void RTRTMethod::AddResource(const Args&... resource) {
    static_assert(false);
}

template <>
void RTRTMethod::AddResource<Ref<Shader>,Ref<Shader>>(const Ref<Shader>& base,const Ref<Shader>& DNShader) {
    m_BaseShader = base;
    m_DeNoiseShader = DNShader;
}

template <>
void RTRTMethod::AddResource<Ref<FrameBuffer>,Ref<FrameBuffer>>(const Ref<FrameBuffer>& GBuffer,const Ref<FrameBuffer>& Main) {
    m_GBuffer = GBuffer;
    m_MainFrame = Main;
}

} // namespace Rengin

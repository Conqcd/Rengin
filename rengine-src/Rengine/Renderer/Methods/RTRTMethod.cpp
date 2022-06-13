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
    FrameBufferSpecification FbSpecLast;
    FbSpecLast.Attachments = {FramebufferTextureFormat::RGBA8,
                                FramebufferTextureFormat::Depth
                                };
    FbSpecLast.Width = 1280;
    FbSpecLast.Height = 720;
    m_LastFrame = FrameBuffer::Create(FbSpecLast);
    m_LastShader = Shader::Create("../../../Rengine-Editor/assets/shaders/CopyFrameVertex.glsl","../../../Rengine-Editor/assets/shaders/CopyFrameFragment.glsl");
    m_PreShader = Shader::Create("../../../Rengine-Editor/assets/shaders/ForDeferVertex.glsl","../../../Rengine-Editor/assets/shaders/ForDeferFragment.glsl");
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
    RenderCommand::DisableAlpha();
    RenderCommand::SetClearColor({0.0f,0.0f,0.0f,0.0f});
    RenderCommand::Clear();
    // float value[4] = {1,1,1,1};
    int value = -1;
    m_GBuffer->ClearAttachment(4,&value);
    static float timeseed = 0;
    static glm::mat4 u_LastView,u_LastProjection;
    if(timeseed == 0)
    {
        u_LastView = camera.GetViewMatrix();
        u_LastProjection = camera.getProjection();
    }
    m_PreShader->Bind();
    m_PreShader->SetUniformMat4("u_View", camera.GetViewMatrix());
    m_PreShader->SetUniformMat4("u_Projection", camera.getProjection());
    for (int i = 0; i < ids.size(); i++)
    {
        m_PreShader->SetUniformMat4("u_Transform",ObjLists[ids[i]]->GetTransform());
        for (int j = 0; j < ObjLists[ids[i]]->GetVertexArraySize(); j++)
        {
            RenderCommand::DrawIndex(ObjLists[ids[i]]->GetVertexArray(j));
        }
    }

    m_BaseShader->Bind();
    m_BaseShader->SetUniformMat4("u_View", camera.GetViewMatrix());
    m_BaseShader->SetUniformMat4("u_Projection", camera.getProjection());
    m_BaseShader->SetUniformFloat3("u_LightDir", lights.Direction);
    m_BaseShader->SetUniformFloat3("u_LightPos", lights.LightPos);
    m_BaseShader->SetUniformFloat3("u_CameraPos", camera.GetPosition());
    m_BaseShader->SetUniformFloat3("u_LightRadiance", lights.LightIntensity);
    m_BaseShader->SetUniformInt("u_LightNums", lights.LightTriNum);
    m_BaseShader->SetUniformInt("u_trianglenums", TriNums);
    m_BaseShader->SetUniformFloat("u_TimeSeed", timeseed);
    m_BaseShader->SetUniformInt("u_Bounce", 1);
    timeseed += 100.5;
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

    // Denoise
    m_DenoiseBuffer->Bind();
    RenderCommand::DisableAlpha();
    RenderCommand::SetClearColor({0.0f,0.0f,0.0f,0.0f});
    RenderCommand::Clear();
    // m_DenoiseBuffer->ClearAttachment(1,&value);
    m_DeNoiseShader->Bind();

    m_GBuffer->BindTexture(0,0);
    m_DeNoiseShader->SetUniformInt("u_Screen", 0);
    m_GBuffer->BindTexture(1,1);
    m_DeNoiseShader->SetUniformInt("u_NormalTex", 1);
    m_GBuffer->BindTexture(2,2);
    m_DeNoiseShader->SetUniformInt("u_PositionTex", 2);
    m_GBuffer->BindTexture(3,3);
    m_DeNoiseShader->SetUniformInt("u_DepthTex", 3);
    m_GBuffer->BindTexture(4,4);
    m_DeNoiseShader->SetUniformInt("u_EntityTex", 4);
    m_DeNoiseShader->SetUniformFloat2("u_WindowSize", camera.GetViewportSize());
    m_DeNoiseShader->SetUniformInt("u_FilterSize", 5);
    m_DeNoiseShader->SetUniformInt("u_Level", 3);
    m_DeNoiseShader->SetUniformFloat("u_sigmap", 10.0);
    m_DeNoiseShader->SetUniformFloat("u_sigmac", 10.0);
    m_DeNoiseShader->SetUniformFloat("u_sigman", 0.5);
    m_DeNoiseShader->SetUniformFloat("u_sigmad", 1.0);
    RenderCommand::DrawIndex(m_ScreenVertex);

    // Accumulate Result
    m_MainFrame->Bind();
    RenderCommand::EnableAlpha();
    m_AccShader->Bind();
    m_DenoiseBuffer->BindTexture(0,0);
    m_AccShader->SetUniformInt("u_Screen", 0);
    m_LastFrame->BindTexture(0,1);
    m_AccShader->SetUniformInt("u_LastScreen", 1);
    m_GBuffer->BindTexture(2,2);
    m_AccShader->SetUniformInt("u_PositionTex", 2);

    m_AccShader->SetUniformFloat("u_alpha", 0.2);
    m_AccShader->SetUniformFloat("u_k", 3);
    m_AccShader->SetUniformInt("u_FilterSize", 5);
    m_AccShader->SetUniformMat4("u_LastView", u_LastView);
    m_AccShader->SetUniformFloat2("u_WindowSize", camera.GetViewportSize());
    m_AccShader->SetUniformMat4("u_LastProjection", u_LastProjection);
    m_GBuffer->BindTexture(4,3);
    m_AccShader->SetUniformInt("u_EntityTex", 3);
    RenderCommand::DrawIndex(m_ScreenVertex);

    //Save Last Frame

    m_LastFrame->Bind();
    RenderCommand::SetClearColor({0.0f,0.0f,0.0f,0.0f});
    RenderCommand::Clear();
    m_LastShader->Bind();
    m_MainFrame->BindTexture(0,0);
    m_LastShader->SetUniformInt("u_Screen", 0);
    RenderCommand::DrawIndex(m_ScreenVertex);
    
    u_LastView = camera.GetViewMatrix();
    u_LastProjection = camera.getProjection();
}

template <typename... Args>
void RTRTMethod::AddResource(const Args&... resource) {
    static_assert(false);
}

template <>
void RTRTMethod::AddResource<Ref<Shader>,Ref<Shader>,Ref<Shader>>(const Ref<Shader>& base,const Ref<Shader>& DNShader,const Ref<Shader>& AccShader) {
    m_BaseShader = base;
    m_DeNoiseShader = DNShader;
    m_AccShader = AccShader;
}

template <>
void RTRTMethod::AddResource<Ref<FrameBuffer>,Ref<FrameBuffer>,Ref<FrameBuffer>>(const Ref<FrameBuffer>& GBuffer,const Ref<FrameBuffer>& Denoise,const Ref<FrameBuffer>& Main) {
    m_GBuffer = GBuffer;
    m_DenoiseBuffer = Denoise;
    m_MainFrame = Main;
}

} // namespace Rengin

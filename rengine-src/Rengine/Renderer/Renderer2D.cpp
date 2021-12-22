#include "repch.hpp"
#include "Renderer2D.hpp"
#include "Rengine/Renderer/VertexArray.hpp"
#include "Rengine/Renderer/Shader.hpp"
#include "Rengine/Renderer/RenderCommand.hpp"
#include <glm/gtc/matrix_transform.hpp>
// #include "Rengine/Platform/OpenGL/OpenGLShader.hpp"

namespace Rengin
{

struct Renderer2DStorage
{
    Ref<VertexArray> vertexArray;
    Ref<Shader> m_shader;
    Ref<Shader> m_Texshader;
    // Ref<VertexBuffer> m_verbuf;
    // Ref<IndexBuffer> m_indbuf;
    Ref<Texture2D> m_WhiteTexture;
};

static Renderer2DStorage* s_data;

void Renderer2D::Init()
{
    s_data = new Renderer2DStorage();
    s_data->vertexArray = VertexArray::Create();

    float SquareVertices[5 * 4] = {
        -0.5f,-0.5f,0.0f,0.0f,0.0f,
        0.5f,-0.5f,0.0f,1.0f,0.0f,
        0.5f,0.5f,0.0f,1.0f,1.0f,
        -0.5f,0.5f,0.0f,0.0f,1.0f
    };
    unsigned int indices[6] = {
        0,1,2,2,3,0
    };

    auto m_verbuf = VertexBuffer::Create(SquareVertices,sizeof(SquareVertices));
    // m_verbuf->Bind();

    BufferLayout layout = {
        {ShadeDataType::Float3 , "a_position"},
        {ShadeDataType::Float2 , "a_texCoords"}
        };
    m_verbuf->SetLayout(layout);

    s_data->vertexArray->AddVertexBuffer(m_verbuf);

    auto m_indbuf = IndexBuffer::Create(indices,sizeof(indices) / sizeof(uint32_t));
    // m_indbuf->Bind();

    s_data->vertexArray->SetIndexBuffer(m_indbuf);

    s_data->m_WhiteTexture = Texture2D::Create(1,1);
    uint32_t whiteColor = 0xffffffff;
    s_data->m_WhiteTexture->setData(&whiteColor,sizeof(whiteColor));

    // s_data->m_shader = Shader::Create("litle","../../../SandBox/assets/shaders/FlatColorVertex.glsl","../../../SandBox/assets/shaders/FlatColorFragment.glsl");
    s_data->m_Texshader = Shader::Create("litle","../../../SandBox/assets/shaders/textureVertex.glsl","../../../SandBox/assets/shaders/textureFragment.glsl");
    s_data->m_Texshader->Bind();
    s_data->m_Texshader->SetUniformInt("u_texture",0);
}

void Renderer2D::Shutdown()
{
    delete s_data;
}

void Renderer2D::OnWindowResized(uint32_t width ,uint32_t height)
{

}

void Renderer2D::BeginScene(OrthoGraphicsCamera& camera)
{
    s_data->m_Texshader->Bind();
    s_data->m_Texshader->SetUniformMat4("u_ViewProjection",camera.GetViewProjectionMatrix());
}

void Renderer2D::EndScene()
{

}

void Renderer2D::Submit(const Ref<Shader>& shader,const Ref<VertexArray>& vertexArray,const glm::mat4& transform)
{

}

void Renderer2D::DrawQuad(const glm::vec2& position,const glm::vec2& size,const glm::vec4& m_SquareColor)
{
    DrawQuad({position.x,position.y,0.0},size,m_SquareColor);
}

void Renderer2D::DrawQuad(const glm::vec3& position,const glm::vec2& size,const glm::vec4& m_SquareColor)
{
    s_data->m_Texshader->Bind();
    glm::mat4 transforms = glm::translate(glm::mat4(1.0),position) * glm::scale(glm::mat4(1.0),{size.x,size.y,1.0});
    s_data->m_Texshader->SetUniformMat4("u_Transform",transforms);
    s_data->m_Texshader->SetUniformFloat4("u_color",m_SquareColor);
    
    s_data->m_WhiteTexture->Bind();
    s_data->vertexArray->Bind();
    RenderCommand::DrawIndex(s_data->vertexArray);
}

void Renderer2D::DrawQuad(const glm::vec2& position,const glm::vec2& size,const Ref<Texture>& texture)
{
    DrawQuad({position.x,position.y,0.0},size,texture);
}

void Renderer2D::DrawQuad(const glm::vec3& position,const glm::vec2& size,const Ref<Texture>& texture)
{
    s_data->m_Texshader->Bind();
    
    s_data->m_Texshader->SetUniformFloat4("u_color",{1.0,1.0,1.0,1.0});
    glm::mat4 transforms = glm::translate(glm::mat4(1.0),position) * glm::scale(glm::mat4(1.0),{size.x,size.y,1.0});
    s_data->m_Texshader->SetUniformMat4("u_Transform",transforms);

    texture->Bind();
    s_data->vertexArray->Bind();
    RenderCommand::DrawIndex(s_data->vertexArray);
}


} // namespace Rengin

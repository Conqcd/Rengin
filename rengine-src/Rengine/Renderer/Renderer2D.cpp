#include "repch.hpp"
#include "Renderer2D.hpp"
#include "Rengine/Renderer/VertexArray.hpp"
#include "Rengine/Renderer/Shader.hpp"
#include "Rengine/Renderer/RenderCommand.hpp"
#include "Rengine/Platform/OpenGL/OpenGLShader.hpp"

namespace Rengin
{

struct Renderer2DStorage
{
    Ref<VertexArray> vertexArray;
    Ref<Shader> m_shader;
    // Ref<VertexBuffer> m_verbuf;
    // Ref<IndexBuffer> m_indbuf;
    // Ref<Texture> m_texture;
};

static Renderer2DStorage* s_data;

void Renderer2D::Init()
{
    s_data = new Renderer2DStorage();
    s_data->vertexArray = VertexArray::Create();

    float SquareVertices[3 * 4] = {
        -0.5f,-0.5f,0.0f,
        0.5f,-0.5f,0.0f,
        0.0f,0.5f,0.0f,
        -0.5f,0.5f,0.0f
    };
    unsigned int indices[6] = {
        0,1,2,2,3,0
    };

    auto m_verbuf = VertexBuffer::Create(SquareVertices,sizeof(SquareVertices));
    // m_verbuf->Bind();

    BufferLayout layout = {{ShadeDataType::Float3 , "a_position"}};
    m_verbuf->SetLayout(layout);

    s_data->vertexArray->AddVertexBuffer(m_verbuf);

    auto m_indbuf = IndexBuffer::Create(indices,sizeof(indices) / sizeof(uint32_t));
    // m_indbuf->Bind();

    s_data->vertexArray->SetIndexBuffer(m_indbuf);

    s_data->m_shader = Shader::Create("litle","assets/shaders/FlatColorVertex.glsl","assets/shaders/FlatColorFragment.glsl");
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
    s_data->m_shader->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(s_data->m_shader)->UpLoadUniformMat4("u_ViewProjection",camera.GetViewProjectionMatrix());
    std::dynamic_pointer_cast<OpenGLShader>(s_data->m_shader)->UpLoadUniformMat4("u_Transform",glm::mat4(1.0));
}

void Renderer2D::EndScene()
{

}

void Renderer2D::Submit(const Ref<Shader>& shader,const Ref<VertexArray>& vertexArray,const glm::mat4& transform)
{

}

void Renderer2D::DrawQuad(const glm::vec2& position,const glm::vec2& size,const glm::vec4& m_SquareColor)
{
    s_data->m_shader->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(s_data->m_shader)->UpLoadUniformFloat4("u_Color",m_SquareColor);
    s_data->vertexArray->Bind();
    RenderCommand::DrawIndex(s_data->vertexArray);
}

void Renderer2D::DrawQuad(const glm::vec3& position,const glm::vec2& size,const glm::vec4& m_SquareColor)
{
    s_data->m_shader->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(s_data->m_shader)->UpLoadUniformFloat4("u_Color",m_SquareColor);
    s_data->vertexArray->Bind();
    RenderCommand::DrawIndex(s_data->vertexArray);
}


} // namespace Rengin

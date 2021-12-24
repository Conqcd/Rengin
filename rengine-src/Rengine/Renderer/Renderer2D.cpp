#include "repch.hpp"
#include "Renderer2D.hpp"
#include "Rengine/Renderer/VertexArray.hpp"
#include "Rengine/Renderer/Shader.hpp"
#include "Rengine/Renderer/RenderCommand.hpp"
#include <glm/gtc/matrix_transform.hpp>
// #include "Rengine/Platform/OpenGL/OpenGLShader.hpp"

namespace Rengin
{

struct QuadVertex
{
    glm::vec3 m_position;
    glm::vec4 m_color;
    glm::vec2 m_texCoord;
    float TexIndex;
    float tiling_factor;
};

struct Renderer2DData
{
    const uint32_t MaxQuads = 10000;
    const uint32_t MaxVertices = MaxQuads * 4;
    const uint32_t MaxIndices = MaxQuads * 6;
    static const uint32_t MaxTextureSlots = 32;

    
    Ref<VertexArray> QuadVertexArray;
    Ref<VertexBuffer> QuadVertexBuffer;

    Ref<VertexArray> vertexArray;
    Ref<Shader> m_Texshader;
    Ref<Texture2D> m_WhiteTexture;
    
    uint32_t IndicesCount = 0;
    QuadVertex* QuadVertexBufferBase = nullptr;
    QuadVertex* QuadVertexBufferPtr = nullptr;

    std::array<Ref<Texture>,MaxTextureSlots> TextureSolts;
    uint32_t TextureSlotIndex = 1;
};

static Renderer2DData s_data;


void Renderer2D::Init()
{
    RE_PROFILE_FUNCTION();
    s_data.vertexArray = VertexArray::Create();

    float SquareVertices[5 * 4] = {
        -0.5f,-0.5f,0.0f,0.0f,0.0f,
        0.5f,-0.5f,0.0f,1.0f,0.0f,
        0.5f,0.5f,0.0f,1.0f,1.0f,
        -0.5f,0.5f,0.0f,0.0f,1.0f
    };
    unsigned int indices[6] = {
        0,1,2,2,3,0
    };
    s_data.QuadVertexBuffer = VertexBuffer::Create(s_data.MaxVertices * sizeof(QuadVertex));
    // auto m_verbuf = VertexBuffer::Create(SquareVertices,sizeof(SquareVertices));
    s_data.QuadVertexBufferBase = new QuadVertex[s_data.MaxVertices]; 

    BufferLayout layout = {
        {ShadeDataType::Float3 , "a_position"},
        {ShadeDataType::Float4 , "a_color"},
        {ShadeDataType::Float2 , "a_texCoords"},
        {ShadeDataType::Float , "a_texIndex"},
        {ShadeDataType::Float , "a_TilingFactor"}
        };
    // m_verbuf->SetLayout(layout);
    s_data.QuadVertexBuffer->SetLayout(layout);
    
    // s_data.vertexArray->AddVertexBuffer(m_verbuf);
    s_data.vertexArray->AddVertexBuffer(s_data.QuadVertexBuffer);

    uint32_t *QuadIndices = new uint32_t[s_data.MaxIndices];

    auto QuadIB = IndexBuffer::Create(QuadIndices,s_data.MaxIndices);

    uint32_t offset = 0;
    for (uint32_t i = 0; i < s_data.MaxIndices; i+=6)
    {
        QuadIndices[i + 0] = offset + 0;
        QuadIndices[i + 1] = offset + 1;
        QuadIndices[i + 2] = offset + 2;
        
        QuadIndices[i + 3] = offset + 2;
        QuadIndices[i + 4] = offset + 3;
        QuadIndices[i + 5] = offset + 0;

        offset += 4;
    }
    

    s_data.vertexArray->SetIndexBuffer(QuadIB);
    delete[] QuadIndices;

    // auto m_indbuf = IndexBuffer::Create(indices,sizeof(indices) / sizeof(uint32_t));

    // s_data.vertexArray->SetIndexBuffer(m_indbuf);

    s_data.m_WhiteTexture = Texture2D::Create(1,1);
    uint32_t whiteColor = 0xffffffff;
    s_data.m_WhiteTexture->setData(&whiteColor,sizeof(whiteColor));

    // s_data.m_shader = Shader::Create("litle","../../../SandBox/assets/shaders/FlatColorVertex.glsl","../../../SandBox/assets/shaders/FlatColorFragment.glsl");
    s_data.m_Texshader = Shader::Create("litle","../../../SandBox/assets/shaders/textureVertex.glsl","../../../SandBox/assets/shaders/textureFragment.glsl");
    s_data.m_Texshader->Bind();

    int32_t samplers[s_data.MaxTextureSlots];

    for (int i = 0; i < s_data.MaxTextureSlots; i++)
        samplers[i] = i;
    
    s_data.m_Texshader->SetUniformIntArray("u_textures",samplers,s_data.MaxTextureSlots);

    s_data.TextureSolts[0] = s_data.m_WhiteTexture;
}

void Renderer2D::Shutdown()
{
    RE_PROFILE_FUNCTION();
}

void Renderer2D::OnWindowResized(uint32_t width ,uint32_t height)
{

}

void Renderer2D::BeginScene(OrthoGraphicsCamera& camera)
{
    RE_PROFILE_FUNCTION();

    s_data.m_Texshader->Bind();
    s_data.m_Texshader->SetUniformMat4("u_ViewProjection",camera.GetViewProjectionMatrix());

    s_data.QuadVertexBufferPtr = s_data.QuadVertexBufferBase;
    s_data.IndicesCount = 0;

    s_data.TextureSlotIndex = 1;
}

void Renderer2D::EndScene()
{
    RE_PROFILE_FUNCTION();

    uint32_t dataSize = reinterpret_cast<uint8_t*>(s_data.QuadVertexBufferPtr) - reinterpret_cast<uint8_t*>(s_data.QuadVertexBufferBase);
    s_data.QuadVertexBuffer->SetData(s_data.QuadVertexBufferBase,dataSize);
    Flush();
}

void Renderer2D::Flush()
{
    for (uint32_t i = 0 ; i < s_data.TextureSlotIndex; i++)
        s_data.TextureSolts[i]->Bind(i);

    RenderCommand::DrawIndex(s_data.vertexArray,s_data.IndicesCount);
}

void Renderer2D::DrawQuad(const glm::vec2& position,const glm::vec2& size,const glm::vec4& m_SquareColor)
{
    DrawQuad({position.x,position.y,0.0},size,m_SquareColor);
}

void Renderer2D::DrawQuad(const glm::vec3& position,const glm::vec2& size,const glm::vec4& m_SquareColor)
{
    RE_PROFILE_FUNCTION();

    const float TexIndex = 0.0f;
    const float tile_factor = 1.0f;

    s_data.QuadVertexBufferPtr->m_position = position;
    s_data.QuadVertexBufferPtr->m_color = m_SquareColor;
    s_data.QuadVertexBufferPtr->m_texCoord = {0.0f,0.0f};
    s_data.QuadVertexBufferPtr->TexIndex = TexIndex;
    s_data.QuadVertexBufferPtr->tiling_factor = tile_factor;
    s_data.QuadVertexBufferPtr++;

    
    s_data.QuadVertexBufferPtr->m_position = {position.x + size.x,position.y,position.z};
    s_data.QuadVertexBufferPtr->m_color = m_SquareColor;
    s_data.QuadVertexBufferPtr->m_texCoord = {1.0f,0.0f};
    s_data.QuadVertexBufferPtr->TexIndex = TexIndex;
    s_data.QuadVertexBufferPtr->tiling_factor = tile_factor;
    s_data.QuadVertexBufferPtr++;
    
    s_data.QuadVertexBufferPtr->m_position = {position.x + size.x,position.y + size.y,position.z};
    s_data.QuadVertexBufferPtr->m_color = m_SquareColor;
    s_data.QuadVertexBufferPtr->m_texCoord = {1.0f,1.0f};
    s_data.QuadVertexBufferPtr->TexIndex = TexIndex;
    s_data.QuadVertexBufferPtr->tiling_factor = tile_factor;
    s_data.QuadVertexBufferPtr++;
    
    s_data.QuadVertexBufferPtr->m_position = {position.x,position.y + size.y,position.z};
    s_data.QuadVertexBufferPtr->m_color = m_SquareColor;
    s_data.QuadVertexBufferPtr->m_texCoord = {0.0f,1.0f};
    s_data.QuadVertexBufferPtr->TexIndex = TexIndex;
    s_data.QuadVertexBufferPtr->tiling_factor = tile_factor;
    s_data.QuadVertexBufferPtr++;


    s_data.IndicesCount += 6;

#if OLD_PATH
    s_data.m_Texshader->Bind();
    glm::mat4 transforms = glm::translate(glm::mat4(1.0),position) * glm::scale(glm::mat4(1.0),{size.x,size.y,1.0});
    s_data.m_Texshader->SetUniformMat4("u_Transform",transforms);
    s_data.m_Texshader->SetUniformFloat4("u_color",m_SquareColor);
    s_data.m_Texshader->SetUniformFloat("u_TilingFactor",1.0f);
    
    s_data.m_WhiteTexture->Bind();
    s_data.vertexArray->Bind();
    RenderCommand::DrawIndex(s_data.vertexArray);
#endif
}

void Renderer2D::DrawQuad(const glm::vec2& position,const glm::vec2& size,const Ref<Texture>& texture,float tile_factor,const glm::vec4& tintColor)
{
    DrawQuad({position.x,position.y,0.0},size,texture,tile_factor,tintColor);
}

void Renderer2D::DrawQuad(const glm::vec3& position,const glm::vec2& size,const Ref<Texture>& texture,float tile_factor,const glm::vec4& tintColor)
{
    RE_PROFILE_FUNCTION();

    constexpr glm::vec4 Color = {1.0f,1.0f,1.0f,1.0f};

    float textureIndex = 0.0f;

    for (uint32_t i = 1 ; i < s_data.TextureSolts.size(); i++)
    {
        if(*s_data.TextureSolts[i].get() == *texture.get())
        {
            textureIndex = static_cast<float>(i);
            break;
        }
    }

    if(textureIndex == 0.0f)
    {
        textureIndex = static_cast<float>(s_data.TextureSlotIndex);
        s_data.TextureSolts[s_data.TextureSlotIndex] = texture;
        s_data.TextureSlotIndex++;
    }

    s_data.QuadVertexBufferPtr->m_position = position;
    s_data.QuadVertexBufferPtr->m_color = Color;
    s_data.QuadVertexBufferPtr->m_texCoord = {0.0f,0.0f};
    s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_data.QuadVertexBufferPtr->tiling_factor = tile_factor;
    s_data.QuadVertexBufferPtr++;

    
    s_data.QuadVertexBufferPtr->m_position = {position.x + size.x,position.y,position.z};
    s_data.QuadVertexBufferPtr->m_color = Color;
    s_data.QuadVertexBufferPtr->m_texCoord = {1.0f,0.0f};
    s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_data.QuadVertexBufferPtr->tiling_factor = tile_factor;
    s_data.QuadVertexBufferPtr++;
    
    s_data.QuadVertexBufferPtr->m_position = {position.x + size.x,position.y + size.y,position.z};
    s_data.QuadVertexBufferPtr->m_color = Color;
    s_data.QuadVertexBufferPtr->m_texCoord = {1.0f,1.0f};
    s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_data.QuadVertexBufferPtr->tiling_factor = tile_factor;
    s_data.QuadVertexBufferPtr++;
    
    s_data.QuadVertexBufferPtr->m_position = {position.x,position.y + size.y,position.z};
    s_data.QuadVertexBufferPtr->m_color = Color;
    s_data.QuadVertexBufferPtr->m_texCoord = {0.0f,1.0f};
    s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_data.QuadVertexBufferPtr->tiling_factor = tile_factor;
    s_data.QuadVertexBufferPtr++;


    s_data.IndicesCount += 6;

#if OLD_PATH
    s_data.m_Texshader->Bind();
    
    s_data.m_Texshader->SetUniformFloat4("u_color",tintColor);
    
    s_data.m_Texshader->SetUniformFloat("u_TilingFactor",tile_factor);
    glm::mat4 transforms = glm::translate(glm::mat4(1.0),position) * glm::scale(glm::mat4(1.0),{size.x,size.y,1.0});
    s_data.m_Texshader->SetUniformMat4("u_Transform",transforms);

    texture->Bind();
    s_data.vertexArray->Bind();
    RenderCommand::DrawIndex(s_data.vertexArray);
#endif
}

void Renderer2D::DrawRotatedQuad(const glm::vec2& position,const glm::vec2& size,float rotation,const glm::vec4& color)
{
    DrawRotatedQuad({position.x,position.y,0.0},size,rotation,color);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& position,const glm::vec2& size,float rotation,const glm::vec4& m_SquareColor)
{
    RE_PROFILE_FUNCTION();
    s_data.m_Texshader->Bind();
    glm::mat4 transforms = glm::translate(glm::mat4(1.0),position) 
    * glm::rotate(glm::mat4(1.0),rotation,{0.0,0.0,1.0})
    * glm::scale(glm::mat4(1.0),{size.x,size.y,1.0});
    s_data.m_Texshader->SetUniformMat4("u_Transform",transforms);
    s_data.m_Texshader->SetUniformFloat4("u_color",m_SquareColor);
    s_data.m_Texshader->SetUniformFloat("u_TilingFactor",1.0f);
    
    s_data.m_WhiteTexture->Bind();
    s_data.vertexArray->Bind();
    RenderCommand::DrawIndex(s_data.vertexArray);
}

void Renderer2D::DrawRotatedQuad(const glm::vec2& position,const glm::vec2& size,float rotation,const Ref<Texture>& texture,float tile_factor,const glm::vec4& tintColor)
{
    DrawRotatedQuad({position.x,position.y,0.0},size,rotation,texture,tile_factor,tintColor);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& position,const glm::vec2& size,float rotation,const Ref<Texture>& texture,float tile_factor,const glm::vec4& tintColor)
{
    RE_PROFILE_FUNCTION();

    s_data.m_Texshader->Bind();
    
    s_data.m_Texshader->SetUniformFloat4("u_color",tintColor);
    
    s_data.m_Texshader->SetUniformFloat("u_TilingFactor",tile_factor);
    glm::mat4 transforms = glm::translate(glm::mat4(1.0),position) 
    * glm::rotate(glm::mat4(1.0),rotation,{0.0,0.0,1.0})
    * glm::scale(glm::mat4(1.0),{size.x,size.y,1.0});
    s_data.m_Texshader->SetUniformMat4("u_Transform",transforms);

    texture->Bind();
    s_data.vertexArray->Bind();
    RenderCommand::DrawIndex(s_data.vertexArray);
}

} // namespace Rengin

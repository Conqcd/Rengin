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

    //attach
    int EntityID = 0;
};

struct CircleVertex
{
    glm::vec3 m_WorldPosition;
    glm::vec3 m_LoaclPosition;
    glm::vec4 m_color;
    float m_thickness;
    float m_fade;

    //attach
    int EntityID = 0;
};


struct Renderer2DData
{
    static const uint32_t MaxQuads = 20000;
    static const uint32_t MaxVertices = MaxQuads * 4;
    static const uint32_t MaxIndices = MaxQuads * 6;
    static const uint32_t MaxTextureSlots = 32;

    // Quad
    Ref<VertexArray> QuadVertexArray;
    Ref<VertexBuffer> QuadVertexBuffer;

    Ref<VertexArray> vertexArray;
    Ref<Shader> m_Texshader;
    Ref<Texture2D> m_WhiteTexture;

    uint32_t IndicesCount = 0;
    QuadVertex* QuadVertexBufferBase = nullptr;
    QuadVertex* QuadVertexBufferPtr = nullptr;

    // Circle
    Ref<VertexArray> CircleVertexArray;
    Ref<VertexBuffer> CircleVertexBuffer;
    Ref<Shader> m_CircleShader;

    uint32_t CircleIndicesCount = 0;
    CircleVertex* CircleVertexBufferBase = nullptr;
    CircleVertex* CircleVertexBufferPtr = nullptr;


    std::array<Ref<Texture>,MaxTextureSlots> TextureSolts;
    uint32_t TextureSlotIndex = 1;

    glm::vec4 QuadVertexPosition[4];

    Renderer2D::Statistic stats;
};

static Renderer2DData s_data;

void Renderer2D::StartBatch()
{
    s_data.QuadVertexBufferPtr = s_data.QuadVertexBufferBase;
    s_data.IndicesCount = 0;

    s_data.CircleVertexBufferPtr = s_data.CircleVertexBufferBase;
    s_data.CircleIndicesCount = 0;

    s_data.TextureSlotIndex = 1;
}

void Renderer2D::Init()
{
    RE_PROFILE_FUNCTION();

    float SquareVertices[5 * 4] = {
        -0.5f,-0.5f,0.0f,0.0f,0.0f,
        0.5f,-0.5f,0.0f,1.0f,0.0f,
        0.5f,0.5f,0.0f,1.0f,1.0f,
        -0.5f,0.5f,0.0f,0.0f,1.0f
    };
    unsigned int indices[6] = {
        0,1,2,2,3,0
    };
    
    s_data.vertexArray = VertexArray::Create();
    s_data.QuadVertexBuffer = VertexBuffer::Create(s_data.MaxVertices * sizeof(QuadVertex));
    // auto m_verbuf = VertexBuffer::Create(SquareVertices,sizeof(SquareVertices));
    s_data.QuadVertexBufferBase = new QuadVertex[s_data.MaxVertices];

    BufferLayout layout = {
        {ShadeDataType::Float3 , "a_position"},
        {ShadeDataType::Float4 , "a_color"},
        {ShadeDataType::Float2 , "a_texCoords"},
        {ShadeDataType::Float , "a_texIndex"},
        {ShadeDataType::Float , "a_TilingFactor"},
        {ShadeDataType::Int , "a_EntityID"}
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

    //Circle
    s_data.CircleVertexArray = VertexArray::Create();
    s_data.CircleVertexBuffer = VertexBuffer::Create(s_data.MaxVertices * sizeof(CircleVertex));
    s_data.CircleVertexBufferBase = new CircleVertex[s_data.MaxVertices];

    BufferLayout CircleLayout = {
        {ShadeDataType::Float3 , "a_WorldPosition"},
        {ShadeDataType::Float3 , "a_LocalPosition"},
        {ShadeDataType::Float4 , "a_color"},
        {ShadeDataType::Float , "a_thickness"},
        {ShadeDataType::Float , "a_fade"},
        {ShadeDataType::Int , "a_EntityID"}
        };
    s_data.CircleVertexBuffer->SetLayout(CircleLayout);
    s_data.CircleVertexArray->SetIndexBuffer(QuadIB);
    s_data.CircleVertexArray->AddVertexBuffer(s_data.CircleVertexBuffer);


    //others
    // auto m_indbuf = IndexBuffer::Create(indices,sizeof(indices) / sizeof(uint32_t));

    // s_data.vertexArray->SetIndexBuffer(m_indbuf);

    s_data.m_WhiteTexture = Texture2D::Create(1,1);
    uint32_t whiteColor = 0xffffffff;
    s_data.m_WhiteTexture->setData(&whiteColor,sizeof(whiteColor));

    // s_data.m_shader = Shader::Create("litle","../../../SandBox/assets/shaders/FlatColorVertex.glsl","../../../SandBox/assets/shaders/FlatColorFragment.glsl");
    s_data.m_Texshader = Shader::Create("litle","..\\..\\SandBox\\assets\\shaders\\textureVertex.glsl","..\\..\\SandBox\\assets\\shaders\\textureFragment.glsl");
    // s_data.m_Texshader = Shader::Create("litle","..\\..\\..\\SandBox\\assets\\shaders\\textureVertex.glsl","..\\..\\..\\SandBox\\assets\\shaders\\textureFragment.glsl");
    s_data.m_Texshader->Bind();

    int32_t samplers[s_data.MaxTextureSlots];

    for (int i = 0; i < s_data.MaxTextureSlots; i++)
        samplers[i] = i;

    s_data.m_Texshader->SetUniformIntArray("u_textures",samplers,s_data.MaxTextureSlots);

    s_data.TextureSolts[0] = s_data.m_WhiteTexture;

    s_data.m_CircleShader = Shader::Create("Circle","..\\..\\SandBox\\assets\\shaders\\CircleVertex.glsl","..\\..\\SandBox\\assets\\shaders\\CircleFragment.glsl");
    
    s_data.QuadVertexPosition[0] = {-0.5f,-0.5f,0.0f,1.0f};
    s_data.QuadVertexPosition[1] = {0.5f,-0.5f,0.0f,1.0f};
    s_data.QuadVertexPosition[2] = {0.5f,0.5f,0.0f,1.0f};
    s_data.QuadVertexPosition[3] = {-0.5f,0.5f,0.0f,1.0f};
}

void Renderer2D::Shutdown()
{
    RE_PROFILE_FUNCTION();
}

void Renderer2D::OnWindowResized(uint32_t width,uint32_t height)
{

}

void Renderer2D::BeginScene(const Camera& camera,const glm::mat4& transform)
{
    RE_PROFILE_FUNCTION();

    glm::mat4 viewPro = camera.getProjection() * glm::inverse(transform);
    s_data.m_Texshader->Bind();
    s_data.m_Texshader->SetUniformMat4("u_ViewProjection",viewPro);

    StartBatch();
}

void Renderer2D::BeginScene(const OrthoGraphicsCamera& camera)
{
    RE_PROFILE_FUNCTION();

    s_data.m_Texshader->Bind();
    s_data.m_Texshader->SetUniformMat4("u_ViewProjection",camera.GetViewProjectionMatrix());


    StartBatch();
}

void Renderer2D::BeginScene(const EditorCamera& camera)
{
    RE_PROFILE_FUNCTION();

    glm::mat4 viewPro = camera.GetViewProjection();
    s_data.m_Texshader->Bind();
    s_data.m_Texshader->SetUniformMat4("u_ViewProjection",viewPro);

    StartBatch();
}

void Renderer2D::EndScene()
{
    RE_PROFILE_FUNCTION();
    
    Flush();
}

void Renderer2D::Flush()
{
    if(s_data.IndicesCount)
    {
        uint32_t dataSize = reinterpret_cast<uint8_t*>(s_data.QuadVertexBufferPtr) - reinterpret_cast<uint8_t*>(s_data.QuadVertexBufferBase);
        s_data.QuadVertexBuffer->SetData(s_data.QuadVertexBufferBase, dataSize);

        for (uint32_t i = 0; i < s_data.TextureSlotIndex; i++)
        s_data.TextureSolts[i]->Bind(i);

        RenderCommand::DrawIndex(s_data.vertexArray, s_data.IndicesCount);
        s_data.stats.DrawCall++;
    }

    if(s_data.CircleIndicesCount)
    {
        uint32_t dataSize = reinterpret_cast<uint8_t*>(s_data.CircleVertexBufferPtr) - reinterpret_cast<uint8_t*>(s_data.CircleVertexBufferBase);
        s_data.CircleVertexBuffer->SetData(s_data.CircleVertexBufferBase, dataSize);

        s_data.m_CircleShader->Bind();
        RenderCommand::DrawIndex(s_data.CircleVertexArray, s_data.CircleIndicesCount);
        s_data.stats.DrawCall++;
    }
}

void Renderer2D::FlushAndReset()
{
    EndScene();
    s_data.QuadVertexBufferPtr = s_data.QuadVertexBufferBase;
    s_data.IndicesCount = 0;

    s_data.TextureSlotIndex = 1;
}

void Renderer2D::DrawQuad(const glm::vec2& position,const glm::vec2& size,const glm::vec4& m_SquareColor)
{
    DrawQuad({position.x,position.y,0.0f},size,m_SquareColor);
}

void Renderer2D::DrawQuad(const glm::vec3& position,const glm::vec2& size,const glm::vec4& m_SquareColor)
{
    RE_PROFILE_FUNCTION();
    constexpr int quadVertexCount = 4;
    const glm::vec2 texCoords[] = {{0.0f,0.0f},{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f}};

    if(s_data.IndicesCount >= Renderer2DData::MaxIndices)
        FlushAndReset();

    const float TexIndex = 0.0f;
    const float tile_factor = 1.0f;

    glm::mat4 transforms = glm::translate(glm::mat4(1.0f),position)
    * glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0});

    for (size_t i = 0; i < quadVertexCount; i++)
    {
        s_data.QuadVertexBufferPtr->m_position = transforms * s_data.QuadVertexPosition[i];
        s_data.QuadVertexBufferPtr->m_color = m_SquareColor;
        s_data.QuadVertexBufferPtr->m_texCoord = texCoords[i];
        s_data.QuadVertexBufferPtr->TexIndex = TexIndex;
        s_data.QuadVertexBufferPtr->tiling_factor = tile_factor;
        s_data.QuadVertexBufferPtr++;
    }

    s_data.IndicesCount += 6;

    s_data.stats.QuadCount++;
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
    constexpr int quadVertexCount = 4;
    const glm::vec2 texCoords[] = {{0.0f,0.0f},{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f}};

    if(s_data.IndicesCount >= Renderer2DData::MaxIndices)
        FlushAndReset();

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
    
    glm::mat4 transforms = glm::translate(glm::mat4(1.0f),position)
    * glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0});

    for (size_t i = 0; i < quadVertexCount; i++)
    {
        s_data.QuadVertexBufferPtr->m_position = transforms * s_data.QuadVertexPosition[i];
        s_data.QuadVertexBufferPtr->m_color = Color;
        s_data.QuadVertexBufferPtr->m_texCoord = texCoords[i];
        s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_data.QuadVertexBufferPtr->tiling_factor = tile_factor;
        s_data.QuadVertexBufferPtr++;
    }

    s_data.IndicesCount += 6;
    
    s_data.stats.QuadCount++;
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

void Renderer2D::DrawQuad(const glm::vec2& position,const glm::vec2& size,const Ref<SubTexture2D>& subtexture,float tile_factor,const glm::vec4& tintColor)
{
    DrawQuad({position.x,position.y,0.0},size,subtexture,tile_factor,tintColor);
}

void Renderer2D::DrawQuad(const glm::vec3& position,const glm::vec2& size,const Ref<SubTexture2D>& subtexture,float tile_factor,const glm::vec4& tintColor)
{
    RE_PROFILE_FUNCTION();

    constexpr int quadVertexCount = 4;
    const glm::vec2* texCoords = subtexture->getTexCoords();
    const Ref<Texture2D> texture = subtexture->getTexture();
    if(s_data.IndicesCount >= Renderer2DData::MaxIndices)
        FlushAndReset();

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
    
    glm::mat4 transforms = glm::translate(glm::mat4(1.0f),position)
    * glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0});

    for (size_t i = 0; i < quadVertexCount; i++)
    {
        s_data.QuadVertexBufferPtr->m_position = transforms * s_data.QuadVertexPosition[i];
        s_data.QuadVertexBufferPtr->m_color = Color;
        s_data.QuadVertexBufferPtr->m_texCoord = texCoords[i];
        s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_data.QuadVertexBufferPtr->tiling_factor = tile_factor;
        s_data.QuadVertexBufferPtr++;
    }


    s_data.IndicesCount += 6;
    
    s_data.stats.QuadCount++;
}

void Renderer2D::DrawQuad(const glm::mat4 &transform, glm::vec4 &color,int entityId)
{
    RE_PROFILE_FUNCTION();
    constexpr int quadVertexCount = 4;
    const glm::vec2 texCoords[] = {{0.0f,0.0f},{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f}};

    if(s_data.IndicesCount >= Renderer2DData::MaxIndices)
        FlushAndReset();

    const float TexIndex = 0.0f;
    const float tile_factor = 1.0f;

    for (size_t i = 0; i < quadVertexCount; i++)
    {
        s_data.QuadVertexBufferPtr->m_position = transform * s_data.QuadVertexPosition[i];
        s_data.QuadVertexBufferPtr->m_color = color;
        s_data.QuadVertexBufferPtr->m_texCoord = texCoords[i];
        s_data.QuadVertexBufferPtr->TexIndex = TexIndex;
        s_data.QuadVertexBufferPtr->tiling_factor = tile_factor;
        s_data.QuadVertexBufferPtr->EntityID = entityId;
        s_data.QuadVertexBufferPtr++;
    }

    s_data.IndicesCount += 6;
    s_data.stats.QuadCount++;

}

void Renderer2D::DrawQuad(const glm::mat4& transform,const Ref<Texture>& texture,float tile_factor,const glm::vec4& tintColor,int entityId)
{
    RE_PROFILE_FUNCTION();
    constexpr int quadVertexCount = 4;
    const glm::vec2 texCoords[] = {{0.0f,0.0f},{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f}};

    if(s_data.IndicesCount >= Renderer2DData::MaxIndices)
        FlushAndReset();

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
    
    for (size_t i = 0; i < quadVertexCount; i++)
    {
        s_data.QuadVertexBufferPtr->m_position = transform * s_data.QuadVertexPosition[i];
        s_data.QuadVertexBufferPtr->m_color = Color;
        s_data.QuadVertexBufferPtr->m_texCoord = texCoords[i];
        s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_data.QuadVertexBufferPtr->tiling_factor = tile_factor;
        s_data.QuadVertexBufferPtr->EntityID = entityId;
        s_data.QuadVertexBufferPtr++;
    }

    s_data.IndicesCount += 6;
    
    s_data.stats.QuadCount++;

}

void Renderer2D::DrawRotatedQuad(const glm::vec2& position,const glm::vec2& size,float rotation,const glm::vec4& color)
{
    DrawRotatedQuad({position.x,position.y,0.0f},size,rotation,color);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& position,const glm::vec2& size,float rotation,const glm::vec4& m_SquareColor)
{
    RE_PROFILE_FUNCTION();

    constexpr int quadVertexCount = 4;
    const glm::vec2 texCoords[] = {{0.0f,0.0f},{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f}};
    if(s_data.IndicesCount >= Renderer2DData::MaxIndices)
        FlushAndReset();

    const float TexIndex = 0.0f;
    const float tile_factor = 1.0f;    
    
    glm::mat4 transforms = glm::translate(glm::mat4(1.0f),position)
    * glm::rotate(glm::mat4(1.0f),glm::radians(rotation),{0.0f,0.0f,1.0f})
    * glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0});
    
    for (size_t i = 0; i < quadVertexCount; i++)
    {
        s_data.QuadVertexBufferPtr->m_position = transforms * s_data.QuadVertexPosition[i];
        s_data.QuadVertexBufferPtr->m_color = m_SquareColor;
        s_data.QuadVertexBufferPtr->m_texCoord = texCoords[i];
        s_data.QuadVertexBufferPtr->TexIndex = TexIndex;
        s_data.QuadVertexBufferPtr->tiling_factor = tile_factor;
        s_data.QuadVertexBufferPtr++;
    }

    s_data.IndicesCount += 6;

    s_data.stats.QuadCount++;
#if OLD_PATH
    s_data.m_Texshader->Bind();
    glm::mat4 transforms = glm::translate(glm::mat4(1.0f),position)
    * glm::rotate(glm::mat4(1.0f),rotation,{0.0f,0.0f,1.0f})
    * glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0});
    s_data.m_Texshader->SetUniformMat4("u_Transform",transforms);
    s_data.m_Texshader->SetUniformFloat4("u_color",m_SquareColor);
    s_data.m_Texshader->SetUniformFloat("u_TilingFactor",1.0f);
    
    s_data.m_WhiteTexture->Bind();
    s_data.vertexArray->Bind();
    RenderCommand::DrawIndex(s_data.vertexArray);
#endif
}

void Renderer2D::DrawRotatedQuad(const glm::vec2& position,const glm::vec2& size,float rotation,const Ref<Texture>& texture,float tile_factor,const glm::vec4& tintColor)
{
    DrawRotatedQuad({position.x,position.y,0.0f},size,rotation,texture,tile_factor,tintColor);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& position,const glm::vec2& size,float rotation,const Ref<Texture>& texture,float tile_factor,const glm::vec4& tintColor)
{
    RE_PROFILE_FUNCTION();
    
    constexpr int quadVertexCount = 4;
    const glm::vec2 texCoords[] = {{0.0f,0.0f},{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f}};
    if(s_data.IndicesCount >= Renderer2DData::MaxIndices)
        FlushAndReset();

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

    glm::mat4 transforms = glm::translate(glm::mat4(1.0f),position)
    * glm::rotate(glm::mat4(1.0f),glm::radians(rotation),{0.0f,0.0f,1.0f})
    * glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0});
    
    for (size_t i = 0; i < quadVertexCount; i++)
    {
        s_data.QuadVertexBufferPtr->m_position = transforms * s_data.QuadVertexPosition[i];
        s_data.QuadVertexBufferPtr->m_color = Color;
        s_data.QuadVertexBufferPtr->m_texCoord = texCoords[i];
        s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_data.QuadVertexBufferPtr->tiling_factor = tile_factor;
        s_data.QuadVertexBufferPtr++;
    }

    s_data.IndicesCount += 6;

    s_data.stats.QuadCount++;
#if OLD_PATH
    s_data.m_Texshader->Bind();
    
    s_data.m_Texshader->SetUniformFloat4("u_color",tintColor);
    
    s_data.m_Texshader->SetUniformFloat("u_TilingFactor",tile_factor);
    glm::mat4 transforms = glm::translate(glm::mat4(1.0f),position)
    * glm::rotate(glm::mat4(1.0f),rotation,{0.0f,0.0f,1.0f})
    * glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0f});
    s_data.m_Texshader->SetUniformMat4("u_Transform",transforms);

    texture->Bind();
    s_data.vertexArray->Bind();
    RenderCommand::DrawIndex(s_data.vertexArray);
#endif
}

void Renderer2D::DrawRotatedQuad(const glm::vec2& position,const glm::vec2& size,float rotation,const Ref<SubTexture2D>& subtexture,float tile_factor,const glm::vec4& tintColor)
{
    DrawRotatedQuad({position.x,position.y,0.0f},size,rotation,subtexture,tile_factor,tintColor);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& position,const glm::vec2& size,float rotation,const Ref<SubTexture2D>& subtexture,float tile_factor,const glm::vec4& tintColor)
{
    RE_PROFILE_FUNCTION();
    
    constexpr int quadVertexCount = 4;
    const glm::vec2* texCoords = subtexture->getTexCoords();
    const Ref<Texture2D> texture = subtexture->getTexture();
    if(s_data.IndicesCount >= Renderer2DData::MaxIndices)
        FlushAndReset();

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

    glm::mat4 transforms = glm::translate(glm::mat4(1.0f),position)
    * glm::rotate(glm::mat4(1.0f),glm::radians(rotation),{0.0f,0.0f,1.0f})
    * glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0});
    
    for (size_t i = 0; i < quadVertexCount; i++)
    {
        s_data.QuadVertexBufferPtr->m_position = transforms * s_data.QuadVertexPosition[i];
        s_data.QuadVertexBufferPtr->m_color = Color;
        s_data.QuadVertexBufferPtr->m_texCoord = texCoords[i];
        s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_data.QuadVertexBufferPtr->tiling_factor = tile_factor;
        s_data.QuadVertexBufferPtr++;
    }

    s_data.IndicesCount += 6;

    s_data.stats.QuadCount++;

}

void Renderer2D::DrawCircle(const glm::mat4& transform,glm::vec4& color,float thickness,float fade,int entityId)
{
    RE_PROFILE_FUNCTION();
    
    //TODO: implement for circles
    // if(s_data.IndicesCount >= Renderer2DData::MaxIndices)
    //     FlushAndReset();

    for (size_t i = 0; i < 4; i++)
    {
        s_data.CircleVertexBufferPtr->m_WorldPosition = transform * s_data.QuadVertexPosition[i];
        s_data.CircleVertexBufferPtr->m_LoaclPosition = s_data.QuadVertexPosition[i] * 2.0f;
        s_data.CircleVertexBufferPtr->m_color = color;
        s_data.CircleVertexBufferPtr->m_thickness = thickness;
        s_data.CircleVertexBufferPtr->m_fade = fade;
        s_data.CircleVertexBufferPtr->EntityID = entityId;
        s_data.CircleVertexBufferPtr++;
    }

    s_data.CircleIndicesCount += 6;
    s_data.stats.CirclCount++;
}

void Renderer2D::DrawSprite(const glm::mat4& transform,SpriteRendererComponent& src,int entityId)
{
    if(src.Texture)
        DrawQuad(transform,src.Texture,src.TilingFactor,src.Color,entityId);
    else
        DrawQuad(transform, src.Color, entityId);
}

Renderer2D::Statistic Renderer2D::getStats()
{
    return s_data.stats;
}

void Renderer2D::resetStats()
{
    memset(&s_data.stats,0,sizeof(Renderer2D::Statistic));
}

} // namespace Rengin

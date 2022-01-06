#include "repch.hpp"
#include "Renderer3D.hpp"
#include "Rengine/Renderer/VertexArray.hpp"
#include "Rengine/Renderer/Shader.hpp"
#include "Rengine/Renderer/RenderCommand.hpp"
#include <glm/gtc/matrix_transform.hpp>
// #include "Rengine/Platform/OpenGL/OpenGLShader.hpp"

namespace Rengin
{

struct CubeVertex
{
    glm::vec3 m_position;
    glm::vec4 m_color;
    glm::vec3 m_texCoord;
    float TexIndex;
    float tiling_factor;
};

struct Renderer3DData
{
    static const uint32_t MaxCubes = 10;
    static const uint32_t MaxVertices = MaxCubes * 8;
    static const uint32_t MaxIndices = MaxCubes * 36;
    static const uint32_t MaxTextureSlots = 32;


    Ref<VertexArray> VolumeVertexArray;
    Ref<VertexBuffer> CubeVertexBuffer;

    Ref<VertexArray> vertexArray;
    Ref<Shader> m_Texshader;
    Ref<Shader> m_VolumeShader;
    Ref<Texture3D> m_WhiteTexture;

    uint32_t IndicesCount = 0;
    CubeVertex* CubeVertexBufferBase = nullptr;
    CubeVertex* CubeVertexBufferPtr = nullptr;

    std::array<Ref<Texture>,MaxTextureSlots> TextureSolts;
    uint32_t TextureSlotIndex = 1;

    glm::vec4 CubeVertexPosition[8];

    Renderer3D::Statistic stats;
};

static Renderer3DData s_data;


void Renderer3D::Init()
{
    RE_PROFILE_FUNCTION();
    s_data.vertexArray = VertexArray::Create();

    float CubicVertices[3 * 8] = {
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f
    };
    unsigned int indices[36] = {
        // front
        0, 1, 2,
        0, 2, 3,
        // right
        1, 5, 6,
        1, 6, 2,
        // back
        5, 4, 7,
        5, 7, 6,
        // left
        4, 0, 3,
        4, 3, 7,
        // top
        2, 6, 7,
        2, 7, 3,
        // bottom
        4, 5, 1,
        4, 1, 0,
    };
    auto VolumeVertexBuffer = VertexBuffer::Create(CubicVertices,sizeof(CubicVertices));
    
    BufferLayout layout_v = {
        {ShadeDataType::Float3 , "a_position"}
        };
    VolumeVertexBuffer->SetLayout(layout_v);
    auto m_indbuf = IndexBuffer::Create(indices,sizeof(indices) / sizeof(uint32_t));
    s_data.VolumeVertexArray->AddVertexBuffer(VolumeVertexBuffer);
    s_data.VolumeVertexArray->SetIndexBuffer(m_indbuf);

    s_data.CubeVertexBuffer = VertexBuffer::Create(s_data.MaxVertices * sizeof(CubeVertex));
    s_data.CubeVertexBufferBase = new CubeVertex[s_data.MaxVertices];

    BufferLayout layout = {
        {ShadeDataType::Float3 , "a_position"},
        {ShadeDataType::Float4 , "a_color"},
        {ShadeDataType::Float2 , "a_texCoords"},
        {ShadeDataType::Float , "a_texIndex"},
        {ShadeDataType::Float , "a_TilingFactor"}
        };

    s_data.CubeVertexBuffer->SetLayout(layout);
    
    s_data.vertexArray->AddVertexBuffer(s_data.CubeVertexBuffer);

    uint32_t *CubeIndices = new uint32_t[s_data.MaxIndices];

    auto CubeIB = IndexBuffer::Create(CubeIndices,s_data.MaxIndices);

    uint32_t offset = 0;
    for (uint32_t i = 0; i < s_data.MaxIndices; i+=6)
    {
        CubeIndices[i + 0] = offset + 0;
        CubeIndices[i + 1] = offset + 1;
        CubeIndices[i + 2] = offset + 2;
        
        CubeIndices[i + 3] = offset + 2;
        CubeIndices[i + 4] = offset + 3;
        CubeIndices[i + 5] = offset + 0;

        offset += 4;
    }
    

    s_data.vertexArray->SetIndexBuffer(CubeIB);
    delete[] CubeIndices;



    s_data.m_WhiteTexture = Texture3D::Create(1,1,1);
    uint32_t whiteColor = 0xffffffff;
    s_data.m_WhiteTexture->setData(&whiteColor,sizeof(whiteColor));

    s_data.m_Texshader = Shader::Create("litle","../../../SandBox/assets/shaders/textureVertex.glsl","../../../SandBox/assets/shaders/textureFragment.glsl");
    s_data.m_VolumeShader = Shader::Create("VoxelRender","../../../Rengine-Editor/assets/shaders/VoxelVertex.glsl","../../../Rengine-Editor/assets/shaders/textureFragment.glsl");
    s_data.m_Texshader->Bind();

    int32_t samplers[s_data.MaxTextureSlots];

    for (int i = 0; i < s_data.MaxTextureSlots; i++)
        samplers[i] = i;

    s_data.m_Texshader->SetUniformIntArray("u_textures",samplers,s_data.MaxTextureSlots);

    s_data.TextureSolts[0] = s_data.m_WhiteTexture;

    
    s_data.CubeVertexPosition[0] = {-0.5f,-0.5f,0.0f,1.0f};
    s_data.CubeVertexPosition[1] = {0.5f,-0.5f,0.0f,1.0f};
    s_data.CubeVertexPosition[2] = {0.5f,0.5f,0.0f,1.0f};
    s_data.CubeVertexPosition[3] = {-0.5f,0.5f,0.0f,1.0f};
    s_data.CubeVertexPosition[4] = {-0.5f,-0.5f,0.0f,1.0f};
    s_data.CubeVertexPosition[5] = {0.5f,-0.5f,0.0f,1.0f};
    s_data.CubeVertexPosition[6] = {0.5f,0.5f,0.0f,1.0f};
    s_data.CubeVertexPosition[7] = {-0.5f,0.5f,0.0f,1.0f};

}

void Renderer3D::Shutdown()
{
    RE_PROFILE_FUNCTION();
}

void Renderer3D::OnWindowResized(uint32_t width,uint32_t height)
{

}

void Renderer3D::BeginScene(const Camera& camera,const glm::mat4& transform)
{
    RE_PROFILE_FUNCTION();

    glm::mat4 viewPro = camera.getProjection() * glm::inverse(transform);
    s_data.m_Texshader->Bind();
    s_data.m_Texshader->SetUniformMat4("u_ViewProjection",viewPro);

    s_data.CubeVertexBufferPtr = s_data.CubeVertexBufferBase;
    s_data.IndicesCount = 0;

    s_data.TextureSlotIndex = 1;
}

void Renderer3D::BeginScene(const OrthoGraphicsCamera& camera)
{
    RE_PROFILE_FUNCTION();

    s_data.m_Texshader->Bind();
    s_data.m_Texshader->SetUniformMat4("u_ViewProjection",camera.GetViewProjectionMatrix());

    s_data.CubeVertexBufferPtr = s_data.CubeVertexBufferBase;
    s_data.IndicesCount = 0;

    s_data.TextureSlotIndex = 1;
}

void Renderer3D::EndScene()
{
    RE_PROFILE_FUNCTION();

    uint32_t dataSize = reinterpret_cast<uint8_t*>(s_data.CubeVertexBufferPtr) - reinterpret_cast<uint8_t*>(s_data.CubeVertexBufferBase);
    s_data.CubeVertexBuffer->SetData(s_data.CubeVertexBufferBase,dataSize);
    
    Flush();
}

void Renderer3D::Flush()
{
    for (uint32_t i = 0 ; i < s_data.TextureSlotIndex; i++)
        s_data.TextureSolts[i]->Bind(i);

    RenderCommand::DrawIndex(s_data.vertexArray,s_data.IndicesCount);
    s_data.stats.DrawCall++;
}

void Renderer3D::FlushAndReset()
{
    EndScene();
    s_data.CubeVertexBufferPtr = s_data.CubeVertexBufferBase;
    s_data.IndicesCount = 0;

    s_data.TextureSlotIndex = 1;
}

void Renderer3D::DrawCube(const glm::vec3& position,const glm::vec3& size,const glm::vec4& m_CubicColor)
{
    RE_PROFILE_FUNCTION();
    constexpr int cubeVertexCount = 4;
    const glm::vec3 texCoords[] = {{0.0f,0.0f,0.0f},{1.0f,0.0f,0.0f},{1.0f,1.0f,0.0f},{0.0f,1.0f,0.0f},{0.0f,0.0f,1.0f},{1.0f,0.0f,1.0f},{1.0f,1.0f,1.0f},{0.0f,1.0f,1.0f}};

    if(s_data.IndicesCount >= Renderer3DData::MaxIndices)
        FlushAndReset();

    const float TexIndex = 0.0f;
    const float tile_factor = 1.0f;

    glm::mat4 transforms = glm::translate(glm::mat4(1.0f),position)
    * glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0});

    for (size_t i = 0; i < cubeVertexCount; i++)
    {
        s_data.CubeVertexBufferPtr->m_position = transforms * s_data.CubeVertexPosition[i];
        s_data.CubeVertexBufferPtr->m_color = m_CubicColor;
        s_data.CubeVertexBufferPtr->m_texCoord = texCoords[i];
        s_data.CubeVertexBufferPtr->TexIndex = TexIndex;
        s_data.CubeVertexBufferPtr->tiling_factor = tile_factor;
        s_data.CubeVertexBufferPtr++;
    }

    s_data.IndicesCount += 6;

    s_data.stats.CubeCount++;
}

void Renderer3D::DrawCube(const glm::vec3& position,const glm::vec3& size,const Ref<Texture>& texture,float tile_factor,const glm::vec4& tintColor)
{
    RE_PROFILE_FUNCTION();
    constexpr int cubeVertexCount = 8;
    const glm::vec3 texCoords[] = {{0.0f,0.0f,0.0f},{1.0f,0.0f,0.0f},{1.0f,1.0f,0.0f},{0.0f,1.0f,0.0f},{0.0f,0.0f,1.0f},{1.0f,0.0f,1.0f},{1.0f,1.0f,1.0f},{0.0f,1.0f,1.0f}};

    if(s_data.IndicesCount >= Renderer3DData::MaxIndices)
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

    for (size_t i = 0; i < cubeVertexCount; i++)
    {
        s_data.CubeVertexBufferPtr->m_position = transforms * s_data.CubeVertexPosition[i];
        s_data.CubeVertexBufferPtr->m_color = Color;
        s_data.CubeVertexBufferPtr->m_texCoord = texCoords[i];
        s_data.CubeVertexBufferPtr->TexIndex = textureIndex;
        s_data.CubeVertexBufferPtr->tiling_factor = tile_factor;
        s_data.CubeVertexBufferPtr++;
    }

    s_data.IndicesCount += 36;
    
    s_data.stats.CubeCount++;
}

void Renderer3D::DrawCube(const glm::vec3& position,const glm::vec3& size,const Ref<SubTexture3D>& subtexture,float tile_factor,const glm::vec4& tintColor)
{
    RE_PROFILE_FUNCTION();

    constexpr int cubeVertexCount = 4;
    const glm::vec3* texCoords = subtexture->getTexCoords();
    const Ref<Texture3D> texture = subtexture->getTexture();
    if(s_data.IndicesCount >= Renderer3DData::MaxIndices)
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

    for (size_t i = 0; i < cubeVertexCount; i++)
    {
        s_data.CubeVertexBufferPtr->m_position = transforms * s_data.CubeVertexPosition[i];
        s_data.CubeVertexBufferPtr->m_color = Color;
        s_data.CubeVertexBufferPtr->m_texCoord = texCoords[i];
        s_data.CubeVertexBufferPtr->TexIndex = textureIndex;
        s_data.CubeVertexBufferPtr->tiling_factor = tile_factor;
        s_data.CubeVertexBufferPtr++;
    }


    s_data.IndicesCount += 6;
    
    s_data.stats.CubeCount++;
}


void Renderer3D::DrawRotatedCube(const glm::vec3& position,const glm::vec3& size,float rotation,const glm::vec4& m_CubicColor)
{
    RE_PROFILE_FUNCTION();

    constexpr int cubeVertexCount = 4;
    const glm::vec3 texCoords[] = {{0.0f,0.0f,0.0f},{1.0f,0.0f,0.0f},{1.0f,1.0f,0.0f},{0.0f,1.0f,0.0f},{0.0f,0.0f,1.0f},{1.0f,0.0f,1.0f},{1.0f,1.0f,1.0f},{0.0f,1.0f,1.0f}};
    if(s_data.IndicesCount >= Renderer3DData::MaxIndices)
        FlushAndReset();

    const float TexIndex = 0.0f;
    const float tile_factor = 1.0f;    
    
    glm::mat4 transforms = glm::translate(glm::mat4(1.0f),position)
    * glm::rotate(glm::mat4(1.0f),glm::radians(rotation),{0.0f,0.0f,1.0f})
    * glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0});
    
    for (size_t i = 0; i < cubeVertexCount; i++)
    {
        s_data.CubeVertexBufferPtr->m_position = transforms * s_data.CubeVertexPosition[i];
        s_data.CubeVertexBufferPtr->m_color = m_CubicColor;
        s_data.CubeVertexBufferPtr->m_texCoord = texCoords[i];
        s_data.CubeVertexBufferPtr->TexIndex = TexIndex;
        s_data.CubeVertexBufferPtr->tiling_factor = tile_factor;
        s_data.CubeVertexBufferPtr++;
    }

    s_data.IndicesCount += 6;

    s_data.stats.CubeCount++;
}

void Renderer3D::DrawRotatedCube(const glm::vec3& position,const glm::vec3& size,float rotation,const Ref<Texture>& texture,float tile_factor,const glm::vec4& tintColor)
{
    RE_PROFILE_FUNCTION();
    
    constexpr int cubeVertexCount = 8;
    const glm::vec3 texCoords[] = {{0.0f,0.0f,0.0f},{1.0f,0.0f,0.0f},{1.0f,1.0f,0.0f},{0.0f,1.0f,0.0f},{0.0f,0.0f,1.0f},{1.0f,0.0f,1.0f},{1.0f,1.0f,1.0f},{0.0f,1.0f,1.0f}};
    if(s_data.IndicesCount >= Renderer3DData::MaxIndices)
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
    
    for (size_t i = 0; i < cubeVertexCount; i++)
    {
        s_data.CubeVertexBufferPtr->m_position = transforms * s_data.CubeVertexPosition[i];
        s_data.CubeVertexBufferPtr->m_color = Color;
        s_data.CubeVertexBufferPtr->m_texCoord = texCoords[i];
        s_data.CubeVertexBufferPtr->TexIndex = textureIndex;
        s_data.CubeVertexBufferPtr->tiling_factor = tile_factor;
        s_data.CubeVertexBufferPtr++;
    }

    s_data.IndicesCount += 36;

    s_data.stats.CubeCount++;
}

void Renderer3D::DrawRotatedCube(const glm::vec3& position,const glm::vec3& size,float rotation,const Ref<SubTexture3D>& subtexture,float tile_factor,const glm::vec4& tintColor)
{
    RE_PROFILE_FUNCTION();
    
    constexpr int cubeVertexCount = 4;
    const glm::vec3* texCoords = subtexture->getTexCoords();
    const Ref<Texture3D> texture = subtexture->getTexture();
    if(s_data.IndicesCount >= Renderer3DData::MaxIndices)
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
    
    for (size_t i = 0; i < cubeVertexCount; i++)
    {
        s_data.CubeVertexBufferPtr->m_position = transforms * s_data.CubeVertexPosition[i];
        s_data.CubeVertexBufferPtr->m_color = Color;
        s_data.CubeVertexBufferPtr->m_texCoord = texCoords[i];
        s_data.CubeVertexBufferPtr->TexIndex = textureIndex;
        s_data.CubeVertexBufferPtr->tiling_factor = tile_factor;
        s_data.CubeVertexBufferPtr++;
    }

    s_data.IndicesCount += 6;

    s_data.stats.CubeCount++;

}

void Renderer3D::DrawVolome(const glm::vec3 &position, const glm::vec3 &size, const Ref<Texture> &texture)
{
    RE_PROFILE_FUNCTION();

    s_data.m_VolumeShader->Bind();
    
    glm::mat4 transforms = glm::translate(glm::mat4(1.0f),position)
    * glm::scale(glm::mat4(1.0f),size);
    s_data.m_VolumeShader->SetUniformMat4("u_Transform",transforms);

    texture->Bind();
    s_data.vertexArray->Bind();
    RenderCommand::DrawIndex(s_data.VolumeVertexArray);
    
    s_data.m_WhiteTexture->Bind();
}

void Renderer3D::DrawRotatedVolome(const glm::vec3 &position, const glm::vec3 &size, const glm::vec3 &rotation, const Ref<Texture> &texture)
{
    RE_PROFILE_FUNCTION();

    s_data.m_VolumeShader->Bind();
    
    
    glm::mat4 transforms = glm::translate(glm::mat4(1.0f),position)
    * glm::rotate(glm::mat4(1.0f),rotation.x,{1.0f,0.0f,0.0f}) 
    * glm::rotate(glm::mat4(1.0f),rotation.y,{0.0f,1.0f,0.0f})
    * glm::rotate(glm::mat4(1.0f),rotation.z,{0.0f,0.0f,1.0f})
    * glm::scale(glm::mat4(1.0f),size);
    s_data.m_VolumeShader->SetUniformMat4("u_Transform",transforms);

    texture->Bind();
    s_data.vertexArray->Bind();
    RenderCommand::DrawIndex(s_data.VolumeVertexArray);
    
    s_data.m_WhiteTexture->Bind();
}

Renderer3D::Statistic Renderer3D::getStats()
{
    return s_data.stats;
}

void Renderer3D::resetStats()
{
    memset(&s_data.stats,0,sizeof(Renderer3D::Statistic));
}

} // namespace Rengin

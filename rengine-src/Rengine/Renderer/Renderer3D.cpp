#include "repch.hpp"
#include "Renderer3D.hpp"
#include "Rengine/Renderer/VertexArray.hpp"
#include "Rengine/Renderer/Shader.hpp"
#include "Rengine/Renderer/RenderCommand.hpp"
#include <glm/gtc/matrix_transform.hpp>
// #include "Rengine/Platform/OpenGL/OpenGLShader.hpp"
#include <GLFW/glfw3.h>
namespace Rengin
{

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}
static void GLCheckError(){
    while (GLenum error = glGetError()){
    std::cout << "OpenGL Error(" << error << ")" << std::endl;
    }
}

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

static Renderer3DData s_data_v;

void Renderer3D::Init()
{
    RE_PROFILE_FUNCTION();
    s_data_v.m_VolumeShader = Shader::Create("VoxelRender","../../../Voxer-Solver/assets/shaders/VoxelOutfaceVertex.glsl","../../../Voxer-Solver/assets/shaders/VoxelOutfaceFragment.glsl");
    // s_data_v.m_VolumeShader = Shader::Create("VoxelRender","../../Voxer-Solver/assets/shaders/VoxelVertex.glsl","../../Voxer-Solver/assets/shaders/VoxelFragment.glsl");
    s_data_v.m_VolumeShader->Bind();
    s_data_v.VolumeVertexArray = VertexArray::Create();

    float CubicVertices[3 * 8] = {
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f
    };
    uint32_t indices[36] = {
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
        4, 1, 0
    };
    s_data_v.VolumeVertexArray->Bind();
    auto VolumeVertexBuffer = VertexBuffer::Create(CubicVertices,sizeof(CubicVertices));
    BufferLayout layout_v = {
        {ShadeDataType::Float3 , "a_position"}
        };
    VolumeVertexBuffer->SetLayout(layout_v);
    auto m_indbuf = IndexBuffer::Create(indices,sizeof(indices) / sizeof(uint32_t));
    s_data_v.VolumeVertexArray->SetIndexBuffer(m_indbuf);
    s_data_v.VolumeVertexArray->AddVertexBuffer(VolumeVertexBuffer);

    s_data_v.m_Texshader = Shader::Create("litle","../../../SandBox/assets/shaders/textureVertex.glsl","../../../SandBox/assets/shaders/textureFragment.glsl");
    // s_data_v.m_Texshader = Shader::Create("litle","../../../SandBox/assets/shaders/textureVertex.glsl","../../../SandBox/assets/shaders/textureFragment.glsl");
    s_data_v.m_Texshader->Bind();
    s_data_v.vertexArray = VertexArray::Create();
    s_data_v.CubeVertexBuffer = VertexBuffer::Create(s_data_v.MaxVertices * sizeof(CubeVertex));
    s_data_v.CubeVertexBufferBase = new CubeVertex[s_data_v.MaxVertices];

    BufferLayout layout = {
        {ShadeDataType::Float3 , "a_position"},
        {ShadeDataType::Float4 , "a_color"},
        {ShadeDataType::Float2 , "a_texCoords"},
        {ShadeDataType::Float , "a_texIndex"},
        {ShadeDataType::Float , "a_TilingFactor"}
        };

    s_data_v.CubeVertexBuffer->SetLayout(layout);
    
    s_data_v.vertexArray->AddVertexBuffer(s_data_v.CubeVertexBuffer);

    uint32_t *CubeIndices = new uint32_t[s_data_v.MaxIndices];

    auto CubeIB = IndexBuffer::Create(CubeIndices,s_data_v.MaxIndices);

    uint32_t offset = 0;
    for (uint32_t i = 0; i < s_data_v.MaxIndices; i+=6)
    {
        CubeIndices[i + 0] = offset + 0;
        CubeIndices[i + 1] = offset + 1;
        CubeIndices[i + 2] = offset + 2;
        
        CubeIndices[i + 3] = offset + 2;
        CubeIndices[i + 4] = offset + 3;
        CubeIndices[i + 5] = offset + 0;

        offset += 4;
    }
    

    s_data_v.vertexArray->SetIndexBuffer(CubeIB);
    delete[] CubeIndices;



    s_data_v.m_WhiteTexture = Texture3D::Create(1,1,1,4);
    uint32_t whiteColor = 0xffffffff;
    s_data_v.m_WhiteTexture->setData(&whiteColor,sizeof(whiteColor));


    int32_t samplers[s_data_v.MaxTextureSlots];

    for (int i = 0; i < s_data_v.MaxTextureSlots; i++)
        samplers[i] = i;

    s_data_v.m_Texshader->SetUniformIntArray("u_textures",samplers,s_data_v.MaxTextureSlots);

    s_data_v.TextureSolts[0] = s_data_v.m_WhiteTexture;

    
    s_data_v.CubeVertexPosition[0] = {-0.5f,-0.5f,0.0f,1.0f};
    s_data_v.CubeVertexPosition[1] = {0.5f,-0.5f,0.0f,1.0f};
    s_data_v.CubeVertexPosition[2] = {0.5f,0.5f,0.0f,1.0f};
    s_data_v.CubeVertexPosition[3] = {-0.5f,0.5f,0.0f,1.0f};
    s_data_v.CubeVertexPosition[4] = {-0.5f,-0.5f,0.0f,1.0f};
    s_data_v.CubeVertexPosition[5] = {0.5f,-0.5f,0.0f,1.0f};
    s_data_v.CubeVertexPosition[6] = {0.5f,0.5f,0.0f,1.0f};
    s_data_v.CubeVertexPosition[7] = {-0.5f,0.5f,0.0f,1.0f};

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
    s_data_v.m_Texshader->Bind();
    s_data_v.m_Texshader->SetUniformMat4("u_ViewProjection",viewPro);

    s_data_v.CubeVertexBufferPtr = s_data_v.CubeVertexBufferBase;
    s_data_v.IndicesCount = 0;

    s_data_v.TextureSlotIndex = 1;
}

void Renderer3D::BeginScene(const OrthoGraphicsCamera& camera)
{
    RE_PROFILE_FUNCTION();

    s_data_v.m_Texshader->Bind();
    s_data_v.m_Texshader->SetUniformMat4("u_ViewProjection",camera.GetViewProjectionMatrix());

    s_data_v.CubeVertexBufferPtr = s_data_v.CubeVertexBufferBase;
    s_data_v.IndicesCount = 0;

    s_data_v.TextureSlotIndex = 1;
}

void Renderer3D::BeginScene(const EditorCamera& camera)
{
    RE_PROFILE_FUNCTION();

    glm::mat4 viewPro = camera.GetViewProjection();
    s_data_v.m_Texshader->Bind();
    s_data_v.m_Texshader->SetUniformMat4("u_ViewProjection",viewPro);

    s_data_v.CubeVertexBufferPtr = s_data_v.CubeVertexBufferBase;
    s_data_v.IndicesCount = 0;

    s_data_v.TextureSlotIndex = 1;
}

void Renderer3D::EndScene()
{
    RE_PROFILE_FUNCTION();

    uint32_t dataSize = reinterpret_cast<uint8_t*>(s_data_v.CubeVertexBufferPtr) - reinterpret_cast<uint8_t*>(s_data_v.CubeVertexBufferBase);
    s_data_v.CubeVertexBuffer->SetData(s_data_v.CubeVertexBufferBase,dataSize);
    
    // Flush();
}

void Renderer3D::Flush()
{
    for (uint32_t i = 0 ; i < s_data_v.TextureSlotIndex; i++)
        s_data_v.TextureSolts[i]->Bind(i);

    RenderCommand::DrawIndex(s_data_v.vertexArray,s_data_v.IndicesCount);
    s_data_v.stats.DrawCall++;
}

void Renderer3D::FlushAndReset()
{
    EndScene();
    s_data_v.CubeVertexBufferPtr = s_data_v.CubeVertexBufferBase;
    s_data_v.IndicesCount = 0;

    s_data_v.TextureSlotIndex = 1;
}

void Renderer3D::DrawCube(const glm::vec3& position,const glm::vec3& size,const glm::vec4& m_CubicColor)
{
    RE_PROFILE_FUNCTION();
    constexpr int cubeVertexCount = 4;
    const glm::vec3 texCoords[] = {{0.0f,0.0f,0.0f},{1.0f,0.0f,0.0f},{1.0f,1.0f,0.0f},{0.0f,1.0f,0.0f},{0.0f,0.0f,1.0f},{1.0f,0.0f,1.0f},{1.0f,1.0f,1.0f},{0.0f,1.0f,1.0f}};

    if(s_data_v.IndicesCount >= Renderer3DData::MaxIndices)
        FlushAndReset();

    const float TexIndex = 0.0f;
    const float tile_factor = 1.0f;

    glm::mat4 transforms = glm::translate(glm::mat4(1.0f),position)
    * glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0});

    for (size_t i = 0; i < cubeVertexCount; i++)
    {
        s_data_v.CubeVertexBufferPtr->m_position = transforms * s_data_v.CubeVertexPosition[i];
        s_data_v.CubeVertexBufferPtr->m_color = m_CubicColor;
        s_data_v.CubeVertexBufferPtr->m_texCoord = texCoords[i];
        s_data_v.CubeVertexBufferPtr->TexIndex = TexIndex;
        s_data_v.CubeVertexBufferPtr->tiling_factor = tile_factor;
        s_data_v.CubeVertexBufferPtr++;
    }

    s_data_v.IndicesCount += 6;

    s_data_v.stats.CubeCount++;
}

void Renderer3D::DrawCube(const glm::vec3& position,const glm::vec3& size,const Ref<Texture>& texture,float tile_factor,const glm::vec4& tintColor)
{
    RE_PROFILE_FUNCTION();
    constexpr int cubeVertexCount = 8;
    const glm::vec3 texCoords[] = {{0.0f,0.0f,0.0f},{1.0f,0.0f,0.0f},{1.0f,1.0f,0.0f},{0.0f,1.0f,0.0f},{0.0f,0.0f,1.0f},{1.0f,0.0f,1.0f},{1.0f,1.0f,1.0f},{0.0f,1.0f,1.0f}};

    if(s_data_v.IndicesCount >= Renderer3DData::MaxIndices)
        FlushAndReset();

    constexpr glm::vec4 Color = {1.0f,1.0f,1.0f,1.0f};

    float textureIndex = 0.0f;

    for (uint32_t i = 1 ; i < s_data_v.TextureSolts.size(); i++)
    {
        if(*s_data_v.TextureSolts[i].get() == *texture.get())
        {
            textureIndex = static_cast<float>(i);
            break;
        }
    }

    if(textureIndex == 0.0f)
    {
        textureIndex = static_cast<float>(s_data_v.TextureSlotIndex);
        s_data_v.TextureSolts[s_data_v.TextureSlotIndex] = texture;
        s_data_v.TextureSlotIndex++;
    }
    
    glm::mat4 transforms = glm::translate(glm::mat4(1.0f),position)
    * glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0});

    for (size_t i = 0; i < cubeVertexCount; i++)
    {
        s_data_v.CubeVertexBufferPtr->m_position = transforms * s_data_v.CubeVertexPosition[i];
        s_data_v.CubeVertexBufferPtr->m_color = Color;
        s_data_v.CubeVertexBufferPtr->m_texCoord = texCoords[i];
        s_data_v.CubeVertexBufferPtr->TexIndex = textureIndex;
        s_data_v.CubeVertexBufferPtr->tiling_factor = tile_factor;
        s_data_v.CubeVertexBufferPtr++;
    }

    s_data_v.IndicesCount += 36;
    
    s_data_v.stats.CubeCount++;
}

void Renderer3D::DrawCube(const glm::vec3& position,const glm::vec3& size,const Ref<SubTexture3D>& subtexture,float tile_factor,const glm::vec4& tintColor)
{
    RE_PROFILE_FUNCTION();

    constexpr int cubeVertexCount = 4;
    const glm::vec3* texCoords = subtexture->getTexCoords();
    const Ref<Texture3D> texture = subtexture->getTexture();
    if(s_data_v.IndicesCount >= Renderer3DData::MaxIndices)
        FlushAndReset();

    constexpr glm::vec4 Color = {1.0f,1.0f,1.0f,1.0f};

    float textureIndex = 0.0f;

    for (uint32_t i = 1 ; i < s_data_v.TextureSolts.size(); i++)
    {
        if(*s_data_v.TextureSolts[i].get() == *texture.get())
        {
            textureIndex = static_cast<float>(i);
            break;
        }
    }

    if(textureIndex == 0.0f)
    {
        textureIndex = static_cast<float>(s_data_v.TextureSlotIndex);
        s_data_v.TextureSolts[s_data_v.TextureSlotIndex] = texture;
        s_data_v.TextureSlotIndex++;
    }
    
    glm::mat4 transforms = glm::translate(glm::mat4(1.0f),position)
    * glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0});

    for (size_t i = 0; i < cubeVertexCount; i++)
    {
        s_data_v.CubeVertexBufferPtr->m_position = transforms * s_data_v.CubeVertexPosition[i];
        s_data_v.CubeVertexBufferPtr->m_color = Color;
        s_data_v.CubeVertexBufferPtr->m_texCoord = texCoords[i];
        s_data_v.CubeVertexBufferPtr->TexIndex = textureIndex;
        s_data_v.CubeVertexBufferPtr->tiling_factor = tile_factor;
        s_data_v.CubeVertexBufferPtr++;
    }


    s_data_v.IndicesCount += 6;
    
    s_data_v.stats.CubeCount++;
}


void Renderer3D::DrawRotatedCube(const glm::vec3& position,const glm::vec3& size,float rotation,const glm::vec4& m_CubicColor)
{
    RE_PROFILE_FUNCTION();

    constexpr int cubeVertexCount = 4;
    const glm::vec3 texCoords[] = {{0.0f,0.0f,0.0f},{1.0f,0.0f,0.0f},{1.0f,1.0f,0.0f},{0.0f,1.0f,0.0f},{0.0f,0.0f,1.0f},{1.0f,0.0f,1.0f},{1.0f,1.0f,1.0f},{0.0f,1.0f,1.0f}};
    if(s_data_v.IndicesCount >= Renderer3DData::MaxIndices)
        FlushAndReset();

    const float TexIndex = 0.0f;
    const float tile_factor = 1.0f;    
    
    glm::mat4 transforms = glm::translate(glm::mat4(1.0f),position)
    * glm::rotate(glm::mat4(1.0f),glm::radians(rotation),{0.0f,0.0f,1.0f})
    * glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0});
    
    for (size_t i = 0; i < cubeVertexCount; i++)
    {
        s_data_v.CubeVertexBufferPtr->m_position = transforms * s_data_v.CubeVertexPosition[i];
        s_data_v.CubeVertexBufferPtr->m_color = m_CubicColor;
        s_data_v.CubeVertexBufferPtr->m_texCoord = texCoords[i];
        s_data_v.CubeVertexBufferPtr->TexIndex = TexIndex;
        s_data_v.CubeVertexBufferPtr->tiling_factor = tile_factor;
        s_data_v.CubeVertexBufferPtr++;
    }

    s_data_v.IndicesCount += 6;

    s_data_v.stats.CubeCount++;
}

void Renderer3D::DrawRotatedCube(const glm::vec3& position,const glm::vec3& size,float rotation,const Ref<Texture>& texture,float tile_factor,const glm::vec4& tintColor)
{
    RE_PROFILE_FUNCTION();
    
    constexpr int cubeVertexCount = 8;
    const glm::vec3 texCoords[] = {{0.0f,0.0f,0.0f},{1.0f,0.0f,0.0f},{1.0f,1.0f,0.0f},{0.0f,1.0f,0.0f},{0.0f,0.0f,1.0f},{1.0f,0.0f,1.0f},{1.0f,1.0f,1.0f},{0.0f,1.0f,1.0f}};
    if(s_data_v.IndicesCount >= Renderer3DData::MaxIndices)
        FlushAndReset();

    constexpr glm::vec4 Color = {1.0f,1.0f,1.0f,1.0f};

    float textureIndex = 0.0f;

    for (uint32_t i = 1 ; i < s_data_v.TextureSolts.size(); i++)
    {
        if(*s_data_v.TextureSolts[i].get() == *texture.get())
        {
            textureIndex = static_cast<float>(i);
            break;
        }
    }

    if(textureIndex == 0.0f)
    {
        textureIndex = static_cast<float>(s_data_v.TextureSlotIndex);
        s_data_v.TextureSolts[s_data_v.TextureSlotIndex] = texture;
        s_data_v.TextureSlotIndex++;
    }

    glm::mat4 transforms = glm::translate(glm::mat4(1.0f),position)
    * glm::rotate(glm::mat4(1.0f),glm::radians(rotation),{0.0f,0.0f,1.0f})
    * glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0});
    
    for (size_t i = 0; i < cubeVertexCount; i++)
    {
        s_data_v.CubeVertexBufferPtr->m_position = transforms * s_data_v.CubeVertexPosition[i];
        s_data_v.CubeVertexBufferPtr->m_color = Color;
        s_data_v.CubeVertexBufferPtr->m_texCoord = texCoords[i];
        s_data_v.CubeVertexBufferPtr->TexIndex = textureIndex;
        s_data_v.CubeVertexBufferPtr->tiling_factor = tile_factor;
        s_data_v.CubeVertexBufferPtr++;
    }

    s_data_v.IndicesCount += 36;

    s_data_v.stats.CubeCount++;
}

void Renderer3D::DrawRotatedCube(const glm::vec3& position,const glm::vec3& size,float rotation,const Ref<SubTexture3D>& subtexture,float tile_factor,const glm::vec4& tintColor)
{
    RE_PROFILE_FUNCTION();
    
    constexpr int cubeVertexCount = 4;
    const glm::vec3* texCoords = subtexture->getTexCoords();
    const Ref<Texture3D> texture = subtexture->getTexture();
    if(s_data_v.IndicesCount >= Renderer3DData::MaxIndices)
        FlushAndReset();

    constexpr glm::vec4 Color = {1.0f,1.0f,1.0f,1.0f};

    float textureIndex = 0.0f;

    for (uint32_t i = 1 ; i < s_data_v.TextureSolts.size(); i++)
    {
        if(*s_data_v.TextureSolts[i].get() == *texture.get())
        {
            textureIndex = static_cast<float>(i);
            break;
        }
    }

    if(textureIndex == 0.0f)
    {
        textureIndex = static_cast<float>(s_data_v.TextureSlotIndex);
        s_data_v.TextureSolts[s_data_v.TextureSlotIndex] = texture;
        s_data_v.TextureSlotIndex++;
    }

    glm::mat4 transforms = glm::translate(glm::mat4(1.0f),position)
    * glm::rotate(glm::mat4(1.0f),glm::radians(rotation),{0.0f,0.0f,1.0f})
    * glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0});
    
    for (size_t i = 0; i < cubeVertexCount; i++)
    {
        s_data_v.CubeVertexBufferPtr->m_position = transforms * s_data_v.CubeVertexPosition[i];
        s_data_v.CubeVertexBufferPtr->m_color = Color;
        s_data_v.CubeVertexBufferPtr->m_texCoord = texCoords[i];
        s_data_v.CubeVertexBufferPtr->TexIndex = textureIndex;
        s_data_v.CubeVertexBufferPtr->tiling_factor = tile_factor;
        s_data_v.CubeVertexBufferPtr++;
    }

    s_data_v.IndicesCount += 6;

    s_data_v.stats.CubeCount++;

}

void Renderer3D::DrawVolume(const glm::mat4 &ProjectionMatrix,const glm::mat4 &viewMatrix,const glm::mat4 &transforms,const Ref<Texture> &texture,const glm::vec3& scale,
                            const glm::vec2& viewPortSize,float focalLength,const glm::vec3& rayOrigin,const glm::vec3& lightPosition,float stepLength,float maxvalue,
                            const TransferFunction<float,float>& transfera,const TransferFunction<float,glm::vec3>& transferc,float threshold,int TWidth,int THeight,int TDepth,int renderMode)
{
    RE_PROFILE_FUNCTION();

    s_data_v.m_VolumeShader->Bind();
    float maxsc = 1.0 / std::max(std::max(scale.x,scale.y),scale.z);
    glm::vec3 extent(scale.x * maxsc,scale.y * maxsc,scale.z * maxsc),top = glm::vec3(0.5) * extent,bottom = glm::vec3(0.5) * -extent;

    s_data_v.m_VolumeShader->SetUniformMat4("u_ViewMatrix", viewMatrix);
    s_data_v.m_VolumeShader->SetUniformMat4("u_TransformViewProjection",ProjectionMatrix * viewMatrix * transforms);
    glm::mat4 normalMatrix = glm::transpose(glm::inverse(viewMatrix * transforms));
    float gamma = 2.2f,aspect = viewPortSize.x / viewPortSize.y;

    s_data_v.m_VolumeShader->SetUniformMat4("u_NormalMatrix", normalMatrix);
    s_data_v.m_VolumeShader->SetUniformFloat("u_aspectRatio", aspect);
    s_data_v.m_VolumeShader->SetUniformFloat("u_focalLength", focalLength);
    s_data_v.m_VolumeShader->SetUniformFloat2("u_viewportSize", viewPortSize);
    s_data_v.m_VolumeShader->SetUniformFloat3("u_rayOrigin", rayOrigin);
    s_data_v.m_VolumeShader->SetUniformFloat3("u_top", top);
    s_data_v.m_VolumeShader->SetUniformFloat3("u_bottom", bottom);
    s_data_v.m_VolumeShader->SetUniformFloat3("u_backgroundColor", {0.1f, 0.1f, 0.1f});
    s_data_v.m_VolumeShader->SetUniformFloat3("u_lightPosition", lightPosition);
    s_data_v.m_VolumeShader->SetUniformFloat3("u_materialColor", {1.0f,1.0f,1.0f});
    s_data_v.m_VolumeShader->SetUniformFloat("u_stepLength", stepLength);
    s_data_v.m_VolumeShader->SetUniformFloat("u_gamma", gamma);
    s_data_v.m_VolumeShader->SetUniformFloat("u_threshold", threshold);
    s_data_v.m_VolumeShader->SetUniformInt("u_TWidth", TWidth);
    s_data_v.m_VolumeShader->SetUniformInt("u_THeight", THeight);
    s_data_v.m_VolumeShader->SetUniformInt("u_TDepth", TDepth);
    s_data_v.m_VolumeShader->SetUniformInt("u_RenderMode", renderMode);
    s_data_v.m_VolumeShader->SetUniformInt("u_volume", 1);
    s_data_v.m_VolumeShader->SetUniformInt("u_Force", 2);
    s_data_v.m_VolumeShader->SetUniformInt("u_Constraint", 3);
    s_data_v.m_VolumeShader->SetUniformUint("u_ResultVolume", 0);
    // s_data_v.m_VolumeShader->SetUniformFloat("maxvalue", 4964.0f);
    s_data_v.m_VolumeShader->SetUniformFloat("u_maxvalue", maxvalue);
    s_data_v.m_VolumeShader->SetUniformInt("u_nodeaNum", transfera.Size());

    auto it = transfera.begin();
    char result[25];
    for (int i = 0; it != transfera.end(); it++, i++) {
        const char s1[] = "u_mapa[%d].data";
        const char s2[] = "u_mapa[%d].opacity";
        sprintf(result, s1, i);
        s_data_v.m_VolumeShader->SetUniformFloat(result, it->first);
        sprintf(result, s2, i);
        s_data_v.m_VolumeShader->SetUniformFloat(result, it->second);
    }

    s_data_v.m_VolumeShader->SetUniformInt("u_nodecNum", transferc.Size());
    auto itt = transferc.begin();
    for (int i = 0; itt != transferc.end(); itt++, i++) {
      const char s1[] = "u_mapc[%d].data";
      const char s2[] = "u_mapc[%d].color";
      sprintf(result, s1, i);
      s_data_v.m_VolumeShader->SetUniformFloat(result, itt->first);
      sprintf(result, s2, i);
      s_data_v.m_VolumeShader->SetUniformFloat3(result, itt->second);
    }
    // texture->Bind();
    s_data_v.VolumeVertexArray->Bind();
    RenderCommand::DrawIndex(s_data_v.VolumeVertexArray);
    
    s_data_v.m_WhiteTexture->Bind();
    s_data_v.m_Texshader->Bind();
}

Renderer3D::Statistic Renderer3D::getStats()
{
    return s_data_v.stats;
}

void Renderer3D::resetStats()
{
    memset(&s_data_v.stats,0,sizeof(Renderer3D::Statistic));
}

} // namespace Rengin

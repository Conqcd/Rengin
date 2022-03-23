#include "repch.hpp"
#include "SkyBox.hpp"
#include "Rengine/Renderer/RenderCommand.hpp"

namespace Rengin
{

SkyBox::SkyBox()
{ 
    m_Cube = VertexArray::Create();
    BufferLayout layout_v = {
    {ShadeDataType::Float3 , "a_Position"},
    {ShadeDataType::Float3 , "a_Normal"},
    // {ShadeDataType::Float2 , "a_TexCoords"}
    };
    float vertices[8 * 3] = {
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
    };
    unsigned int indices [12 * 3] = {
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
    auto VertexBuffer = VertexBuffer::Create(vertices,sizeof(vertices));
    VertexBuffer->SetLayout(layout_v);
    auto IndexBuf = IndexBuffer::Create(indices, 12 * 3);
    m_Cube->SetIndexBuffer(IndexBuf);
    m_Cube->AddVertexBuffer(VertexBuffer);
}

void SkyBox::AddCubeMap(const std::string &path)
{
    // SH coeff;
    std::fstream SHfile;
    SHfile.open((path + "/light.txt").c_str());
    RE_CORE_ASSERT(SHfile.is_open(), "Can not open the SH para file!");

    glm::mat3 SHR,SHG,SHB;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            SHfile >> SHR[i][j] >> SHG[i][j] >> SHB[i][j];
        }
    }
    

    PreComSHR.push_back(SHR);
    PreComSHG.push_back(SHG);
    PreComSHB.push_back(SHB);

    SHfile.close();

    //cube Texture
    m_CubeTextures.push_back(TextureCube::Create(path + "negx.jpg",path + "negy.jpg",path + "negz.jpg",path +
    "posx.jpg",path + "posy.jpg",path + "posz.jpg"));
}

void SkyBox::RenderCube(int id,const EditorCamera& camera)
{

    m_Shader->Bind();
    m_CubeTextures[id]->Bind();
    m_Shader->SetUniformInt("u_skybox", 0);
    m_Shader->SetUniformMat4("u_View",glm::mat4(glm::mat3(camera.GetViewMatrix())));
    m_Shader->SetUniformMat4("u_Projection", camera.getProjection());
    // m_Shader->SetUniformMat4("u_MoveWithCamera", camera.getProjection());
    // m_BaseShader->SetUniformMat4("u_Transform",ObjLists[ids[i]].GetTransform());
    RenderCommand::DrawIndex(m_Cube);
}

void SkyBox::ComputePRTSH()
{

}

} // namespace Rengin

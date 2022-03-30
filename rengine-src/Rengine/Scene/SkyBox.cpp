#include "repch.hpp"
#include "SkyBox.hpp"
#include "Rengine/Renderer/RenderCommand.hpp"
#include "Rengine/Utils/PrtCompute.hpp"

namespace Rengin
{

SkyBox::SkyBox()
{ 
    m_Cube = VertexArray::Create();
    BufferLayout layout_v = {
    {ShadeDataType::Float3 , "a_Position"},
    // {ShadeDataType::Float3 , "a_Normal"},
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
    //cube Texture
    m_CubeTextures.push_back(TextureCube::Create(path + "posx.jpg",path + "negx.jpg",path + "posy.jpg",path +
    "negy.jpg",path + "posz.jpg",path + "negz.jpg"));

    // SH coeff;
    std::fstream SHfile;
    SHfile.open((path + "/light.txt").c_str());
    if(!SHfile.is_open())
    {
        RE_CORE_ASSERT(SHfile.is_open(), "Can not open the SH para file!");
        ComputePRTOK = false;
        ComputePRTSH(path);
    }else
    {
        glm::mat3 SHR,SHG,SHB;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                SHfile >> SHR[i][j] >> SHG[i][j] >> SHB[i][j];
            }
        }

        PreComSHR.push_back(SHR);
        PreComSHG.push_back(SHG);
        PreComSHB.push_back(SHB);

        SHfile.close();
        ComputePRTOK = true;
    }
}

void SkyBox::RenderCube(int id,const EditorCamera& camera)
{
    RE_CORE_ASSERT(id < m_CubeTextures.size(),"Can not use this texture");
    m_Shader->Bind();
    m_CubeTextures[id]->Bind();
    m_Shader->SetUniformInt("u_skybox", 0);
    m_Shader->SetUniformMat4("u_View",glm::mat4(glm::mat3(camera.GetViewMatrix())));
    m_Shader->SetUniformMat4("u_Projection", camera.getProjection());
    // m_Shader->SetUniformMat4("u_MoveWithCamera", camera.getProjection());
    // m_BaseShader->SetUniformMat4("u_Transform",ObjLists[ids[i]].GetTransform());
    RenderCommand::DrawIndex(m_Cube);
    
    if(ComputePRTOK && m_PRTShader)
    {
        m_PRTShader->Bind();
        m_PRTShader->SetUniformMat3("u_PrecomputeLr", PreComSHR[id]);
        m_PRTShader->SetUniformMat3("u_PrecomputeLg", PreComSHG[id]);
        m_PRTShader->SetUniformMat3("u_PrecomputeLb", PreComSHB[id]);
    }
}

void SkyBox::ComputePRTSH(const std::string& path)
{
    std::ofstream SHfile;
    SHfile.open((path + "/light.txt").c_str());

    // if(SHfile.is_open())
    //     std::cout <<":ascobc" << std::endl;
    glm::mat3 SHR, SHG, SHB;

    PrecomputeCubemapSH<2>(path, SHR, SHG, SHB);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            SHfile << SHR[i][j] << " " << SHG[i][j] << " " << SHB[i][j] << std::endl;
        }
    }

    PreComSHR.push_back(SHR);
    PreComSHG.push_back(SHG);
    PreComSHB.push_back(SHB);

    SHfile.close();
    ComputePRTOK = true;
}

} // namespace Rengin

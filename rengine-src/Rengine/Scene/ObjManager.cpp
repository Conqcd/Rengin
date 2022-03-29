#include "repch.hpp"
#include "ObjManager.hpp"
#include <cstring>
#include "Rengine/Utils/PrtCompute.hpp"
#include "Rengine/Math/Math.hpp"
#include <Eigen/Core>

namespace Rengin
{
 
ObjManager::ObjManager(const std::string& path,const std::string& material_path,const glm::mat4& transform)
           : m_transform(transform), m_Path(path)
{
    std::string* warn = new std::string(),*err = new std::string();
    auto attrib = new tinyobj::attrib_t();
    auto shapes = new std::vector<tinyobj::shape_t>();
    auto material = new std::vector<tinyobj::material_t>();

    tinyobj::LoadObj(attrib, shapes, material, warn, err, path.c_str(), material_path.c_str());
 
    tinyobj::real_t* vertices = new tinyobj::real_t[attrib->vertices.size() / 3 * 8];
    memset(vertices,0,sizeof(tinyobj::real_t) * attrib->vertices.size() / 3 * 8);
    for (size_t i = 0; i < shapes->size(); i++)
    {
        m_VertexArrays.push_back(VertexArray::Create());
        for (size_t j = 0; j < (*shapes)[i].mesh.indices.size(); j ++) {
            int idv = (*shapes)[i].mesh.indices[j].vertex_index,idn = (*shapes)[i].mesh.indices[j].normal_index;
            vertices[(*shapes)[i].mesh.indices[j].vertex_index * 8 + 3] = attrib->normals[(*shapes)[i].mesh.indices[j].normal_index * 3];
            vertices[(*shapes)[i].mesh.indices[j].vertex_index * 8 + 4] = attrib->normals[(*shapes)[i].mesh.indices[j].normal_index * 3 + 1];
            vertices[(*shapes)[i].mesh.indices[j].vertex_index * 8 + 5] = attrib->normals[(*shapes)[i].mesh.indices[j].normal_index * 3 + 2];
        }
        Material materiall;
        materiall.Ka.r = (*material)[i].ambient[0];
        materiall.Ka.g = (*material)[i].ambient[1];
        materiall.Ka.b = (*material)[i].ambient[2];
        materiall.Ks.r = (*material)[i].specular[0];
        materiall.Ks.g = (*material)[i].specular[1];
        materiall.Ks.b = (*material)[i].specular[2];
        materiall.Kd.r = (*material)[i].diffuse[0];
        materiall.Kd.g = (*material)[i].diffuse[1];
        materiall.Kd.b = (*material)[i].diffuse[2];
        materiall.Ns = (*material)[i].shininess;
        materiall.Ni = (*material)[i].ior;
        m_Materials.emplace_back(std::move(materiall));
        int maId = 0;
        for (; maId < material->size(); maId++)
        {
            if(strcmp((*material)[maId].name.c_str(),(*shapes)[i].name.c_str()) == 0)
                break;
        }
            
        if(maId < material->size() && (*material)[maId].diffuse_texname != "")
            m_Textures.push_back(Texture2D::Create(material_path + "/" + (*material)[maId].diffuse_texname));
        else
        {
            m_Textures.push_back(Texture2D::Create(1,1));
            uint32_t whiteColor = 0xffffffff;
            m_Textures.back()->setData(&whiteColor, sizeof(whiteColor));
        }
    }

    BufferLayout layout_v = {
        {ShadeDataType::Float3 , "a_position"},
        {ShadeDataType::Float3 , "a_normal"},
        {ShadeDataType::Float2 , "a_texCoords"}
        };
    int size = attrib->vertices.size() / 3;
    for (size_t i = 0; i < size; i++)
    {
        vertices[i * 8] = attrib->vertices[i * 3];
        vertices[i * 8 + 1] = attrib->vertices[i * 3 + 1];
        vertices[i * 8 + 2] = attrib->vertices[i * 3 + 2];

        vertices[i * 8 + 6] = attrib->texcoords[i * 2];
        vertices[i * 8 + 7] = attrib->texcoords[i * 2 + 1];
    }
    
    auto VertexBuffer = VertexBuffer::Create(vertices,attrib->vertices.size() / 3 * 8 * sizeof(tinyobj::real_t));
    VertexBuffer->SetLayout(layout_v);

    for (size_t i = 0; i < shapes->size(); i++)
    {
        m_VertexArrays[i]->Bind();
        uint32_t *indices;
        indices = new uint32_t[(*shapes)[i].mesh.indices.size()];
        for (size_t j = 0; j < (*shapes)[i].mesh.indices.size(); j++) {
            indices[j] = (*shapes)[i].mesh.indices[j].vertex_index;
        }
        auto IndexBuf = IndexBuffer::Create(indices, (*shapes)[i].mesh.indices.size());
        m_VertexArrays[i]->SetIndexBuffer(IndexBuf);
        m_VertexArrays[i]->AddVertexBuffer(VertexBuffer);
        delete[] indices;
    }

    RE_CORE_WARN("{0}",(*warn).c_str());
    RE_CORE_ERROR("{0}",(*err).c_str());
    delete[] vertices;
    delete warn;
    delete err;
    delete attrib;
    delete shapes;
    delete material;
}

ObjManager::~ObjManager()
{
}

void ObjManager::BindTexture(int id,int slot) const
{
    m_Textures[id]->Bind(slot % 32);
}

PRTObjManager::PRTObjManager(const std::string& path,const std::string& material_path,const glm::mat4& transform)
{
    isPrt = true;
    std::string* warn = new std::string(),*err = new std::string();
    auto attrib = new tinyobj::attrib_t();
    auto shapes = new std::vector<tinyobj::shape_t>();
    auto material = new std::vector<tinyobj::material_t>();

    tinyobj::LoadObj(attrib, shapes, material, warn, err, path.c_str(), material_path.c_str());
 
    int size = attrib->vertices.size() / 3;
    m_Vertices.resize(size * 3);
    m_Normals.resize(size * 3);
    for (size_t i = 0; i < shapes->size(); i++)
    {
        m_VertexArrays.push_back(VertexArray::Create());
        for (size_t j = 0; j < (*shapes)[i].mesh.indices.size(); j ++) {
            int idv = (*shapes)[i].mesh.indices[j].vertex_index,idn = (*shapes)[i].mesh.indices[j].normal_index;
            m_Normals[(*shapes)[i].mesh.indices[j].vertex_index * 3 + 0] = attrib->normals[(*shapes)[i].mesh.indices[j].normal_index * 3];
            m_Normals[(*shapes)[i].mesh.indices[j].vertex_index * 3 + 1] = attrib->normals[(*shapes)[i].mesh.indices[j].normal_index * 3 + 1];
            m_Normals[(*shapes)[i].mesh.indices[j].vertex_index * 3 + 2] = attrib->normals[(*shapes)[i].mesh.indices[j].normal_index * 3 + 2];
        }
        Material materiall;
        materiall.Ka.r = (*material)[i].ambient[0];
        materiall.Ka.g = (*material)[i].ambient[1];
        materiall.Ka.b = (*material)[i].ambient[2];
        materiall.Ks.r = (*material)[i].specular[0];
        materiall.Ks.g = (*material)[i].specular[1];
        materiall.Ks.b = (*material)[i].specular[2];
        materiall.Kd.r = (*material)[i].diffuse[0];
        materiall.Kd.g = (*material)[i].diffuse[1];
        materiall.Kd.b = (*material)[i].diffuse[2];
        materiall.Ns = (*material)[i].shininess;
        materiall.Ni = (*material)[i].ior;
        m_Materials.emplace_back(std::move(materiall));
        int maId = 0;
        for (; maId < material->size(); maId++)
        {
            if(strcmp((*material)[maId].name.c_str(),(*shapes)[i].name.c_str()) == 0)
                break;
        }
            
        if(maId < material->size() && (*material)[maId].diffuse_texname != "")
            m_Textures.push_back(Texture2D::Create(material_path + "/" + (*material)[maId].diffuse_texname));
        else
        {
            m_Textures.push_back(Texture2D::Create(1,1));
            uint32_t whiteColor = 0xffffffff;
            m_Textures.back()->setData(&whiteColor, sizeof(whiteColor));
        }
    }

    BufferLayout layout_v = {
        {ShadeDataType::Float3 , "a_position"},
        // {ShadeDataType::Float3 , "a_normal"},
        // {ShadeDataType::Float2 , "a_texCoords"},
        // {ShadeDataType::Mat3, "a_PrecomputeLT"}
        };

    for (size_t i = 0; i < size; i++)
    {
        m_Vertices[i * 3] = attrib->vertices[i * 3];
        m_Vertices[i * 3 + 1] = attrib->vertices[i * 3 + 1];
        m_Vertices[i * 3 + 2] = attrib->vertices[i * 3 + 2];

        // m_Vertices[i * 17 + 6] = attrib->texcoords[i * 2];
        // m_Vertices[i * 17 + 7] = attrib->texcoords[i * 2 + 1];
    }
    
    auto VertexBuffer = VertexBuffer::Create(m_Vertices.data(),size * 3 * sizeof(tinyobj::real_t));
    VertexBuffer->SetLayout(layout_v);

    for (size_t i = 0; i < shapes->size(); i++)
    {
        m_VertexArrays[i]->Bind();
        std::vector<uint32_t> indices;
        indices.resize((*shapes)[i].mesh.indices.size());
        for (size_t j = 0; j < (*shapes)[i].mesh.indices.size(); j++) {
            indices[j] = (*shapes)[i].mesh.indices[j].vertex_index;
        }
        auto IndexBuf = IndexBuffer::Create(indices.data(), (*shapes)[i].mesh.indices.size());
        m_VertexArrays[i]->SetIndexBuffer(IndexBuf);
        m_VertexArrays[i]->AddVertexBuffer(VertexBuffer);
        m_IndicesList.push_back(std::move(indices));
    }

    AddPRTVertex(material_path + "/transport.txt");

    RE_CORE_WARN("{0}",(*warn).c_str());
    RE_CORE_ERROR("{0}",(*err).c_str());
    delete warn;
    delete err;
    delete attrib;
    delete shapes;
    delete material;
}

void PRTObjManager::AddPRTVertex(const std::string &prtpath)
{
    BufferLayout layout_v = {
        {ShadeDataType::Mat3, "a_PrecomputeLT"}
    };

    std::fstream tsFile;

    tsFile.open(prtpath);

    if(!tsFile.is_open())
    {

    }else
    {
        auto Transport = new float[m_Vertices.size() * 3];
        int vernum = 0;
        tsFile >> vernum;
        int idx = 0;
        for (int i = 0; i < vernum; i++) {
            tsFile >> Transport[idx++] >> Transport[idx++] >> Transport[idx++] >>
                Transport[idx++] >> Transport[idx++] >> Transport[idx++] >>
                Transport[idx++] >> Transport[idx++] >> Transport[idx++];
        }

        tsFile.close();
        auto VertexBuffer = VertexBuffer::Create(Transport, vernum * 9 * sizeof(float));
        VertexBuffer->SetLayout(layout_v);

        for (size_t i = 0; i < m_VertexArrays.size(); i++) {
            m_VertexArrays[i]->Bind();
            m_VertexArrays[i]->AddVertexBuffer(VertexBuffer);
        }

        delete[] Transport;
        hasTransportSH = true;
    }
}

const int SHCoeffLength = 9;
const int SHOrder = 2;

void PRTObjManager::ComputeTransportSH(PRTType type)
{
    if(!hasTransportSH)
    {
        BufferLayout layout_v = {
            {ShadeDataType::Mat3, "a_PrecomputeLT"}
        };
        std::ofstream fout;
        fout.open((m_Path + "/transport.txt"));
        int VerticesSize = m_Vertices.size() / 3;
        fout << VerticesSize << std::endl;
        auto Transport = new float[VerticesSize * SHCoeffLength];
        Eigen::Matrix4f transform;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                transform(i,j) = m_transform[i][j];
            }
        }
        for (int i = 0; i < VerticesSize; i++)
        {
            // Eigen::Vector3f v(m_Vertices[i * 3],m_Vertices[i * 3 + 1],m_Vertices[i * 3 + 2]);
            // v = transform * Eigen::Vector4f(v,1.0f);
            // Eigen::Vector3f n(m_Normals[i * 3],m_Normals[i * 3 + 1],m_Normals[i * 3 + 2]); 
            glm::vec3 v(m_Vertices[i * 3],m_Vertices[i * 3 + 1],m_Vertices[i * 3 + 2]);
            v = glm::vec3(m_transform * glm::vec4(v,1.0f));
            glm::vec3 n(m_Normals[i * 3],m_Normals[i * 3 + 1],m_Normals[i * 3 + 2]);
            auto shFunc = [&](double phi, double theta) -> double {
                const auto wi = Math::ToVector(phi, theta);
                // const Eigen::Vector3f wi(d.x(), d.y(), d.z());
                if (type == PRTType::Unshadowed)
                    return std::max(0.0f,static_cast<float>(glm::dot(wi,n)));
                    // return std::max(0.0f,static_cast<float>(wi.transpose() * n));
                // else
                //     return std::max(0.0f,static_cast<float>(wi.transpose() * n)) * static_cast<float>(1.0 - scene->rayIntersect({v,wi}));
            };
            auto shCoeff = ProjectFunction(SHOrder, shFunc, m_SampleCount);
            for (int j = 0; j < shCoeff.size(); j++)
            {
                Transport[i * 9 + j] = shCoeff[j];
            }
        }

        auto VertexBuffer = VertexBuffer::Create(Transport, VerticesSize * 9 * sizeof(float));
        VertexBuffer->SetLayout(layout_v);

        for (size_t i = 0; i < m_VertexArrays.size(); i++) {
            m_VertexArrays[i]->Bind();
            m_VertexArrays[i]->AddVertexBuffer(VertexBuffer);
        }
        for (int f = 0; f < VerticesSize; f++)
        {
            for (int j = 0; j < SHCoeffLength; j++)
            {
                fout << Transport[f * SHCoeffLength + j] << " ";
            }
            fout << std::endl;
        }
        delete[] Transport;
        fout.close();
    }
}

} // namespace Rengin

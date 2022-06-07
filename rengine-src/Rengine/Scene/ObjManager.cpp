#include "repch.hpp"
#include <random>
#include "ObjManager.hpp"
#include <cstring>
#include "Rengine/Utils/PrtCompute.hpp"
#include "Rengine/Math/Math.hpp"
#include <Eigen/Core>
#include "Rengine/Renderer/RendererObject.hpp"

namespace Rengin
{
 
const int SHCoeffLength = 9;
const int SHOrder = 2;

ObjManager::ObjManager(const std::string& path,const std::string& material_path,const glm::mat4& transform)
           : m_transform(transform), m_Path(material_path)
{
    std::unordered_map<std::string,int> MatNameId;
    std::string* warn = new std::string(),*err = new std::string();
    auto attrib = new tinyobj::attrib_t();
    auto shapes = new std::vector<tinyobj::shape_t>();
    auto material = new std::vector<tinyobj::material_t>();

    tinyobj::LoadObj(attrib, shapes, material, warn, err, path.c_str(), material_path.c_str());
    
    for (int i = 0; i < material->size(); i++)
    {
        MatNameId[(*material)[i].name] = i;
    }
 
    tinyobj::real_t* vertices = new tinyobj::real_t[attrib->vertices.size() / 3 * 8];
    memset(vertices,0,sizeof(tinyobj::real_t) * attrib->vertices.size() / 3 * 8);
    for (size_t i = 0; i < shapes->size(); i++)
    {
        m_VertexArrays.push_back(VertexArray::Create());
        for (size_t j = 0; j < (*shapes)[i].mesh.indices.size(); j ++) {
            int idv = (*shapes)[i].mesh.indices[j].vertex_index,idn = (*shapes)[i].mesh.indices[j].normal_index;
            vertices[idv * 8 + 3] = attrib->normals[idn * 3];
            vertices[idv * 8 + 4] = attrib->normals[idn * 3 + 1];
            vertices[idv * 8 + 5] = attrib->normals[idn * 3 + 2];
            if((*material)[(*shapes)[i].mesh.material_ids[j / 3]].unknown_parameter.find("Le") != (*material)[(*shapes)[i].mesh.material_ids[j / 3]].unknown_parameter.end())
            {
                glm::vec4 v(attrib->vertices[idv * 3],attrib->vertices[idv * 3 + 1],attrib->vertices[idv * 3 + 2],1.0);
                v = v * transform;
                m_LightsVertex.push_back(v[0]);
                m_LightsVertex.push_back(v[1]);
                m_LightsVertex.push_back(v[2]);
            }
        }
        // int maId = MatNameId[(*shapes)[i].name];
        int maId = (*shapes)[i].mesh.material_ids[0];
        Material materiall;
        materiall.Ka.r = (*material)[maId].ambient[0];
        materiall.Ka.g = (*material)[maId].ambient[1];
        materiall.Ka.b = (*material)[maId].ambient[2];
        materiall.Ks.r = (*material)[maId].specular[0];
        materiall.Ks.g = (*material)[maId].specular[1];
        materiall.Ks.b = (*material)[maId].specular[2];
        materiall.Kd.r = (*material)[maId].diffuse[0];
        materiall.Kd.g = (*material)[maId].diffuse[1];
        materiall.Kd.b = (*material)[maId].diffuse[2];
        materiall.Ns = (*material)[maId].shininess;
        materiall.Ni = (*material)[maId].ior;
        m_Materials.emplace_back(std::move(materiall));
        // for (; maId < material->size(); maId++)
        // {
        //     if(strcmp((*material)[maId].name.c_str(),(*shapes)[i].name.c_str()) == 0)
        //         break;
        // }
            
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
    m_transform = transform;
    m_Path = material_path;
    isPrt = true;
    std::string* warn = new std::string(),*err = new std::string();
    auto attrib = new tinyobj::attrib_t();
    auto shapes = new std::vector<tinyobj::shape_t>();
    auto material = new std::vector<tinyobj::material_t>();

    tinyobj::LoadObj(attrib, shapes, material, warn, err, path.c_str(), material_path.c_str());
 
    int size = attrib->vertices.size() / 3;
    m_Vertices.resize(size * 3);
    m_Normals.resize(size * 3);
    m_Transport.resize(size * SHCoeffLength);
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
        int vernum = 0;
        tsFile >> vernum;
        int idx = 0;
        for (int i = 0; i < vernum; i++) {
            tsFile >> m_Transport[idx++] >> m_Transport[idx++] >> m_Transport[idx++] >>
                m_Transport[idx++] >> m_Transport[idx++] >> m_Transport[idx++] >>
                m_Transport[idx++] >> m_Transport[idx++] >> m_Transport[idx++];
        }

        tsFile.close();
        auto VertexBuffer = VertexBuffer::Create(m_Transport.data(), vernum * 9 * sizeof(float));
        VertexBuffer->SetLayout(layout_v);

        for (size_t i = 0; i < m_VertexArrays.size(); i++) {
            m_VertexArrays[i]->Bind();
            m_VertexArrays[i]->AddVertexBuffer(VertexBuffer);
        }

        hasTransportSH = true;
    }
}


void PRTObjManager::ComputeTransportSH(PRTType type,RendererObject* total)
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
            glm::vec3 v(m_Vertices[i * 3],m_Vertices[i * 3 + 1],m_Vertices[i * 3 + 2]);
            v = glm::vec3(m_transform * glm::vec4(v,1.0f));
            glm::vec3 n(m_Normals[i * 3],m_Normals[i * 3 + 1],m_Normals[i * 3 + 2]);
            n = glm::vec3(m_transform * glm::vec4(n,0.0f));
            n = glm::normalize(n);
            auto shFunc = [&](double phi, double theta) -> double {
                const auto wi = Math::ToVector(phi, theta);
                if (type == PRTType::Unshadowed)
                    return std::max(0.0f,glm::dot(wi,n));
                else
                    return std::max(0.0f,glm::dot(wi,n)) * static_cast<float>(1.0 - total->rayIntersect(v,wi));
            };
            auto shCoeff = ProjectFunction(SHOrder, shFunc, m_SampleCount);
            for (int j = 0; j < shCoeff.size(); j++)
            {
                m_Transport[i * 9 + j] = shCoeff[j];
            }
        }
        if (type == PRTType::Interreflection)
        {
            for (int i = 0; i < m_Bounce; i++)
            {                
                decltype(m_Transport) Transport_tmp(VerticesSize * SHCoeffLength);
                for (int j = 0; j < VerticesSize; j++)
                {
                    glm::vec3 v(m_Vertices[i * 3],m_Vertices[i * 3 + 1],m_Vertices[i * 3 + 2]);
                    v = glm::vec3(m_transform * glm::vec4(v,1.0f));
                    glm::vec3 n(m_Normals[i * 3],m_Normals[i * 3 + 1],m_Normals[i * 3 + 2]);
                    n = glm::vec3(m_transform * glm::vec4(n,0.0f));
                    n = glm::normalize(n);

                    const int sample_side = static_cast<int>(floor(sqrt(m_SampleCount)));

                    std::unique_ptr<std::vector<double>> coeffs(new std::vector<double>());
                    coeffs->assign(GetCoefficientCount(SHOrder), 0.0);

                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_real_distribution<> rng(0.0, 1.0);
                    
                    double weight = 4.0 * M_PI / (sample_side * sample_side);
                    for (int t = 0; t < sample_side; t++) {
                        for (int p = 0; p < sample_side; p++) {
                            double alpha = (t + rng(gen)) / sample_side;
                            double beta = (p + rng(gen)) / sample_side;
                            
                            double phi = 2.0 * M_PI * beta;
                            double theta = acos(2.0 * alpha - 1.0);

                            const auto wi = Math::ToVector(phi, theta);

                            // double value = 0.0;
                            glm::vec3 bary;
                            std::vector<float> value(SHCoeffLength);
                            if(total->rayIntersect(v,wi,bary,value))
                            {
                                for (int k = 0; k < SHCoeffLength; k++)
                                {
                                    Transport_tmp[j * 9 + k] = value[k] * weight * std::max(0.0f,static_cast<float>(glm::dot(wi, n)));
                                }
                            }
                        }
                    }
                }
                int length = VerticesSize * SHCoeffLength;
                for(int j = 0; j < length;j ++)
                    m_Transport[j] += Transport_tmp[j];
            }
        }
        auto VertexBuffer = VertexBuffer::Create(m_Transport.data(), VerticesSize * SHCoeffLength * sizeof(float));
        VertexBuffer->SetLayout(layout_v);

        for (size_t i = 0; i < m_VertexArrays.size(); i++) {
            m_VertexArrays[i]->Bind();
            m_VertexArrays[i]->AddVertexBuffer(VertexBuffer);
        }
        for (int f = 0; f < VerticesSize; f++)
        {
            for (int j = 0; j < SHCoeffLength; j++)
            {
                fout << m_Transport[f * SHCoeffLength + j] << " ";
            }
            fout << std::endl;
        }
        fout.close();
    }
}

bool PRTObjManager::hit(const glm::vec3 &v, const glm::vec3 &wi)
{
    for (int i = 0; i < m_IndicesList.size(); i++)
    {
        for (int j = 0; j < m_IndicesList[i].size(); j += 3)
        {
            int id1 = m_IndicesList[i][j];
            int id2 = m_IndicesList[i][j + 1];
            int id3 = m_IndicesList[i][j + 2];
            glm::vec3 v0(m_Vertices[id1 * 3], m_Vertices[id1 * 3 + 1], m_Vertices[id1 * 3 + 2]);
            glm::vec3 v1(m_Vertices[id2 * 3], m_Vertices[id2 * 3 + 1], m_Vertices[id2 * 3 + 2]);
            glm::vec3 v2(m_Vertices[id3 * 3], m_Vertices[id3 * 3 + 1], m_Vertices[id3 * 3 + 2]);

            glm::vec3 E1 = v1 - v0;
            glm::vec3 E2 = v2 - v0;
            glm::vec3 S = v - v0;
            glm::vec3 S1 = glm::cross(wi, E2);
            glm::vec3 S2 = glm::cross(S, E1);
            float coeff = 1.0 / glm::dot(S1, E1);
            float tt = coeff * glm::dot(S2, E2);
            float b1 = coeff * glm::dot(S1, S);
            float b2 = coeff * glm::dot(S2, wi);
            if (tt >= 0.f && b1 >= 0.f && b2 >= 0.f && (1 - b1 - b2) >= 0.f)
                return true;
        }
    }
    return false;
}

bool PRTObjManager::hit(const glm::vec3 &v, const glm::vec3 &wi, glm::vec3 &bary,float &t,std::vector<float>& value)
{
    bool flag = false;
    for (int i = 0; i < m_IndicesList.size(); i++)
    {
        for (int j = 0; j < m_IndicesList[i].size(); j += 3)
        {
            int id1 = m_IndicesList[i][j];
            int id2 = m_IndicesList[i][j + 1];
            int id3 = m_IndicesList[i][j + 2];
            glm::vec3 v0(m_Vertices[id1 * 3], m_Vertices[id1 * 3 + 1], m_Vertices[id1 * 3 + 2]);
            glm::vec3 v1(m_Vertices[id2 * 3], m_Vertices[id2 * 3 + 1], m_Vertices[id2 * 3 + 2]);
            glm::vec3 v2(m_Vertices[id3 * 3], m_Vertices[id3 * 3 + 1], m_Vertices[id3 * 3 + 2]);

            glm::vec3 E1 = v1 - v0;
            glm::vec3 E2 = v2 - v0;
            glm::vec3 S = v - v0;
            glm::vec3 S1 = glm::cross(wi, E2);
            glm::vec3 S2 = glm::cross(S, E1);
            float coeff = 1.0 / glm::dot(S1, E1);
            float tt = coeff * glm::dot(S2, E2);
            float b1 = coeff * glm::dot(S1, S);
            float b2 = coeff * glm::dot(S2, wi);
            if (tt >= 0.f && tt < t && b1 >= 0.f && b2 >= 0.f && (1 - b1 - b2) >= 0.f)
            {
                t = tt;
                bary[0] = b1;
                bary[1] = b2;
                bary[2] = 1 - b1 - b2;
                int SHLen = value.size();
                for (int k = 0; k < SHLen; k++)
                {
                    value[k] = m_Transport[id1 * SHLen + k] * b1 + m_Transport[id2 * SHLen + k] * b2 + m_Transport[id3 * SHLen + k] * bary[2];
                }
                flag = true;
            }
        }
    }
    return flag;
}

} // namespace Rengin

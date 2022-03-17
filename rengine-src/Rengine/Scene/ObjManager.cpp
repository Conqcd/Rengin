#include "repch.hpp"
#include "ObjManager.hpp"
#include "tiny_obj_loader.hpp"

namespace Rengin
{
 
ObjManager::ObjManager(const std::string& path,const std::string& material_path,const glm::mat4& transform)
           : m_transform(transform)
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
        if((*material)[i].diffuse_texname != "")
            m_Textures.push_back(Texture2D::Create(material_path + "/" + (*material)[i].diffuse_texname));
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
        // if(i < size2)
        // {
        //     vertices[i * 8 + 3] = attrib->normals[i * 3];
        //     vertices[i * 8 + 4] = attrib->normals[i * 3 + 1];
        //     vertices[i * 8 + 5] = attrib->normals[i * 3 + 2];
        // } else {
        //     vertices[i * 8 + 3] =  0;
        //     vertices[i * 8 + 4] =  0;
        //     vertices[i * 8 + 5] =  0;
        // }
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

void ObjManager::BindTexture(int slot)
{
    m_Textures[slot]->Bind(slot % 32);
}

} // namespace Rengin

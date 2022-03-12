#include "repch.hpp"
#include "ObjManager.hpp"
#include "tiny_obj_loader.hpp"

namespace Rengin
{
    
ObjManager::ObjManager(const std::string& path,const std::string& material_path)
{
    std::string texName = material_path + "/MC003_Kozakura_Mari.png";
    m_Textures.push_back(Texture2D::Create(texName));
    std::string* warn = new std::string(),*err = new std::string();
    auto attrib = new tinyobj::attrib_t();
    auto shapes = new std::vector<tinyobj::shape_t>();
    auto material = new std::vector<tinyobj::material_t>();

    tinyobj::LoadObj(attrib, shapes, material, warn, err, path.c_str(), material_path.c_str());
    
    m_VertexArray = VertexArray::Create();
    m_VertexArray->Bind();
    BufferLayout layout_v = {
        {ShadeDataType::Float3 , "a_position"},
        {ShadeDataType::Float3 , "a_normal"},
        {ShadeDataType::Float2 , "a_texCoords"}
        };
    tinyobj::real_t* vertices = new tinyobj::real_t[attrib->vertices.size() / 3 * 8];
    int size = attrib->vertices.size() / 3;
    int size2 = attrib->normals.size() / 3;
    for (size_t i = 0; i < size; i++)
    {
        vertices[i * 8] = attrib->vertices[i * 3];
        vertices[i * 8 + 1] = attrib->vertices[i * 3 + 1];
        vertices[i * 8 + 2] = attrib->vertices[i * 3 + 2];
        if(i < size2)
        {
            vertices[i * 8 + 3] = attrib->normals[i * 3];
            vertices[i * 8 + 4] = attrib->normals[i * 3 + 1];
            vertices[i * 8 + 5] = attrib->normals[i * 3 + 2];
        } else {
            vertices[i * 8 + 3] =  0;
            vertices[i * 8 + 4] =  0;
            vertices[i * 8 + 5] =  0;
        }
        vertices[i * 8 + 6] = attrib->texcoords[i * 2];
        vertices[i * 8 + 7] = attrib->texcoords[i * 2 + 1];
    }
    
    auto VertexBuffer = VertexBuffer::Create(vertices,attrib->vertices.size() / 3 * 8 * sizeof(tinyobj::real_t));
    VertexBuffer->SetLayout(layout_v);
    uint32_t *indices = new uint32_t[(*shapes)[0].mesh.indices.size()];
    for (size_t i = 0; i < (*shapes)[0].mesh.indices.size(); i++)
    {
        indices[i] = (*shapes)[0].mesh.indices[i].vertex_index;
    }
    
    auto IndexBuf = IndexBuffer::Create(indices,(*shapes)[0].mesh.indices.size());
    m_VertexArray->SetIndexBuffer(IndexBuf);
    m_VertexArray->AddVertexBuffer(VertexBuffer);

    RE_CORE_WARN("{0}",(*warn).c_str());
    RE_CORE_ERROR("{0}",(*err).c_str());
    delete[] vertices;
    delete[] indices;
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
    m_Textures[slot]->Bind(slot);
}

} // namespace Rengin

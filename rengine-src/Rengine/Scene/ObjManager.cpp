#include "repch.hpp"
#include "ObjManager.hpp"
#include "tiny_obj_loader.hpp"

namespace Rengin
{
    
ObjManager::ObjManager(const std::string& path,const std::string& material_path)
{
    std::string* warn = new std::string(),*err = new std::string();
    auto attrib = new tinyobj::attrib_t();
    auto shapes = new std::vector<tinyobj::shape_t>();
    auto material = new std::vector<tinyobj::material_t>();

    tinyobj::LoadObj(attrib, shapes, material, warn, err, path.c_str(), material_path.c_str());
    
    m_VertexArray = VertexArray::Create();
    m_VertexArray->Bind();
    auto VertexBuffer = VertexBuffer::Create(attrib->vertices.data(),attrib->vertices.size() * sizeof(int));
    BufferLayout layout_v = {
        {ShadeDataType::Float3 , "a_position"}
        };
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


} // namespace Rengin

#include "repch.hpp"
#include "OpenGLVertexArray.hpp"
#include "Rengine/Renderer/Renderer.hpp"
#include "Rengine/core.hpp"
#include <glad/glad.h>

namespace Rengin
{
static GLenum ShadeDataType2OpenGLType(ShadeDataType type)
{
    switch (type)
    {
        case ShadeDataType::Float: return GL_FLOAT;
        case ShadeDataType::Float2: return GL_FLOAT;
        case ShadeDataType::Float3: return GL_FLOAT;
        case ShadeDataType::Float4: return GL_FLOAT;
        case ShadeDataType::Mat3: return GL_FLOAT;
        case ShadeDataType::Mat4: return GL_FLOAT;
        case ShadeDataType::Int: return GL_INT;
        case ShadeDataType::Int2: return GL_INT;
        case ShadeDataType::Int3: return GL_INT;
        case ShadeDataType::Int4: return GL_INT;
        case ShadeDataType::Bool: return GL_BOOL;
    }
    RE_CORE_ASSERT(false,"Unknown Shader Type");
    return 0;
}

OpenGLVertexArray::OpenGLVertexArray()
{
    glCreateVertexArrays(1,&m_render_id);
}

void OpenGLVertexArray::Bind()const
{
    glBindVertexArray(m_render_id);
}

void OpenGLVertexArray::Unbind()const
{
    glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexbuffer)
{
    glBindVertexArray(m_render_id);
    vertexbuffer->Bind();
    RE_CORE_ASSERT(vertexbuffer->getLayout().getElements().size(),"Vertex Buffer has no layout!");
    uint32_t index = 0;
    const auto& layout = vertexbuffer->getLayout();
    for (const auto& element : layout)
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index,
            element.getComponentCount(),
            ShadeDataType2OpenGLType(element.Type),
            element.m_normalized? GL_TRUE : GL_FALSE,
            layout.getStride(),
            (const void*)element.Offset
        );
        index++;
    }
    
    m_vertexbuffers.push_back(vertexbuffer);
}

void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexbuffer)
{
    glBindVertexArray(m_render_id);
    indexbuffer->Bind();
    m_indexbuffers = indexbuffer;
}

const std::vector<std::shared_ptr<VertexBuffer>>& OpenGLVertexArray::getVertexBuffer()const
{
    return m_vertexbuffers;
}

const std::shared_ptr<IndexBuffer>& OpenGLVertexArray::getIndexBuffer()const
{
    return m_indexbuffers;
}

} // namespace Rengin

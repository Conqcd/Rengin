#include "repch.hpp"
#include "OpenGLBuffer.hpp"
#include "Rengine/Debug/Instrumentor.hpp"
#include <glad/glad.h>

namespace Rengin
{

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//VertexBuffer ///////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

OpenGLVertexBuffer::OpenGLVertexBuffer(float *vertices,uint32_t size)
{
    RE_PROFILE_FUNCTION();
    glCreateBuffers(1,&m_render_id);
    glBindBuffer(GL_ARRAY_BUFFER,m_render_id);
    glBufferData(GL_ARRAY_BUFFER,size,vertices,GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
    RE_PROFILE_FUNCTION();
    glDeleteBuffers(1,&m_render_id);
}

void OpenGLVertexBuffer::Bind()const
{
    RE_PROFILE_FUNCTION();
    glBindBuffer(GL_ARRAY_BUFFER,m_render_id);
}

void OpenGLVertexBuffer::Unbind()const
{
    RE_PROFILE_FUNCTION();
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

void OpenGLVertexBuffer::SetLayout(const BufferLayout& layout)
{
    m_layout = layout;
}

const BufferLayout& OpenGLVertexBuffer::getLayout()const
{
    return m_layout;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//IndexBuffer ////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t *indices,uint32_t count)
    : m_count(count)
{
    RE_PROFILE_FUNCTION();
    glCreateBuffers(1,&m_render_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_render_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,count * sizeof(uint32_t),indices,GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
    RE_PROFILE_FUNCTION();
    glDeleteBuffers(1,&m_render_id);
}

void OpenGLIndexBuffer::Bind()const
{
    RE_PROFILE_FUNCTION();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_render_id);
}

void OpenGLIndexBuffer::Unbind()const
{
    RE_PROFILE_FUNCTION();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}
} // namespace Rengin

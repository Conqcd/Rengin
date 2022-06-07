#include "repch.hpp"
#include "OpenGLBuffer.hpp"
#include "Rengine/Debug/Instrumentor.hpp"
#include <glad/glad.h>

namespace Rengin
{

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//VertexBuffer ///////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
{
    RE_PROFILE_FUNCTION();
 
    glCreateBuffers(1,&m_render_id);
    glBindBuffer(GL_ARRAY_BUFFER,m_render_id);
    glBufferData(GL_ARRAY_BUFFER,size,nullptr,GL_DYNAMIC_DRAW);
}

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

void OpenGLVertexBuffer::SetData(const void* data,uint32_t size)
{
    glBindBuffer(GL_ARRAY_BUFFER,m_render_id);
    glBufferSubData(GL_ARRAY_BUFFER,0,size,data);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//StorageBuffer //////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

OpenGLStorageBuffer::OpenGLStorageBuffer(uint32_t size)
{
    RE_PROFILE_FUNCTION();
 
    glCreateBuffers(1,&m_render_id);
}

OpenGLStorageBuffer::OpenGLStorageBuffer(float *vertices,uint32_t size)
{
    RE_PROFILE_FUNCTION();
    glCreateBuffers(1,&m_render_id);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER,m_render_id);
    glBufferData(GL_SHADER_STORAGE_BUFFER,size,vertices,GL_DYNAMIC_DRAW);
}

OpenGLStorageBuffer::~OpenGLStorageBuffer()
{
    RE_PROFILE_FUNCTION();
    glDeleteBuffers(1,&m_render_id);
}

void OpenGLStorageBuffer::Bind()const
{
    RE_PROFILE_FUNCTION();
    glBindBuffer(GL_SHADER_STORAGE_BUFFER,m_render_id);
}

void OpenGLStorageBuffer::Unbind()const
{
    RE_PROFILE_FUNCTION();
    glBindBuffer(GL_SHADER_STORAGE_BUFFER,0);
}

void OpenGLStorageBuffer::SetData(const void* data,uint32_t size)
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER,m_render_id);
    glBufferData(GL_SHADER_STORAGE_BUFFER,size,data,GL_DYNAMIC_DRAW);
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

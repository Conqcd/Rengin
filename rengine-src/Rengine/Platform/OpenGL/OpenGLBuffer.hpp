#pragma once
#include "Rengine/core.hpp"
#include "Rengine/Renderer/Buffer.hpp"

namespace Rengin
{
class RE_API OpenGLVertexBuffer : public VertexBuffer
{
private:
    uint32_t m_render_id;
public:
    OpenGLVertexBuffer(float *vertices,uint32_t size);
    virtual ~OpenGLVertexBuffer();

    virtual void Bind()const;
    virtual void Unbind()const;
};

class RE_API OpenGLIndexBuffer : public IndexBuffer
{
private:
    uint32_t m_render_id;
    uint32_t m_count;
public:
    OpenGLIndexBuffer(uint32_t *indices,uint32_t count);
    virtual ~OpenGLIndexBuffer();
    virtual void Bind()const;
    virtual void Unbind()const;
    virtual uint32_t GetCount() const {return m_count;}
};

} // namespace Rengin

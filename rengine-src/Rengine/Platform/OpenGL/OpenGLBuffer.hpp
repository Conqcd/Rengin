#pragma once
#include "Rengine/Core/core.hpp"
#include "Rengine/Renderer/Buffer.hpp"

namespace Rengin
{
class RE_API OpenGLVertexBuffer : public VertexBuffer
{
private:
    uint32_t m_render_id;
    BufferLayout m_layout;
public:
    OpenGLVertexBuffer(float *vertices,uint32_t size);
    virtual ~OpenGLVertexBuffer();

    virtual void Bind()const override;
    virtual void Unbind()const override;

    virtual void SetLayout(const BufferLayout& layout) override;
    virtual const BufferLayout& getLayout()const override;
};

class RE_API OpenGLIndexBuffer : public IndexBuffer
{
private:
    uint32_t m_render_id;
    uint32_t m_count;
public:
    OpenGLIndexBuffer(uint32_t *indices,uint32_t count);
    virtual ~OpenGLIndexBuffer();
    virtual void Bind()const override;
    virtual void Unbind()const override;
    virtual uint32_t GetCount() const {return m_count;}
};

} // namespace Rengin

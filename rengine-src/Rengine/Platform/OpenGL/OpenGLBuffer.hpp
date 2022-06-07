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
    OpenGLVertexBuffer(uint32_t size);
    OpenGLVertexBuffer(float *vertices,uint32_t size);
    virtual ~OpenGLVertexBuffer();

    virtual void Bind()const override;
    virtual void Unbind()const override;

    virtual void SetLayout(const BufferLayout& layout) override;
    virtual const BufferLayout& getLayout()const override;

    virtual void SetData(const void* data,uint32_t size) override;
};

class RE_API OpenGLStorageBuffer : public StorageBuffer
{
private:
    uint32_t m_render_id;
public:
    OpenGLStorageBuffer(uint32_t size);
    OpenGLStorageBuffer(float *vertices,uint32_t size);
    virtual ~OpenGLStorageBuffer();

    virtual void Bind()const override;
    virtual void Unbind()const override;

    virtual void SetData(const void* data,uint32_t size) override;
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

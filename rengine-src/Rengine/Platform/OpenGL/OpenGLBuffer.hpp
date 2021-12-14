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
    virtual ~OpenGLVertexBuffer() {}

    virtual void Bind()const override;
    virtual void Unbind()const override;

    static OpenGLVertexBuffer* Create(float *vertices,uint32_t size);
};

class RE_API OpenGLIndexBuffer : public IndexBuffer
{
private:
    uint32_t m_render_id;
public:
    virtual ~OpenGLIndexBuffer() {}
    virtual void Bind()const override;
    virtual void Unbind()const override;
    
    static OpenGLIndexBuffer* Create(uint32_t *indices,uint32_t size);

};

} // namespace Rengin

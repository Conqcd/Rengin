#pragma once
#include "Rengine/Renderer/VertexArray.hpp"

namespace Rengin
{
class OpenGLVertexArray : public VertexArray
{
private:
    std::vector<Ref<VertexBuffer>> m_vertexbuffers;
    Ref<IndexBuffer> m_indexbuffers;
    uint32_t m_render_id;
public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray();
    
    virtual void Bind()const override;
    virtual void Unbind()const override;

    virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexbuffer) override;
    virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexbuffer) override;

    virtual const std::vector<Ref<VertexBuffer>>& getVertexBuffer()const override;
    virtual const Ref<IndexBuffer>& getIndexBuffer()const override;
};


} // namespace Rengin

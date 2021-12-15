#pragma once
#include "Rengine/Renderer/VertexArray.hpp"

namespace Rengin
{
class OpenGLVertexArray : public VertexArray
{
private:
    std::vector<std::shared_ptr<VertexBuffer>> m_vertexbuffers;
    std::shared_ptr<IndexBuffer> m_indexbuffers;
    uint32_t m_render_id;
public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray() {}

    virtual void Bind()const override;
    virtual void Unbind()const override;

    virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexbuffer) override;
    virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexbuffer) override;

    virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffer()const override;
    virtual const std::shared_ptr<IndexBuffer>& getIndexBuffer()const override;
};


} // namespace Rengin

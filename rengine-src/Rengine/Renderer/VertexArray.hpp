#pragma once
#include <memory>
#include "Buffer.hpp"

namespace Rengin
{
class RE_API VertexArray
{
private:
    /* data */
public:
    virtual ~VertexArray() {}

    virtual void Bind()const = 0;
    virtual void Unbind()const = 0;

    virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexbuffer) = 0;
    virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexbuffer) = 0;

    static VertexArray* Create();
    
    virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffer()const = 0;
    virtual const std::shared_ptr<IndexBuffer>& getIndexBuffer()const = 0;
};

} // namespace Rengin

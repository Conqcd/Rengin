#pragma once
#include "Rengine/core.hpp"

namespace Rengin
{
class RE_API VertexBuffer
{
private:
    /* data */
public:
    virtual ~VertexBuffer() {}

    virtual void Bind()const = 0;
    virtual void Unbind()const = 0;

    static VertexBuffer* Create(float *vertices,uint32_t size);
};




class RE_API IndexBuffer
{
private:
    /* data */
public:
    virtual ~IndexBuffer() {}
    virtual void Bind()const = 0;
    virtual void Unbind()const = 0;
    
    static IndexBuffer* Create(uint32_t *indices,uint32_t size);
};
    

    
} // namespace Rengin

#pragma once
#include "Rengine/Core/core.hpp"
#include "glm/glm.hpp"

namespace Rengin
{
struct FrameBufferSpecification
{
    uint32_t Width, Height;
    uint32_t Samples = 1;
    bool SwapChainTargets = false;
};


class RE_API FrameBuffer
{
public:
    virtual const FrameBufferSpecification& getSpecification()const = 0;
    virtual uint32_t getColorAttachment()const = 0;
    static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
    
    virtual void Bind() = 0;

    virtual void Unbind() = 0;
    
    virtual void Resize(uint32_t width,uint32_t height) = 0;
};


} // namespace Rengin

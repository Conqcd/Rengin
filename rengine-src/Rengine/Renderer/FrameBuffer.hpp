#pragma once
#include "Rengine/Core/core.hpp"
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
    static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
};


} // namespace Rengin

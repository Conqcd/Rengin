#pragma once
#include "Rengine/Renderer/FrameBuffer.hpp"


namespace Rengin
{
class RE_API OpenGLFrameBuffer : public FrameBuffer
{
private:
    FrameBufferSpecification m_specification;
    uint32_t m_render_id;
    uint32_t m_ColorAttachment;
    uint32_t m_DepthAttachment
public:
    OpenGLFrameBuffer(const FrameBufferSpecification& spec);
    virtual ~OpenGLFrameBuffer();

    void Invalidate();
};

    
} // namespace Rengin

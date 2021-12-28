#pragma once
#include "Rengine/Renderer/FrameBuffer.hpp"


namespace Rengin
{
class RE_API OpenGLFrameBuffer : public FrameBuffer
{
private:
    FrameBufferSpecification m_specification;
    uint32_t m_render_id = 0;
    uint32_t m_ColorAttachment = 0;
    uint32_t m_DepthAttachment = 0;
public:
    OpenGLFrameBuffer(const FrameBufferSpecification& spec);
    virtual ~OpenGLFrameBuffer();
    
    virtual const FrameBufferSpecification& getSpecification()const override;
    virtual uint32_t getColorAttachment()const override;
    
    void Invalidate();

    virtual void Bind() override;
    virtual void Unbind() override;
    
    virtual void Resize(uint32_t width,uint32_t height) override;
};

    
} // namespace Rengin

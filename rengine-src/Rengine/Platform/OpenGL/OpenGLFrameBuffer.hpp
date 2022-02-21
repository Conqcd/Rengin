#pragma once
#include "Rengine/Renderer/FrameBuffer.hpp"


namespace Rengin
{
class RE_API OpenGLFrameBuffer : public FrameBuffer
{
private:
    FrameBufferSpecification m_specification;
    uint32_t m_render_id = 0;
    std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecs;
    FramebufferTextureSpecification m_DepthAttachmentSpecs = FramebufferTextureFormat::None;
    std::vector<uint32_t> m_ColorAttachments;
    uint32_t m_DepthAttachment;
public:
    OpenGLFrameBuffer(const FrameBufferSpecification& spec);
    virtual ~OpenGLFrameBuffer();
    
    virtual const FrameBufferSpecification& getSpecification()const override {return m_specification;}
    virtual uint32_t getColorAttachment(uint32_t index = 0)const override {RE_CORE_ASSERT((index < m_ColorAttachments.size())) return m_ColorAttachments[index];}
    
    void Invalidate();

    virtual void Bind() override;
    virtual void Unbind() override;
    
    virtual void Resize(uint32_t width,uint32_t height) override;
    virtual int ReadPixel(uint32_t attachmentIndex,int x,int y)override;
};

    
} // namespace Rengin

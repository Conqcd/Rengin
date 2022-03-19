#pragma once
#include "Rengine/Core/core.hpp"
#include "glm/glm.hpp"

namespace Rengin
{

enum class FramebufferTextureFormat
{
    None = 0,
    
    //Color
    RGBA8,
    RGBAI32,
    RGB8,
    RGBI32,
    RED_INTEGER,

    //Depth,Stencil
    DEPTH24_STENCIL8,

    //Default
    Depth = DEPTH24_STENCIL8
};

struct FramebufferTextureSpecification
{
    FramebufferTextureSpecification() = default;
    FramebufferTextureSpecification(FramebufferTextureFormat format)
        : TextureFormat(format){}
    FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
};

struct FramebufferAttachmentSpecification
{
    FramebufferAttachmentSpecification() = default;
    FramebufferAttachmentSpecification(const std::initializer_list<FramebufferTextureSpecification>& attachments)
                : Attachments(attachments)  {}
    std::vector<FramebufferTextureSpecification> Attachments;
};


struct FrameBufferSpecification
{
    uint32_t Width, Height;
    FramebufferAttachmentSpecification Attachments;

    uint32_t Samples = 1;
    bool SwapChainTargets = false;
};


class RE_API FrameBuffer
{
public:
    virtual ~FrameBuffer() = default;
    virtual const FrameBufferSpecification& getSpecification()const = 0;
    virtual uint32_t getColorAttachment(uint32_t index = 0)const = 0;
    static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);

    virtual void Resize(uint32_t width,uint32_t height) = 0;
    
    virtual void ReadPixel(uint32_t attachmentIndex,int x,int y,void* pixel) = 0;
    virtual void ReadRangePixel(uint32_t attachmentIndex,int x,int y,int w,int h,void* pixels) = 0;

    virtual void ClearAttachment(uint32_t attachmentIndex,void* value) = 0;

    virtual void Bind() = 0;
    virtual void BindTexture(int id = 0,int slot = 0) = 0;

    virtual void Unbind() = 0;
};


} // namespace Rengin

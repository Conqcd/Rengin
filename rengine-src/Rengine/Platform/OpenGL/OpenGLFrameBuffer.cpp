#include "repch.hpp"
#include "OpenGLFrameBuffer.hpp"
#include "glad/glad.h"

namespace Rengin
{

static const uint32_t s_MaxFramebufferSize = 8192;

namespace Utils{

static GLenum TextureTarget(bool multisample)
{
    return multisample? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
}

static void CreateTexture(bool multisample,uint32_t* outID,uint32_t count)
{
    glCreateTextures(TextureTarget(multisample),count,outID);
}

static void BindTexture(bool multisample,uint32_t id)
{
    glBindTexture(TextureTarget(multisample),id);
}

static void AttachDepthTexture(uint32_t id,int samples, GLenum format,GLenum attachmentType,uint32_t width,uint32_t height)
{
    bool multisampled = samples > 1;
    if(multisampled)
    {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,samples,format,width,height,GL_FALSE);
    }else
    {
        glTexStorage2D(GL_TEXTURE_2D,1,format,width,height);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    }
    glFramebufferTexture2D(GL_FRAMEBUFFER,attachmentType,TextureTarget(multisampled),id,0);
}

static bool IsDepthFormat(FramebufferTextureFormat format)
{
    switch (format)
    {
    case FramebufferTextureFormat::DEPTH24_STENCIL8 :
        return true;
    }
    return false;
}

static GLenum FBTextureFormat2GL(FramebufferTextureFormat format)
{
    switch (format)
    {
    case FramebufferTextureFormat::RGBA8 :
        return GL_RGBA8;
    case FramebufferTextureFormat::RGB8 :
        return GL_RGB8;
    case FramebufferTextureFormat::RGBI32 :
        return GL_RGB32I;
    case FramebufferTextureFormat::RGBAI32 :
        return GL_RGBA32I;
    case FramebufferTextureFormat::RGBF32 :
        return GL_RGB32F;
    case FramebufferTextureFormat::RGBAF32 :
        return GL_RGBA32F;
    case FramebufferTextureFormat::RED_INTEGER :
        return GL_RED_INTEGER;
    case FramebufferTextureFormat::RF32 :
    case FramebufferTextureFormat::RF32_4_NEAREST :
        return GL_R32F;
    }
    RE_CORE_ASSERT(false);
    return 0;
}

static GLenum FBTextureFormat2GLFormat(FramebufferTextureFormat format)
{
    switch (format)
    {
    case FramebufferTextureFormat::RGBA8 :
        return GL_RGBA;
    case FramebufferTextureFormat::RGB8 :
        return GL_RGB;
    case FramebufferTextureFormat::RGBI32 :
        return GL_RGB_INTEGER;
    case FramebufferTextureFormat::RGBAI32 :
        return GL_RGBA_INTEGER;
    case FramebufferTextureFormat::RGBF32 :
        return GL_RGB;
    case FramebufferTextureFormat::RGBAF32 :
        return GL_RGBA;
    case FramebufferTextureFormat::RED_INTEGER :
        return GL_RED_INTEGER;
    case FramebufferTextureFormat::RF32 :
    case FramebufferTextureFormat::RF32_4_NEAREST :
        return GL_RED;
    }
    RE_CORE_ASSERT(false);
    return 0;
}

static GLenum FBTextureFormat2GLType(FramebufferTextureFormat format)
{
    switch (format)
    {
    case FramebufferTextureFormat::RGBA8 :
    case FramebufferTextureFormat::RGB8 :
        return GL_UNSIGNED_BYTE;
    case FramebufferTextureFormat::RGBAF32 :
    case FramebufferTextureFormat::RGBF32 :
    case FramebufferTextureFormat::RF32 :
    case FramebufferTextureFormat::RF32_4_NEAREST :
        return GL_FLOAT;
    case FramebufferTextureFormat::RGBI32 :
    case FramebufferTextureFormat::RGBAI32 :
    case FramebufferTextureFormat::RED_INTEGER :
        return GL_INT;
    }
    RE_CORE_ASSERT(false);
    return 0;
}

static void AttachColorTexture(uint32_t id,int samples,GLenum internalFormat, GLenum format,GLenum type,uint32_t width,uint32_t height,int index,int Interpolation = GL_LINEAR,int Level = 0)
{
    bool multisampled = samples > 1;
    if(multisampled)
    {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,samples,internalFormat,width,height,GL_FALSE);
    }else
    {
        glTexImage2D(GL_TEXTURE_2D,0,internalFormat,width,height,0,format,type,nullptr);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,Interpolation);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,Interpolation);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        if(Level > 0)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL,Level);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0 + index,TextureTarget(multisampled),id,0);
}
}

static void GLCheckError() {
    while (GLenum error = glGetError()) {
        std::cout << "OpenGL Error(" << error << ")" << std::endl;
    }
}


OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
    : m_specification(spec)
{
    for (auto format : m_specification.Attachments.Attachments)
    {
        if(!Utils::IsDepthFormat(format.TextureFormat))
            m_ColorAttachmentSpecs.emplace_back(format);
        else
            m_DepthAttachmentSpecs = format;
    }
    
    Invalidate();
    // GLint maxAttach = 0;
    // glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxAttach);
}

OpenGLFrameBuffer::~OpenGLFrameBuffer()
{
    glDeleteFramebuffers(1,&m_render_id);
    glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
    glDeleteTextures(1, &m_DepthAttachment);
}

void OpenGLFrameBuffer::Invalidate()
{
    if(m_render_id)
    {
        glDeleteFramebuffers(1,&m_render_id);
        glDeleteTextures(m_ColorAttachments.size(),m_ColorAttachments.data());
        glDeleteTextures(1,&m_DepthAttachment);

        m_ColorAttachments.clear();
        m_DepthAttachment = 0;
    }

    glCreateFramebuffers(1,&m_render_id);
    glBindFramebuffer(GL_FRAMEBUFFER,m_render_id);

    //Attachments
    bool multiSamples = m_specification.Samples > 1;
    if(m_ColorAttachmentSpecs.size())
    {
        m_ColorAttachments.resize(m_ColorAttachmentSpecs.size());
        Utils::CreateTexture(multiSamples,m_ColorAttachments.data(),m_ColorAttachments.size());
        for (size_t i = 0; i < m_ColorAttachments.size(); i++)
        {
            Utils::BindTexture(multiSamples,m_ColorAttachments[i]);
            switch (m_ColorAttachmentSpecs[i].TextureFormat)
            {
            case FramebufferTextureFormat::RGBA8:
                Utils::AttachColorTexture(m_ColorAttachments[i],m_specification.Samples,GL_RGBA8,GL_RGBA,GL_UNSIGNED_BYTE,m_specification.Width,m_specification.Height,i);
                break;
            case FramebufferTextureFormat::RGB8:
                Utils::AttachColorTexture(m_ColorAttachments[i],m_specification.Samples,GL_RGB8,GL_RGB,GL_UNSIGNED_BYTE,m_specification.Width,m_specification.Height,i);
                break;
            case FramebufferTextureFormat::RGBI32:
                Utils::AttachColorTexture(m_ColorAttachments[i],m_specification.Samples,GL_RGB32I,GL_RGB_INTEGER,GL_INT,m_specification.Width,m_specification.Height,i);
                break;
            case FramebufferTextureFormat::RGBAI32:
                Utils::AttachColorTexture(m_ColorAttachments[i],m_specification.Samples,GL_RGBA32I,GL_RGBA_INTEGER,GL_INT,m_specification.Width,m_specification.Height,i);
                break;
            case FramebufferTextureFormat::RGBF32:
                Utils::AttachColorTexture(m_ColorAttachments[i],m_specification.Samples,GL_RGB32F,GL_RGB,GL_FLOAT,m_specification.Width,m_specification.Height,i);
                break;
            case FramebufferTextureFormat::RGBAF32:
                Utils::AttachColorTexture(m_ColorAttachments[i],m_specification.Samples,GL_RGBA32F,GL_RGBA,GL_FLOAT,m_specification.Width,m_specification.Height,i);
                break;
            case FramebufferTextureFormat::RED_INTEGER:
                Utils::AttachColorTexture(m_ColorAttachments[i], m_specification.Samples, GL_R32I,GL_RED_INTEGER,GL_INT, m_specification.Width, m_specification.Height, i);
                break;
            case FramebufferTextureFormat::RF32:
                Utils::AttachColorTexture(m_ColorAttachments[i],m_specification.Samples,GL_R32F,GL_RED,GL_FLOAT,m_specification.Width,m_specification.Height,i);
                break;
            case FramebufferTextureFormat::RF32_4_NEAREST:
                Utils::AttachColorTexture(m_ColorAttachments[i],m_specification.Samples,GL_R32F,GL_RED,GL_FLOAT,m_specification.Width,m_specification.Height,i,GL_NEAREST,4);
                break;
            default:
                break;
            }
        }
    }
    if(m_DepthAttachmentSpecs.TextureFormat != FramebufferTextureFormat::None)
    {
        Utils::CreateTexture(multiSamples,&m_DepthAttachment,1);
        Utils::BindTexture(multiSamples,m_DepthAttachment);
        switch (m_DepthAttachmentSpecs.TextureFormat)
        {
        case FramebufferTextureFormat::DEPTH24_STENCIL8 :
            Utils::AttachDepthTexture(m_DepthAttachment,m_specification.Samples,GL_DEPTH24_STENCIL8,GL_DEPTH_STENCIL_ATTACHMENT,m_specification.Width,m_specification.Height);
            break;
        default:
            break;
        }
    }
    if(m_ColorAttachments.size() > 1)
    {
        RE_CORE_ASSERT((m_ColorAttachments.size() < 8));
        GLenum buffers[8] = {GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3,
        GL_COLOR_ATTACHMENT4,GL_COLOR_ATTACHMENT5,GL_COLOR_ATTACHMENT6,GL_COLOR_ATTACHMENT7};
        glDrawBuffers(m_ColorAttachments.size(),buffers);
    }else if(m_ColorAttachments.empty())
    {
        glDrawBuffer(GL_NONE);
    }

    RE_CORE_ASSERT((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE),"FrameBuffer is incomplete!");

    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void OpenGLFrameBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER,m_render_id);
    glViewport(0, 0, m_specification.Width, m_specification.Height);
}

void OpenGLFrameBuffer::BindTexture(int id, int slot)
{
    RE_PROFILE_FUNCTION();
    glBindTextureUnit(slot,m_ColorAttachments[id]);
}

void OpenGLFrameBuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void OpenGLFrameBuffer::Resize(uint32_t width,uint32_t height)
{
    if(width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
    {
        RE_CORE_WARN("Attempted to resize framebuffer to {0} , {1}",width,height);
        return;
    }
    m_specification.Width = width;
    m_specification.Height = height;
    Invalidate();
}

void OpenGLFrameBuffer::ReadPixel(uint32_t attachmentIndex,int x,int y,void* pixel)
{
    RE_CORE_ASSERT((attachmentIndex < m_ColorAttachments.size()));

    auto& spec = m_ColorAttachmentSpecs[attachmentIndex];
    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
    glReadPixels(x,y,1,1,Utils::FBTextureFormat2GLFormat(spec.TextureFormat),Utils::FBTextureFormat2GLType(spec.TextureFormat),pixel);
    // GLCheckError();
}

void OpenGLFrameBuffer::ReadRangePixel(uint32_t attachmentIndex,int x,int y,int w,int h,void* pixels)
{
    RE_CORE_ASSERT((attachmentIndex < m_ColorAttachments.size()));

    auto& spec = m_ColorAttachmentSpecs[attachmentIndex];
    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
    glReadPixels(x,y,w,h,Utils::FBTextureFormat2GLFormat(spec.TextureFormat),Utils::FBTextureFormat2GLType(spec.TextureFormat),pixels);
    GLCheckError();
}

void OpenGLFrameBuffer::ClearAttachment(uint32_t attachmentIndex,void* value)
{
    RE_CORE_ASSERT((attachmentIndex < m_ColorAttachments.size()));

    auto& spec = m_ColorAttachmentSpecs[attachmentIndex];

    glClearTexImage(m_ColorAttachments[attachmentIndex],0,Utils::FBTextureFormat2GLFormat(spec.TextureFormat),Utils::FBTextureFormat2GLType(spec.TextureFormat),value);
}

} // namespace Rengin

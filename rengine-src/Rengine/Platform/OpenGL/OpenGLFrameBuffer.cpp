#include "repch.hpp"
#include "OpenGLFrameBuffer.hpp"
#include "glad/glad.h"

namespace Rengin
{

OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
    : m_specification(spec)
{
    Invalidate();
}

OpenGLFrameBuffer::~OpenGLFrameBuffer()
{
    glDeleteFramebuffers(1,&m_render_id);
}

void OpenGLFrameBuffer::Invalidate()
{
    glCreateFramebuffers(1,&m_render_id);
    glBindFramebuffers(GL_FRAMEBUFFER,m_render_id);

    glCreateTextures(GL_TEXTURE_2D,1,&m_ColorAttachment);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,m_specification.Width,m_specification.Height,0,GL_RGBA,GL_UNSIGNED_BYTE,nullptr);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,m_ColorAttachment,0);

    glCreateTextures(GL_TEXTURE_2D,1,&m_DepthAttachment);
    glBindTexture(GL_TEXTURE_2D,m_DepthAttachment);
    glTexStorage2D(GL_TEXTURE_2D,0,GL_RGBA8,m_specification.Width,m_specification.Height,0,GL_RGBA,GL_UNSIGNED_BYTE,nullptr);

    glBindFramebuffers(GL_FRAMEBUFFER,0);
}

} // namespace Rengin

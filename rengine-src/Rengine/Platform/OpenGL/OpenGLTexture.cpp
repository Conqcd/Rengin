#include "repch.hpp"
#include "OpenGLTexture.hpp"
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Rengin
{
OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
    : m_path(path)
{
    int width,height,channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc *data =stbi_load(path.c_str(),&width,&height,&channels,0);
    RE_CORE_ASSERT(data,"fail to load image!");
    m_width = static_cast<uint32_t>(width);
    m_height = static_cast<uint32_t>(height);

    GLenum interFormat = 0 , dataFormat = 0;

    if (channels == 4)
    {
        interFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
    }else if(channels == 3)
    {
        interFormat = GL_RGB8;
        dataFormat = GL_RGB;
    }

    RE_CORE_ASSERT(interFormat & dataFormat,"Format not Support!");
    
    glCreateTextures(GL_TEXTURE_2D,1,&m_render_id);

    glTextureStorage2D(m_render_id,1,interFormat,m_width,m_height);

    glTextureParameteri(m_render_id,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTextureParameteri(m_render_id,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    glTextureParameteri(m_render_id,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTextureParameteri(m_render_id,GL_TEXTURE_WRAP_T,GL_REPEAT);


    glTextureSubImage2D(m_render_id,0,0,0,m_width,m_height,dataFormat,GL_UNSIGNED_BYTE,data);

    stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
    glDeleteTextures(1,&m_render_id);
}

void OpenGLTexture2D::Bind(uint32_t slot) const
{
    glBindTextureUnit(slot,m_render_id);
}

void OpenGLTexture2D::Unbind() const
{
    glBindTextureUnit(0,0);
}
} // namespace Rengin

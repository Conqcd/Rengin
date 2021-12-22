#include "repch.hpp"
#include "OpenGLTexture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Rengin
{

OpenGLTexture2D::OpenGLTexture2D(uint32_t width,uint32_t height)
        :m_width(width), m_height(height)
{

    GLenum interFormat = GL_RGBA8 , dataFormat = GL_RGBA;

    m_interFormat = interFormat;
    m_dataFormat = dataFormat;

    glCreateTextures(GL_TEXTURE_2D,1,&m_render_id);

    glTextureStorage2D(m_render_id,1,interFormat,m_width,m_height);

    glTextureParameteri(m_render_id,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTextureParameteri(m_render_id,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    glTextureParameteri(m_render_id,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTextureParameteri(m_render_id,GL_TEXTURE_WRAP_T,GL_REPEAT);

}

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

    m_interFormat = interFormat;
    m_dataFormat = dataFormat;

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

void OpenGLTexture2D::setData(void* data,uint32_t size)
{
    uint32_t bpp = (m_dataFormat == GL_RGBA)? 4 : ((m_dataFormat == GL_RGB)? 3: 0); 
    RE_CORE_ASSERT((size == m_height * m_width * bpp),"Can not match the size!");
    glTextureSubImage2D(m_render_id,0,0,0,m_width,m_height,m_dataFormat,GL_UNSIGNED_BYTE,data);
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

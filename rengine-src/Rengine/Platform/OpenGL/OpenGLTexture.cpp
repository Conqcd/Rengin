#include "repch.hpp"
#include "OpenGLTexture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Rengin
{

OpenGLTexture2D::OpenGLTexture2D(uint32_t width,uint32_t height)
        :m_width(width), m_height(height)
{
    RE_PROFILE_FUNCTION();

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
    RE_PROFILE_FUNCTION();
    int width,height,channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc *data = nullptr;
    {
        RE_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
        data = stbi_load(path.c_str(),&width,&height,&channels,0);
    }
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
    RE_PROFILE_FUNCTION();
    glDeleteTextures(1,&m_render_id);
}

void OpenGLTexture2D::setData(void* data,uint32_t size)
{
    RE_PROFILE_FUNCTION();
    uint32_t bpp = (m_dataFormat == GL_RGBA)? 4 : ((m_dataFormat == GL_RGB)? 3: 0); 
    RE_CORE_ASSERT((size == m_height * m_width * bpp),"Can not match the size!");
    glTextureSubImage2D(m_render_id,0,0,0,m_width,m_height,m_dataFormat,GL_UNSIGNED_BYTE,data);
}

void OpenGLTexture2D::Bind(uint32_t slot) const
{
    RE_PROFILE_FUNCTION();
    glBindTextureUnit(slot,m_render_id);
}

void OpenGLTexture2D::Unbind() const
{
    RE_PROFILE_FUNCTION();
    glBindTextureUnit(0,0);
}

///////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////3D///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

OpenGLTexture3D::OpenGLTexture3D(uint32_t width,uint32_t height,uint32_t depth)
        :m_width(width), m_height(height), m_depth(depth)
{
    RE_PROFILE_FUNCTION();

    GLenum interFormat = GL_RGBA8 , dataFormat = GL_RGBA;

    m_interFormat = interFormat;
    m_dataFormat = dataFormat;

    glCreateTextures(GL_TEXTURE_3D,1,&m_render_id);

    glTextureStorage3D(m_render_id,1,interFormat,m_width,m_height,m_depth);

    glTextureParameteri(m_render_id,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTextureParameteri(m_render_id,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    glTextureParameteri(m_render_id,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTextureParameteri(m_render_id,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTextureParameteri(m_render_id,GL_TEXTURE_WRAP_R,GL_REPEAT);

}

OpenGLTexture3D::OpenGLTexture3D(const std::string& path)
    : m_path(path)
{
    RE_PROFILE_FUNCTION();
    int width = 512,height = 512,depth = 507;
    float *data = nullptr;
    
    FILE* f = fopen(path.c_str(), "rb");
    RE_ASSERT(f,"Can't Open the Texture file");
    
    int wd = height * depth;
    data = new float[wd * width];
    unsigned short *s = new unsigned short[width];

    for (int k = 0; k < height; k++)
    {
        for (int j = 0; j < depth; j++)
        {
            fread(s, sizeof(unsigned short), width, f);
            for (int i = 0; i < width; i++)
            {
                data[k * wd + j * width + i] = s[i];
                // if (s[i] > max_number)
                //     max_number = s[i];
            }
        }
    }
    delete[] s;

    RE_CORE_ASSERT(data,"fail to load image!");
    m_width = static_cast<uint32_t>(width);
    m_height = static_cast<uint32_t>(height);
    m_depth = static_cast<uint32_t>(depth);

    GLenum interFormat = GL_R32F , dataFormat = GL_RED;

    m_interFormat = interFormat;
    m_dataFormat = dataFormat;

    RE_CORE_ASSERT(interFormat & dataFormat,"Format not Support!");
    
    glCreateTextures(GL_TEXTURE_3D,1,&m_render_id);

    glTextureStorage3D(m_render_id,1,interFormat,m_width,m_height,m_depth);

    glTextureParameteri(m_render_id,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTextureParameteri(m_render_id,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    glTextureParameteri(m_render_id,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_render_id,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_render_id,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);


    glTextureSubImage3D(m_render_id,0,0,0,0,m_width,m_height,m_depth,m_dataFormat,GL_UNSIGNED_BYTE,data);

    delete[] data;
}

OpenGLTexture3D::~OpenGLTexture3D()
{
    RE_PROFILE_FUNCTION();
    glDeleteTextures(1,&m_render_id);
}

void OpenGLTexture3D::setData(void* data,uint32_t size)
{
    RE_PROFILE_FUNCTION();
    uint32_t bpp = (m_dataFormat == GL_RGBA)? 4 : ((m_dataFormat == GL_RGB)? 3: 0); 
    RE_CORE_ASSERT((size == m_height * m_width * bpp),"Can not match the size!");
    glTextureSubImage3D(m_render_id,0,0,0,0,m_width,m_height,m_depth,m_dataFormat,GL_UNSIGNED_BYTE,data);
}

void OpenGLTexture3D::Bind(uint32_t slot) const
{
    RE_PROFILE_FUNCTION();
    glBindTextureUnit(slot,m_render_id);
}

void OpenGLTexture3D::Unbind() const
{
    RE_PROFILE_FUNCTION();
    glBindTextureUnit(0,0);
}

} // namespace Rengin

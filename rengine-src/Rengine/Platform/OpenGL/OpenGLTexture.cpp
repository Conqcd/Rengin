#include "repch.hpp"
#include "OpenGLTexture.hpp"
#include "Rengine/Utils/IO/RawReader.hpp"
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
//////////////////////////////////////Cube/////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////


OpenGLTextureCube::OpenGLTextureCube(const std::string& posx,const std::string& negx,const std::string& posy,const std::string& negy,const std::string& posz,const std::string& negz)
    : m_path({posx,negx,posy,negy,posz,negz})
{
    RE_PROFILE_FUNCTION();
    stbi_set_flip_vertically_on_load(0);

    glGenTextures(1, &m_render_id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_render_id);

    // glCreateTextures(GL_TEXTURE_CUBE_MAP,1,&m_render_id);

    int width, height, channels;
    GLenum interFormat = 0, dataFormat = 0;
    for (int i = 0; i < 6; i++)
    {
        stbi_uc *data = nullptr;
        {
            RE_PROFILE_SCOPE(
                "stbi_load - OpenGLTextureCube::OpenGLTextureCube(const std::string&,const std::string&,const std::string&,const std::string&,const std::string&,const std::string&)");
            data = stbi_load(m_path[i].c_str(), &width, &height, &channels, 0);
        }
        RE_CORE_ASSERT(data, "fail to load image!");

        m_width[i] = static_cast<uint32_t>(width);
        m_height[i] = static_cast<uint32_t>(height);

        if (channels == 4) {
            interFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        } else if (channels == 3) {
            interFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }

        m_interFormat[i] = interFormat;
        m_dataFormat[i] = dataFormat;
        RE_CORE_ASSERT(interFormat & dataFormat, "Format not Support!");
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, interFormat, m_width[i],
                     m_height[i], 0, dataFormat, GL_UNSIGNED_BYTE, data);
        // glTextureSubImage2D(m_render_id, 0, 0, 0, m_width[i], m_height[i], dataFormat,
        //                     GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    // glTextureStorage2D(m_render_id,1,interFormat,m_width,m_height);

    glTextureParameteri(m_render_id,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTextureParameteri(m_render_id,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTextureParameteri(m_render_id,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_render_id,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_render_id,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

OpenGLTextureCube::~OpenGLTextureCube()
{
    RE_PROFILE_FUNCTION();
    glDeleteTextures(1,&m_render_id);
}

void OpenGLTextureCube::setData(void* data,uint32_t size)
{
    RE_PROFILE_FUNCTION();
    // uint32_t bpp = (m_dataFormat == GL_RGBA)? 4 : ((m_dataFormat == GL_RGB)? 3: 0); 
    // RE_CORE_ASSERT((size == m_height * m_width * bpp),"Can not match the size!");
    // glTextureSubImage2D(m_render_id,0,0,0,m_width,m_height,m_dataFormat,GL_UNSIGNED_BYTE,data);
}

void OpenGLTextureCube::Bind(uint32_t slot) const
{
    RE_PROFILE_FUNCTION();
    glBindTextureUnit(slot,m_render_id);
}

void OpenGLTextureCube::Unbind() const
{
    RE_PROFILE_FUNCTION();
    glBindTextureUnit(0,0);
}

///////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////3D///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

OpenGLTexture3D::OpenGLTexture3D(uint32_t width,uint32_t height,uint32_t depth,uint32_t bpp,int InorNe)
        :m_width(width), m_height(height), m_depth(depth)
{
    RE_PROFILE_FUNCTION();

    GLenum interFormat = GL_RGB32F , dataFormat = GL_RGB;

    if(bpp == 1)
    {
        interFormat = GL_R32F,dataFormat = GL_RED;
    }
    else if(bpp == 4)
    {
        interFormat = GL_RGBA8, dataFormat = GL_RGBA;
    }
    else
    {
        interFormat = GL_RGB32F,dataFormat = GL_RGB;
    }

    m_interFormat = interFormat;
    m_dataFormat = dataFormat;

    glCreateTextures(GL_TEXTURE_3D,1,&m_render_id);

    glTextureStorage3D(m_render_id,1,interFormat,m_width,m_height,m_depth);

    glTextureParameteri(m_render_id,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTextureParameteri(m_render_id,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    glTextureParameteri(m_render_id,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_render_id,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_render_id,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);

    m_tex.resize(width * height * depth * bpp);
}

OpenGLTexture3D::OpenGLTexture3D(const std::string& path,int InorNe)
    : m_path(path)
{
    RE_PROFILE_FUNCTION();
    // int width = 512,height = 512,depth = 507;
    // int width = 245,height = 335,depth = 451;
    // int width = 335,height = 451,depth = 245;
    RawReader reader(path);
    m_width = reader.dimensions[0];
    m_height = reader.dimensions[1];
    m_depth = reader.dimensions[2];
    
    int hw = m_height * m_width;
    m_tex.resize(hw * m_depth);
    float maxval = 0.0f;

    auto data = reader.load();
 
    for (int k = 0; k < m_depth; k++)
    {
        for (int j = 0; j < m_height; j++)
        {
            for (int i = 0; i < m_width; i++)
            {
                int id = k * hw + j * m_width + i;
                m_tex[id] = data[id];
                maxval = std::max(maxval,m_tex[id]);
            }
        }
    }


    GLenum interFormat = GL_R32F , dataFormat = GL_RED;

    m_interFormat = interFormat;
    m_dataFormat = dataFormat;

    RE_CORE_ASSERT(interFormat & dataFormat,"Format not Support!");
    
    glCreateTextures(GL_TEXTURE_3D,1,&m_render_id);
    
    glTextureStorage3D(m_render_id,1,interFormat,m_width,m_height,m_depth);

    glTextureParameteri(m_render_id,GL_TEXTURE_MIN_FILTER,InorNe);
    glTextureParameteri(m_render_id,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    glTextureParameteri(m_render_id,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_render_id,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_render_id,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);

    glTextureSubImage3D(m_render_id,0,0,0,0,m_width,m_height,m_depth,m_dataFormat,GL_FLOAT,m_tex.data());

}

OpenGLTexture3D::~OpenGLTexture3D()
{
    RE_PROFILE_FUNCTION();
    glDeleteTextures(1,&m_render_id);
}

void OpenGLTexture3D::setData(void* data,uint32_t size)
{
    RE_PROFILE_FUNCTION();
    uint32_t bpp = (m_dataFormat == GL_RGBA)? 4 : ((m_dataFormat == GL_RGB)? 3: (m_dataFormat == GL_RED? 1 : 0));
    RE_CORE_ASSERT((size == m_height * m_width * m_depth * bpp),"Can not match the size!");
    if(m_interFormat == GL_RGB32F)
      glTextureSubImage3D(m_render_id, 0, 0, 0, 0, m_width, m_height, m_depth,
                          m_dataFormat, GL_FLOAT, data);
    else
      glTextureSubImage3D(m_render_id, 0, 0, 0, 0, m_width, m_height, m_depth,
                          m_dataFormat, GL_FLOAT, data);
}

void OpenGLTexture3D::setData(std::vector<float>& data)
{
    RE_PROFILE_FUNCTION();
    uint32_t bpp = (m_dataFormat == GL_RGBA)? 4 : ((m_dataFormat == GL_RGB)? 3: (m_dataFormat == GL_RED? 1 : 0));
    RE_CORE_ASSERT((data.size() == m_height * m_width * m_depth * bpp),"Can not match the size!");
    m_tex = data;
    if(m_interFormat == GL_RGB32F)
      glTextureSubImage3D(m_render_id, 0, 0, 0, 0, m_width, m_height, m_depth,
                          m_dataFormat, GL_FLOAT, m_tex.data());
    else
      glTextureSubImage3D(m_render_id, 0, 0, 0, 0, m_width, m_height, m_depth,
                          m_dataFormat, GL_FLOAT, m_tex.data());
}

void OpenGLTexture3D::Bind(uint32_t slot) const
{
    RE_PROFILE_FUNCTION();
    RE_CORE_ASSERT(slot < 32,"slot can not more than 31"); 
    glBindTextureUnit(slot,m_render_id);
}

void OpenGLTexture3D::Unbind() const
{
    RE_PROFILE_FUNCTION();
    glBindTextureUnit(0,0);
}

} // namespace Rengin

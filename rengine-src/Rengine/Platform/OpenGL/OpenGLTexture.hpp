#pragma once
#include "Rengine/Core/core.hpp"
#include "Rengine/Renderer/Texture.hpp"
#include <glad/glad.h>

namespace Rengin
{
class RE_API OpenGLTexture2D : public Texture2D
{
private:
    uint32_t m_height;
    uint32_t m_width;
    std::string m_path;
    uint32_t m_render_id;
    GLenum m_interFormat;
    GLenum m_dataFormat;
public:
    OpenGLTexture2D(uint32_t width,uint32_t height);
    OpenGLTexture2D(const std::string& path);
    virtual ~OpenGLTexture2D();
    
    inline virtual uint32_t getWidth() const override {return m_width;}
    inline virtual uint32_t getHeight() const override {return m_height;}
    
    virtual uint32_t getRendererID() const override {return m_render_id;}
    virtual void setData(void* data,uint32_t size) override;

    virtual void Bind(uint32_t slot = 0) const override;
    virtual void Unbind() const override;
    
    virtual bool operator==(const Texture& other) const override
    {
        return m_render_id == ((OpenGLTexture2D&)other).m_render_id;
    }
};

class RE_API OpenGLTextureCube : public TextureCube
{
private:
    uint32_t m_height[6];
    uint32_t m_width[6];
    std::vector<std::string> m_path;
    uint32_t m_render_id;
    GLenum m_interFormat[6];
    GLenum m_dataFormat[6];
public:
    OpenGLTextureCube(const std::string& posx,const std::string& negx,const std::string& posy,const std::string& negy,const std::string& posz,const std::string& negz);
    virtual ~OpenGLTextureCube();
    
    inline virtual uint32_t getWidth() const override {return m_width[0];}
    inline virtual uint32_t getHeight() const override {return m_height[0];}
    
    virtual uint32_t getRendererID() const override {return m_render_id;}
    virtual void setData(void* data,uint32_t size) override;

    virtual void Bind(uint32_t slot = 0) const override;
    virtual void Unbind() const override;
    
    virtual bool operator==(const Texture& other) const override
    {
        return m_render_id == ((OpenGLTextureCube&)other).m_render_id;
    }
};

class RE_API OpenGLTexture3D : public Texture3D
{
private:
    uint32_t m_height;
    uint32_t m_width;
    uint32_t m_depth;
    std::string m_path;
    uint32_t m_render_id;
    GLenum m_interFormat;
    GLenum m_dataFormat;
    std::vector<float> m_tex;
public:
    OpenGLTexture3D(uint32_t width,uint32_t height,uint32_t depth,uint32_t bpp,int InorNe = GL_NEAREST);
    OpenGLTexture3D(const std::string& path,int InorNe = GL_NEAREST);
    virtual ~OpenGLTexture3D();
    
    inline virtual uint32_t getWidth() const override {return m_width;}
    inline virtual uint32_t getHeight() const override {return m_height;}
    inline virtual uint32_t getDepth() const override {return m_depth;}
    inline virtual std::vector<float>& getTexture() override {return m_tex;}
    inline virtual uint32_t getBPP() const override{
        if(m_interFormat == GL_R32F)
            return 1;
        else if(m_interFormat == GL_RGBA8)
            return 4;
        else
            return 3;
    }
    
    virtual uint32_t getRendererID() const override {return m_render_id;}
    virtual void setData(void* data,uint32_t size) override;
    virtual void setData(std::vector<float>& data) override;

    virtual void Bind(uint32_t slot = 0) const override;
    virtual void Unbind() const override;
    
    virtual bool operator==(const Texture& other) const override
    {
        return m_render_id == ((OpenGLTexture3D&)other).m_render_id;
    }
};
    
} // namespace Rengin

#pragma once
#include "Rengine/core.hpp"
#include "Rengine/Renderer/Texture.hpp"

namespace Rengin
{
class RE_API OpenGLTexture2D : public Texture2D
{
private:
    uint32_t m_height;
    uint32_t m_width;
    std::string m_path;
    uint32_t m_render_id;
public:
    OpenGLTexture2D(const std::string& path);
    virtual ~OpenGLTexture2D();
    
    inline virtual uint32_t getWidth() const override {return m_width;}
    inline virtual uint32_t getHeight() const override {return m_height;}

    virtual void Bind(uint32_t slot = 0) const override;
    virtual void Unbind() const override;
};


    
} // namespace Rengin

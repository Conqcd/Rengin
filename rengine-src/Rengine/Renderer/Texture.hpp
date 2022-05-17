#pragma once
#include "Rengine/Core/core.hpp"

namespace Rengin
{
class RE_API Texture
{
public:
    virtual ~Texture() = default;
    virtual uint32_t getWidth() const = 0;
    virtual uint32_t getHeight() const = 0;

    virtual uint32_t getRendererID() const = 0;

    virtual void setData(void* data,uint32_t size) = 0;

    virtual void Bind(uint32_t slot = 0) const = 0;
    virtual void Unbind() const = 0;

    virtual bool operator==(const Texture& other) const = 0;
};

class RE_API Texture2D :public Texture
{
public:
    virtual ~Texture2D() = default;
    static Ref<Texture2D> Create(uint32_t width,uint32_t height);
    static Ref<Texture2D> Create(const std::string& path);

};

class RE_API TextureCube :public Texture
{
public:
    virtual ~TextureCube() = default;
    static Ref<TextureCube> Create(const std::string& negx,const std::string& negy,const std::string& negz,const std::string& posx,const std::string& posy,const std::string& posz);
};

class RE_API Texture3D :public Texture
{
public:
    virtual ~Texture3D() = default;
    static Ref<Texture3D> Create(uint32_t width,uint32_t height,uint32_t depth,uint32_t bpp = 1);
    static Ref<Texture3D> Create(const std::string& path);

    virtual uint32_t getDepth() const = 0;
    virtual uint32_t getBPP() const = 0;
    virtual std::vector<float>& getTexture() = 0;
    virtual void setData(std::vector<float>& data) = 0;
    virtual void setData(void* data,uint32_t size) = 0;
};

} // namespace Rengin

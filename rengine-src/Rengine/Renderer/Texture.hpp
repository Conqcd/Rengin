#pragma once
#include "Rengine/core.hpp"

namespace Rengin
{
class RE_API Texture
{
public:
    virtual ~Texture() = default;
    virtual uint32_t getWidth() const = 0;
    virtual uint32_t getHeight() const = 0;

    virtual void Bind(uint32_t slot = 0) const = 0;
    virtual void Unbind() const = 0;
};

class RE_API Texture2D :public Texture
{
public:
    virtual ~Texture2D() = default;
    static Ref<Texture2D> Create(const std::string& path);
};



} // namespace Rengin

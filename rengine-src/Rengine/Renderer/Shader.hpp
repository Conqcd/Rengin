#pragma once
#include "Rengine/core.hpp"
#include <string>
namespace Rengin
{
class RE_API Shader
{
public:
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void UnBind() const = 0;

    static Shader* Create(const std::string& vertexSrc,const std::string& fragmentSrc);
    static Shader* Create(const std::string& filename);
};

    
} // namespace Rengin

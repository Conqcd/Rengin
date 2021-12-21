#pragma once
#include "Rengine/Core/core.hpp"
#include <string>
#include <unordered_map>
namespace Rengin
{
class RE_API Shader
{
public:
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void UnBind() const = 0;
    virtual const std::string& getName() const = 0;

    static Ref<Shader> Create(const std::string& name,const std::string& vertexPath,const std::string& fragmentPath);
    static Ref<Shader> Create(const std::string& vertexPath,const std::string& fragmentPath);
};

class RE_API ShaderLibrary
{
public:
    void Add(const Ref<Shader>& shader);
    void Add(const std::string& name,const Ref<Shader>& shader);
    Ref<Shader> Load(const std::string& vertexPath,const std::string& fragmentPath);
    Ref<Shader> Load(const std::string& name,const std::string& vertexPath,const std::string& fragmentPath);

    Ref<Shader> Get(const std::string& name);
    bool Exists(const std::string& name)const;
private:
    std::unordered_map<std::string,Ref<Shader>> m_shaders;
};
} // namespace Rengin

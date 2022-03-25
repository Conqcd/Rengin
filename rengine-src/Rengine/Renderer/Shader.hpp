#pragma once
#include "Rengine/Core/core.hpp"
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
namespace Rengin
{
class RE_API Shader
{
public:
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void UnBind() const = 0;
    virtual const std::string& getName() const = 0;

    virtual void SetUniformInt(const std::string& name,int value) = 0;
    virtual void SetUniformUint(const std::string& name,unsigned int value) = 0;
    virtual void SetUniformIntArray(const std::string& name,int* values,uint32_t count) = 0;
    virtual void SetUniformMat3(const std::string& name,const glm::mat3& mat) = 0;
    virtual void SetUniformMat4(const std::string& name,const glm::mat4& mat) = 0;
    virtual void SetUniformFloat(const std::string& name,float value) = 0;
    virtual void SetUniformFloat2(const std::string& name,const glm::vec2& vec) = 0;
    virtual void SetUniformFloat3(const std::string& name,const glm::vec3& vec) = 0;
    virtual void SetUniformFloat4(const std::string& name,const glm::vec4& vec) = 0;

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

#pragma once
#include "Rengine/Core/core.hpp"
#include "Rengine/Renderer/Shader.hpp"
#include <unordered_map>
#include <glm/glm.hpp>
#include <glad/glad.h>

namespace Rengin
{
class RE_API OpenGLShader : public Shader
{
private:
    uint32_t m_render_id;
    std::string m_name;
public: 
    OpenGLShader(const std::string& name,const std::string& vertexPath,const std::string& fragmentPath);
    OpenGLShader(const std::string& vertexPath,const std::string& fragmentPath);
    virtual ~OpenGLShader();

    virtual void Bind() const override;
    virtual void UnBind() const override;
    virtual const std::string& getName() const override;

    
    virtual void SetUniformInt(const std::string& name,int value)override;
    virtual void SetUniformMat4(const std::string& name,const glm::mat4& mat) override;
    virtual void SetUniformFloat(const std::string& name,float value) override;
    virtual void SetUniformFloat3(const std::string& name,const glm::vec3& vec) override;
    virtual void SetUniformFloat4(const std::string& name,const glm::vec4& vec) override;

    void UpLoadUniformInt(const std::string& name, int value);
    void UpLoadUniformFloat(const std::string& name, float value);
    void UpLoadUniformFloat2(const std::string& name, const glm::vec2& value);
    void UpLoadUniformFloat3(const std::string& name, const glm::vec3& value);
    void UpLoadUniformFloat4(const std::string& name, const glm::vec4& value);
    
    void UpLoadUniformMat3(const std::string& name, const glm::mat3& matrix);
    void UpLoadUniformMat4(const std::string& name, const glm::mat4& matrix);
private:
    const std::string ReadFile(const std::string& src);
    void Compile(const std::unordered_map<GLenum,std::string>& shaderSrc);
};




} // namespace Rengin

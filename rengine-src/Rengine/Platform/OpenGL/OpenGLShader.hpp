#pragma once
#include "Rengine/core.hpp"
#include "Rengine/Renderer/Shader.hpp"
#include <glm/glm.hpp>

namespace Rengin
{
class RE_API OpenGLShader : public Shader
{
private:
    uint32_t m_render_id;
public: 
    OpenGLShader(const std::string& vertexSrc,const std::string& fragmentSrc);
    virtual ~OpenGLShader();

    virtual void Bind() const override;
    virtual void UnBind() const override;

    void UpLoadUniformInt(const std::string& name, int value);

    void UpLoadUniformFloat(const std::string& name, float value);
    void UpLoadUniformFloat2(const std::string& name, const glm::vec2& value);
    void UpLoadUniformFloat3(const std::string& name, const glm::vec3& value);
    void UpLoadUniformFloat4(const std::string& name, const glm::vec4& value);
    
    void UpLoadUniformMat3(const std::string& name, const glm::mat3& matrix);
    void UpLoadUniformMat4(const std::string& name, const glm::mat4& matrix);
};




} // namespace Rengin

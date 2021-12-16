#pragma once
#include <string>
#include <glm/glm.hpp>
namespace Rengin
{
class Shader
{
private:
    uint32_t m_render_id;
public:
    Shader(const std::string& vertexSrc,const std::string& fragmentSrc);
    ~Shader();

    void Bind() const;
    void UnBind() const;

    void UpLoadUniformFloat4(const std::string& name, const glm::vec4& value);
    void UpLoadUniformMat4(const std::string& name, const glm::mat4& matrix);
};

    
} // namespace Rengin

#pragma once
#include <string>
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
};

    
} // namespace Rengin

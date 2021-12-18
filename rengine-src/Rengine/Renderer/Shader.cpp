#include "repch.hpp"
#include "Shader.hpp"
#include "Rengine/log.hpp"
#include "RendererAPI.hpp"
#include "Renderer.hpp"
#include "Rengine/Platform/OpenGL/OpenGLShader.hpp"

namespace Rengin
{

Ref<Shader> Shader::Create(const std::string& name,const std::string& vertexSrc,const std::string& fragmentSrc)
{
    switch (Renderer::getRenderer())
    {
    case RendererAPI::API::None :
        RE_CORE_ASSERT(false,"not support for No Render API");
        return nullptr;
    case RendererAPI::API::OpenGL :
        return std::make_shared<OpenGLShader>(name,vertexSrc,fragmentSrc);
    case RendererAPI::API::Direct3D :
        RE_CORE_ASSERT(false,"not support for DirectX3D Render API");
        break;
    }
    RE_CORE_ASSERT(false,"Unknown RendererAPI!");
    return nullptr;
}
Ref<Shader> Shader::Create(const std::string& filename)
{
    switch (Renderer::getRenderer())
    {
    case RendererAPI::API::None :
        RE_CORE_ASSERT(false,"not support for No Render API");
        break;
    case RendererAPI::API::OpenGL :
        return std::make_shared<OpenGLShader>(filename);
    case RendererAPI::API::Direct3D :
        RE_CORE_ASSERT(false,"not support for DirectX3D Render API");
        break;
    }
    RE_CORE_ASSERT(false,"Unknown RendererAPI!");
    return nullptr;
}

void ShaderLibrary::Add(const Ref<Shader>& shader)
{
    auto& name = shader->getName();
    RE_CORE_ASSERT(!Exists(name),"Shaders already exists!");
    m_shaders[name] = shader;
}

void ShaderLibrary::Add(const std::string& name,const Ref<Shader>& shader)
{
    RE_CORE_ASSERT(!Exists(name),"Shaders already exists!");
    m_shaders[name] = shader;
}

Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
{
    auto shader = Shader::Create(filepath);
    Add(shader);
    return shader;
}

Ref<Shader> ShaderLibrary::Load(const std::string& filepath,const std::string& name)
{
    auto shader = Shader::Create(filepath);
    Add(name,shader);
    return shader;
}

Ref<Shader> ShaderLibrary::Get(const std::string& name)
{
    RE_CORE_ASSERT(Exists(name),"Shaders not found!");
    return m_shaders[name];
}

bool ShaderLibrary::Exists(const std::string& name)const 
{
    return m_shaders.find(name) != m_shaders.end();
}

} // namespace Rengin

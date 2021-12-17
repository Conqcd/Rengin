#include "repch.hpp"
#include "Shader.hpp"
#include "Rengine/log.hpp"
#include "RendererAPI.hpp"
#include "Renderer.hpp"
#include "Rengine/Platform/OpenGL/OpenGLShader.hpp"

namespace Rengin
{

Shader* Shader::Create(const std::string& vertexSrc,const std::string& fragmentSrc)
{
    switch (Renderer::getRenderer())
    {
    case RendererAPI::API::None :
        RE_CORE_ASSERT(false,"not support for No Render API");
        return nullptr;
        break;
    case RendererAPI::API::OpenGL :
        return new OpenGLShader(vertexSrc,fragmentSrc);
        break;
    case RendererAPI::API::Direct3D :
        RE_CORE_ASSERT(false,"not support for DirectX3D Render API");
        return nullptr;
        break;
    }
    RE_CORE_ASSERT(false,"Unknown RendererAPI!");
    return nullptr;
}

} // namespace Rengin

#include "repch.hpp"
#include "Texture.hpp"
#include "RendererAPI.hpp"
#include "Renderer.hpp"
#include "Rengine/Platform/OpenGL/OpenGLTexture.hpp"

namespace Rengin
{
    
Ref<Texture2D> Texture2D::Create(uint32_t width,uint32_t height)
{
    switch (Renderer::getRenderer())
    {
    case RendererAPI::API::None :
        RE_CORE_ASSERT(false,"not support for No Render API");
        return nullptr;
    case RendererAPI::API::OpenGL :
        return CreateRef<OpenGLTexture2D>(width,height);
    case RendererAPI::API::Direct3D :
        RE_CORE_ASSERT(false,"not support for DirectX3D Render API");
        return nullptr;
    }
    RE_CORE_ASSERT(false,"Unknown RendererAPI!");
    return nullptr;
}

Ref<Texture2D> Texture2D::Create(const std::string& path)
{
    switch (Renderer::getRenderer())
    {
    case RendererAPI::API::None :
        RE_CORE_ASSERT(false,"not support for No Render API");
        return nullptr;
    case RendererAPI::API::OpenGL :
        return CreateRef<OpenGLTexture2D>(path);
    case RendererAPI::API::Direct3D :
        RE_CORE_ASSERT(false,"not support for DirectX3D Render API");
        return nullptr;
    }
    RE_CORE_ASSERT(false,"Unknown RendererAPI!");
    return nullptr;
}

Ref<Texture3D> Texture3D::Create(uint32_t width,uint32_t height,uint32_t depth)
{
    switch (Renderer::getRenderer())
    {
    case RendererAPI::API::None :
        RE_CORE_ASSERT(false,"not support for No Render API");
        return nullptr;
    case RendererAPI::API::OpenGL :
        return CreateRef<OpenGLTexture3D>(width,height,depth);
    case RendererAPI::API::Direct3D :
        RE_CORE_ASSERT(false,"not support for DirectX3D Render API");
        return nullptr;
    }
    RE_CORE_ASSERT(false,"Unknown RendererAPI!");
    return nullptr;
}

Ref<Texture3D> Texture3D::Create(const std::string& path)
{
    switch (Renderer::getRenderer())
    {
    case RendererAPI::API::None :
        RE_CORE_ASSERT(false,"not support for No Render API");
        return nullptr;
    case RendererAPI::API::OpenGL :
        return CreateRef<OpenGLTexture3D>(path);
    case RendererAPI::API::Direct3D :
        RE_CORE_ASSERT(false,"not support for DirectX3D Render API");
        return nullptr;
    }
    RE_CORE_ASSERT(false,"Unknown RendererAPI!");
    return nullptr;
}  
} // namespace Rengin

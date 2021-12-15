#pragma once
#include "Rengine/core.hpp"
#include "RendererAPI.hpp"
namespace Rengin
{


class RE_API Renderer
{
private:
    static RendererAPI::API m_render_api;
public:
    inline static RendererAPI::API getRenderer(){return m_render_api;}
};

} // namespace Rengin

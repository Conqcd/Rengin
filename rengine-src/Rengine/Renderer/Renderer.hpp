#pragma once

namespace Rengin
{

enum class RendererAPI
{
    None = 0,
    OpenGL = 1,
    Direct3D = 2
};

class Renderer
{
private:
    static RendererAPI m_render_api;
public:
    inline static RendererAPI getRenderer(){return m_render_api;}
};

} // namespace Rengin

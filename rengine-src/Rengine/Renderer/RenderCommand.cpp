#include "repch.hpp"
#include "RenderCommand.hpp"
#include "Rengine/Platform/OpenGL/OpenGLRendererAPI.hpp"

namespace Rengin
{

RendererAPI* RenderCommand::s_api = new OpenGLRendererAPI();
} // namespace Rengin

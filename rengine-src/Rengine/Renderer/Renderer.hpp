#pragma once
#include "Rengine/Core/core.hpp"
#include "RendererAPI.hpp"
#include "RenderCommand.hpp"
#include "VertexArray.hpp"
#include "OrthoGraphicsCamera.hpp"
#include "Shader.hpp"

namespace Rengin
{

class RE_API Renderer
{
public:
    static void Init();
    static void OnWindowResized(uint32_t width ,uint32_t height);
    static void BeginScene(OrthoGraphicsCamera& camera);
    static void EndScene();
    static void Shutdown();
    static void Submit(const Ref<Shader>& shader,const Ref<VertexArray>& vertexArray,const glm::mat4& transform = glm::mat4(1.0f));
    inline static RendererAPI::API getRenderer(){return RendererAPI::getAPI();}
private:
    struct SceneData
    {
        glm::mat4 ViewProjectionMat;
    };
    static SceneData* m_scene_data;
};

} // namespace Rengin

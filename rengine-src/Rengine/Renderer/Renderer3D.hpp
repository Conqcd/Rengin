#pragma once
#include "Rengine/Core/core.hpp"
#include "OrthoGraphicsCamera.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "Texture.hpp"
#include "SubTexture3D.hpp"

namespace Rengin
{
class Renderer3D
{
private:

public:
    static void Init();
    static void Shutdown();
    static void OnWindowResized(uint32_t width ,uint32_t height);

    static void BeginScene(const Camera& camera,const glm::mat4& transform);
    static void BeginScene(const OrthoGraphicsCamera& camera); // TODO: Remove
    static void EndScene();
    static void Flush();

    //Primitives
    static void DrawCube(const glm::vec3& position,const glm::vec3& size,const glm::vec4& color);
    static void DrawCube(const glm::vec3& position,const glm::vec3& size,const Ref<Texture>& texture,float tile_factor = 1.0,const glm::vec4& tintColor = glm::vec4(1.0f));
    static void DrawCube(const glm::vec3& position,const glm::vec3& size,const Ref<SubTexture3D>& subtexture,float tile_factor = 1.0,const glm::vec4& tintColor = glm::vec4(1.0f));

    static void DrawRotatedCube(const glm::vec3& position,const glm::vec3& size,float rotation,const glm::vec4& color);
    static void DrawRotatedCube(const glm::vec3& position,const glm::vec3& size,float rotation,const Ref<Texture>& texture,float tile_factor = 1.0,const glm::vec4& tintColor = glm::vec4(1.0f));
    static void DrawRotatedCube(const glm::vec3& position,const glm::vec3& size,float rotation,const Ref<SubTexture3D>& subtexture,float tile_factor = 1.0,const glm::vec4& tintColor = glm::vec4(1.0f));

    static void DrawVolume(const glm::mat4 &transforms,const Ref<Texture>& texture);
    struct Statistic
    {
        uint32_t DrawCall = 0;
        uint32_t CubeCount = 0;

        uint32_t GetTotalVertexCount()const {return CubeCount * 8;}
        uint32_t GetTotalIndexCount()const {return CubeCount * 36;}
    };
    static Statistic getStats();
    static void resetStats();

private:
    static void FlushAndReset();
};


} // namespace Rengin



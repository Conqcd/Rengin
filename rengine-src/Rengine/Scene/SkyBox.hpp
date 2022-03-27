#pragma once
#include <Rengine/Core/core.hpp>
#include <Rengine/Renderer/Texture.hpp>
#include <Rengine/Renderer/Shader.hpp>
#include "Rengine/Renderer/VertexArray.hpp"
#include "Rengine/Renderer/EditorCamera.hpp"
#include <vector>
#include <string>
#include <glm/glm.hpp>

namespace Rengin
{
class SkyBox
{
private:
    std::vector<Ref<TextureCube>> m_CubeTextures;
    Ref<VertexArray> m_Cube;
    Ref<Shader> m_Shader,m_PRTShader;
    std::vector<glm::mat3> PreComSHR,PreComSHG,PreComSHB;
    bool ComputePRTOK = true;
public:
    SkyBox();
    ~SkyBox() = default;
    void AddCubeMap(const std::string& path);
    inline void SetShader(const Ref<Shader> shader) { m_Shader = shader;}
    inline void SetPRTShader(const Ref<Shader> shader) { m_PRTShader = shader;}

    void RenderCube(int id,const EditorCamera& camera);
    // TODO
    void ComputePRTSH(const std::string& path);
};

} // namespace Rengin

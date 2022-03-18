#pragma once
#include "Rengine/Core/core.hpp"
#include "Rengine/Scene/Component.hpp"
#include "Rengine/Scene/ObjManager.hpp"
#include "Methods/BaseMethod.hpp"
#include "Shader.hpp"
#include "EditorCamera.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>
#include <glm/glm.hpp>

namespace Rengin
{

class RendererObject
{
private:
    std::vector<ObjManager> ObjLists;
    std::unordered_map<std::string,Ref<RenderMethod>> m_Methods;

    glm::vec3 LightPos,LightIntensity;
public:
    RendererObject();
    ~RendererObject() = default;
    void DrawObject(uint32_t id,Ref<Shader> shader,const EditorCamera& camera);
    void AddMethod(const std::string& name,Ref<RenderMethod> method);
    void AddObj(const ObjManager& obj);
};

} // namespace Rengin

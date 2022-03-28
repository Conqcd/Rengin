#pragma once
#include "Rengine/Core/core.hpp"
#include "Rengine/Scene/Component.hpp"
#include "Rengine/Scene/ObjManager.hpp"
#include "Rengine/Scene/Lights.hpp"
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
    std::vector<Ref<ObjManager>> ObjLists;
    std::unordered_map<std::string,Ref<RenderMethod>> m_Methods;

    Lights lights;
public:
    RendererObject();
    ~RendererObject() = default;
    void DrawObject(const std::string& methodName,const std::vector<int>& ids,const EditorCamera& camera);
    void AddMethod(const std::string& name,Ref<RenderMethod> method);
    void AddObj(const Ref<ObjManager> obj);
    void ComputePrt();
};

} // namespace Rengin

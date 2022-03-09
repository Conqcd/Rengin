#pragma once
#include "Rengine/Core/core.hpp"
#include "Rengine/Scene/Component.hpp"
#include "Rengine/Scene/ObjManager.hpp"
#include "Rengine/Renderer/Shader.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>

namespace Rengin
{

class RendererObject
{
private:
    std::vector<ObjManager> ObjLists;
    std::unordered_map<std::string,std::function<void()>> m_Methods;
public:
    RendererObject(/* args */) = default;
    ~RendererObject() = default;
    void DrawObject(uint32_t id,Ref<Shader> shader);
    void AddObj(const ObjManager& obj);
};

} // namespace Rengin

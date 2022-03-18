#pragma once
#include "Rengine/Core/core.hpp"
#include "../Shader.hpp"
#include "Rengine/Scene/ObjManager.hpp"
#include "Rengine/Scene/Lights.hpp"
#include "Rengine/Renderer/EditorCamera.hpp"

namespace Rengin
{
    
class RE_API RenderMethod
{
protected:
    Ref<Shader> m_BaseShader;
public:
    virtual ~RenderMethod() = default;

    virtual void Render(const std::vector<int>& ids,const std::vector<ObjManager>& ObjLists,const EditorCamera& camera,const Lights& lights) = 0; 

    // template<typename ...Args>
    // void AddResource(const Args&... resource);
    // {
    //     static_assert(false);
    // }
};

} // namespace Rengin

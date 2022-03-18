#pragma once
#include "Rengine/Core/core.hpp"
#include "../Shader.hpp"

namespace Rengin
{
    
class RE_API RenderMethod
{
protected:
    Ref<Shader> m_BaseShader;
public:
    virtual ~RenderMethod() = default;

    virtual void Render() = 0; 

    template<typename ...Args>
    void AddResource(const Args&... resource);
    // {
    //     static_assert(false);
    // }
};

} // namespace Rengin

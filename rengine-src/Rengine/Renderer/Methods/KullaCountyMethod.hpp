#pragma once
#include "Rengine/Core/core.hpp"
#include "BaseMethod.hpp"

namespace Rengin
{
class PRTMethod : public RenderMethod
{
private:
    Ref<Shader> m_SkyBoxShader;
public:
    PRTMethod();
    virtual ~PRTMethod();

    virtual void Render(const std::vector<int>& ids,const std::vector<Ref<ObjManager>>& ObjLists,const EditorCamera& camera,const Lights& lights) override;

    template<typename ...Args>
    void AddResource(const Args&... resource); 
};
} // namespace Rengin

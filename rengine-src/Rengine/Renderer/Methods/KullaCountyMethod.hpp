#pragma once
#include "Rengine/Core/core.hpp"
#include "Rengine/Platform/OpenGL/OpenGLTexture.hpp"
#include "BaseMethod.hpp"

namespace Rengin
{
class RE_API KullaCountyMethod : public RenderMethod
{
private:
    Ref<Shader> m_SkyBoxShader;
    //LUT
    Ref<Texture2D> m_BRDFLUT,m_Eavg;
public:
    KullaCountyMethod();
    virtual ~KullaCountyMethod();

    virtual void Render(const std::vector<int>& ids,const std::vector<Ref<ObjManager>>& ObjLists,const EditorCamera& camera,const Lights& lights) override;

    template<typename ...Args>
    void AddResource(const Args&... resource); 
};
} // namespace Rengin

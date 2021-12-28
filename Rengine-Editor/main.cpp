#include <rengine.hpp>
#include <Rengine/Core/entrypoint.hpp>
#include "EditorLayer.hpp"
#include <imgui.h>

namespace Rengin{
class Editor :public Application
{
public:
    Editor() :Application()
    {

        PushLayer(new EditorLayer());
    }

    ~Editor(){}
};


Application* CreateApplication()
{
    return new Editor();
}
}
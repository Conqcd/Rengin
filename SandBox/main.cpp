#include <rengine.hpp>

#include <imgui.h>

class ExampleLayer : public Rengin::Layer
{
public:
    ExampleLayer():Layer("Example"){
        
    }

    void OnUpdate(){
        // RE_INFO("Updating");
        if(Rengin::Input::isKeyPressed(RE_KEY_TAB))
        {
            RE_TRACE("Tab is Pressed!");
        }
    }
    
    virtual void OnImGuiRender() override
    {
        ImGui::Begin("Test");
        ImGui::Text("Hello World");
        ImGui::End();
    }

    void OnEvent(Rengin::Event& ev){
        
        RE_TRACE("Event {0}",ev);
    }
};

class SandBox :public Rengin::Application
{
public:
    SandBox() :Rengin::Application()
    {

        PushLayer(new ExampleLayer());
    }

    ~SandBox(){}
};


Rengin::Application* Rengin::CreateApplication()
{
    return new SandBox();
}
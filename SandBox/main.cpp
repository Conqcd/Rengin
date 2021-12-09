#include <rengine.hpp>

class ExampleLayer : public Rengin::Layer
{
public:
    ExampleLayer():Layer("hihi"){}
    void OnUpdate(){
        RE_INFO("Updating");
    }
    
    void OnEvent(Rengin::Event& ev){
        
        RE_TRACE("Event {0}",ev);
    }
};

class SandBox :public Rengin::Application
{
public:
    SandBox(){

        PushLayer(new ExampleLayer());
        PushLayer(new Rengin::ImGuiLayer());
    }

    ~SandBox(){}
};


Rengin::Application* Rengin::CreateApplication()
{
    return new SandBox();
}
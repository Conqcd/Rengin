#pragma once
#include "Rengine/core.hpp"
#include "Rengine/Window.hpp"
#include "Rengine/Renderer/GraphicsContext.hpp"

struct GLFWwindow;
namespace Rengin
{
class WindowsWindow :public Window
{
private:
    GLFWwindow* m_win;
    GraphicsContext* m_context;

    struct WinData
    {
        std::string m_title;
        unsigned int m_height;
        unsigned int m_width;
        bool VSync;
        EventCallBackFunc m_callback;
    };
    WinData m_data;
public:
    WindowsWindow(const WindowProps& props);
    virtual ~WindowsWindow();

    void OnUpdate() override;

    inline unsigned int getHeight()const override { return m_data.m_height;}
    inline unsigned int getWidth()const override { return m_data.m_width;}

    void setEventCallBack(const EventCallBackFunc& fn) override;
    void setVSync(bool enable) override;
    bool isVSync()const override;
    
    inline virtual void* getNativeWindow()const override{return m_win;}
private:
    virtual void Init(const WindowProps& props);
    virtual void Shutdown();
};

}
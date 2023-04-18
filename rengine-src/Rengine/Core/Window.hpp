#pragma once
#include "core.hpp"
#include "Rengine/Event/Event.hpp"

namespace Rengin
{
struct WindowProps
{
    std::string title;
    uint32_t width;
    uint32_t height;
    WindowProps(const std::string &name = "Voxel Base FEM System",uint32_t w = 1280,uint32_t h = 720):title(name),width(w),height(h){}
};

class RE_API Window
{
public:
    using EventCallBackFunc = std::function<void(Event&)>;

    virtual ~Window(){}

    virtual void OnUpdate() = 0;

    virtual uint32_t getHeight()const = 0;
    virtual uint32_t getWidth()const = 0;

    //Windows Attributes

    virtual void setEventCallBack(const EventCallBackFunc& fn) = 0;
    virtual void setVSync(bool enable) = 0;
    virtual bool isVSync() const = 0;

    virtual void* getNativeWindow()const = 0;

    static Scope<Window> WindowCreate(const WindowProps& props = WindowProps());
};


}
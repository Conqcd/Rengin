#pragma once
#include "core.hpp"
#include "Rengine/Event/Event.hpp"

namespace Rengin
{
struct WindowProps
{
    std::string title;
    unsigned int width;
    unsigned int height;
    WindowProps(const std::string &name = "Hohen Engine",unsigned int w = 1280,unsigned int h = 720):title(name),width(w),height(h){}
};

class RE_API Window
{
public:
    using EventCallBackFunc = std::function<void(Event&)>;

    virtual ~Window(){}

    virtual void OnUpdate() = 0;

    virtual unsigned int getHeight()const = 0;
    virtual unsigned int getWidth()const = 0;

    //Windows Attributes

    virtual void setEventCallBack(const EventCallBackFunc& fn) = 0;
    virtual void setVSync(bool enable) = 0;
    virtual bool isVSync() const = 0;

    virtual void* getNativeWindow()const = 0;

    static Window* WindowCreate(const WindowProps& props = WindowProps());
};


}
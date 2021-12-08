#include"Event.hpp"

namespace Rengin
{
class RE_API WindowResizeEvent :public Event
{
private:
    unsigned int m_Height,m_Width;
public:
    WindowResizeEvent(unsigned int width,unsigned int height):m_Width(width),m_Height(height){}

    std::string ToString() const override
    {
        std::stringstream ss;
        ss<<"WindowResizeEvent Width:"<< m_Width << " ,Height" <<m_Height;
        return ss.str();
    }
    inline unsigned int getHeight() const { return m_Height; }
    inline unsigned int getWidth() const { return m_Width; }

    EVENT_CLASS_TYPE(WindowResize)

    EVENT_CLASS_CATAGORY(EventCategoryApplication)
};

class RE_API WindowCloseEvent :public Event
{
private:
public:
    WindowCloseEvent(){}
    EVENT_CLASS_TYPE(WindowClose)
    EVENT_CLASS_CATAGORY(EventCategoryApplication)
};

class RE_API AppTickEvent :public Event
{
private:
public:
    AppTickEvent(){}
    EVENT_CLASS_TYPE(AppTick)
    EVENT_CLASS_CATAGORY(EventCategoryApplication)
};

class RE_API AppUpdateEvent :public Event
{
private:
public:
    AppUpdateEvent(){} 
    EVENT_CLASS_TYPE(AppUpdate)
    EVENT_CLASS_CATAGORY(EventCategoryApplication)
};

class RE_API AppRenderEvent :public Event
{
private:
public:
    AppRenderEvent(){}

    EVENT_CLASS_TYPE(AppRender)

    EVENT_CLASS_CATAGORY(EventCategoryApplication)
};
}
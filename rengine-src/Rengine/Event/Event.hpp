#pragma once
#include "../../repch.hpp"
#include "../core.hpp"

namespace Rengin
{
enum class EventType
{
    None = 0,
    WindowClose,WindowResize,WindowFocus,WindowLoseFocus,WindowMove,
    AppTick,AppUpdate,AppRender,
    KeyPress,KeyRelease,
    MouseButtonPress,MouseButtonRelease,MouseMoved,MouseScrolled
};

enum EventCategory
{
    None = 0,
    EventCategoryApplication    = LEFT(0),
    EventCategoryInput          = LEFT(1),
    EventCategoryKeyboard       = LEFT(2),
    EventCategoryMouse          = LEFT(3),
    EventCategoryMouseButton    = LEFT(4)
};

#define EVENT_CLASS_TYPE(type)  static EventType getStaticType() {return EventType::##type;} \
                                virtual EventType getEventType() const override {return getStaticType();}   \
                                virtual const char* getName() const override { return #type;}

#define EVENT_CLASS_CATAGORY(category) virtual int getCategoryFlags() const override {return category;}

class RE_API Event
{
    friend class EventDispatcher;
public:
    virtual EventType getEventType() const = 0;
    virtual const char* getName() const = 0;
    virtual int getCategoryFlags() const = 0;
    virtual std::string ToString() const { return getName();}

    inline bool isSameCategory(EventCategory cate)
    {
        return getCategoryFlags() & cate;
    }
protected:
    bool m_handle = false;
};

class EventDispatcher
{
    template<typename T>
    using EventFunc = std::function<bool(T&)>;
public:
    explicit EventDispatcher(Event& event):m_Event(event){}

    template<typename T>
    bool Dispatch(EventFunc<T> fn)
    {
        if(m_Event.getEventType()==T::getStaticType())
        {
            // m_Event.m_handle = fn(*(T*)&m_Event);
            m_Event.m_handle = fn(static_cast<T>(m_Event));
        }
        return false;
    }
private:
    Event& m_Event;
};

inline std::ostream& operator<<(std::ostream& os,const Event& ev)
{
    return os << ev.ToString();
}
}
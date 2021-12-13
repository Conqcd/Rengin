#pragma once
#include"Event.hpp"

namespace Rengin
{
class RE_API KeyEvent : public Event
{
private:
    /* data */
public:

    inline int getKeyValue()const {return m_KeyValue;}

    EVENT_CLASS_CATAGORY(EventCategoryKeyboard|EventCategoryInput)
protected:
    explicit KeyEvent(int keycode):m_KeyValue(keycode){}
    int m_KeyValue;
};

class RE_API KeyPressEvent :public KeyEvent
{
private:
    int m_RepeatCount;
public:
    KeyPressEvent(int keyvalue,int repeatcount):KeyEvent(keyvalue),m_RepeatCount(repeatcount){}
    inline int getRepeatCount()const {return m_RepeatCount;}

    std::string ToString() const override
    {
        std::stringstream ss;
        ss<<"KeyPressEvent "<< m_KeyValue <<"(" << m_RepeatCount << "repeats).";

        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyPress)
};

class RE_API KeyReleaseEvent :public KeyEvent
{
private:
public:
    explicit KeyReleaseEvent(int keyvalue):KeyEvent(keyvalue){}

    std::string ToString() const override
    {
        std::stringstream ss;
        ss<<"KeyReleaseEvent "<< m_KeyValue;
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyRelease)
};


class RE_API KeyTypeEvent :public KeyEvent
{
private:
public:
    KeyTypeEvent(int keyvalue):KeyEvent(keyvalue){}

    std::string ToString() const override
    {
        std::stringstream ss;
        ss<<"KeyTypeEvent "<< m_KeyValue ;

        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyType)
};
}
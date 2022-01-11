#pragma once
#include "Rengine/Core/core.hpp"
#include <map>
#include <vector>
#include <glm/glm.hpp>

namespace Rengin
{
template<class T1,class T2>
class TransferFunction
{
public:
    TransferFunction() = default;
	TransferFunction(const std::vector<T1>& _ini1,const std::vector<T2>& _ini2)
	{
        RE_ASSERT(_ini1.size() == _ini2.size(),"Data and Something do not match!");
        if (_ini1.size())
        {
          first = _ini1[0];
          last = _ini1[_ini1.size() - 1];
        }

        for (int i = 0; i < _ini1.size(); i++)
        {
            list[_ini1[i]] = _ini2[i];
            init_[_ini1[i]] = _ini2[i];
        }
    }

    void Reset()
    {
        list = init_;
    }

	void Clear()
	{
		list.clear();
        Reset();
	}
	
	void Insert(const T1& d,const T2& v){
		list[d] = v;
	}
	
    void Insert(const T1& d){
        if(d != last && d != first)
        {
            T2 v;
	    	list[d] = v;
	    	list[d] = (--list.find(d))->second + ((++list.find(d))->second - (--list.find(d))->second) * (d - (--list.find(d))->first) / ((++list.find(d))->first - (--list.find(d))->first);
        }
	}

	void Erase(const T1& key)
	{
        if(init_.find(key) == init_.end())
		    list.erase(key);
	}

    T2 GetView(const T1& key) const {
		return list[key];
	}
	
	size_t Size() const
	{
		return list.size();
	}

    auto begin() {return list.begin();}
    auto end() {return list.end();}
    auto begin()const { return list.begin(); }
    auto end()const {return list.end();}

private:
    T1 first,last;
	std::map<T1,T2> list,init_;
};

} // namespace Rengin
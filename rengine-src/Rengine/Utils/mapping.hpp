#pragma once
#include <set>
#include <glm/glm.hpp>

template<typename T>
struct MapNodeC
{
	T data;
    glm::vec3 color;
    MapNodeC(const T& _d, const glm::vec3& _c) :data(_d), color(_c) {}
};

template<typename T>
struct MapNodeA
{
	T data;
	float opacity;
	MapNodeA(const T& _d, const double& _a) :data(_d), opacity(_a) {}
};

template<typename T>
struct Compare
{
	bool operator()(const T& t1, const T& t2)const
	{
		return t1.data < t2.data;
	}
};

template<class T>
class MapList
{
public:
	MapList(T _max_number) :max_number(_max_number)
	{
		reset_c();
		reset_a();
	}

	void reset_c()
	{
        listC.insert(MapNodeC<T>(0, glm::vec3(0, 0, 1)));
        white = listC.insert(MapNodeC<T>(max_number / 2, glm::vec3(1, 1, 1))).first;
        listC.insert(MapNodeC<T>(max_number, glm::vec3(1, 0, 0)));
	}
	void reset_a()
	{
		listA.insert(MapNodeA<T>(0, 0));
		listA.insert(MapNodeA<T>(max_number, 1.0));
	}
	
	typename std::set<MapNodeC<T>, Compare<MapNodeC<T>>>::iterator set_white(T data)
	{
        white = listC.insert(MapNodeC<T>(data, glm::vec3(1, 1, 1))).first;
		return white;
	}

	bool it_is_white(typename std::set<MapNodeC<T>, Compare<MapNodeC<T>>>::iterator it)
	{
		return white == it;
	}
	
	bool it_is_white(const MapNodeC<T>& mc)
	{
		auto it=listC.find(mc);
		return white == it;
	}
	
	typename std::set<MapNodeC<T>, Compare<MapNodeC<T>>>::iterator get_white()const {
		return white;
	}
	
	typename std::set<MapNodeC<T>, Compare<MapNodeC<T>>>::iterator end_c()const {
		return listC.end();
	}

	typename std::set<MapNodeA<T>, Compare<MapNodeA<T>>>::iterator end_a()const {
		return listA.end();
	}
	
	typename std::set<MapNodeC<T>, Compare<MapNodeC<T>>>::iterator begin_c()const {
		return listC.begin();
	}

	typename std::set<MapNodeA<T>, Compare<MapNodeA<T>>>::iterator begin_a()const {
		return listA.begin();
	}

	
	void clear_c()
	{
		listC.clear();
	}
	
	void clear_a()
	{
		listA.clear();
	}

	typename std::set<MapNodeC<T>, Compare<MapNodeC<T>>>::iterator insert_color_map(const MapNodeC<T>& mc){
		return listC.insert(mc).first;
	}
	
	typename std::set<MapNodeA<T>, Compare<MapNodeA<T>>>::iterator insert_opacity_map(const MapNodeA<T>& ma) {
		return listA.insert(ma).first;
	}


	void erase_color_map(typename std::set<MapNodeC<T>, Compare<MapNodeC<T>>>::iterator it)
	{
		listC.erase(it);
	}

	void erase_opacity_map(typename std::set<MapNodeA<T>, Compare<MapNodeA<T>>>::iterator it)
	{
		listA.erase(it);
	}
	
    void insert_color_map(const T& data, const glm::vec3& color) {
		listC.insert(MapNodeC<T>(data, color));
	}
	void insert_opacity_map(const T& data, const float& opacity) {
		listA.insert(MapNodeA<T>(data, opacity));
	}
	
    glm::vec3 get_color(const T& data) const {
        glm::vec3 color;
		for (auto it = listC.begin(); it != listC.end(); it++)
		{
			if (it->data <= data)
			{
				color = it->color;
				T forward = it->data, back = (++it)->data;
				double rate = (double(data) - forward) / (back - forward);
				color = color * rate + (1 - rate) * it->color;
				break;
			}
		}
		return color;
	}
	
	typename std::set<MapNodeA<T>, Compare<MapNodeA<T>>>::iterator get_a_begin() const
	{
		return listA.begin();
	}
	typename std::set<MapNodeA<T>, Compare<MapNodeA<T>>>::iterator get_a_end() const
	{
		return listA.end();
	}
	
	typename std::set<MapNodeC<T>, Compare<MapNodeC<T>>>::iterator get_c_begin() const
	{
		return listC.begin();
	}
	typename std::set<MapNodeC<T>, Compare<MapNodeC<T>>>::iterator get_c_end() const
	{
		return listC.end();
	}
	
	int get_A_size() const
	{
		return listA.size();
	}

	int get_C_size() const
	{
		return listC.size();
	}
	
	float get_opacity(const T& data) const {
		float opacity = 0;
		for (auto it = listA.begin(); it != listA.end(); it++)
		{
			if (it->data <= data)
			{
				opacity = it->opacity;
				T forward = it->data, back = (++it)->data;
				float rate = (float(data) - forward) / (back - forward);
				opacity = opacity * rate + (1 - rate) * it->opacity;
				break;
			}
		}
		return opacity;
	}
    std::pair<const glm::vec3, double> get_color_and_opacity(const T& data) const {
        glm::vec3 color;
		for (auto it = listC.begin(); it != listC.end(); it++)
		{
			if (it->data <= data)
			{
				color = it->color;
				T forward = it->data, back = (++it)->data;
				double rate = (double(data) - forward) / (back - forward);
				color = color * rate + (1 - rate) * it->color;
				break;
			}
		}
		float opacity = 0;
		for (auto it = listA.begin(); it != listA.end(); it++)
		{
			if (it->data <= data)
			{
				opacity = it->opacity;
				T forward = it->data, back = (++it)->data;
				float rate = (float(data) - forward) / (back - forward);
				opacity = opacity * rate + (1 - rate) * it->opacity;
				break;
			}
		}
        return std::pair<const glm::vec3, float>(color, opacity);
	}
private:
	T max_number;
	std::set<MapNodeC<T>, Compare<MapNodeC<T>>> listC;
	std::set<MapNodeA<T>, Compare<MapNodeA<T>>> listA;
	typename std::set<MapNodeC<T>, Compare<MapNodeC<T>>>::iterator white;
};

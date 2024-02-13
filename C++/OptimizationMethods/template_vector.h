#pragma once
using namespace std;
#include <functional>
#include <algorithm>
#include <iostream>
#include <string>
#include <iostream>
#include <iterator>
#include <numeric>
#include "vector_itaretors.h"


template <typename T>
class template_vector_
{
private:
	T* alloc(const int& cap)
	{
		return (T*)(malloc(cap * sizeof(T)));
	}
	void dealloc() 
	{
		if (m_data) free(m_data);
	}
	void upscale()
	{
		if (filling() != capacity())return;
		resize(int(m_capacity * 1.5));
	}
	T*  m_data;
	uint32_t m_capacity;
	uint32_t m_filling;
	bool     m_is_silce = false;
public:
	static T reduce(const template_vector_<T>& vector, std::function<T(const T&, const T&)> reduce_f, T init_value = T{ 0 });

	static bool all(const template_vector_<T>& vector, std::function<bool(const T&)> condition_f);
	
	static bool any(const template_vector_<T>& vector, std::function<bool(const T&)> condition_f);

	static  template_vector_<T> filter(const template_vector_<T>& vector, std::function<bool(const T&)> condition_f);

	template<typename T1>
	static map_vector_values<T, T1>map(const template_vector_<T>& left, std::function<T1(const T&)> map_f);

	template<typename T1>
	static zip_vector_values<T, T1>zip(const template_vector_<T>& left, const template_vector_<T1>& right);

	template<typename T1>
	static zip_vector_values<T, T1>zip(const template_vector_<T>& left, const T1& right);

	template<typename T1>
	static zip_vector_values<T, T1>zip(const T& left, const template_vector_<T1>& right);

	template<typename T1>
	static combine_vector_values<T, T1>combine(const template_vector_<T>& left, const template_vector_<T>& right, std::function<T1(const T&, const T&)> combine_f);

	template<typename T1>
	static combine_vector_values<T, T1>combine(const T& left, const template_vector_<T>& right, std::function<T1(const T&, const T&)> combine_f);
	
	template<typename T1>
	static combine_vector_values<T, T1>combine(const template_vector_<T>& left, const T& right, std::function<T1(const T&, const T&)> combine_f);

	T reduce(std::function<T(const T&, const T&)> reduce_f, T init_value = T{ 0 })const
	{
		vector_values<T> values = (*this).values();
		return template_vector_::reduce(values, reduce_f, init_value);
	}

	bool all(std::function<bool(const T&)> condition_f) 
	{
		return template_vector_::all((*this), condition_f);
	}

	bool any(std::function<bool(const T&)> condition_f)
	{
		return template_vector_::any((*this), condition_f);
	}

	void fill(std::function<T(const int&)>  fill_f) {
		for (int index : indices()) m_data[index] = fill_f(index);
	}

	void apply(const template_vector_<T>& vector, std::function<T(const T&)>  apply_f) {
		int index = 0;
		for(const T& value: vector.values())
		{
			if (index == filling()) break;
			unchecked_access(index++) = apply_f(value);
		}
	}

	void apply(std::function<T(const T&)>  apply_f) 
	{
		apply((*this), apply_f);
	}

	void applyEnumerate(const template_vector_<T>& vector, std::function<T(const int, const T&)>  apply_f) {
		int index = 0;
		for (const T& value : vector.values())
		{
			if (index == filling()) break;
			unchecked_access(index) = apply_f(index, value);
			index++;
		}
	}

	void applyEnumerate(std::function<T(const int, const T&)>  apply_f)
	{
		applyEnumerate((*this), apply_f);
	}

	template_vector_<T> filter(std::function<bool(const T&)> condition_f)
	{
		return template_vector_::filter((*this), condition_f);
	}

	vector_indices indices() const{
		return vector_indices(0, this->filling(), 1);
	};

	vector_values<T> values() const{
		return vector_values<T>(m_data, indices());
	};

	template<typename T1>
	map_vector_values<T, T1> map(std::function<T1(const T&)> map_f) const {
		return template_vector_::map((*this), map_f);
	};

	template<typename T1>
	zip_vector_values<T, T1> zip(const template_vector_<T1>& other) const {
		return template_vector_::zip((*this), other);
	};

	template<typename T1>
	combine_vector_values<T, T1> combine(const T& right, std::function<T1(const T&, const T&)> combine_f) const {
		return template_vector_::combine((*this), right, combine_f);
	};

	template<typename T1>
	combine_vector_values<T, T1> combine(const template_vector_<T>& right, std::function<T1(const T&, const T&)> combine_f) const {
		return template_vector_::combine((*this), right, combine_f);
	};

	uint32_t capacity()const { return m_capacity; }

	uint32_t filling()const { return m_filling; }

	bool is_empty()const { return filling() == 0; }

	bool is_slice()const { return m_is_silce; }

	const T& unchecked_access(const uint32_t& index) const { return m_data[index]; }

	T& unchecked_access(const uint32_t& index) { return m_data[index]; }

	bool in_range(const int& index)const { return (index >= 0) && (index < filling()); }

	void resize(const uint32_t& new_size)
	{
		if (capacity() == new_size) return;
		m_capacity = new_size;
		m_filling = capacity() > filling() ? filling() : capacity();
		T* _new_values = alloc(new_size);
		memcpy(_new_values, m_data, filling() * sizeof(T));
		dealloc();
		m_data = std::move(_new_values);
	}

	template_vector_<T>& clear() 
	{
		m_filling = 0; 
		return (*this);
	}

	int32_t index_of(const T& value)const
	{
		return -1;
		// upscale();
		// m_data[m_filling++] = value;
		// return (*this);
	}

	bool contans(const T& value)const
	{
		return index_of(value) != -1;
	}

	template_vector_<T>& remove(const T& value)
	{
		const int index = index_of(value);
		if (index == -1) return (*this);
		return remove_at(index);
	}

	template_vector_<T>& push_back(const T& value)
	{
		upscale();
		m_data[m_filling++] = value;
		return (*this);
	}

	template_vector_<T>& push_back(T&& value)
	{
		// ????????????
		upscale();
		m_data[m_filling++] = value;
		return (*this);
	}
	
	template_vector_<T>& remove_at(const int32_t& index) 
	{
		if (!in_range(index))return (*this);
		memcpy(m_data, index + 1, m_data, index, (filling() - index - 1) * sizeof(T));
		m_filling--;
		return (*this);
	}
	
	template_vector_<T>& insert(const int32_t& index, const T& value)
	{
		if (index < 0) return (*this);
		if (index > filling()) return push_back(value);
		upscale();
		memccpy(m_data, index, m_data, index + 1, (filling() - index) * sizeof(T));
		m_data[index] = value;
		m_filling++;
		return this;
	}
	
	template_vector_<T>const& operator&()const { return (*this); }
	
	template_vector_<T>const* operator*()const { return (&this); }
	
	template_vector_<T>& operator&(){ return (*this); }
	
	template_vector_<T>* operator*(){ return (&this); }
	
	template_vector_<T>& operator=(template_vector_<T>const& other)
	{
		if (this == &other)
			return *this;

		if (capacity() < other.filling())           // resource in *this cannot be reused
		{
			dealloc();                                      // release resource in *this
			m_data = alloc(other.capacity());
		}
		m_capacity = other.capacity();
		m_filling = other.filling();
		memcpy(m_data, other.m_data, filling() * sizeof(T));
		return (*this);
	}
	
	template_vector_<T>& operator=(template_vector_<T>&& other)
	{
		if (this == &other)
			return *this;
		dealloc();
		m_data     = std::exchange(other.m_data, nullptr); // leave other in valid state
		m_filling  = std::exchange(other.m_filling, 0);
		m_capacity = std::exchange(other.m_capacity, 0);
		return (*this);
	}
	
	T& operator[](const uint32_t& index)
	{
		if (!in_range(index))
			throw new std::runtime_error(
				"Vector_ :: trying to access index: {" +
				std::to_string(index) + "}, while cap is {" +
				std::to_string(capacity()) + "}");
		return unchecked_access(index);
	}
	
	const T& operator[](const uint32_t& index)const
	{
		if (!in_range(index))
			throw new std::runtime_error(
				"Vector_ :: trying to access index: {" +
				std::to_string(index) + "}, while cap is {" +
				std::to_string(capacity()) + "}");
		return unchecked_access(index);
	}
	
	template_vector_(const template_vector_& other)
	{
		m_data = alloc(other.capacity());
		m_capacity = other.capacity();
		m_filling = other.filling();
		if (other.is_slice())
		{
			apply(other, [](const T& v) {return v; });
			return;
		}
		memcpy(m_data, other.m_data, other.filling() * sizeof(T));
	};
	
	template_vector_(template_vector_&& other)
	{
		// free(m_data);
		m_data = std::exchange(other.m_data, nullptr); // leave other in valid state
		m_filling = std::exchange(other.m_filling, 0);
		m_capacity = std::exchange(other.m_capacity, 0);
	};
	
	template_vector_(const int& cap )
	{
		m_filling = cap;
		m_data = alloc((int)(cap * 1.5));
	};

	template_vector_()
	{
		m_filling = 0;
		m_data = alloc(32);
	};

	template_vector_(const vector_values<T>& values):template_vector_()
	{
		for (const T& value : values) push_back(value);
	};

	template_vector_(const map_vector_values<T, T>& values):template_vector_()
	{
		for (const T& value : values) push_back(value);
	};

	template_vector_(const combine_vector_values<T, T>& values) :template_vector_()
	{
		for (const T& value : values) push_back(value);
	};
	
	~template_vector_() 
	{
		if (is_slice())return;
		dealloc();
	}
	template <typename T>
	friend std::ostream& operator<<(std::ostream& steram, const template_vector_<T>& pair);
};

template <typename T>
inline std::ostream& operator<<(std::ostream& steram, const template_vector_<T>& vector)
{
	steram << "{ ";
	for (const int& index: vector.indices())steram << vector[index] << (index != vector.filling() - 1 ? ", " : "");
	steram << " }";
	return steram;
}


template<typename T>
inline T template_vector_<T>::reduce(const template_vector_<T>& vector, std::function<T(const T&, const T&)> reduce_f, T init_value)
{
	for (const T& value: vector.values()) init_value = reduce_f(init_value, value);
	return init_value;
}

template<typename T>
static bool template_vector_<T>::all(const template_vector_<T>& vector, std::function<bool(const T&)> condition_f)
{
	for (const T& item : vector) if (!condition_f(item)) return false;
	return true;
}

template<typename T>
static bool template_vector_<T>::any(const template_vector_<T>& vector, std::function<bool(const T&)> condition_f)
{
	for (const T& item : vector) if (condition_f(item)) return true;
	return false;
}

template<typename T>
static  template_vector_<T> filter(const template_vector_<T>& vector, std::function<bool(const T&)> condition_f)
{	
	template_vector_<T> filtered_vector;
	for (const T& item : vector) if (condition_f(item)) filtered_vector.push_back(item);
	return filtered_vector;
}


template<typename T>
template<typename T1>
inline zip_vector_values<T, T1> template_vector_<T>::zip(const template_vector_<T>& left, const template_vector_<T1>& right)
{
	return zip_vector_values<T, T1>(left.values(), right.values());
}

template<typename T>
template<typename T1>
inline zip_vector_values<T, T1> template_vector_<T>::zip(const T& left, const template_vector_<T1>& right)
{
	return zip_vector_values<T, T1>(left, right.values());
}

template<typename T>
template<typename T1>
inline zip_vector_values<T, T1> template_vector_<T>::zip(const template_vector_<T>& left, const T1& right)
{
	return zip_vector_values<T, T1>(left.values(), right);
}

template<typename T>
template<typename T1>
inline map_vector_values<T, T1> template_vector_<T>::map(const template_vector_<T>& left, std::function<T1(const T&)> map_f)
{
	return map_vector_values<T, T1>(left.values(), map_f);
}

template<typename T>
template<typename T1>
combine_vector_values<T, T1>template_vector_<T>::combine(const template_vector_<T>& left, 
														 const template_vector_<T>& right,
												         std::function<T1(const T&, const T&)> combine_f)
{
	return combine_vector_values<T, T1>(zip(left, right), combine_f);
}

template<typename T>
template<typename T1>
combine_vector_values<T, T1>template_vector_<T>::combine(const T& left, const template_vector_<T>& right,
												         std::function<T1(const T&, const T&)> combine_f)
{
	return combine_vector_values<T, T1>(zip(left, right), combine_f);
}

template<typename T>
template<typename T1>
combine_vector_values<T, T1>template_vector_<T>::combine(const template_vector_<T>& left, 
														 const T& right, std::function<T1(const T&, const T&)> combine_f)
{
	return combine_vector_values<T, T1>(zip(left, right), combine_f);
}

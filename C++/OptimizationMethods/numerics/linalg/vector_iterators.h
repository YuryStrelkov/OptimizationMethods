#pragma once
#ifndef __TEMPLATE_ITERATOR_H__
#define __TEMPLATE_ITERATOR_H__
#include "../common.h"
NUMERICS_NAMESPACE_BEGIN
/// template <typename Type>
/// struct iterator_ : public std::iterator<
/// 	std::input_iterator_tag, // iterator_category
/// 	Type,                    // value_type
/// 	Type,                    // difference_type
/// 	const Type*,             // pointer
/// 	const Type&>             // reference
/// {
/// 	// public:
/// 	//		bool             operator!=(const iterator_<Type>& other) const { return !(*this == other); }
/// 	//		bool             operator==(const iterator_<Type>& other) const { return  false; }
/// 	//		iterator_<Type>  operator++(int)                                { iterator_<Type> retval = *this; ++(*this); return retval; }
/// 	//		iterator_<Type>& operator++()                                   { return *this; }
/// 	//		const Type*      operator* ()                             const { return nullptr; }
/// 	//		const Type&      operator& ()                             const { return 0; }
/// };

template <typename T> struct iterable_
{
public:
	virtual T begin()const = 0;
	virtual T end()  const = 0;
};

//////////////////////////////////////
///  Iterator over indices range   ///
//////////////////////////////////////
class indices_iterator //  : public iterator_<I32>
{
private:
	I32 m_index;
	I32 m_step;
public:
	indices_iterator(const I32 index, const I32 step)
	{
		m_index = index;
		m_step = step;
	}
	bool operator==(const indices_iterator& other) const { return m_index == other.m_index; }
	bool operator!=(const indices_iterator& other) const { return !(*this == other); }
	indices_iterator& operator++() { m_index += m_step; return *this; }
	indices_iterator  operator++(int) { indices_iterator& retval = *this; ++(*this); return retval; }
	I32 operator*() const { return m_index; }
	const I32* operator&() const { return &m_index; }
};

struct vector_indices : iterable_<indices_iterator>
{
private:
	const I32 m_begin;
	const I32 m_end;
	const I32 m_step;
public:
	vector_indices(const I32 start, const I32 stop, const I32 step) :m_begin(start), m_end(stop), m_step(step) {}
	vector_indices(const I32 start, const I32 stop) :m_begin(start), m_end(stop), m_step(1) {}
	vector_indices(const I32 range) :m_begin(0), m_end(range), m_step(1) {}
	indices_iterator begin() const override { return indices_iterator(m_begin, m_step); }
	indices_iterator end() const override { return indices_iterator(m_end, m_step); }
};

//////////////////////////////////////
///  Iterator over vector values   ///
//////////////////////////////////////
template<typename T>
struct values_iterator // : public iterator_<T>
{
private:
	const T* m_data;
	indices_iterator m_index;
public:
	values_iterator<T>(const T* data, indices_iterator&& indices) : m_data(data), m_index(std::move(indices)) {}
	values_iterator<T>(const T value) : m_data(&value), m_index(vector_indices(0, 0, 0)) {}
	bool               operator==(const values_iterator<T>& other) const { return m_index == other.m_index; }
	bool               operator!=(const values_iterator<T>& other) const { return !(*this == other); }
	values_iterator<T>& operator++() { m_index++; return *this; }
	values_iterator<T> operator++(int) { values_iterator retval = *this; ++(*this); return retval; }
	const T& operator* ()const { return m_data[*m_index]; }
	const T* operator& ()const { return m_data[*m_index]; }
	// T& operator* (){ return m_data[*m_index]; }
	// T* operator& (){ return m_data[*m_index]; }
};

template <typename T>
struct vector_values : public iterable_<values_iterator<T>>
{
private:
	const T* m_data;
	const vector_indices m_indices;
public:
	vector_values(const T* data, vector_indices&& indices) : m_data(data), m_indices(indices) {}
	vector_values(const T& value) : m_data(&value), m_indices(vector_indices(0, 1, 0)) {}
	values_iterator<T> begin() const override { return values_iterator<T>(m_data, m_indices.begin()); }
	values_iterator<T> end()   const override { return values_iterator<T>(m_data, m_indices.end()); }
};

/////////////////////////////////////////////
///  Iterator over mapped vector values   ///
/////////////////////////////////////////////
template<typename T1, typename T2>
struct map_values_iterator //  : public  iterator_<T2>
{
private:
	values_iterator<T1> m_values;
	const std::function<T2(const T1&)> m_map_function;
public:
	map_values_iterator<T1, T2>(values_iterator<T1>&& values, std::function<T2(const T1&)> map_function) : m_map_function(map_function), m_values(values) {}
	bool                         operator==(const map_values_iterator<T1, T2>& other) const { return m_values == other.m_values; }
	bool                         operator!=(const map_values_iterator<T1, T2>& other) const { return !(*this == other); }
	map_values_iterator<T1, T2>& operator++() { m_values++; return *this; }
	map_values_iterator<T1, T2>  operator++(int) { map_values_iterator retval = *this; ++(*this); return retval; }
	const T2 operator* () const { return m_map_function(*m_values); }
	const T2 operator& () const { return m_map_function(*m_values); }

	// const T2&                    operator* ()                                         const {return m_map_function(*m_values);}
	// const T2*                    operator& ()                                         const {return m_map_function(*m_values);}
};

template<typename T1, typename T2>
struct map_values : public iterable_<map_values_iterator<T1, T2>>
{
private:
	const std::function<T2(const T1&)> m_map_function;
	const vector_values<T1> m_values;
public:
	map_values<T1, T2>(vector_values<T1>&& values, std::function<T2(const T1&)> map_function) : m_values(values), m_map_function(map_function) {}
	map_values_iterator<T1, T2> begin() const override { return map_values_iterator<T1, T2>(m_values.begin(), m_map_function); }
	map_values_iterator<T1, T2> end()   const override { return map_values_iterator<T1, T2>(m_values.end(), m_map_function); }
};


///////////////////////////////////////////////
///  Iterator over values of vectors pair   ///
///////////////////////////////////////////////
template <typename T1, typename T2>
struct pair_
{
public:
	const T1& first;
	const T2& second;
	pair_<T1, T2>(const T1& first, const T2& second) : first(first), second(second) {}
	template <typename T_1, typename T_2>
	friend std::ostream& operator<<(std::ostream& steram, const pair_<T_1, T_2>& pair);
};

template <typename T_1, typename T_2>
inline std::ostream& operator<<(std::ostream& steram, const pair_<T_1, T_2>& pair)
{
	// TODO: âñòàâüòå çäåñü îïåðàòîð return
	steram << "{ \"first\" : " << pair.first << ", \"second\": " << pair.second << " }";
	return steram;
}

template<typename T1, typename T2>
struct zip_values_iterator ///  : public iterator_<pair_<T1, T2>>
{
private:
	values_iterator<T1> m_iterable1;
	values_iterator<T2> m_iterable2;
public:
	zip_values_iterator<T1, T2>(values_iterator<T1>&& values1, values_iterator<T2>&& values2) : m_iterable1(values1), m_iterable2(values2) {}
	bool                         operator==(const zip_values_iterator<T1, T2>& other) const { return (m_iterable1 == other.m_iterable1) || (m_iterable2 == other.m_iterable2); }
	bool                         operator!=(const zip_values_iterator<T1, T2>& other) const { return !(*this == other); }
	zip_values_iterator<T1, T2>& operator++() { m_iterable1++; m_iterable2++; return *this; }
	zip_values_iterator<T1, T2>  operator++(int) { zip_values_iterator retval = *this; ++(*this); return retval; }
	const pair_<T1, T2>          operator*() const { return pair_<T1, T2>(*m_iterable1, *m_iterable2); }
	const pair_<T1, T2>          operator&() const { return pair_<T1, T2>(*m_iterable1, *m_iterable2); }
};

template<typename T1, typename T2>
struct zip_values : public iterable_<zip_values_iterator<T1, T2>>
{
private:
	const vector_values<T1>m_values1;
	const vector_values<T2>m_values2;
public:
	zip_values<T1, T2>(vector_values<T1>&& values1, vector_values<T2>&& values2) : m_values1(values1), m_values2(values2) {}
	zip_values<T1, T2>(const T1& value, vector_values<T2>&& values2) : m_values1(vector_values<T1>(value)), m_values2(values2) {}
	zip_values<T1, T2>(vector_values<T1>&& values1, const T2& value) : m_values1(values1), m_values2(vector_values<T2>(value)) {}
	zip_values_iterator<T1, T2> begin() const override { return zip_values_iterator<T1, T2>(m_values1.begin(), m_values2.begin()); }
	zip_values_iterator<T1, T2> end()   const override { return zip_values_iterator<T1, T2>(m_values1.end(), m_values2.end()); }
};

///////////////////////////////////////////////
///            Combine iterator             ///
///////////////////////////////////////////////

template<typename T1, typename T2>
struct combine_values_iterator// : public iterator_<T2>
{
private:
	zip_values_iterator<T1, T1> m_iterables;
	const std::function<T2(const T1&, const T1&)> m_combine_function;
public:
	combine_values_iterator<T1, T2>(zip_values_iterator<T1, T1>&& zip_values,
		std::function<T2(const T1&, const T1&)> combine_f) : m_iterables(zip_values), m_combine_function(combine_f) {}
	bool                             operator==(const combine_values_iterator<T1, T2>& other) const { return m_iterables == other.m_iterables; }
	bool                             operator!=(const combine_values_iterator<T1, T2>& other) const { return !(*this == other); }
	combine_values_iterator<T1, T2>& operator++() { m_iterables++; return *this; }
	combine_values_iterator<T1, T2>  operator++(int) { combine_values_iterator retval = *this; ++(*this); return retval; }
	const T2                         operator*() const { return m_combine_function((*m_iterables).first, (*m_iterables).second); }
	const T2                         operator&() const { return m_combine_function((*m_iterables).first, (*m_iterables).second); }
};

template<typename T1, typename T2>
struct combine_values : public iterable_<combine_values_iterator<T1, T2>>
{
private:
	const zip_values<T1, T1> m_iterables;
	const std::function<T2(const T1&, const T1&)> m_combine_function;
public:
	combine_values<T1, T2>(zip_values<T1, T1>&& zip_vals, std::function<T2(const T1&, const T1&)> combine_f) :
		m_iterables(zip_vals), m_combine_function(combine_f) {
	}

	combine_values<T1, T2>(vector_values<T1>&& values1,
		vector_values<T1>&& values2,
		std::function<T2(const T1&, const T1&)> combine_f) :
		m_iterables(zip_values<T1, T1>(std::move(values1), std::move(values2))), m_combine_function(combine_f) {
	}
	combine_values<T1, T2>(const T1& value, vector_values<T1>&& values2, std::function<T2(const T1&, const T1&)> combine_f) : m_iterables(zip_values<T1, T1>(value, std::move(values2))), m_combine_function(combine_f) {}
	combine_values<T1, T2>(vector_values<T1>&& values1, const T1& value, std::function<T2(const T1&, const T1&)> combine_f) : m_iterables(zip_values<T1, T1>(std::move(values1), value)), m_combine_function(combine_f) {}
	combine_values_iterator<T1, T2> begin() const override { return combine_values_iterator<T1, T2>(m_iterables.begin(), m_combine_function); }
	combine_values_iterator<T1, T2> end()   const override { return combine_values_iterator<T1, T2>(m_iterables.end(), m_combine_function); }
};
NUMERICS_NAMESPACE_END
#endif
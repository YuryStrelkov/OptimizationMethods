#pragma once
#include "common.h"

template <typename Type>
struct iterator_ : public std::iterator<
	std::input_iterator_tag, // iterator_category
	Type,                    // value_type
	Type,                    // difference_type
	const Type*,             // pointer
	const Type&>             // reference
{
// public:
// 	bool             operator!=(const iterator_<Type>& other) const { return !(*this == other); }
// 	bool             operator==(const iterator_<Type>& other) const { return  false; }
// 	iterator_<Type>  operator++(int)                                { iterator_<Type> retval = *this; ++(*this); return retval; }
// 	iterator_<Type>& operator++()                                   { return *this; }
// 	const Type*      operator* ()                             const { return nullptr; }
// 	const Type&      operator& ()                             const { return 0; }
};

template <typename T> struct iterable_
{
public:
	virtual T begin()const  = 0;
	virtual T end()  const  = 0;
};

//////////////////////////////////////
///  Iterator over indices range   ///
//////////////////////////////////////
class indices_iterator: public iterator_<int32_t>
{
private:
	int32_t _index;
	int32_t _step;
public:
	indices_iterator(const int32_t& index, const int32_t& step)
	{
		_index = index;
		_step = step;
	}
	// ”наследовано через iterator_
	bool              operator==(const indices_iterator& other) const /*override*/ { return _index == other._index; }
    bool              operator!=(const indices_iterator& other) const /*override*/ { return !(*this == other); }
	indices_iterator& operator++()                                    /*override*/ { _index += _step; return *this; }
	indices_iterator  operator++(int)                                 /*override*/ { indices_iterator& retval = *this; ++(*this); return retval; }
	reference         operator*()                               const /*override*/ { return _index; }
	pointer           operator&()                               const /*override*/ { return &_index; }
};

struct vector_indices: iterable_<indices_iterator>
{
private:
	const int32_t _begin;
	const int32_t _end;
	const int32_t _step;
public:
	vector_indices(const int32_t& start, const int32_t& stop, const int32_t& step) :_begin(start), _end(stop), _step(step) {}
	vector_indices(const int32_t& start, const int32_t& stop) :_begin(start), _end(stop), _step(1) {}
	vector_indices(const int32_t& range) :_begin(0), _end(range), _step(1) {}

	// ”наследовано через iterable_
	indices_iterator begin() const override { return indices_iterator(_begin, _step); }
	indices_iterator end() const override { return indices_iterator(_end, _step); }
};

//////////////////////////////////////
///  Iterator over vector values   ///
//////////////////////////////////////
template<typename T>
struct values_iterator: public iterator_<T>
{
private:
	const T* data;
	indices_iterator index;
public:
	values_iterator<T>(const T* data, indices_iterator&& indices) : data(data), index(indices) {}
	values_iterator<T>(const T value): data(&value), index(vector_indices(0, 0, 0)){}
	bool               operator==(const values_iterator<T>& other) const { return index == other.index; }
	bool               operator!=(const values_iterator<T>& other) const { return !(*this == other); }
	values_iterator<T>&operator++()                                      { index++; return *this; }
	values_iterator<T> operator++(int)                                   { values_iterator retval = *this; ++(*this); return retval; }
	const T&           operator* ()                                const { return data[*index]; }
	const T*           operator& ()                                const { return data[*index]; }
};

template <typename T>
struct vector_values: public iterable_<values_iterator<T>>
{
private:
	const T* _data;
	const vector_indices _indices;
public:
	vector_values(const T* data, vector_indices&& indices) : _data(data), _indices(indices) {}
	vector_values(const T& value) : _data(&value), _indices(vector_indices(0, 1, 0)) {}
	values_iterator<T> begin() const override { return values_iterator<T>(_data, _indices.begin()); }
	values_iterator<T> end()   const override { return values_iterator<T>(_data, _indices.end()); }
};

/////////////////////////////////////////////
///  Iterator over mapped vector values   ///
/////////////////////////////////////////////
template<typename T1, typename T2>
struct map_values_iterator : public  iterator_<T2>
{
private:
	values_iterator<T1> values;
	const std::function<T2(const T1&)> map_function;
public:
	map_values_iterator<T1, T2>(values_iterator<T1>&& values, std::function<T2(const T1&)> map) : map_function(map), values(values) {}
	bool                         operator==(const map_values_iterator<T1, T2>& other) const { return values == other.values; }
	bool                         operator!=(const map_values_iterator<T1, T2>& other) const { return !(*this == other); }
	map_values_iterator<T1, T2>& operator++()                                               { values++; return *this; }
	map_values_iterator<T1, T2>  operator++(int)                                            { map_values_iterator retval = *this; ++(*this); return retval; }
	const T2&                    operator* ()                                         const {return map_function(*values);}
	const T2*                    operator& ()                                         const {return map_function(*values);}
};

template<typename T1, typename T2>
struct map_values: public iterable_<map_values_iterator<T1, T2>>
{
private:
	const std::function<T2(const T1&)> map_f;
	const vector_values<T1> values;
public:
	map_values<T1, T2>(vector_values<T1>&& values, std::function<T2(const T1&)> map_f) : values(values), map_f(map_f) {}
	map_values_iterator<T1, T2> begin() const override { return map_values_iterator<T1, T2>(values.begin(), map_f); }
	map_values_iterator<T1, T2> end()   const override { return map_values_iterator<T1, T2>(values.end()  , map_f); }
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
	template <typename T1, typename T2>
	friend std::ostream& operator<<(std::ostream& steram, const pair_<T1, T2>& pair);
};

template<typename T1, typename T2>
inline std::ostream& operator<<(std::ostream& steram, const pair_<T1, T2>& pair)
{
	// TODO: вставьте здесь оператор return
	steram << "{ \"first\" : " << pair.first << ", \"second\": " << pair.second << " }";
	return steram;
}

template<typename T1, typename T2>
struct zip_values_iterator : public iterator_<pair_<T1, T2>>
{
private:
	values_iterator<T1> iterable1;
	values_iterator<T2> iterable2;
public:
	zip_values_iterator<T1, T2>(values_iterator<T1>&&      values1, values_iterator<T2>&&      values2) : iterable1(values1), iterable2(values2) {}
	bool                         operator==(const zip_values_iterator<T1, T2>& other) const { return (iterable1 == other.iterable1) || (iterable2 == other.iterable2);}
	bool                         operator!=(const zip_values_iterator<T1, T2>& other) const { return !(*this == other); }
	zip_values_iterator<T1, T2>& operator++()                                               { iterable1++; iterable2++; return *this; }
	zip_values_iterator<T1, T2>  operator++(int)                                            { zip_values_iterator retval = *this; ++(*this); return retval; }
	const pair_<T1, T2>&         operator*()                                          const { return pair_<T1, T2>(*iterable1, *iterable2); }
	const pair_<T1, T2>*         operator&()                                          const { return pair_<T1, T2>(*iterable1, *iterable2); }
};

template<typename T1, typename T2>
struct zip_values : public iterable_<zip_values_iterator<T1, T2>>
{
private:
	const vector_values<T1>values1;
	const vector_values<T2>values2;
public:
	zip_values<T1, T2>(vector_values<T1>&& values1, vector_values<T2>&& values2) : values1(values1),                  values2(values2) {}
	zip_values<T1, T2>(const T1&           value,   vector_values<T2>&& values2) : values1(vector_values<T1>(value)), values2(values2) {}
	zip_values<T1, T2>(vector_values<T1>&& values1, const T2&           value  ) : values1(values1),                  values2(vector_values<T2>(value)) {}
	zip_values_iterator<T1, T2> begin() const override { return zip_values_iterator<T1, T2>(values1.begin(), values2.begin()); }
	zip_values_iterator<T1, T2> end()   const override { return zip_values_iterator<T1, T2>(values1.end(),   values2.end()); }
};

///////////////////////////////////////////////
///            Combine iterator             ///
///////////////////////////////////////////////

template<typename T1, typename T2>
struct combine_values_iterator : public iterator_<T2>
{
private:
	zip_values_iterator<T1, T1> iterables;
	const std::function<T2(const T1&, const T1&)> combine_f;
public:
	combine_values_iterator<T1, T2>(zip_values_iterator<T1, T1>&& zip_values,
									std::function<T2(const T1&, const T1&)> combine_f) : iterables(zip_values), combine_f(combine_f){}
	bool                             operator==(const combine_values_iterator<T1, T2>& other) const { return iterables == other.iterables;}
	bool                             operator!=(const combine_values_iterator<T1, T2>& other) const { return !(*this == other); }
	combine_values_iterator<T1, T2>& operator++()                                                   { iterables++; return *this; }
	combine_values_iterator<T1, T2>  operator++(int)                                                { combine_values_iterator retval = *this; ++(*this); return retval; }
	const T2&                        operator*() const                                              { return combine_f((*iterables).first, (*iterables).second); }
	const T2*                        operator&() const                                              { return combine_f((*iterables).first, (*iterables).second);}
};

template<typename T1, typename T2>
struct combine_values : public iterable_<combine_values_iterator<T1, T2>>
{
private:
	const zip_values<T1, T1> zip_vals;
	const std::function<T2(const T1&, const T1&)> combine_f;
public:
	combine_values<T1, T2>(zip_values<T1, T1>&& zip_vals, std::function<T2(const T1&, const T1&)> combine_f):
		zip_vals(zip_vals), combine_f(combine_f) {
	}
	
	combine_values<T1, T2>(vector_values<T1>&& values1,
		                   vector_values<T1>&& values2,
						   std::function<T2(const T1&, const T1&)> combine_f):
		zip_vals(zip_values<T1, T1>(std::move(values1), std::move(values2))), combine_f(combine_f){
	}
	combine_values<T1, T2>(const T1&           value,    vector_values<T1>&& values2, std::function<T2(const T1&, const T1&)> combine_f) : zip_vals(zip_values<T1, T1>(value, std::move(values2))), combine_f(combine_f) {}
	combine_values<T1, T2>(vector_values<T1>&& values1,  const T1&           value,   std::function<T2(const T1&, const T1&)> combine_f) : zip_vals(zip_values<T1, T1>(std::move(values1), value)), combine_f(combine_f) {}
	combine_values_iterator<T1, T2> begin() const override { return combine_values_iterator<T1, T2>(zip_vals.begin(), combine_f); }
	combine_values_iterator<T1, T2> end()   const override { return combine_values_iterator<T1, T2>(zip_vals.end(),   combine_f); }
};
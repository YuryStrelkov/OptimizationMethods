#pragma once
#include "template_vector.h"

template<typename T>class      numeric_vector;
typedef numeric_vector<double> double_vector;
typedef numeric_vector<float>  float_vector;
typedef numeric_vector<int>    int_vector;

template <typename T>
T sum_f(const T& lhs, const T& rhs) 
{
	return lhs + rhs;
}
template <typename T>
T dif_f(const T& lhs, const T& rhs)
{
	return lhs - rhs;
}
template <typename T>
T div_f(const T& lhs, const T& rhs)
{
	return lhs / rhs;
}

template <typename T>
T mul_f(const T& lhs, const T& rhs)
{
	return lhs * rhs;
}


template<typename T>
class numeric_vector: public template_vector_<T>
{
	numeric_vector<T>&       normalize ();
	numeric_vector<T>        normalized()const;
	T                        magintude ()const;
	static T                 dot       (const numeric_vector<T>& lhs, const numeric_vector<T>& rhs);
	static numeric_vector<T> direction (const numeric_vector<T>& lhs, const numeric_vector<T>& rhs);
	static numeric_vector<T> gradient  (std::function<T(const numeric_vector<T>&)> func, const numeric_vector<T>& x);
	static T                 partial   (std::function<T(const numeric_vector<T>&)> func, const numeric_vector<T>& x, const int& index);
	static T                 partial2  (std::function<T(const numeric_vector<T>&)> func, const numeric_vector<T>& x, const int& index1, const int& index2);

	numeric_vector<T>& operator+=(const numeric_vector<T>& rhs);
	numeric_vector<T>& operator-=(const numeric_vector<T>& rhs);
	numeric_vector<T>& operator*=(const numeric_vector<T>& rhs);
	numeric_vector<T>& operator/=(const numeric_vector<T>& rhs);

	numeric_vector<T>& operator+=(const T& rhs);
	numeric_vector<T>& operator-=(const T& rhs);
	numeric_vector<T>& operator*=(const T& rhs);
	numeric_vector<T>& operator/=(const T& rhs);

	template<typename T>friend numeric_vector<T> operator+(const numeric_vector<T>& lhs, const numeric_vector<T>& rhs);
	template<typename T>friend numeric_vector<T> operator-(const numeric_vector<T>& lhs, const numeric_vector<T>& rhs);
	template<typename T>friend numeric_vector<T> operator*(const numeric_vector<T>& lhs, const numeric_vector<T>& rhs);
	template<typename T>friend numeric_vector<T> operator/(const numeric_vector<T>& lhs, const numeric_vector<T>& rhs);

	template<typename T>friend numeric_vector<T> operator+(const T& lhs, const numeric_vector<T>& rhs);
	template<typename T>friend numeric_vector<T> operator-(const T& lhs, const numeric_vector<T>& rhs);
	template<typename T>friend numeric_vector<T> operator*(const T& lhs, const numeric_vector<T>& rhs);
	template<typename T>friend numeric_vector<T> operator/(const T& lhs, const numeric_vector<T>& rhs);

	template<typename T>friend numeric_vector<T> operator+(const numeric_vector<T>& lhs, const T& rhs);
	template<typename T>friend numeric_vector<T> operator-(const numeric_vector<T>& lhs, const T& rhs);
	template<typename T>friend numeric_vector<T> operator*(const numeric_vector<T>& lhs, const T& rhs);
	template<typename T>friend numeric_vector<T> operator/(const numeric_vector<T>& lhs, const T& rhs);
};

template<typename T>
inline numeric_vector<T>& numeric_vector<T>::normalize()
{
	// TODO: вставьте здесь оператор return
}

template<typename T>
inline numeric_vector<T> numeric_vector<T>::normalized() const
{
	return numeric_vector<T>(combine_values_iterator(this, T{1.0} / magintude(), [](const T& lhs, const T& rhs) {return lhs * rhs; }));
}

template<typename T>
inline T numeric_vector<T>::magintude() const
{
	return T(std::sqrt(reduce([](const T& accum, const T& value) {return accum + value * value; })));
}

template<typename T>
inline T numeric_vector<T>::dot(const numeric_vector<T>& lhs, const numeric_vector<T>& rhs)
{
	T val = T{ 0 };
	for (auto pair : numeric_vector<T>::zip(lhs, rhs)) val += pair.first * pair.scond;
	return val;
}

template<typename T>
inline numeric_vector<T> numeric_vector<T>::direction(const numeric_vector<T>& lhs, const numeric_vector<T>& rhs)
{
	return numeric_vector<T>(rhs - lhs).normalize();
}

template<typename T>
inline numeric_vector<T> numeric_vector<T>::gradient(std::function<T(const numeric_vector<T>&)> func, const numeric_vector<T>& x)
{
	return numeric_vector<T>();
}

template<typename T>
inline T numeric_vector<T>::partial(std::function<T(const numeric_vector<T>&)> func, const numeric_vector<T>& x, const int& index)
{
	return T();
}

template<typename T>
inline T numeric_vector<T>::partial2(std::function<T(const numeric_vector<T>&)> func, const numeric_vector<T>& x, const int& index1, const int& index2)
{
	return T();
}

template<typename T>
inline numeric_vector<T>& numeric_vector<T>::operator+=(const numeric_vector<T>& rhs)
{
	// TODO: вставьте здесь оператор return
}

template<typename T>
inline numeric_vector<T>& numeric_vector<T>::operator-=(const numeric_vector<T>& rhs)
{
	// TODO: вставьте здесь оператор return
}

template<typename T>
inline numeric_vector<T>& numeric_vector<T>::operator*=(const numeric_vector<T>& rhs)
{
	// TODO: вставьте здесь оператор return
}

template<typename T>
inline numeric_vector<T>& numeric_vector<T>::operator/=(const numeric_vector<T>& rhs)
{
	// TODO: вставьте здесь оператор return
}

template<typename T>
inline numeric_vector<T>& numeric_vector<T>::operator+=(const T& rhs)
{
	apply([](const T& value) {return value + rhs; });
	return (*this)
}

template<typename T>
inline numeric_vector<T>& numeric_vector<T>::operator-=(const T& rhs)
{
	apply([](const T& value) {return value - rhs; });
	return (*this)
}

template<typename T>
inline numeric_vector<T>& numeric_vector<T>::operator*=(const T& rhs)
{
	apply([](const T& value) {return value * rhs; });
	return (*this)
}

template<typename T>
inline numeric_vector<T>& numeric_vector<T>::operator/=(const T& rhs)
{
	return (*this) * (T{ 1.0 } / rhs);
}

template<typename T>
inline numeric_vector<T> operator+(const numeric_vector<T>& lhs, const numeric_vector<T>& rhs)
{
	return numeric_vector<T>(combine_vector_values(lhs, rhs, sum_f));
}

template<typename T>
inline numeric_vector<T> operator-(const numeric_vector<T>& lhs, const numeric_vector<T>& rhs)
{
	return numeric_vector<T>(combine_vector_values(lhs, rhs, dif_f));
}

template<typename T>
inline numeric_vector<T> operator*(const numeric_vector<T>& lhs, const numeric_vector<T>& rhs)
{
	return numeric_vector<T>(combine_vector_values(lhs, rhs, mul_f));
}

template<typename T>
inline numeric_vector<T> operator/(const numeric_vector<T>& lhs, const numeric_vector<T>& rhs)
{
	return numeric_vector<T>(combine_vector_values(lhs, rhs, div_f));
}

template<typename T>
inline numeric_vector<T> operator+(const T& lhs, const numeric_vector<T>& rhs)
{
	return numeric_vector<T>(combine_vector_values(lhs, rhs, sum_f));
}

template<typename T>
inline numeric_vector<T> operator-(const T& lhs, const numeric_vector<T>& rhs)
{
	return numeric_vector<T>(combine_vector_values(lhs, rhs, dif_f));
}

template<typename T>
inline numeric_vector<T> operator*(const T& lhs, const numeric_vector<T>& rhs)
{
	return numeric_vector<T>(combine_vector_values(lhs, rhs, mul_f));
}

template<typename T>
inline numeric_vector<T> operator/(const T& lhs, const numeric_vector<T>& rhs)
{
	return numeric_vector<T>(combine_vector_values(lhs, rhs, div_f));
}

template<typename T>
inline numeric_vector<T> operator+(const numeric_vector<T>& lhs, const T& rhs)
{
	return numeric_vector<T>(combine_vector_values(lhs, rhs, sum_f));
}

template<typename T>
inline numeric_vector<T> operator-(const numeric_vector<T>& lhs, const T& rhs)
{
	return numeric_vector<T>(combine_vector_values(lhs, rhs, dif_f));
}

template<typename T>
inline numeric_vector<T> operator*(const numeric_vector<T>& lhs, const T& rhs)
{
	return numeric_vector<T>(combine_vector_values(lhs, rhs, mul_f));
}

template<typename T>
inline numeric_vector<T> operator/(const numeric_vector<T>& lhs, const T& rhs)
{
	return numeric_vector<T>(combine_vector_values(lhs, rhs, div_f));
}

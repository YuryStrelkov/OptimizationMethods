#pragma once
#include "template_vector.h"

template<typename T>class      numeric_vector;
typedef numeric_vector<double> double_vector;
typedef numeric_vector<float>  float_vector;
typedef numeric_vector<int>    int_vector;

auto sum_f = [](const auto& lhs, const auto& rhs) { return lhs + rhs; };
auto dif_f = [](const auto& lhs, const auto& rhs) { return lhs - rhs; };
auto div_f = [](const auto& lhs, const auto& rhs) { return lhs / rhs; };
auto mul_f = [](const auto& lhs, const auto& rhs) { return lhs * rhs; };

template<typename T>
class numeric_vector: public template_vector_<T>
{
public:
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

	numeric_vector(const int& cap) : template_vector_<T>(cap) {
	};

	numeric_vector() : template_vector_<T>() {
	};

	numeric_vector(const numeric_vector<T>& vector) : template_vector_<T>(vector) {
	};

	template<typename T1, typename T2>
	numeric_vector(const map_values<T1, T2>& values) : template_vector_<T2>(values) {
	};

	template<typename T1, typename T2>
	numeric_vector(const combine_values<T1, T2>& values) : template_vector_<T2>(values) {
	};
};

template<typename T>
inline numeric_vector<T>& numeric_vector<T>::normalize()
{
	// TODO: вставьте здесь оператор return
}

template<typename T>
inline numeric_vector<T> numeric_vector<T>::normalized() const
{
	return numeric_vector<T>(combine_values<T, T>(this, T{1.0} / magintude(), [](const T& lhs, const T& rhs) {return lhs * rhs; }));
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
	return (*this);
}

template<typename T>
inline numeric_vector<T>& numeric_vector<T>::operator-=(const numeric_vector<T>& rhs)
{
	// TODO: вставьте здесь оператор return
	return (*this);
}

template<typename T>
inline numeric_vector<T>& numeric_vector<T>::operator*=(const numeric_vector<T>& rhs)
{
	// TODO: вставьте здесь оператор return
	return (*this);
}

template<typename T>
inline numeric_vector<T>& numeric_vector<T>::operator/=(const numeric_vector<T>& rhs)
{
	// TODO: вставьте здесь оператор return
	return (*this);
}

template<typename T>
inline numeric_vector<T>& numeric_vector<T>::operator+=(const T& rhs)
{
	// apply([](const T& value) {return value + rhs; });
	return (*this);
}

template<typename T>
inline numeric_vector<T>& numeric_vector<T>::operator-=(const T& rhs)
{
	// apply([](const T& value) {return value - rhs; });
	return (*this);
}

template<typename T>
inline numeric_vector<T>& numeric_vector<T>::operator*=(const T& rhs)
{
	// apply([](const T& value) {return value * rhs; });
	return (*this);
}

template<typename T>
inline numeric_vector<T>& numeric_vector<T>::operator/=(const T& rhs)
{
	return (*this) * (T{ 1.0 } / rhs);
}

template<typename T>
inline numeric_vector<T> operator+(const numeric_vector<T>& lhs, const numeric_vector<T>& rhs)
{
	const combine_values<T, T> combine_vals = combine_values<T, T>(lhs.values(), rhs.values(), sum_f);
	return numeric_vector<T>(combine_vals);
}

template<typename T>
inline numeric_vector<T> operator-(const numeric_vector<T>& lhs, const numeric_vector<T>& rhs)
{
	const combine_values<T, T> combine_vals = combine_values<T, T>(lhs.values(), rhs.values(), dif_f); 
	return numeric_vector<T>(combine_vals);
}

template<typename T>
inline numeric_vector<T> operator*(const numeric_vector<T>& lhs, const numeric_vector<T>& rhs)
{
	const combine_values<T, T> combine_vals = combine_values<T, T>(lhs.values(), rhs.values(), mul_f);
	return numeric_vector<T>(combine_vals);
}

template<typename T>
inline numeric_vector<T> operator/(const numeric_vector<T>& lhs, const numeric_vector<T>& rhs)
{
	const combine_values<T, T> combine_vals = combine_values<T, T>(lhs.values(), rhs.values(), div_f);
	return numeric_vector<T>(combine_vals);
}

template<typename T>
inline numeric_vector<T> operator+(const T& lhs, const numeric_vector<T>& rhs)
{
	return numeric_vector<T>(combine_values<T, T>(lhs, rhs.values(), sum_f));
}

template<typename T>
inline numeric_vector<T> operator-(const T& lhs, const numeric_vector<T>& rhs)
{
	return numeric_vector<T>(combine_values<T, T>(lhs, rhs.values(), dif_f));
}

template<typename T>
inline numeric_vector<T> operator*(const T& lhs, const numeric_vector<T>& rhs)
{
	return numeric_vector<T>(combine_values<T, T>(lhs, rhs.values(), mul_f));
}

template<typename T>
inline numeric_vector<T> operator/(const T& lhs, const numeric_vector<T>& rhs)
{
	return numeric_vector<T>(combine_values<T, T>(lhs, rhs.values(), div_f));
}

template<typename T>
inline numeric_vector<T> operator+(const numeric_vector<T>& lhs, const T& rhs)
{
	return numeric_vector<T>(combine_values<T, T>(lhs.values(), rhs, sum_f));
}

template<typename T>
inline numeric_vector<T> operator-(const numeric_vector<T>& lhs, const T& rhs)
{
	return numeric_vector<T>(combine_values<T, T>(lhs.values(), rhs, dif_f));
}

template<typename T>
inline numeric_vector<T> operator*(const numeric_vector<T>& lhs, const T& rhs)
{
	return numeric_vector<T>(combine_values<T, T>(lhs.values(), rhs, mul_f));
}

template<typename T>
inline numeric_vector<T> operator/(const numeric_vector<T>& lhs, const T& rhs)
{
	return numeric_vector<T>(combine_values<T, T>(lhs.values(), rhs, div_f));
}

void numeric_vector_test();

void numeric_vector_test() 
{
	double_vector lhs, rhs;
	lhs.push_back(1.0).push_back(2.0).push_back(3.0).push_back(4.0);
	rhs.push_back(9.0).push_back(8.0).push_back(7.0).push_back(6.0);
	std::cout << "lhs       : " << lhs << "\n";
	std::cout << "rhs       : " << rhs << "\n";
	std::cout << "rhs - copy: " << double_vector(rhs) << "\n";
	std::cout << "lhs + rhs : " << lhs + rhs << "\n";
	std::cout << "lhs - rhs : " << lhs - rhs << "\n";
	std::cout << "lhs * rhs : " << lhs * rhs << "\n";
	std::cout << "lhs / rhs : " << lhs / rhs << "\n";

	std::cout << "2 + rhs   : " << 2.0 + rhs << "\n";
	std::cout << "2 - rhs   : " << 2.0 - rhs << "\n";
	std::cout << "2 * rhs   : " << 2.0 * rhs << "\n";
	std::cout << "2 / rhs   : " << 2.0 / rhs << "\n";

	std::cout << "rhs + 2   : " << rhs + 2.0 << "\n";
	std::cout << "rhs - 2   : " << rhs - 2.0 << "\n";
	std::cout << "rhs * 2   : " << rhs * 2.0 << "\n";
	std::cout << "rhs / 2   : " << rhs / 2.0 << "\n";
}
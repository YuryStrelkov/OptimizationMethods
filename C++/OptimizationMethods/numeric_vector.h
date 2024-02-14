#pragma once
#include "template_vector.h"

template<typename T>class      numeric_vector;
typedef numeric_vector<double> double_vector;
typedef numeric_vector<float>  float_vector;
typedef numeric_vector<int>    int_vector;

auto sum_f = [&](const auto& lhs, const auto& rhs) { return lhs + rhs; };
auto dif_f = [&](const auto& lhs, const auto& rhs) { return lhs - rhs; };
auto div_f = [&](const auto& lhs, const auto& rhs) { return lhs / rhs; };
auto mul_f = [&](const auto& lhs, const auto& rhs) { return lhs * rhs; };

template<typename T>
class numeric_vector: public template_vector_<T>
{
public:
	numeric_vector<T>&       normalize ();
	numeric_vector<T>        normalized()const;
	T                        magintude ()const;
	static T                 dot       (const numeric_vector<T>& lhs, const numeric_vector<T>& rhs);
	static numeric_vector<T> direction (const numeric_vector<T>& lhs, const numeric_vector<T>& rhs);
	static numeric_vector<T> gradient  (std::function<T(const numeric_vector<T>&)> func, numeric_vector<T>& x, const T& accuracy);
	static T                 partial   (std::function<T(const numeric_vector<T>&)> func, numeric_vector<T>& x, const int& index, const T& accuracy);
	static T                 partial2  (std::function<T(const numeric_vector<T>&)> func, numeric_vector<T>& x, const int& index1, const int& index2, const T& accuracy);

	numeric_vector<T>& operator+=(const numeric_vector<T>& rhs);
	numeric_vector<T>& operator-=(const numeric_vector<T>& rhs);
	numeric_vector<T>& operator*=(const numeric_vector<T>& rhs);
	numeric_vector<T>& operator/=(const numeric_vector<T>& rhs);

	numeric_vector<T>& operator+=(const T& rhs);
	numeric_vector<T>& operator-=(const T& rhs);
	numeric_vector<T>& operator*=(const T& rhs);
	numeric_vector<T>& operator/=(const T& rhs);

	/// <summary>
	/// TODO 
	/// </summary>
	/*
	numeric_vector<T>&       operator*();
	numeric_vector<T>*       operator&();
	const numeric_vector<T>& operator*();
	const numeric_vector<T>* operator&();

	template<typename T>friend bool operator==(const numeric_vector<T>& lhs, const numeric_vector<T>& rhs);
	template<typename T>friend bool operator!=(const numeric_vector<T>& lhs, const numeric_vector<T>& rhs);

	template<typename T>friend bool operator>(const numeric_vector<T>& lhs, const numeric_vector<T>& rhs);
	template<typename T>friend bool operator<(const numeric_vector<T>& lhs, const numeric_vector<T>& rhs);

	template<typename T>friend bool operator>=(const numeric_vector<T>& lhs, const numeric_vector<T>& rhs);
	template<typename T>friend bool operator<=(const numeric_vector<T>& lhs, const numeric_vector<T>& rhs);

	template<typename T>friend bool operator >(const T& lhs, const numeric_vector<T>& rhs);
	template<typename T>friend bool operator <(const T& lhs, const numeric_vector<T>& rhs);

	template<typename T>friend bool operator>=(const T& lhs, const numeric_vector<T>& rhs);
	template<typename T>friend bool operator<=(const T& lhs, const numeric_vector<T>& rhs);

	template<typename T>friend bool operator>(const numeric_vector<T>& lhs, const T& rhs);
	template<typename T>friend bool operator<(const numeric_vector<T>& lhs, const T& rhs);

	template<typename T>friend bool operator>=(const numeric_vector<T>& lhs, const T& rhs);
	template<typename T>friend bool operator<=(const numeric_vector<T>& lhs, const T& rhs);
	*/
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

	numeric_vector(const initializer_list<T>& values) : template_vector_<T>(values) {
	};

	// template< typename... Args>
	// numeric_vector(T val, Args... values) :template_vector_<T>(val, values...){};

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
	return  ((*this) *= (T{ 1.0 } / (*this).magintude()));
}

template<typename T>
inline numeric_vector<T> numeric_vector<T>::normalized() const
{
	return numeric_vector<T>(combine_values<T, T>((*this).values(), T{1.0} / magintude(), mul_f));
}

template<typename T>
inline T numeric_vector<T>::magintude() const
{
	return T(std::sqrt(numeric_vector<T>::reduce([](const T& accum, const T& value) {return accum + value * value; })));
}

template<typename T>
inline T numeric_vector<T>::dot(const numeric_vector<T>& lhs, const numeric_vector<T>& rhs)
{
	T val = T{ 0 };
	for (const pair_<T, T>& pair : template_vector_<T>::template zip<T>(lhs, rhs)) val += pair.first * pair.second;
	return val;
}

template<typename T>
inline numeric_vector<T> numeric_vector<T>::direction(const numeric_vector<T>& lhs, const numeric_vector<T>& rhs)
{
	return (rhs - lhs).normalize();
}

template<typename T>
inline numeric_vector<T> numeric_vector<T>::gradient(std::function<T(const numeric_vector<T>&)> func, numeric_vector<T>& x, const T& accuracy)
{
	numeric_vector<T> grad(x.filling());
	grad.applyEnumerate([&](const int index, const T& value) {return numeric_vector<T>::partial(func, x, index, accuracy); });
	return grad;
}


template<typename T>
inline T numeric_vector<T>::partial(std::function<T(const numeric_vector<T>&)> func, numeric_vector<T>& x, const int& index, const T& accuracy)
{
	assert(x.in_range(index), "index value out of vector indices rannge");
	x.unchecked_access(index) = x.unchecked_access(index) + accuracy;
	const T f_r               = func(x);
	x.unchecked_access(index) = x.unchecked_access(index) - T{ 2.0 } * accuracy;
	const T f_l               = func(x);
	x.unchecked_access(index) = x.unchecked_access(index) + accuracy;
	return (f_r - f_l) / accuracy * T{ 0.5 };
}

template<typename T>
inline T numeric_vector<T>::partial2(std::function<T(const numeric_vector<T>&)> func, numeric_vector<T>& x, const int& index1, const int& index2, const T& accuracy)
{
	assert(x.in_range(index2), "index value out of vector indices rannge");
	x.unchecked_access(index2) = x.unchecked_access(index2) + accuracy;
	const T f_r                = numeric_vector<T>::partial(func, x, index1, accuracy);
	x.unchecked_access(index2) = x.unchecked_access(index2) - T{ 2.0 } * accuracy;
	const T f_l                = numeric_vector<T>::partial(func, x, index1, accuracy);
	x.unchecked_access(index2) = x.unchecked_access(index2) + accuracy;
	return (f_r - f_l) / accuracy * T{ 0.5 };
}

template<typename T>
inline numeric_vector<T>& numeric_vector<T>::operator+=(const numeric_vector<T>& rhs)
{
	assert((*this).filling() == rhs.filling(), "operator += :: vector with not equal sizes");
	template_vector_<T>::apply(combine_values<T, T>((*this).values(), rhs.values(), sum_f));
	return (*this);
}

template<typename T>
inline numeric_vector<T>& numeric_vector<T>::operator-=(const numeric_vector<T>& rhs)
{
	assert((*this).filling() == rhs.filling(), "operator -= :: vector with not equal sizes");
	template_vector_<T>::apply(combine_values<T, T>((*this).values(), rhs.values(), dif_f));
	return (*this);
}

template<typename T>
inline numeric_vector<T>& numeric_vector<T>::operator*=(const numeric_vector<T>& rhs)
{
	assert((*this).filling() == rhs.filling(), "operator *= :: vector with not equal sizes");
	template_vector_<T>::apply(combine_values<T, T>((*this).values(), rhs.values(), mul_f));
	return (*this);
}

template<typename T>
inline numeric_vector<T>& numeric_vector<T>::operator/=(const numeric_vector<T>& rhs)
{
	assert((*this).filling() == rhs.filling(), "operator /= :: vector with not equal sizes");
	template_vector_<T>::apply(combine_values<T, T>((*this).values(), rhs.values(), div_f));
	return (*this);
}

template<typename T>
inline numeric_vector<T>& numeric_vector<T>::operator+=(const T& rhs)
{
	template_vector_<T>::apply(combine_values<T, T>((*this).values(), rhs, sum_f));
	return (*this);
}

template<typename T>
inline numeric_vector<T>& numeric_vector<T>::operator-=(const T& rhs)
{
	template_vector_<T>::apply(combine_values<T, T>((*this).values(), rhs, dif_f));
	return (*this);
}

template<typename T>
inline numeric_vector<T>& numeric_vector<T>::operator*=(const T& rhs)
{
	template_vector_<T>::apply(combine_values<T, T>((*this).values(), rhs, mul_f));
	return (*this);
}

template<typename T>
inline numeric_vector<T>& numeric_vector<T>::operator/=(const T& rhs)
{
	template_vector_<T>::apply(combine_values<T, T>((*this).values(), T{ 1.0 } / rhs, mul_f));
	return (*this);
}

template<typename T>
inline numeric_vector<T> operator+(const numeric_vector<T>& lhs, const numeric_vector<T>& rhs)
{
	assert(lhs.filling() == rhs.filling(), "operator + :: vector with not equal sizes");
	const combine_values<T, T> combine_vals = combine_values<T, T>(lhs.values(), rhs.values(), sum_f);
	return numeric_vector<T>(combine_vals);
}

template<typename T>
inline numeric_vector<T> operator-(const numeric_vector<T>& lhs, const numeric_vector<T>& rhs)
{
	assert(lhs.filling() == rhs.filling(), "operator - :: vector with not equal sizes");
	const combine_values<T, T> combine_vals = combine_values<T, T>(lhs.values(), rhs.values(), dif_f);
	return numeric_vector<T>(combine_vals);
}

template<typename T>
inline numeric_vector<T> operator*(const numeric_vector<T>& lhs, const numeric_vector<T>& rhs)
{
	assert(lhs.filling() == rhs.filling(), "operator * :: vector with not equal sizes");
	const combine_values<T, T> combine_vals = combine_values<T, T>(lhs.values(), rhs.values(), mul_f);
	return numeric_vector<T>(combine_vals);
}

template<typename T>
inline numeric_vector<T> operator/(const numeric_vector<T>& lhs, const numeric_vector<T>& rhs)
{
	assert(lhs.filling() == rhs.filling(), "operator / :: vector with not equal sizes");
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

auto test_f = [&](const double_vector& vector)
{
	double result = 0.0;
	int index = 1;
	for (auto v : vector.values())
	{
		result += index * (v - index - 1) * (v - index - 1);
	}
	return result;
};

void numeric_vector_test();

void numeric_vector_test() 
{
	double_vector lhs({ 1.0, 2.0, 3.0, 4.0 });
	double_vector rhs({ 9.0, 8.0, 7.0, 6.0 });
	// rhs.push_back(9.0).push_back(8.0).push_back(7.0).push_back(6.0);
	std::cout << "lhs            : " << lhs << "\n";
	std::cout << "rhs            : " << rhs << "\n";
	std::cout << "rhs - copy     : " << double_vector(rhs) << "\n";
	std::cout << "lhs + rhs      : " << lhs + rhs << "\n";
	std::cout << "lhs - rhs      : " << lhs - rhs << "\n";
	std::cout << "lhs * rhs      : " << lhs * rhs << "\n";
	std::cout << "lhs / rhs      : " << lhs / rhs << "\n";

	std::cout << "2 + rhs        : " << 2.0 + rhs << "\n";
	std::cout << "2 - rhs        : " << 2.0 - rhs << "\n";
	std::cout << "2 * rhs        : " << 2.0 * rhs << "\n";
	std::cout << "2 / rhs        : " << 2.0 / rhs << "\n";
							     
	std::cout << "rhs + 2        : " << rhs + 2.0 << "\n";
	std::cout << "rhs - 2        : " << rhs - 2.0 << "\n";
	std::cout << "rhs * 2        : " << rhs * 2.0 << "\n";
	std::cout << "rhs / 2        : " << rhs / 2.0 << "\n";
								 
	std::cout << "lhs += rhs     : " << (lhs += rhs) << "\n";
	std::cout << "lhs -= rhs     : " << (lhs -= rhs) << "\n";
	std::cout << "lhs *= rhs     : " << (lhs *= rhs) << "\n";
	std::cout << "lhs /= rhs     : " << (lhs /= rhs) << "\n";
								 
	std::cout << "lhs += 2.0     : " << (lhs += 2.0) << "\n";
	std::cout << "lhs -= 2.0     : " << (lhs -= 2.0) << "\n";
	std::cout << "lhs *= 2.0     : " << (lhs *= 2.0) << "\n";
	std::cout << "lhs /= 2.0     : " << (lhs /= 2.0) << "\n";
	std::cout << "mag(lhs)       : " << lhs.magintude() << "\n";
	std::cout << "dot(lhs, rhs)  : " << double_vector::dot(lhs, rhs) << "\n";
	std::cout << "dir(lhs, rhs)  : " << double_vector::direction(lhs, rhs) << "\n";
	std::cout << "normalized(lhs): " << lhs.normalized() << "\n";
	std::cout << "normalize (lhs): " << lhs.normalize() << "\n";
	std::cout << "gradient  (lhs): " << double_vector::gradient(test_f, lhs, 1e-9) << "\n";

}
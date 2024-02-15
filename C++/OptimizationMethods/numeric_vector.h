#pragma once
#include "template_vector.h"

template<typename T>class      numeric_vector_;
typedef numeric_vector_<double> double_vector;
typedef numeric_vector_<float>  float_vector;
typedef numeric_vector_<int>    int_vector;

auto sum_f = [&](const auto& lhs, const auto& rhs) { return lhs + rhs; };
auto dif_f = [&](const auto& lhs, const auto& rhs) { return lhs - rhs; };
auto div_f = [&](const auto& lhs, const auto& rhs) { return lhs / rhs; };
auto mul_f = [&](const auto& lhs, const auto& rhs) { return lhs * rhs; };

template<typename T>
class numeric_vector_: public template_vector_<T>
{
public:
	numeric_vector_<T>&      normalize ();
	numeric_vector_<T>       normalized()const;
	T                        magintude ()const;
	static T                 dot       (const numeric_vector_<T>& lhs, const numeric_vector_<T>& rhs);
	static numeric_vector_<T>direction (const numeric_vector_<T>& lhs, const numeric_vector_<T>& rhs);
	static numeric_vector_<T>gradient  (std::function<T(const numeric_vector_<T>&)> func, numeric_vector_<T>& x, const T& accuracy);
	static T                 partial   (std::function<T(const numeric_vector_<T>&)> func, numeric_vector_<T>& x, const int& index, const T& accuracy);
	static T                 partial2  (std::function<T(const numeric_vector_<T>&)> func, numeric_vector_<T>& x, const int& index1, const int& index2, const T& accuracy);

	numeric_vector_<T>& operator+=(const numeric_vector_<T>& rhs);
	numeric_vector_<T>& operator-=(const numeric_vector_<T>& rhs);
	numeric_vector_<T>& operator*=(const numeric_vector_<T>& rhs);
	numeric_vector_<T>& operator/=(const numeric_vector_<T>& rhs);

	numeric_vector_<T>& operator+=(const T& rhs);
	numeric_vector_<T>& operator-=(const T& rhs);
	numeric_vector_<T>& operator*=(const T& rhs);
	numeric_vector_<T>& operator/=(const T& rhs);

	template<typename T>friend bool operator==(const numeric_vector_<T>& lhs, const numeric_vector_<T>& rhs);
	template<typename T>friend bool operator!=(const numeric_vector_<T>& lhs, const numeric_vector_<T>& rhs);
	
	template<typename T>friend bool operator>(const numeric_vector_<T>& lhs, const numeric_vector_<T>& rhs);
	template<typename T>friend bool operator<(const numeric_vector_<T>& lhs, const numeric_vector_<T>& rhs);

	template<typename T>friend bool operator>=(const numeric_vector_<T>& lhs, const numeric_vector_<T>& rhs);
	template<typename T>friend bool operator<=(const numeric_vector_<T>& lhs, const numeric_vector_<T>& rhs);
	
	template<typename T>friend bool operator >(const T& lhs, const numeric_vector_<T>& rhs);
	template<typename T>friend bool operator <(const T& lhs, const numeric_vector_<T>& rhs);

	template<typename T>friend bool operator>=(const T& lhs, const numeric_vector_<T>& rhs);
	template<typename T>friend bool operator<=(const T& lhs, const numeric_vector_<T>& rhs);

	template<typename T>friend bool operator>(const numeric_vector_<T>& lhs, const T& rhs);
	template<typename T>friend bool operator<(const numeric_vector_<T>& lhs, const T& rhs);

	template<typename T>friend bool operator>=(const numeric_vector_<T>& lhs, const T& rhs);
	template<typename T>friend bool operator<=(const numeric_vector_<T>& lhs, const T& rhs);
	/*
	*/
	template<typename T>friend numeric_vector_<T> operator+(const numeric_vector_<T>& lhs, const numeric_vector_<T>& rhs);
	template<typename T>friend numeric_vector_<T> operator-(const numeric_vector_<T>& lhs, const numeric_vector_<T>& rhs);
	template<typename T>friend numeric_vector_<T> operator*(const numeric_vector_<T>& lhs, const numeric_vector_<T>& rhs);
	template<typename T>friend numeric_vector_<T> operator/(const numeric_vector_<T>& lhs, const numeric_vector_<T>& rhs);

	template<typename T>friend numeric_vector_<T> operator+(const T& lhs, const numeric_vector_<T>& rhs);
	template<typename T>friend numeric_vector_<T> operator-(const T& lhs, const numeric_vector_<T>& rhs);
	template<typename T>friend numeric_vector_<T> operator*(const T& lhs, const numeric_vector_<T>& rhs);
	template<typename T>friend numeric_vector_<T> operator/(const T& lhs, const numeric_vector_<T>& rhs);

	template<typename T>friend numeric_vector_<T> operator+(const numeric_vector_<T>& lhs, const T& rhs);
	template<typename T>friend numeric_vector_<T> operator-(const numeric_vector_<T>& lhs, const T& rhs);
	template<typename T>friend numeric_vector_<T> operator*(const numeric_vector_<T>& lhs, const T& rhs);
	template<typename T>friend numeric_vector_<T> operator/(const numeric_vector_<T>& lhs, const T& rhs);

	numeric_vector_(const int& cap) : template_vector_<T>(cap) {
	};

	numeric_vector_(const initializer_list<T>& values) : template_vector_<T>(values) {
	};

	// template< typename... Args>
	// numeric_vector_(T val, Args... values) :template_vector_<T>(val, values...){};

	numeric_vector_() : template_vector_<T>() {
	};

	numeric_vector_(const numeric_vector_<T>& vector) : template_vector_<T>(vector) {
	};

	numeric_vector_(numeric_vector_<T>&& vector) : template_vector_<T>(vector) {
	};

	template<typename T1, typename T2>
	numeric_vector_(const map_values<T1, T2>& values) : template_vector_<T2>(values) {
	};

	template<typename T1, typename T2>
	numeric_vector_(const combine_values<T1, T2>& values) : template_vector_<T2>(values) {
	};
};

template<typename T>
inline numeric_vector_<T>& numeric_vector_<T>::normalize()
{
	return  ((*this) *= (T{ 1.0 } / (*this).magintude()));
}

template<typename T>
inline numeric_vector_<T> numeric_vector_<T>::normalized() const
{
	return numeric_vector_<T>(combine_values<T, T>((*this).values(), T{1.0} / magintude(), mul_f));
}

template<typename T>
inline T numeric_vector_<T>::magintude() const
{
	return T(std::sqrt(numeric_vector_<T>::reduce([](const T& accum, const T& value) {return accum + value * value; })));
}

template<typename T>
inline T numeric_vector_<T>::dot(const numeric_vector_<T>& lhs, const numeric_vector_<T>& rhs)
{
	T val = T{ 0 };
	for (const pair_<T, T>& pair : template_vector_<T>::template zip<T>(lhs, rhs)) val += pair.first * pair.second;
	return val;
}

template<typename T>
inline numeric_vector_<T> numeric_vector_<T>::direction(const numeric_vector_<T>& lhs, const numeric_vector_<T>& rhs)
{
	return (rhs - lhs).normalize();
}

template<typename T>
inline numeric_vector_<T> numeric_vector_<T>::gradient(std::function<T(const numeric_vector_<T>&)> func, numeric_vector_<T>& x, const T& accuracy)
{
	numeric_vector_<T> grad(x.filling());
	grad.applyEnumerate([&](const int index, const T& value) {return numeric_vector_<T>::partial(func, x, index, accuracy); });
	return grad;
}

template<typename T>
inline T numeric_vector_<T>::partial(std::function<T(const numeric_vector_<T>&)> func, numeric_vector_<T>& x, const int& index, const T& accuracy)
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
inline T numeric_vector_<T>::partial2(std::function<T(const numeric_vector_<T>&)> func, numeric_vector_<T>& x, const int& index1, const int& index2, const T& accuracy)
{
	assert(x.in_range(index2), "index value out of vector indices rannge");
	x.unchecked_access(index2) = x.unchecked_access(index2) + accuracy;
	const T f_r                = numeric_vector_<T>::partial(func, x, index1, accuracy);
	x.unchecked_access(index2) = x.unchecked_access(index2) - T{ 2.0 } * accuracy;
	const T f_l                = numeric_vector_<T>::partial(func, x, index1, accuracy);
	x.unchecked_access(index2) = x.unchecked_access(index2) + accuracy;
	return (f_r - f_l) / accuracy * T{ 0.5 };
}

template<typename T>
inline numeric_vector_<T>& numeric_vector_<T>::operator+=(const numeric_vector_<T>& rhs)
{
	assert((*this).filling() == rhs.filling(), "operator += :: vector with not equal sizes");
	template_vector_<T>::apply(combine_values<T, T>((*this).values(), rhs.values(), sum_f));
	return (*this);
}

template<typename T>
inline numeric_vector_<T>& numeric_vector_<T>::operator-=(const numeric_vector_<T>& rhs)
{
	assert((*this).filling() == rhs.filling(), "operator -= :: vector with not equal sizes");
	template_vector_<T>::apply(combine_values<T, T>((*this).values(), rhs.values(), dif_f));
	return (*this);
}

template<typename T>
inline numeric_vector_<T>& numeric_vector_<T>::operator*=(const numeric_vector_<T>& rhs)
{
	assert((*this).filling() == rhs.filling(), "operator *= :: vector with not equal sizes");
	template_vector_<T>::apply(combine_values<T, T>((*this).values(), rhs.values(), mul_f));
	return (*this);
}

template<typename T>
inline numeric_vector_<T>& numeric_vector_<T>::operator/=(const numeric_vector_<T>& rhs)
{
	assert((*this).filling() == rhs.filling(), "operator /= :: vector with not equal sizes");
	template_vector_<T>::apply(combine_values<T, T>((*this).values(), rhs.values(), div_f));
	return (*this);
}

template<typename T>
inline numeric_vector_<T>& numeric_vector_<T>::operator+=(const T& rhs)
{
	template_vector_<T>::apply(combine_values<T, T>((*this).values(), rhs, sum_f));
	return (*this);
}

template<typename T>
inline numeric_vector_<T>& numeric_vector_<T>::operator-=(const T& rhs)
{
	template_vector_<T>::apply(combine_values<T, T>((*this).values(), rhs, dif_f));
	return (*this);
}

template<typename T>
inline numeric_vector_<T>& numeric_vector_<T>::operator*=(const T& rhs)
{
	template_vector_<T>::apply(combine_values<T, T>((*this).values(), rhs, mul_f));
	return (*this);
}

template<typename T>
inline numeric_vector_<T>& numeric_vector_<T>::operator/=(const T& rhs)
{
	template_vector_<T>::apply(combine_values<T, T>((*this).values(), T{ 1.0 } / rhs, mul_f));
	return (*this);
}

template<typename T>
inline bool operator==(const numeric_vector_<T>& lhs, const numeric_vector_<T>& rhs)
{
	assert(lhs.filling() == rhs.filling(), "compare operator :: vector with not equal sizes");
	const zip_values<T, T> zip_vals = zip_values<T, T>(lhs.values(), rhs.values());
	for (const pair_<T, T>& pair : zip_vals) if (pair.first != pair.second) return false;
	return true;
}

template<typename T>
inline bool operator!=(const numeric_vector_<T>& lhs, const numeric_vector_<T>& rhs)
{
	return !(lhs == rhs);
}

template<typename T>
inline bool operator>(const numeric_vector_<T>& lhs, const numeric_vector_<T>& rhs)
{
	assert(lhs.filling() == rhs.filling(), "operator > :: vector with not equal sizes");
	const zip_values<T, T> zip_vals = zip_values<T, T>(lhs.values(), rhs.values());
	for (const pair_<T, T>& pair : zip_vals) if (pair.first <= pair.second) return false;
	return true;
}

template<typename T>
inline bool operator<(const numeric_vector_<T>& lhs, const numeric_vector_<T>& rhs)
{
	assert(lhs.filling() == rhs.filling(), "operator < :: vector with not equal sizes");
	const zip_values<T, T> zip_vals = zip_values<T, T>(lhs.values(), rhs.values());
	for (const pair_<T, T>& pair : zip_vals) if (pair.first >= pair.second) return false;
	return true;
}

template<typename T>
inline bool operator>=(const numeric_vector_<T>& lhs, const numeric_vector_<T>& rhs)
{
	return !(lhs < rhs);
}

template<typename T>
inline bool operator<=(const numeric_vector_<T>& lhs, const numeric_vector_<T>& rhs)
{
	return !(lhs > rhs);
}

template<typename T>
inline bool operator>(const T& lhs, const numeric_vector_<T>& rhs)
{
	for (const pair_<T, T>& pair : zip_values<T, T>(lhs, rhs.values())) if (pair.first <= pair.second) return false;
	return true;
}

template<typename T>
inline bool operator<(const T& lhs, const numeric_vector_<T>& rhs)
{
	for (const pair_<T, T>& pair : zip_values<T, T>(lhs, rhs.values())) if (pair.first >= pair.second) return false;
	return true;
}

template<typename T>
inline bool operator>=(const T& lhs, const numeric_vector_<T>& rhs)
{
	return !(lhs < rhs);
}

template<typename T>
inline bool operator<=(const T& lhs, const numeric_vector_<T>& rhs)
{
	return !(lhs > rhs);
}

template<typename T>
inline bool operator>(const numeric_vector_<T>& lhs, const T& rhs)
{
	for (const pair_<T, T>& pair : zip_values<T, T>(lhs.values(), rhs)) if (pair.first <= pair.second) return false;
	return true;
}

template<typename T>
inline bool operator<(const numeric_vector_<T>& lhs, const T& rhs)
{
	for (const pair_<T, T>& pair : zip_values<T, T>(lhs.values(), rhs)) if (pair.first >= pair.second) return false;
	return true;
}

template<typename T>
inline bool operator>=(const numeric_vector_<T>& lhs, const T& rhs)
{
	return !(lhs < rhs);
}

template<typename T>
inline bool operator<=(const numeric_vector_<T>& lhs, const T& rhs)
{
	return !(lhs > rhs);
}

template<typename T>
inline numeric_vector_<T> operator+(const numeric_vector_<T>& lhs, const numeric_vector_<T>& rhs)
{
	assert(lhs.filling() == rhs.filling(), "operator + :: vector with not equal sizes");
	const combine_values<T, T> combine_vals = combine_values<T, T>(lhs.values(), rhs.values(), sum_f);
	return numeric_vector_<T>(combine_vals);
}

template<typename T>
inline numeric_vector_<T> operator-(const numeric_vector_<T>& lhs, const numeric_vector_<T>& rhs)
{
	assert(lhs.filling() == rhs.filling(), "operator - :: vector with not equal sizes");
	const combine_values<T, T> combine_vals = combine_values<T, T>(lhs.values(), rhs.values(), dif_f);
	return numeric_vector_<T>(combine_vals);
}

template<typename T>
inline numeric_vector_<T> operator*(const numeric_vector_<T>& lhs, const numeric_vector_<T>& rhs)
{
	assert(lhs.filling() == rhs.filling(), "operator * :: vector with not equal sizes");
	const combine_values<T, T> combine_vals = combine_values<T, T>(lhs.values(), rhs.values(), mul_f);
	return numeric_vector_<T>(combine_vals);
}

template<typename T>
inline numeric_vector_<T> operator/(const numeric_vector_<T>& lhs, const numeric_vector_<T>& rhs)
{
	assert(lhs.filling() == rhs.filling(), "operator / :: vector with not equal sizes");
	const combine_values<T, T> combine_vals = combine_values<T, T>(lhs.values(), rhs.values(), div_f);
	return numeric_vector_<T>(combine_vals);
}

template<typename T>
inline numeric_vector_<T> operator+(const T& lhs, const numeric_vector_<T>& rhs)
{
	return numeric_vector_<T>(combine_values<T, T>(lhs, rhs.values(), sum_f));
}

template<typename T>
inline numeric_vector_<T> operator-(const T& lhs, const numeric_vector_<T>& rhs)
{
	return numeric_vector_<T>(combine_values<T, T>(lhs, rhs.values(), dif_f));
}

template<typename T>
inline numeric_vector_<T> operator*(const T& lhs, const numeric_vector_<T>& rhs)
{
	return numeric_vector_<T>(combine_values<T, T>(lhs, rhs.values(), mul_f));
}

template<typename T>
inline numeric_vector_<T> operator/(const T& lhs, const numeric_vector_<T>& rhs)
{
	return numeric_vector_<T>(combine_values<T, T>(lhs, rhs.values(), div_f));
}

template<typename T>
inline numeric_vector_<T> operator+(const numeric_vector_<T>& lhs, const T& rhs)
{
	return numeric_vector_<T>(combine_values<T, T>(lhs.values(), rhs, sum_f));
}

template<typename T>
inline numeric_vector_<T> operator-(const numeric_vector_<T>& lhs, const T& rhs)
{
	return numeric_vector_<T>(combine_values<T, T>(lhs.values(), rhs, dif_f));
}

template<typename T>
inline numeric_vector_<T> operator*(const numeric_vector_<T>& lhs, const T& rhs)
{
	return numeric_vector_<T>(combine_values<T, T>(lhs.values(), rhs, mul_f));
}

template<typename T>
inline numeric_vector_<T> operator/(const numeric_vector_<T>& lhs, const T& rhs)
{
	return numeric_vector_<T>(combine_values<T, T>(lhs.values(), rhs, div_f));
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
	std::cout << "lhs == rhs     : " << (lhs == rhs) << "\n";
	std::cout << "lhs != rhs     : " << (lhs != rhs) << "\n";
	std::cout << "lhs > rhs      : " << (lhs > rhs) << "\n";
	std::cout << "lhs < rhs      : " << (lhs < rhs) << "\n";
	std::cout << "lhs >= rhs     : " << (lhs >= rhs) << "\n";
	std::cout << "lhs <= rhs     : " << (lhs <= rhs) << "\n";
	std::cout << "lhs >= 100     : " << (lhs >= 100.0) << "\n";
	std::cout << "lhs <= 100     : " << (lhs <= 100.0) << "\n";

}
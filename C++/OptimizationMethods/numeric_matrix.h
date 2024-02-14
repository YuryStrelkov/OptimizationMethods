#pragma once
#include "numeric_vector.h"

template<typename T>class      numeric_matrix_;
typedef numeric_matrix_<double> double_vector;
typedef numeric_matrix_<float>  float_vector;
typedef numeric_matrix_<int>    int_vector;

#define matrices_pair pair_<numeric_matrix_<T>, numeric_matrix_<T>>;



template<typename T>
class numeric_matrix_ : public numeric_vector_<T>
{
private:
	uint32_t m_rows;
	uint32_t m_cols;
	size_check(const numeric_matrix_<T>& other)const 
	{
		if (rows() != other.rows()) return false;
		if (cols() != other.cols()) return false;
		return true;
	}
public:
	uint32_t rows() const { return m_rows;};
	uint32_t cols() const { return m_cols;};

	numeric_matrix_<T>& add_col(const numeric_vector_<T>& col);
	numeric_matrix_<T>& add_row(const numeric_vector_<T>& col);
	
	T trace      ()const;
	T determinant()const;
	numeric_matrix_<T>& transpose();
	numeric_matrix_<T>& invert   ();

	static numeric_vector_<T> linsolve (const numeric_matrix_<T>& low, const numeric_matrix_<T>& up, const numeric_vector_<T>& b);
	static numeric_vector_<T> linsolve (const numeric_matrix_<T>& matrix, const numeric_vector_<T>& b);
	static uint32_t           rank     (const numeric_matrix_<T>& matrix);
	static numeric_matrix_<T> transpose(const numeric_matrix_<T>& matrix);
	static numeric_matrix_<T> invert   (const numeric_matrix_<T>& matrix);
	static numeric_matrix_<T> mexp     (const numeric_matrix_<T>& matrix);
	static numeric_matrix_<T> zeros    (const int& n_rows, const int& m_cols);
	static numeric_matrix_<T> identity (const int& n_rows, const int& m_cols);
	static numeric_matrix_<T> zeros    (const int& size);
	static numeric_matrix_<T> identity (const int& size);
	static matrices_pair      lu       (const numeric_matrix_<T>& matrix);
	static matrices_pair      qr       (const numeric_matrix_<T>& matrix);
	static numeric_matrix_<T> svd      (const numeric_matrix_<T>& matrix);
	static numeric_matrix_<T> chol     (const numeric_matrix_<T>& matrix);

	numeric_matrix_<T>& operator+=(const numeric_matrix_<T>& rhs);
	numeric_matrix_<T>& operator-=(const numeric_matrix_<T>& rhs);
	numeric_matrix_<T>& operator*=(const numeric_matrix_<T>& rhs);
	numeric_matrix_<T>& operator/=(const numeric_matrix_<T>& rhs);

	template<typename T>friend numeric_matrix_<T> operator+(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs);
	template<typename T>friend numeric_matrix_<T> operator-(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs);
	template<typename T>friend numeric_matrix_<T> operator*(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs);
	template<typename T>friend numeric_matrix_<T> operator/(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs);

	template<typename T>friend numeric_matrix_<T> operator+(const T& lhs, const numeric_matrix_<T>& rhs);
	template<typename T>friend numeric_matrix_<T> operator-(const T& lhs, const numeric_matrix_<T>& rhs);
	template<typename T>friend numeric_matrix_<T> operator*(const T& lhs, const numeric_matrix_<T>& rhs);
	template<typename T>friend numeric_matrix_<T> operator/(const T& lhs, const numeric_matrix_<T>& rhs);

	template<typename T>friend numeric_matrix_<T> operator+(const numeric_matrix_<T>& lhs, const T& rhs);
	template<typename T>friend numeric_matrix_<T> operator-(const numeric_matrix_<T>& lhs, const T& rhs);
	template<typename T>friend numeric_matrix_<T> operator*(const numeric_matrix_<T>& lhs, const T& rhs);
	template<typename T>friend numeric_matrix_<T> operator/(const numeric_matrix_<T>& lhs, const T& rhs);
	
	/*
	numeric_matrix_<T>& operator+=(const T& rhs);
	numeric_matrix_<T>& operator-=(const T& rhs);
	numeric_matrix_<T>& operator*=(const T& rhs);
	numeric_matrix_<T>& operator/=(const T& rhs);
	*/
	/*
	template<typename T>friend bool operator==(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs);
	template<typename T>friend bool operator!=(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs);

	template<typename T>friend bool operator>(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs);
	template<typename T>friend bool operator<(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs);

	template<typename T>friend bool operator>=(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs);
	template<typename T>friend bool operator<=(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs);

	template<typename T>friend bool operator >(const T& lhs, const numeric_matrix_<T>& rhs);
	template<typename T>friend bool operator <(const T& lhs, const numeric_matrix_<T>& rhs);

	template<typename T>friend bool operator>=(const T& lhs, const numeric_matrix_<T>& rhs);
	template<typename T>friend bool operator<=(const T& lhs, const numeric_matrix_<T>& rhs);

	template<typename T>friend bool operator>(const numeric_matrix_<T>& lhs, const T& rhs);
	template<typename T>friend bool operator<(const numeric_matrix_<T>& lhs, const T& rhs);

	template<typename T>friend bool operator>=(const numeric_matrix_<T>& lhs, const T& rhs);
	template<typename T>friend bool operator<=(const numeric_matrix_<T>& lhs, const T& rhs);
	*/
};

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::operator+=(const numeric_matrix_<T>& rhs)
{
	assert(size_check(rhs), "...");
	numeric_vector_<T>::operator+=(rhs);
	return (*this);
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::operator-=(const numeric_matrix_<T>& rhs)
{
	assert(size_check(rhs), "...");
	numeric_vector_<T>::operator-=(rhs); 
	return (*this);
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::operator*=(const numeric_matrix_<T>& rhs)
{
	// TODO: вставьте здесь оператор return
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::operator/=(const numeric_matrix_<T>& rhs)
{
	// TODO: вставьте здесь оператор return
}

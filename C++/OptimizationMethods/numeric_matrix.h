#pragma once
#include "numeric_vector.h"

template<typename T>class      numeric_matrix_;
typedef numeric_matrix_<double> double_vector;
typedef numeric_matrix_<float>  float_vector;
typedef numeric_matrix_<int>    int_vector;

#define matrices_pair pair_<numeric_matrix_<T>, numeric_matrix_<T>>;
#define MAT_MUL_SZIE_CHECK 0
#define MAT_SIZE_CHECK 1

template<typename T>
class numeric_matrix_ : public numeric_vector_<T>
{
private:
	uint32_t m_rows;
	uint32_t m_cols;
	static size_check(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs, const uint8_t& comparing_mode = MAT_SIZE_CHECK)
	{
		return lhs.size_check(rhs, comparing_mode);
	}
	size_check(const numeric_matrix_<T>& other, const uint8_t& comparing_mode = MAT_SIZE_CHECK)const
	{
		switch (comparing_mode)
		{
			case MAT_SIZE_CHECK:
				if (rows() != other.rows()) return false;
				if (cols() != other.cols()) return false;
				return true;
			case MAT_MUL_SZIE_CHECK
				if (rows() != other.cols()) return false;
				return true;
			default:
				return false;
		}
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
	// static matrices_pair      qr       (const numeric_matrix_<T>& matrix);
	// static numeric_matrix_<T> svd      (const numeric_matrix_<T>& matrix);
	// static numeric_matrix_<T> chol     (const numeric_matrix_<T>& matrix);

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
	numeric_matrix_(numeric_matrix_<T>&& src) : numeric_matrix_(src, src.rows(), src.cols()) {}
	
	numeric_matrix_(const numeric_vector_<T>& src) : numeric_matrix_(src, src.rows(), src.cols()) {}
	
	numeric_matrix_(numeric_vector_<T>&& src, const uint32_t& rows, const uint32_t& cols)
	{
		assert(rows * cols == src.filling());
		m_rows = rows;
		m_cols = cols;
		(*this) = src;
	}

	numeric_matrix_(const numeric_vector_<T>& src, const uint32_t& rows, const uint32_t& cols)
	{
		assert(rows * cols == src.filling());
		m_rows = rows;
		m_cols = cols;
		(*this) = src;
	}
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

template<typename T>
inline numeric_matrix_<T> operator+(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs)
{
	assert(numeric_matrix_<T>::size_check(rhs, lhs), "...");
	return numeric_matrix_<T>(numeric_vector_<T>::operator+(lhs, rhs), rhs.rows(), rhs.cols());
}

template<typename T>
inline numeric_matrix_<T> operator-(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs)
{
	assert(numeric_matrix_<T>::size_check(rhs, lhs), "...");
	return numeric_matrix_<T>(numeric_vector_<T>::operator-(lhs, rhs), rhs.rows(), rhs.cols());
}

template<typename T>
inline numeric_matrix_<T> operator+(const T& lhs, const numeric_matrix_<T>& rhs)
{
	assert(numeric_matrix_<T>::size_check(rhs, lhs), "...");
	return numeric_matrix_<T>(numeric_vector_<T>::operator+(lhs, rhs), rhs.rows(), rhs.cols());
}

template<typename T>
inline numeric_matrix_<T> operator-(const T& lhs, const numeric_matrix_<T>& rhs)
{
	return numeric_matrix_<T>(numeric_vector_<T>::operator-(lhs, rhs), rhs.rows(), rhs.cols());
}

template<typename T>
inline numeric_matrix_<T> operator*(const T& lhs, const numeric_matrix_<T>& rhs)
{
	return numeric_matrix_<T>(numeric_vector_<T>::operator*(lhs, rhs), rhs.rows(), rhs.cols());
}

template<typename T>
inline numeric_matrix_<T> operator/(const T& lhs, const numeric_matrix_<T>& rhs)
{
	return numeric_matrix_<T>(numeric_vector_<T>::operator/(lhs, rhs), rhs.rows(), rhs.cols());
}

template<typename T>
inline numeric_matrix_<T> operator+(const numeric_matrix_<T>& lhs, const T& rhs)
{
	return numeric_matrix_<T>(numeric_vector_<T>::operator+(lhs, rhs), lhs.rows(), lhs.cols());
}

template<typename T>
inline numeric_matrix_<T> operator-(const numeric_matrix_<T>& lhs, const T& rhs)
{
	return numeric_matrix_<T>(numeric_vector_<T>::operator-(lhs, rhs), lhs.rows(), lhs.cols());
}

template<typename T>
inline numeric_matrix_<T> operator*(const numeric_matrix_<T>& lhs, const T& rhs)
{
	return numeric_matrix_<T>(numeric_vector_<T>::operator*(lhs, rhs), lhs.rows(), lhs.cols());
}

template<typename T>
inline numeric_matrix_<T> operator/(const numeric_matrix_<T>& lhs, const T& rhs)
{
	return numeric_matrix_<T>(numeric_vector_<T>::operator/(lhs, rhs), lhs.rows(), lhs.cols());
}

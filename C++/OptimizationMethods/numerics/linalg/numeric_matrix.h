#pragma once
#ifndef __NUMERIC_MATRIX_H__
#define __NUMERIC_MATRIX_H__
#include "numeric_vector.h"
NUMERICS_NAMESPACE_BEGIN
template<typename T> class numeric_matrix_;
typedef numeric_matrix_<F64> matrix_f64;
typedef numeric_matrix_<F32> matrix_f32;
typedef numeric_matrix_<I32> matrix_i32;

#define MAT_MUL_SZIE_CHECK 0
#define MAT_VECT_MUL_SZIE_CHECK 1
#define VECT_MAT_MUL_SZIE_CHECK 2
#define MAT_SIZE_CHECK 3

template<typename T>
class numeric_matrix_ : public template_vector_<T>
{
public:
//////////////////////////////////////
///   Iterator over matrix rows    ///
//////////////////////////////////////
	class rows_iterator // : public iterator_<numeric_vector_<T>>
	{
	private:
		numeric_matrix_<T>* m_matrix;
		I32 m_index;
	public:
		rows_iterator(numeric_matrix_<T>& src, const I32& start_index)
		{
			m_matrix =*src;
			m_index = start_index;
		}
		// Унаследовано через iterator_
		bool                operator==(const rows_iterator& other) const { return m_index == other.m_index; }
		bool                operator!=(const rows_iterator& other) const { return !(*this == other); }
		rows_iterator&      operator++()    { m_index++; return *this; }
		rows_iterator       operator++(I32) { rows_iterator& retval = *this; ++(*this); return retval; }
		numeric_vector_<T>  operator* ()    { return m_matrix->get_row(m_index); }
		numeric_vector_<T>* operator& ()    { return *(m_matrix->get_row(m_index)); }
	};
	struct matrix_rows// : iterable_<rows_iterator>
	{
	private:
		numeric_matrix_<T>* m_matrix;
	public:
		matrix_rows(numeric_matrix_<T>& src) :m_matrix(*src) {}
		// Унаследовано через iterable_
		rows_iterator begin() { return rows_iterator(*m_matrix, 0); }
		rows_iterator end()   { return rows_iterator(*m_matrix, m_matrix->rows_count()); }
	};
//////////////////////////////////////
///   Iterator over matrix cols    ///
//////////////////////////////////////
	class cols_iterator // : public iterator_<numeric_vector_<T>>
	{
	private:
		numeric_matrix_<T>* m_matrix;
		I32 m_index;
	public:
		cols_iterator(numeric_matrix_<T>& src, const I32& start_index)
		{
			m_matrix = *src;
			m_index = start_index;
		}
		// Унаследовано через iterator_
		bool                operator==(const cols_iterator& other) const { return m_index == other.m_index; }
		bool                operator!=(const cols_iterator& other) const { return !(*this == other); }
		cols_iterator&      operator++()                                 { m_index++; return *this; }
		cols_iterator       operator++(I32)                              { cols_iterator& retval = *this; ++(*this); return retval; }
		numeric_vector_<T>  operator* ()                                 { return m_matrix->get_col(m_index); }
		numeric_vector_<T>* operator& ()                                 { return *(m_matrix->get_col(m_index)); }
	};
	struct matrix_cols// : iterable_<rows_iterator>
	{
	private:
		numeric_matrix_<T>* m_matrix;
	public:
		matrix_cols(numeric_matrix_<T>& src) :m_matrix(*src) {}
		// Унаследовано через iterable_
		cols_iterator begin() { return cols_iterator(*m_matrix, 0); }
		cols_iterator end() { return cols_iterator(*m_matrix, m_matrix->rows_count()); }
	};
private:
	I32 m_rows;
	I32 m_cols;
	static bool size_check(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs, const uint8_t& comparing_mode = MAT_SIZE_CHECK)
	{
		return lhs.size_check(rhs, comparing_mode);
	}

	static bool size_check(const numeric_matrix_<T>& lhs, const numeric_vector_<T>& rhs)
	{
		return lhs.size_check(rhs, MAT_VECT_MUL_SZIE_CHECK);
	}

	static bool size_check(const numeric_vector_<T>& rhs, const numeric_matrix_<T>& lhs)
	{
		return lhs.size_check(rhs, VECT_MAT_MUL_SZIE_CHECK);
	}

	bool  size_check(const numeric_matrix_<T>& other, const uint8_t& comparing_mode = MAT_SIZE_CHECK)const
	{
		switch (comparing_mode)
		{
			case MAT_SIZE_CHECK:
				return (cols_count() == other.cols_count()) && (rows_count() == other.rows_count());
			case MAT_MUL_SZIE_CHECK:
				return rows_count() == other.cols_count();
			default:
				return false;
		}
	}
	bool size_check(const numeric_vector_<T>& other, const uint8_t& comparing_mode = MAT_VECT_MUL_SZIE_CHECK)const
	{
		switch (comparing_mode)
		{
		case MAT_VECT_MUL_SZIE_CHECK:
			return cols_count() == other.filling();
		case VECT_MAT_MUL_SZIE_CHECK:
			return rows_count() == other.filling();
		default:
			return false;
		}
	}
	static numeric_vector_<T> linsolve(const numeric_matrix_<T>& low, const numeric_matrix_<T>& up, const numeric_vector_<T>& b);
protected:
	const T& unchecked_access(const I32 row, const I32 col)const
	{
		return template_vector_<T>::unchecked_access(row * cols_count() + col);
	}
	T& unchecked_access(const I32 row, const I32 col)
	{
		return template_vector_<T>::unchecked_access(row * cols_count() + col);
	}
public:
	const T& get(const I32 row, const I32 col)const
	{
		assert((row >= 0 && row < rows_count()) && "numeric_matrix :: get(i, j) error :: row is out of range...\n");
		assert((col >= 0 && row < cols_count()) && "numeric_matrix :: get(i, j) error :: col is out of range...\n");
		return unchecked_access(row, col);
	}
	T& get(const I32 row, const I32 col)
	{
		assert((row >= 0 && row < rows_count()) && "numeric_matrix :: get(i, j) error :: row is out of range...\n");
		assert((col >= 0 && row < cols_count()) && "numeric_matrix :: get(i, j) error :: col is out of range...\n"); 
		return unchecked_access(row, col);
	}

	matrix_rows rows()
	{
		return matrix_rows(*this);
	}
	matrix_cols cols()
	{
		return matrix_cols(*this);
	}

	I32 rows_count() const { return m_rows;};
	I32 cols_count() const { return m_cols;};

	numeric_matrix_<T>& add_col(const numeric_vector_<T>& col);
	numeric_matrix_<T>& add_row(const numeric_vector_<T>& row);
	numeric_matrix_<T>& add_col();
	numeric_matrix_<T>& add_row();
	T trace      ()const;
	T determinant()const;
	numeric_matrix_<T>& transpose();
	numeric_matrix_<T>& invert   ();

	static numeric_vector_<T> linsolve (const numeric_matrix_<T>& matrix, const numeric_vector_<T>& b);
	// static numeric_matrix_<T> mexp     (const numeric_matrix_<T>& matrix);
	// static I32           rank     (const numeric_matrix_<T>& matrix);
	static numeric_matrix_<T> transpose(const numeric_matrix_<T>& matrix);
	static numeric_matrix_<T> invert   (const numeric_matrix_<T>& matrix);
	static numeric_matrix_<T> zeros    (const I32 n_rows, const I32 m_cols);
	static numeric_matrix_<T> identity (const I32 n_rows, const I32 m_cols);
	static numeric_matrix_<T> zeros    (const I32 size);
	static numeric_matrix_<T> identity (const I32 size);
	static void               lu       (const numeric_matrix_<T>& matrix, numeric_matrix_<T>& low, numeric_matrix_<T>& up);
	static numeric_matrix_<T> hessian  (std::function<T(const numeric_vector_<T>&)> func, numeric_vector_<T>& arg, const T& eps = ACCURACY);
	numeric_vector_<T>		  get_row(const I32 index);
	numeric_vector_<T>		  get_col(const I32 index);
	// static matrices_pair      qr       (const numeric_matrix_<T>& matrix);
	// static numeric_matrix_<T> svd      (const numeric_matrix_<T>& matrix);
	// static numeric_matrix_<T> chol     (const numeric_matrix_<T>& matrix);
	numeric_matrix_<T>const& operator&()const { return (*this); }
	numeric_matrix_<T>const* operator*()const { return this; }
	numeric_matrix_<T>& operator&() { return (*this); }
	numeric_matrix_<T>* operator*() { return this; }
	numeric_vector_<T> operator[](const I32 index) { return get_row(index); }
	
	const T& operator()(const I32 row, const I32 col)const{ return get(row, col); }
	T& operator()(const I32 row, const I32 col) { return get(row, col); }
	
	numeric_matrix_<T>& operator=(const numeric_matrix_<T>& rhs);
	numeric_matrix_<T>& operator=(numeric_matrix_<T>&& rhs)noexcept;

	numeric_matrix_<T>& operator+=(const numeric_matrix_<T>& rhs);
	numeric_matrix_<T>& operator-=(const numeric_matrix_<T>& rhs);
	numeric_matrix_<T>& operator*=(const numeric_matrix_<T>& rhs);
	numeric_matrix_<T>& operator^=(const numeric_matrix_<T>& rhs);
	numeric_matrix_<T>& operator/=(const numeric_matrix_<T>& rhs);

	numeric_matrix_<T>& operator+=(const T& rhs);
	numeric_matrix_<T>& operator-=(const T& rhs);
	numeric_matrix_<T>& operator*=(const T& rhs);
	numeric_matrix_<T>& operator/=(const T& rhs);
	
	template<typename U>friend numeric_matrix_<U> operator+(const numeric_matrix_<U>& lhs, const numeric_matrix_<U>& rhs);
	template<typename U>friend numeric_matrix_<U> operator-(const numeric_matrix_<U>& lhs, const numeric_matrix_<U>& rhs);
	template<typename U>friend numeric_matrix_<U> operator^(const numeric_matrix_<U>& lhs, const numeric_matrix_<U>& rhs);
	template<typename U>friend numeric_matrix_<U> operator/(const numeric_matrix_<U>& lhs, const numeric_matrix_<U>& rhs);
	template<typename U>friend numeric_matrix_<U> operator*(const numeric_matrix_<U>& lhs, const numeric_matrix_<U>& rhs);
	template<typename U>friend numeric_vector_<U> operator*(const numeric_vector_<U>& lhs, const numeric_matrix_<U>& rhs);
	template<typename U>friend numeric_vector_<U> operator*(const numeric_matrix_<U>& lhs, const numeric_vector_<U>& rhs);

	template<typename U>friend numeric_matrix_<U> operator+(const U& lhs, const numeric_matrix_<U>& rhs);
	template<typename U>friend numeric_matrix_<U> operator-(const U& lhs, const numeric_matrix_<U>& rhs);
	template<typename U>friend numeric_matrix_<U> operator*(const U& lhs, const numeric_matrix_<U>& rhs);
	template<typename U>friend numeric_matrix_<U> operator/(const U& lhs, const numeric_matrix_<U>& rhs);

	template<typename U>friend numeric_matrix_<U> operator+(const numeric_matrix_<U>& lhs, const U& rhs);
	template<typename U>friend numeric_matrix_<U> operator-(const numeric_matrix_<U>& lhs, const U& rhs);
	template<typename U>friend numeric_matrix_<U> operator*(const numeric_matrix_<U>& lhs, const U& rhs);
	template<typename U>friend numeric_matrix_<U> operator/(const numeric_matrix_<U>& lhs, const U& rhs);
	
	template<typename U>friend bool operator==(const numeric_matrix_<U>& lhs, const numeric_matrix_<U>& rhs);
	template<typename U>friend bool operator!=(const numeric_matrix_<U>& lhs, const numeric_matrix_<U>& rhs);

	template<typename U>friend bool operator>(const numeric_matrix_<U>& lhs, const numeric_matrix_<U>& rhs);
	template<typename U>friend bool operator<(const numeric_matrix_<U>& lhs, const numeric_matrix_<U>& rhs);

	template<typename U>friend bool operator>=(const numeric_matrix_<U>& lhs, const numeric_matrix_<U>& rhs);
	template<typename U>friend bool operator<=(const numeric_matrix_<U>& lhs, const numeric_matrix_<U>& rhs);

	template<typename U>friend bool operator >(const U& lhs, const numeric_matrix_<U>& rhs);
	template<typename U>friend bool operator <(const U& lhs, const numeric_matrix_<U>& rhs);

	template<typename U>friend bool operator>=(const U& lhs, const numeric_matrix_<U>& rhs);
	template<typename U>friend bool operator<=(const U& lhs, const numeric_matrix_<U>& rhs);

	template<typename U>friend bool operator>(const numeric_matrix_<U>& lhs, const U& rhs);
	template<typename U>friend bool operator<(const numeric_matrix_<U>& lhs, const U& rhs);

	template<typename U>friend bool operator>=(const numeric_matrix_<U>& lhs, const U& rhs);
	template<typename U>friend bool operator<=(const numeric_matrix_<U>& lhs, const U& rhs);
	
	template<typename U>friend std::ostream& operator<<(std::ostream& , const numeric_matrix_<U>& rhs);

	numeric_matrix_(numeric_matrix_<T>&& src)noexcept: numeric_matrix_(std::move(src), src.rows_count(), src.cols_count()) {}
	numeric_matrix_(numeric_vector_<T>&& src)noexcept: numeric_matrix_(std::move(src), 1, src.filling()) {}
	numeric_matrix_(numeric_vector_<T>&& src, const I32 rows, const I32 cols)noexcept
	{
		assert(rows * cols == src.filling());
		m_rows = rows;
		m_cols = cols;
		template_vector_<T>::operator=((template_vector_<T>&&)src);
	}
	numeric_matrix_(const numeric_vector_<T>& src) : numeric_matrix_(src, 1, src.filling()) {}
	numeric_matrix_(const numeric_matrix_<T>& src) : numeric_matrix_(src, src.rows_count(), src.cols_count()) {}
	numeric_matrix_(const numeric_vector_<T>& src, const I32 rows, const I32 cols)
	{
		assert(rows * cols == src.filling());
		m_rows = rows;
		m_cols = cols;
		template_vector_<T>::operator=((const template_vector_<T>&)src);
	}
	numeric_matrix_(const std::initializer_list<T>& values, const I32 rows, const I32 cols) : template_vector_<T>(values)
	{
		m_rows = rows;
		m_cols = cols;
	}
	numeric_matrix_(const I32 rows, const I32 cols, const bool fill_by_default_value = true) : template_vector_<T>(rows * cols)
	{
		if (fill_by_default_value)
			template_vector_<T>::fill([](const T& v) {return T{ 0 }; });
		m_rows = rows;
		m_cols = cols;
	}
	numeric_matrix_() : numeric_matrix_<T>(1, 1, true){}
protected:
	template<typename T1, typename T2>
	numeric_matrix_(const I32 rows, const I32 cols, const map_values<T1, T2>& values) : template_vector_<T2>(values) {
		m_rows = rows;
		m_cols = cols;
	};
	template<typename T1, typename T2>
	numeric_matrix_(const I32 rows, const I32 cols, const combine_values<T1, T2>& values) : template_vector_<T2>(values) {
		m_rows = rows;
		m_cols = cols;
	};
};

template<typename T>
inline numeric_matrix_<T> numeric_matrix_<T>::hessian(std::function<T(const numeric_vector_<T>&)> func, numeric_vector_<T>& arg, const T& eps)
{
	numeric_matrix_<T> res = numeric_matrix_<T>::zeros(arg.size(), arg.size());
	I32 row, col;
	for (row = 0; row < arg.size(); row++)
	{
		for (col = 0; col <= row; col++)
		{
			res.unchecked_access(row, col) = numeric_vector_<T>::partial2(func, arg, row, col, eps);
			res.unchecked_access(col, row) = res.unchecked_access(row, col);
		}
	}
	return res;
}

template<typename T>
inline void numeric_matrix_<T>::lu(const numeric_matrix_<T>& matrix, numeric_matrix_<T>& low, numeric_matrix_<T>& up)
{
	const I32 rows_count = matrix.rows_count();

	const I32 cols_count = matrix.cols_count();

	if (rows_count != cols_count) throw std::runtime_error("error :: matrix lu decomposition :: non square matrix");

	if (!numeric_matrix_<T>::size_check(matrix, low)) 
	{
		low = zeros(rows_count, cols_count);
	}
	
	if (!numeric_matrix_<T>::size_check(matrix, up)) 
	{
		up = zeros(rows_count, cols_count);
	}

	I32 row = 0, col = 0, index = 0;

	for (row = 0; row < cols_count; row++)
	{
		for (col = 0; col < cols_count; col++)
		{
			if (col >= row){
				low.unchecked_access(col, row) = matrix.unchecked_access(col, row);
				for (index = 0; index < row; index++)
					low.unchecked_access(col, row) -= low.unchecked_access(col, index) * up.unchecked_access(index, row);
			}
		}

		for (col = 0; col < cols_count; col++)
		{
			if (col < row) continue;
			if (col == row)
			{
				up.unchecked_access(row, col) = T{ 1 };
				continue;
			}
			up.unchecked_access(row, col) = matrix.unchecked_access(row, col) / low.unchecked_access(row, row);
			for (index = 0; index < row; index++) 
				up.unchecked_access(row, col) -= low.unchecked_access(row, index) * up.unchecked_access(index, col) / low.unchecked_access(row, row);
		}
	}
}

template<typename T>
inline numeric_vector_<T> numeric_matrix_<T>::get_row(const I32 index)
{
	if (index < 0 || index >= rows_count())
		throw std::runtime_error("Matrix::get_row::row index: " + std::to_string(index) + " is out of rows indices range...");
	const slice s = slice(index * cols_count(), (index + 1) * cols_count());
	return ((numeric_vector_<T>&)(*this))[s];
}

template<typename T>
inline numeric_vector_<T> numeric_matrix_<T>::get_col(const I32 index)
{
	if (index < 0 || index >= cols_count())
		throw std::runtime_error("Matrix::get_col::col index: " + std::to_string(index) + " is out of cols indices range...");
	const slice s = slice(index, index + cols_count() * rows_count(), cols_count());
	return ((numeric_vector_<T>&)(*this))[s];
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::operator=(const numeric_matrix_<T>& rhs)
{
	m_rows = rhs.rows_count();
	m_cols = rhs.cols_count();
	template_vector_<T>::reassign_data((const numeric_matrix_<T>&)rhs);
	return *this;
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::operator=(numeric_matrix_<T>&& rhs) noexcept
{
	m_rows = rhs.rows_count();
	m_cols = rhs.cols_count();
	template_vector_<T>::exchange_data((template_vector_<T>&&)rhs);
	return *this;
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::operator+=(const numeric_matrix_<T>& rhs)
{
	assert(size_check(*this, rhs) && "numeric_matrix::operator+=::matrices sizes mismatch...\n");
	template_vector_<T>::apply(combine_values<T, T>(this->values(), rhs.values(), sum_f));
	return *this;
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::operator-=(const numeric_matrix_<T>& rhs)
{
	assert(size_check(*this, rhs) && "numeric_matrix::operator-=::matrices sizes mismatch...\n");
	template_vector_<T>::apply(combine_values<T, T>(this->values(), rhs.values(), dif_f));
	return *this;
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::operator^=(const numeric_matrix_<T>& rhs)
{
	assert(size_check(*this, rhs) && "numeric_matrix::operator^= (per-elemet multiplication) ::matrices sizes mismatch...\n");
	template_vector_<T>::apply(combine_values<T, T>(this->values(), rhs.values(), mul_f));
	return *this;
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::operator*=(const numeric_matrix_<T>& rhs)
{
	assert(size_check(*this, rhs, MAT_MUL_SZIE_CHECK) && "numeric_matrix::operator*=::matrices sizes mismatch...\n");
	template_vector_<T>::exchange_data((template_vector_<T>&&)((*this) * rhs));
	return *this;
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::operator/=(const numeric_matrix_<T>& rhs)
{
	return (*this) *= numeric_matrix_<T>::invert(rhs);
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::operator+=(const T& rhs)
{
	template_vector_<T>::apply(combine_values<T, T>(this->values(), rhs, sum_f));
	return *this;
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::operator-=(const T& rhs)
{
	template_vector_<T>::apply(combine_values<T, T>(this->values(), rhs, dif_f));
	return *this;
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::operator*=(const T& rhs)
{
	template_vector_<T>::apply(combine_values<T, T>(this->values(), rhs, mul_f));
	return *this;
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::operator/=(const T& rhs)
{
	template_vector_<T>::apply(combine_values<T, T>(this->values(), rhs, div_f));
	return *this;
}

template<typename U>
inline numeric_vector_<U> operator*(const numeric_vector_<U>& lhs, const numeric_matrix_<U>& rhs)
{
	assert(numeric_matrix_<U>::size_check(rhs, lhs) && "numeric_matrix::operator * :: matrix multiply by vector sizes mismatch...\n");
	numeric_vector_<U> result(rhs.cols_count());
	U value;
	for (I32 col = 0; col < rhs.cols_count(); col++)
	{
		value = U{ 0 };
		for (I32 row = 0; row < rhs.rows_count(); row++)
			value += rhs.unchecked_access(row, col) * lhs.unchecked_access(row);
		result.unchecked_access(col) = value;
	}
	return result;
}

template<typename U>
inline numeric_vector_<U> operator * (const numeric_matrix_<U>& lhs, const numeric_vector_<U>& rhs)
{
	assert(numeric_matrix_<U>::size_check(rhs, lhs) && "numeric_matrix::operator * :: vector multiply by matrix sizes mismatch...\n");
	numeric_vector_<U> result(lhs.rows_count());
	U value;
	for (I32 row = 0; row < lhs.rows_count(); row++)
	{
		value = U{ 0 };
		for (I32 col = 0; col < lhs.rows_count(); col++)
			value += lhs.unchecked_access(row, col) * rhs.unchecked_access(col);
		result.unchecked_access(row) = value;
	}
	return result;
}

template<typename U>
inline numeric_matrix_<U> operator + (const numeric_matrix_<U>& lhs, const numeric_matrix_<U>& rhs)
{	
	assert(numeric_matrix_<U>::size_check(rhs, lhs) && "numeric_matrix::operator + ::matrices sizes mismatch...\n");
	const combine_values<U, U> combine_vals = combine_values<U, U>(lhs.values(), rhs.values(), sum_f);
	return numeric_matrix_<U>(rhs.rows_count(), rhs.cols_count(), combine_vals);
}

template<typename U>
inline numeric_matrix_<U> operator-(const numeric_matrix_<U>& lhs, const numeric_matrix_<U>& rhs)
{
	assert(numeric_matrix_<U>::size_check(rhs, lhs) && "numeric_matrix::operator - ::matrices sizes mismatch...\n");
	const combine_values<U, U> combine_vals = combine_values<U, U>(lhs.values(), rhs.values(), dif_f);
	return numeric_matrix_<U>(rhs.rows_count(), rhs.cols_count(), combine_vals);
}

template<typename U>
inline numeric_matrix_<U> operator^(const numeric_matrix_<U>& lhs, const numeric_matrix_<U>& rhs)
{
	assert(numeric_matrix_<U>::size_check(rhs, lhs) && "numeric_matrix::operator ^ (per-element multiplication) ::matrices sizes mismatch...\n");
	const combine_values<U, U> combine_vals = combine_values<U, U>(lhs.values(), rhs.values(), mul_f);
	return numeric_matrix_<U>(rhs.rows_count(), rhs.cols_count(), combine_vals);
}

template<typename U>
inline numeric_matrix_<U> operator*(const numeric_matrix_<U>& lhs, const numeric_matrix_<U>& rhs)
{
	assert(numeric_matrix_<U>::size_check(rhs, lhs, MAT_MUL_SZIE_CHECK) && "numeric_matrix::operator * ::matrices sizes mismatch...\n");

	numeric_matrix_<U> result(lhs.cols_count(), rhs.rows_count(), false);

	for (I32 row = 0; row < lhs.rows_count(); row++)
	{	
		const U* row_data = &lhs.unchecked_access(row, 0);
		
		for (I32 col = 0; col < rhs.cols_count(); col++)
		{
			U value = U{ 0 };

			for (I32 k = 0; k < rhs.rows_count(); k++) 
			{
				value += row_data[k] * rhs.unchecked_access(k, col);
			}

			result.unchecked_access(row, col) = value;
		}
	}
	return result;
}

template<typename U>
inline numeric_matrix_<U> operator/(const numeric_matrix_<U>& lhs, const numeric_matrix_<U>& rhs)
{
	assert(numeric_matrix_<U>::size_check(rhs, lhs, MAT_MUL_SZIE_CHECK) && "numeric_matrix::operator * ::matrices sizes mismatch...\n");
	return lhs  * numeric_matrix_<U>::invert(rhs);
}

template<typename U>
inline numeric_matrix_<U> operator+(const U& lhs, const numeric_matrix_<U>& rhs)
{
	const combine_values<U, U> combine_vals = combine_values<U, U>(lhs, rhs.values(), sum_f);
	return numeric_matrix_<U>(rhs.rows_count(), rhs.cols_count(), combine_vals);
}

template<typename U>
inline numeric_matrix_<U> operator-(const U& lhs, const numeric_matrix_<U>& rhs)
{
	const combine_values<U, U> combine_vals = combine_values<U, U>(lhs, rhs.values(), dif_f);
	return numeric_matrix_<U>(rhs.rows_count(), rhs.cols_count(), combine_vals);
}

template<typename U>
inline numeric_matrix_<U> operator*(const U& lhs, const numeric_matrix_<U>& rhs)
{
	const combine_values<U, U> combine_vals = combine_values<U, U>(lhs, rhs.values(), mul_f);
	return numeric_matrix_<U>(rhs.rows_count(), rhs.cols_count(), combine_vals);
}

template<typename U>
inline numeric_matrix_<U> operator/(const U& lhs, const numeric_matrix_<U>& rhs)
{
	const combine_values<U, U> combine_vals = combine_values<U, U>(lhs, rhs.values(), div_f);
	return numeric_matrix_<U>(rhs.rows_count(), rhs.cols_count(), combine_vals);
}

template<typename U>
inline numeric_matrix_<U> operator+(const numeric_matrix_<U>& lhs, const U& rhs)
{
	const combine_values<U, U> combine_vals = combine_values<U, U>(lhs.values(), rhs, sum_f);
	return numeric_matrix_<U>(lhs.rows_count(), lhs.cols_count(), combine_vals);
}

template<typename U>
inline numeric_matrix_<U> operator-(const numeric_matrix_<U>& lhs, const U& rhs)
{
	const combine_values<U, U> combine_vals = combine_values<U, U>(lhs.values(), rhs, dif_f);
	return numeric_matrix_<U>(lhs.rows_count(), lhs.cols_count(), combine_vals);
}

template<typename U>
inline numeric_matrix_<U> operator*(const numeric_matrix_<U>& lhs, const U& rhs)
{
	const combine_values<U, U> combine_vals = combine_values<U, U>(lhs.values(), rhs, mul_f);
	return numeric_matrix_<U>(lhs.rows_count(), lhs.cols_count(), combine_vals);
}

template<typename U>
inline numeric_matrix_<U> operator/(const numeric_matrix_<U>& lhs, const U& rhs)
{
	const combine_values<U, U> combine_vals = combine_values<U, U>(lhs.values(), rhs, div_f);
	return numeric_matrix_<U>(lhs.rows_count(), lhs.cols_count(), combine_vals);
}

template<typename U>
inline bool operator==(const numeric_matrix_<U>& lhs, const numeric_matrix_<U>& rhs)
{
	assert(numeric_matrix_<U>::size_check(rhs, lhs) && "numeric_matrix::operator == ::matrices sizes mismatch...\n");
	return reinterpret_cast<const numeric_vector_<U>&>(lhs) == reinterpret_cast<const numeric_vector_<U>&>(rhs);
}

template<typename U>
inline bool operator!=(const numeric_matrix_<U>& lhs, const numeric_matrix_<U>& rhs)
{
	assert(numeric_matrix_<U>::size_check(rhs, lhs) && "numeric_matrix::operator != ::matrices sizes mismatch...\n");
	return reinterpret_cast<const numeric_vector_<U>&>(lhs) != reinterpret_cast<const numeric_vector_<U>&>(rhs);
}

template<typename U>
inline bool operator>(const numeric_matrix_<U>& lhs, const numeric_matrix_<U>& rhs)
{
	assert(numeric_matrix_<U>::size_check(rhs, lhs) && "numeric_matrix::operator > ::matrices sizes mismatch...\n");
	return reinterpret_cast<const numeric_vector_<U>&>(lhs) > reinterpret_cast<const numeric_vector_<U>&>(rhs);
}

template<typename U>
inline bool operator<(const numeric_matrix_<U>& lhs, const numeric_matrix_<U>& rhs)
{
	assert(numeric_matrix_<U>::size_check(rhs, lhs) && "numeric_matrix::operator < ::matrices sizes mismatch...\n");
	return reinterpret_cast<const numeric_vector_<U>&>(lhs) < reinterpret_cast<const numeric_vector_<U>&>(rhs);
}

template<typename U>
inline bool operator>=(const numeric_matrix_<U>& lhs, const numeric_matrix_<U>& rhs)
{
	assert(numeric_matrix_<U>::size_check(rhs, lhs) && "numeric_matrix::operator >= ::matrices sizes mismatch...\n");
	return reinterpret_cast<const numeric_vector_<U>&>(lhs) >= reinterpret_cast<const numeric_vector_<U>&>(rhs);
}

template<typename U>
inline bool operator<=(const numeric_matrix_<U>& lhs, const numeric_matrix_<U>& rhs)
{
	assert(numeric_matrix_<U>::size_check(rhs, lhs) && "numeric_matrix::operator <= ::matrices sizes mismatch...\n");
	return reinterpret_cast<const numeric_vector_<U>&>(lhs) <= reinterpret_cast<const numeric_vector_<U>&>(rhs);
}

template<typename U>
inline bool operator>(const U& lhs, const numeric_matrix_<U>& rhs)
{
	return lhs > reinterpret_cast<const numeric_vector_<U>&>(rhs);
}

template<typename U>
inline bool operator<(const U& lhs, const numeric_matrix_<U>& rhs)
{
	return lhs < reinterpret_cast<const numeric_vector_<U>&>(rhs);
}

template<typename U>
inline bool operator>=(const U& lhs, const numeric_matrix_<U>& rhs)
{
	return lhs >= reinterpret_cast<const numeric_vector_<U>&>(rhs);
}

template<typename U>
inline bool operator<=(const U& lhs, const numeric_matrix_<U>& rhs)
{
	return lhs <= reinterpret_cast<const numeric_vector_<U>&>(rhs);
}

template<typename U>
inline bool operator>(const numeric_matrix_<U>& lhs, const U& rhs)
{
	return reinterpret_cast<const numeric_vector_<U>&>(lhs) > rhs;
}

template<typename U>
inline bool operator<(const numeric_matrix_<U>& lhs, const U& rhs)
{
	return reinterpret_cast<const numeric_vector_<U>&>(lhs) < rhs;

}

template<typename U>
inline bool operator>=(const numeric_matrix_<U>& lhs, const U& rhs)
{
	return reinterpret_cast<const numeric_vector_<U>&>(lhs) >= rhs;
}

template<typename U>
inline bool operator<=(const numeric_matrix_<U>& lhs, const U& rhs)
{
	return reinterpret_cast<const numeric_vector_<U>&>(lhs) <= rhs;
}

template<typename U>
inline std::ostream& operator<<(std::ostream& stream, const numeric_matrix_<U>& rhs)
{
	I32  index    = 0;
	I32  col      = 0;
	I32  row      = 0;
	stream << "[\n";
#if RATIONAL_NUMBERS_REPRESNTATION
	for (row = 0; row < rhs.rows_count(); row++)
	{
		stream << "  [";
		for (col = 0; col < rhs.cols_count(); col++)
		{
			stream << std::setw(NUMBER_CHARS_COUNT) << rational_str(rhs.unchecked_access(row, col), false);
			stream << (col < rhs.cols_count() - 1 ? "," : "]");
		}
		stream << (col == rhs.cols_count() - 1 ? "]\n" : ",\n");
	}
	stream << "]";
#else
	for (row = 0; row < rhs.rows_count(); row++)
	{
		stream << "  [";
		for (col = 0; col < rhs.cols_count(); col++)
		{
			stream << std::setw(NUMBER_CHARS_COUNT) << rhs.unchecked_access(row, col);
			stream << (col < rhs.cols_count() - 1 ? "," : "]");
		}
		stream << (col == rhs.cols_count() - 1 ? "]\n" : ",\n");
	}
	stream << "]";
#endif
	return stream;
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::add_col(const numeric_vector_<T>& col)
{
	if (rows_count() != col.filling())return *this;
	I32 insert_index = cols_count();
	for (const T& item : col.values())
	{
		template_vector_<T>::insert(insert_index, item);
		insert_index += cols_count() + 1;
	}
	this->m_cols += 1;
	return *this;
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::add_row(const numeric_vector_<T>& row)
{
	if (cols_count() != row.filling())return *this;
	for (const T& item: row.values()) template_vector_<T>::push_back(item);
	this->m_rows += 1;
	return *this;
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::add_col()
{
	I32 insert_index = cols_count();
	for (I32 index = 0; index < rows_count(); index++)
	{
		template_vector_<T>::insert(insert_index, T{ 0 });
		insert_index += cols_count() + 1;
	}
	this->m_cols += 1;
	return *this;
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::add_row()
{
	for (I32 index = 0; index < cols_count(); index++)
		template_vector_<T>::push_back(T{ 0 });
	this->m_rows += 1;
	return *this;
}

template<typename T>
inline T numeric_matrix_<T>::trace() const
{
	T tr = T{ 0 };
	for (I32 index = 0; index < MIN(rows_count(), cols_count()); index++)tr += unchecked_access(index, index);
	return tr;
}

template<typename T>
inline T numeric_matrix_<T>::determinant() const
{
	numeric_matrix_<T> copy(*this);
	T det = { 1 };
	I32 row, col, pivot;
	for (row = 0; row < copy.rows_count(); row++) {
		pivot = row;
		for (col = row + 1; col < copy.rows_count(); col++) {
			if (std::abs(copy.unchecked_access(col, row)) > std::abs(copy.unchecked_access(pivot, row))) {
				pivot = col;
			}
		}
		if (pivot != row) {
			T tmp = copy.unchecked_access(pivot, col);
			copy.unchecked_access(pivot, col) = copy.unchecked_access(row, col);
			copy.unchecked_access(row, col) = tmp;
			det *= T{ -1 };
		}
		if (std::abs(copy.unchecked_access(row, row)) < ACCURACY) {
			return T{ 0 };
		}
		det *= copy.unchecked_access(row, row);
		for (I32 j = row + 1; j < copy.rows_count(); j++) {
			T factor = copy.unchecked_access(j, row) / copy.unchecked_access(row, row);
			for (I32 k = row + 1; k < copy.rows_count(); k++) {
				copy.unchecked_access(j, k) = copy.unchecked_access(j, k) - factor * copy.unchecked_access(row, k);
			}
		}
	}
	return det;
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::transpose()
{
	for (I32 row = 0; row < rows_count(); row++)
		for (I32 col = 0; col < cols_count(); col++)
			unchecked_access(col, row) = unchecked_access(row, col);
	return (*this);
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::invert()
{
	template_vector_<T>::exchange_data((template_vector_<T>&&)numeric_matrix_<T>::invert(*this));
	return *this;
}

template<typename T>
inline numeric_vector_<T> numeric_matrix_<T>::linsolve(const numeric_matrix_<T>& low, const numeric_matrix_<T>& up, const numeric_vector_<T>& b)
{
	T det = T{ 1 };

	for (I32 row = 0; row < up.rows_count(); row++)
		det *= (up.unchecked_access(row, row) * up.unchecked_access(row, row));

	if (std::abs(det) < ACCURACY)
		throw std::runtime_error("Matrix is probably singular :: unable to solve A^-1 b = x"); // return numeric_vector_<T>({ 0.0 });
	
	numeric_vector_<T> x(b.filling()), z(b.filling());

	T tmp;
	I32 row, col;
	for (row = 0; row < z.filling(); row++)
	{
		tmp = T{ 0 };
		for (col = 0; col < row; col++)
			tmp += z.unchecked_access(col) * low.unchecked_access(row, col);
		z.unchecked_access(row) = (b.unchecked_access(row) - tmp) / low.unchecked_access(row, row);
	}

	for (row = z.filling() - 1; row >= 0; row--)
	{
		tmp = T{ 0 };
		for (col = row + 1; col < z.filling(); col++)
			tmp += x.unchecked_access(col) * up.unchecked_access(row, col);
		x.unchecked_access(row) = (z.unchecked_access(row) - tmp);
	}
	return x;
}

template<typename T>
inline numeric_vector_<T> numeric_matrix_<T>::linsolve(const numeric_matrix_<T>& matrix, const numeric_vector_<T>& b)
{
	if (matrix.rows_count() != matrix.cols_count())
		throw std::runtime_error("error :: matrix inversion :: non square matrix");
	
	numeric_matrix_<T> low(1, 1), up(1,1);
	
	numeric_matrix_<T>::lu(matrix, low, up);
	
	return numeric_matrix_<T>::linsolve(low, up, b);
}

template<typename T>
inline numeric_matrix_<T> numeric_matrix_<T>::transpose(const numeric_matrix_<T>& matrix)
{
	numeric_matrix_<T> t_matrix(matrix.cols_count(), matrix.rows_count());
	for (I32 row = 0; row < matrix.rows_count(); row++)
		for (I32 col = 0; col < matrix.cols_count(); col++)
			t_matrix.unchecked_access(col, row) = t_matrix.unchecked_access(row, col);
	return t_matrix;
}

template<typename T>
inline numeric_matrix_<T> numeric_matrix_<T>::invert(const numeric_matrix_<T>& matrix)
{
	if (matrix.rows_count() != matrix.cols_count())
		throw std::runtime_error("error :: matrix inversion :: non square matrix");

	numeric_matrix_<T> low(1, 1), up(1, 1); //  inv(1, 1);

	lu(matrix, low, up);

	T det = T{ 1 };
	for (I32 rows = 0; rows < low.rows_count(); rows++)
		det *= (low.unchecked_access(rows, rows) * low.unchecked_access(rows, rows));

	if (std::abs(det) < ACCURACY)
		throw std::runtime_error("mathUtils.Matrix is probably singular :: unable to calculate invert matrix");

	numeric_vector_<T> b(matrix.rows_count()), column(matrix.rows_count());

	numeric_matrix_<T> inv = numeric_matrix_<T>::zeros(matrix.rows_count());

	for (I32 col = 0; col < matrix.cols_count(); col++)
	{
		b.unchecked_access(col) = T{ 1 };
		column = numeric_matrix_<T>::linsolve(low, up, b);
		if (column.filling() != b.filling())
			throw std::runtime_error("error :: unable to find matrix inversion");
		b.unchecked_access(col) = T{ 0 };
		for (I32 row = 0; row < matrix.rows_count(); row++)
			inv.unchecked_access(row, col) = column.unchecked_access(row);
	}
	return inv;
}

template<typename T>
inline numeric_matrix_<T> numeric_matrix_<T>::zeros(const I32 n_rows, const I32 m_cols)
{
	return numeric_matrix_<T>(n_rows, m_cols);
}

template<typename T>
inline numeric_matrix_<T> numeric_matrix_<T>::identity(const I32 n_rows, const I32 n_cols)
{
	numeric_matrix_<T> matrix = numeric_matrix_<T>::zeros(n_rows, n_cols);
	for (I32 index = 0; index < MIN(n_rows, n_cols); index++)matrix.unchecked_access(index, index) = T{ 1 };
	return matrix;
}

template<typename T>
inline numeric_matrix_<T> numeric_matrix_<T>::zeros(const I32 size)
{
	return numeric_matrix_<T>::zeros(size, size);
}

template<typename T>
inline numeric_matrix_<T> numeric_matrix_<T>::identity(const I32 size)
{
	return numeric_matrix_<T>::identity(size, size);
}
void numeric_matrix_test();
NUMERICS_NAMESPACE_END
#endif
#pragma once
#include "numeric_vector.h"

template<typename T>   class     numeric_matrix_;
typedef numeric_matrix_<double>  double_matrix;
typedef numeric_matrix_<float>   float_matrix;
typedef numeric_matrix_<int32_t> int_matrix;

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
	class rows_iterator : public iterator_<numeric_vector_<T>>
	{
	private:
		numeric_matrix_<T>* m_matrix;
		int32_t m_index;
	public:
		rows_iterator(numeric_matrix_<T>& src, const int32_t& start_index)
		{
			m_matrix =*src;
			m_index = start_index;
		}
		// Унаследовано через iterator_
		bool                operator==(const rows_iterator& other) const { return m_index == other.m_index; }
		bool                operator!=(const rows_iterator& other) const { return !(*this == other); }
		rows_iterator&      operator++()    { m_index++; return *this; }
		rows_iterator       operator++(int) { rows_iterator& retval = *this; ++(*this); return retval; }
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
	class cols_iterator : public iterator_<numeric_vector_<T>>
	{
	private:
		numeric_matrix_<T>* m_matrix;
		int32_t m_index;
	public:
		cols_iterator(numeric_matrix_<T>& src, const int32_t& start_index)
		{
			m_matrix = *src;
			m_index = start_index;
		}
		// Унаследовано через iterator_
		bool                operator==(const cols_iterator& other) const { return m_index == other.m_index; }
		bool                operator!=(const cols_iterator& other) const { return !(*this == other); }
		cols_iterator&      operator++()                                 { m_index++; return *this; }
		cols_iterator       operator++(int)                              { cols_iterator& retval = *this; ++(*this); return retval; }
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
	uint32_t m_rows;
	uint32_t m_cols;
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
	const T& unchecked_access(const uint32_t& row, const uint32_t& col)const
	{
		return template_vector_<T>::unchecked_access(row * cols_count() + col);
	}
	T& unchecked_access(const uint32_t& row, const uint32_t& col)
	{
		return template_vector_<T>::unchecked_access(row * cols_count() + col);
	}
public:
	const T& get(const uint32_t& row, const uint32_t& col)const
	{
		assert((row >= 0 && row < rows_count()) && "numeric_matrix :: get(i, j) error :: row is out of range...\n");
		assert((col >= 0 && row < cols_count()) && "numeric_matrix :: get(i, j) error :: col is out of range...\n");
		return unchecked_access(row, col);
	}
	T& get(const uint32_t& row, const uint32_t& col)
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

	uint32_t rows_count() const { return m_rows;};
	uint32_t cols_count() const { return m_cols;};

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
	// static uint32_t           rank     (const numeric_matrix_<T>& matrix);
	static numeric_matrix_<T> transpose(const numeric_matrix_<T>& matrix);
	static numeric_matrix_<T> invert   (const numeric_matrix_<T>& matrix);
	static numeric_matrix_<T> zeros    (const int32_t n_rows, const int32_t m_cols);
	static numeric_matrix_<T> identity (const int32_t n_rows, const int32_t m_cols);
	static numeric_matrix_<T> zeros    (const int32_t size);
	static numeric_matrix_<T> identity (const int32_t size);
	static void               lu       (const numeric_matrix_<T>& matrix, numeric_matrix_<T>& low, numeric_matrix_<T>& up);
	static numeric_matrix_<T> hessian  (std::function<T(const numeric_vector_<T>&)> func, numeric_vector_<T>& arg, const T& eps = N_DIM_ACCURACY);
	numeric_vector_<T>		  get_row(const int32_t index);
	numeric_vector_<T>		  get_col(const int32_t index);
	// static matrices_pair      qr       (const numeric_matrix_<T>& matrix);
	// static numeric_matrix_<T> svd      (const numeric_matrix_<T>& matrix);
	// static numeric_matrix_<T> chol     (const numeric_matrix_<T>& matrix);
	numeric_matrix_<T>const& operator&()const { return (*this); }
	numeric_matrix_<T>const* operator*()const { return this; }
	numeric_matrix_<T>& operator&() { return (*this); }
	numeric_matrix_<T>* operator*() { return this; }

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
	
	template<typename T>friend numeric_matrix_<T> operator+(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs);
	template<typename T>friend numeric_matrix_<T> operator-(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs);
	template<typename T>friend numeric_matrix_<T> operator^(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs);
	template<typename T>friend numeric_matrix_<T> operator/(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs);
	template<typename T>friend numeric_matrix_<T> operator*(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs);
	template<typename T>friend numeric_vector_<T> operator*(const numeric_vector_<T>& lhs, const numeric_matrix_<T>& rhs);
	template<typename T>friend numeric_vector_<T> operator*(const numeric_matrix_<T>& lhs, const numeric_vector_<T>& rhs);

	template<typename T>friend numeric_matrix_<T> operator+(const T& lhs, const numeric_matrix_<T>& rhs);
	template<typename T>friend numeric_matrix_<T> operator-(const T& lhs, const numeric_matrix_<T>& rhs);
	template<typename T>friend numeric_matrix_<T> operator*(const T& lhs, const numeric_matrix_<T>& rhs);
	template<typename T>friend numeric_matrix_<T> operator/(const T& lhs, const numeric_matrix_<T>& rhs);

	template<typename T>friend numeric_matrix_<T> operator+(const numeric_matrix_<T>& lhs, const T& rhs);
	template<typename T>friend numeric_matrix_<T> operator-(const numeric_matrix_<T>& lhs, const T& rhs);
	template<typename T>friend numeric_matrix_<T> operator*(const numeric_matrix_<T>& lhs, const T& rhs);
	template<typename T>friend numeric_matrix_<T> operator/(const numeric_matrix_<T>& lhs, const T& rhs);
	
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
	
	template<typename T>friend std::ostream& operator<<(std::ostream& , const numeric_matrix_<T>& rhs);

	numeric_matrix_(numeric_matrix_<T>&& src)noexcept: numeric_matrix_(src, src.rows_count(), src.cols_count()) {}
	numeric_matrix_(numeric_vector_<T>&& src)noexcept: numeric_matrix_(src, 1, src.filling()) {}
	numeric_matrix_(numeric_vector_<T>&& src, const uint32_t rows, const uint32_t cols)noexcept
	{
		assert(rows * cols == src.filling());
		m_rows = rows;
		m_cols = cols;
		template_vector_<T>::operator=((template_vector_<T>&&)src);
	}
	numeric_matrix_(const numeric_vector_<T>& src) : numeric_matrix_(src, 1, src.filling()) {}
	numeric_matrix_(const numeric_matrix_<T>& src) : numeric_matrix_(src, src.rows_count(), src.cols_count()) {}
	numeric_matrix_(const numeric_vector_<T>& src, const uint32_t rows, const uint32_t cols)
	{
		assert(rows * cols == src.filling());
		m_rows = rows;
		m_cols = cols;
		template_vector_<T>::operator=((const template_vector_<T>&)src);
	}
	numeric_matrix_(const initializer_list<T>& values, const uint32_t rows, const uint32_t cols) : template_vector_<T>(values)
	{
		m_rows = rows;
		m_cols = cols;
	}
	numeric_matrix_(const uint32_t rows, const uint32_t cols, const bool fill_by_default_value = true) : template_vector_<T>(rows * cols)
	{
		if (fill_by_default_value)
			template_vector_<T>::fill([](const T& v) {return T{ 0.0f }; });
		m_rows = rows;
		m_cols = cols;
	}
	numeric_matrix_() : numeric_matrix_<T>(1, 1, true){}
protected:
	template<typename T1, typename T2>
	numeric_matrix_(const uint32_t& rows, const uint32_t& cols, const map_values<T1, T2>& values) : template_vector_<T2>(values) {
		m_rows = rows;
		m_cols = cols;
	};
	template<typename T1, typename T2>
	numeric_matrix_(const uint32_t& rows, const uint32_t& cols, const combine_values<T1, T2>& values) : template_vector_<T2>(values) {
		m_rows = rows;
		m_cols = cols;
	};
};

template<typename T>
inline numeric_matrix_<T> numeric_matrix_<T>::hessian(std::function<T(const numeric_vector_<T>&)> func, numeric_vector_<T>& arg, const T& eps)
{
	numeric_matrix_<T> res = numeric_matrix_<T>::zeros(arg.size(), arg.size());
	int row, col;
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
	int rows_count = matrix.rows_count();

	int cols_count = matrix.cols_count();

	if (rows_count != cols_count) throw std::runtime_error("error :: matrix lu decomposition :: non square matrix");

	if (!numeric_matrix_<T>::size_check(matrix, low)) 
	{
		low = zeros(rows_count, cols_count);
	}
	
	if (!numeric_matrix_<T>::size_check(matrix, up)) 
	{
		up = zeros(rows_count, cols_count);
	}

	int row = 0, col = 0, index = 0;

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
				up.unchecked_access(row, col) = T{ 1.0 };
				continue;
			}
			up.unchecked_access(row, col) = matrix.unchecked_access(row, col) / low.unchecked_access(row, row);
			for (index = 0; index < row; index++) 
				up.unchecked_access(row, col) -= low.unchecked_access(row, index) * up.unchecked_access(index, col) / low.unchecked_access(row, row);
		}
	}
}

template<typename T>
inline numeric_vector_<T> numeric_matrix_<T>::get_row(const int32_t index)
{
	if (index < 0 || index >= rows_count())
		throw std::runtime_error("Matrix::get_row::row index: " + std::to_string(index) + " is out of rows indices range...");
	const slice s = slice(index * cols_count(), (index + 1) * cols_count());
	return ((numeric_vector_<T>&)(*this))[s];
}

template<typename T>
inline numeric_vector_<T> numeric_matrix_<T>::get_col(const int32_t index)
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

template<typename T>
inline numeric_vector_<T> operator*(const numeric_vector_<T>& lhs, const numeric_matrix_<T>& rhs)
{
	assert(numeric_matrix_<T>::size_check(rhs, lhs) && "numeric_matrix::operator * :: matrix multiply by vector sizes mismatch...\n");
	numeric_vector_<T> result(rhs.cols_count());
	T value;
	for (uint32_t col = 0; col < rhs.cols_count(); col++)
	{
		value = T{ 0.0 };
		for (uint32_t row = 0; row < rhs.rows_count(); row++)
			value += rhs.unchecked_access(row, col) * lhs.unchecked_access(row);
		result.unchecked_access(col) = value;
	}
	return result;
}

template<typename T>
inline numeric_vector_<T> operator * (const numeric_matrix_<T>& lhs, const numeric_vector_<T>& rhs)
{
	assert(numeric_matrix_<T>::size_check(rhs, lhs) && "numeric_matrix::operator * :: vector multiply by matrix sizes mismatch...\n");
	numeric_vector_<T> result(lhs.rows_count());
	T value;
	for (uint32_t row = 0; row < lhs.rows_count(); row++)
	{
		value = T{ 0.0 };
		for (uint32_t col = 0; col < lhs.rows_count(); col++)
			value += lhs.unchecked_access(row, col) * rhs.unchecked_access(col);
		result.unchecked_access(row) = value;
	}
	return result;
}

template<typename T>
inline numeric_matrix_<T> operator + (const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs)
{	
	assert(numeric_matrix_<T>::size_check(rhs, lhs) && "numeric_matrix::operator + ::matrices sizes mismatch...\n");
	const combine_values<T, T> combine_vals = combine_values<T, T>(lhs.values(), rhs.values(), sum_f);
	return numeric_matrix_<T>(rhs.rows_count(), rhs.cols_count(), combine_vals);
}

template<typename T>
inline numeric_matrix_<T> operator-(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs)
{
	assert(numeric_matrix_<T>::size_check(rhs, lhs) && "numeric_matrix::operator - ::matrices sizes mismatch...\n");
	const combine_values<T, T> combine_vals = combine_values<T, T>(lhs.values(), rhs.values(), dif_f);
	return numeric_matrix_<T>(rhs.rows_count(), rhs.cols_count(), combine_vals);
}

template<typename T>
inline numeric_matrix_<T> operator^(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs)
{
	assert(numeric_matrix_<T>::size_check(rhs, lhs) && "numeric_matrix::operator ^ (per-element multiplication) ::matrices sizes mismatch...\n");
	const combine_values<T, T> combine_vals = combine_values<T, T>(lhs.values(), rhs.values(), mul_f);
	return numeric_matrix_<T>(rhs.rows_count(), rhs.cols_count(), combine_vals);
}

template<typename T>
inline numeric_matrix_<T> operator*(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs)
{
	assert(numeric_matrix_<T>::size_check(rhs, lhs, MAT_MUL_SZIE_CHECK) && "numeric_matrix::operator * ::matrices sizes mismatch...\n");

	numeric_matrix_<T> result(lhs.cols_count(), rhs.rows_count(), false);

	for (int row = 0; row < lhs.rows_count(); row++)
	{	
		const T* row_data = &lhs.unchecked_access(row, 0);
		
		for (int col = 0; col < rhs.cols_count(); col++)
		{
			T value = T{ 0 };

			for (int k = 0; k < rhs.rows_count(); k++) 
			{
				value += row_data[k] * rhs.unchecked_access(k, col);
			}

			result.unchecked_access(row, col) = value;
		}
	}
	return result;
}

template<typename T>
inline numeric_matrix_<T> operator/(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs)
{
	assert(numeric_matrix_<T>::size_check(rhs, lhs, MAT_MUL_SZIE_CHECK) && "numeric_matrix::operator * ::matrices sizes mismatch...\n");
	return lhs  * numeric_matrix_<T>::invert(rhs);
}

template<typename T>
inline numeric_matrix_<T> operator+(const T& lhs, const numeric_matrix_<T>& rhs)
{
	const combine_values<T, T> combine_vals = combine_values<T, T>(lhs, rhs.values(), sum_f);
	return numeric_matrix_<T>(rhs.rows_count(), rhs.cols_count(), combine_vals);
}

template<typename T>
inline numeric_matrix_<T> operator-(const T& lhs, const numeric_matrix_<T>& rhs)
{
	const combine_values<T, T> combine_vals = combine_values<T, T>(lhs, rhs.values(), dif_f);
	return numeric_matrix_<T>(rhs.rows_count(), rhs.cols_count(), combine_vals);
}

template<typename T>
inline numeric_matrix_<T> operator*(const T& lhs, const numeric_matrix_<T>& rhs)
{
	const combine_values<T, T> combine_vals = combine_values<T, T>(lhs, rhs.values(), mul_f);
	return numeric_matrix_<T>(rhs.rows_count(), rhs.cols_count(), combine_vals);
}

template<typename T>
inline numeric_matrix_<T> operator/(const T& lhs, const numeric_matrix_<T>& rhs)
{
	const combine_values<T, T> combine_vals = combine_values<T, T>(lhs, rhs.values(), div_f);
	return numeric_matrix_<T>(rhs.rows_count(), rhs.cols_count(), combine_vals);
}

template<typename T>
inline numeric_matrix_<T> operator+(const numeric_matrix_<T>& lhs, const T& rhs)
{
	const combine_values<T, T> combine_vals = combine_values<T, T>(lhs.values(), rhs, sum_f);
	return numeric_matrix_<T>(lhs.rows_count(), lhs.cols_count(), combine_vals);
}

template<typename T>
inline numeric_matrix_<T> operator-(const numeric_matrix_<T>& lhs, const T& rhs)
{
	const combine_values<T, T> combine_vals = combine_values<T, T>(lhs.values(), rhs, dif_f);
	return numeric_matrix_<T>(lhs.rows_count(), lhs.cols_count(), combine_vals);
}

template<typename T>
inline numeric_matrix_<T> operator*(const numeric_matrix_<T>& lhs, const T& rhs)
{
	const combine_values<T, T> combine_vals = combine_values<T, T>(lhs.values(), rhs, mul_f);
	return numeric_matrix_<T>(lhs.rows_count(), lhs.cols_count(), combine_vals);
}

template<typename T>
inline numeric_matrix_<T> operator/(const numeric_matrix_<T>& lhs, const T& rhs)
{
	const combine_values<T, T> combine_vals = combine_values<T, T>(lhs.values(), rhs, div_f);
	return numeric_matrix_<T>(lhs.rows_count(), lhs.cols_count(), combine_vals);
}

template<typename T>
inline bool operator==(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs)
{
	assert(numeric_matrix_<T>::size_check(rhs, lhs) && "numeric_matrix::operator == ::matrices sizes mismatch...\n");
	return (const numeric_vector_<T>&)lhs == (const numeric_vector_<T>&)rhs;
}

template<typename T>
inline bool operator!=(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs)
{
	assert(numeric_matrix_<T>::size_check(rhs, lhs) && "numeric_matrix::operator != ::matrices sizes mismatch...\n");
	return (const numeric_vector_<T>&)lhs != (const numeric_vector_<T>&)rhs;
}

template<typename T>
inline bool operator>(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs)
{
	assert(numeric_matrix_<T>::size_check(rhs, lhs) && "numeric_matrix::operator > ::matrices sizes mismatch...\n");
	return (const numeric_vector_<T>&)lhs > (const numeric_vector_<T>&)rhs;
}

template<typename T>
inline bool operator<(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs)
{
	assert(numeric_matrix_<T>::size_check(rhs, lhs) && "numeric_matrix::operator < ::matrices sizes mismatch...\n");
	return (const numeric_vector_<T>&)lhs < (const numeric_vector_<T>&)rhs;
}

template<typename T>
inline bool operator>=(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs)
{
	assert(numeric_matrix_<T>::size_check(rhs, lhs) && "numeric_matrix::operator >= ::matrices sizes mismatch...\n");
	return (const numeric_vector_<T>&)lhs >= (const numeric_vector_<T>&)rhs;
}

template<typename T>
inline bool operator<=(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs)
{
	assert(numeric_matrix_<T>::size_check(rhs, lhs) && "numeric_matrix::operator <= ::matrices sizes mismatch...\n");
	return (const numeric_vector_<T>&)lhs <= (const numeric_vector_<T>&)rhs;
}

template<typename T>
inline bool operator>(const T& lhs, const numeric_matrix_<T>& rhs)
{
	return lhs > (const numeric_vector_<T>&)rhs;
}

template<typename T>
inline bool operator<(const T& lhs, const numeric_matrix_<T>& rhs)
{
	return lhs < (const numeric_vector_<T>&)rhs;
}

template<typename T>
inline bool operator>=(const T& lhs, const numeric_matrix_<T>& rhs)
{
	return lhs >= (const numeric_vector_<T>&)rhs;
}

template<typename T>
inline bool operator<=(const T& lhs, const numeric_matrix_<T>& rhs)
{
	return lhs <= (const numeric_vector_<T>&)rhs;
}

template<typename T>
inline bool operator>(const numeric_matrix_<T>& lhs, const T& rhs)
{
	return (const numeric_vector_<T>&)lhs > rhs;
}

template<typename T>
inline bool operator<(const numeric_matrix_<T>& lhs, const T& rhs)
{
	return (const numeric_vector_<T>&)lhs < rhs;
}

template<typename T>
inline bool operator>=(const numeric_matrix_<T>& lhs, const T& rhs)
{
	return (const numeric_vector_<T>&)lhs >= rhs;
}

template<typename T>
inline bool operator<=(const numeric_matrix_<T>& lhs, const T& rhs)
{
	return (const numeric_vector_<T>&)lhs <= rhs;
}

template<typename T>
inline std::ostream& operator<<(std::ostream& stream, const numeric_matrix_<T>& rhs)
{
	int  index    = 0;
	int  col      = 0;
	int  row      = 0;
	stream << "[\n";
#ifdef RATIONAL_NUMBERS_REPRESNTATION
	for (row = 0; row < rhs.rows_count(); row++)
	{
		stream << "  [";
		for (col = 0; col < rhs.cols_count(); col++)
		{
			stream << std::setw(NUMBER_CHARS_COUNT) << rational::rational_str(rhs.unchecked_access(row, col), false);
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
	}
#endif
	return stream;
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::add_col(const numeric_vector_<T>& col)
{
	if (rows_count() != col.filling())return *this;
	int insert_index = cols_count();
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
	int insert_index = cols_count();
	for (int32_t index = 0; index < rows_count(); index++)
	{
		template_vector_<T>::insert(insert_index, T{ 0.0 });
		insert_index += cols_count() + 1;
	}
	this->m_cols += 1;
	return *this;
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::add_row()
{
	for (int32_t index = 0; index < cols_count(); index++)
		template_vector_<T>::push_back(T{ 0.0 });
	this->m_rows += 1;
	return *this;
}

template<typename T>
inline T numeric_matrix_<T>::trace() const
{
	T tr = T{ 0.0 };
	for (int index = 0; index < MIN(rows_count(), cols_count()); index++)tr += unchecked_access(index, index);
	return tr;
}

template<typename T>
inline T numeric_matrix_<T>::determinant() const
{
	numeric_matrix_<T> copy(*this);
	T det = { 1.0 };
	int row, col, pivot;
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
			det *= T{-1.0};
		}
		if (std::abs(copy.unchecked_access(row, row)) < ACCURACY) {
			return T{0.0};
		}
		det *= copy.unchecked_access(row, row);
		for (int j = row + 1; j < copy.rows_count(); j++) {
			T factor = copy.unchecked_access(j, row) / copy.unchecked_access(row, row);
			for (int k = row + 1; k < copy.rows_count(); k++) {
				copy.unchecked_access(j, k) = copy.unchecked_access(j, k) - factor * copy.unchecked_access(row, k);
			}
		}
	}
	return det;
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::transpose()
{
	for (int row = 0; row < rows_count(); row++)
		for (int col = 0; col < cols_count(); col++)
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
	T det = T{ 1.0 };

	for (int row = 0; row < up.rows_count(); row++)
		det *= (up.unchecked_access(row, row) * up.unchecked_access(row, row));

	if (std::abs(det) < ACCURACY)
		throw std::runtime_error("Matrix is probably singular :: unable to solve A^-1 b = x"); // return numeric_vector_<T>({ 0.0 });
	
	numeric_vector_<T> x(b.filling()), z(b.filling());

	T tmp;
	int row, col;
	for (row = 0; row < z.filling(); row++)
	{
		tmp = T{ 0.0f };
		for (col = 0; col < row; col++)
			tmp += z.unchecked_access(col) * low.unchecked_access(row, col);
		z.unchecked_access(row) = (b.unchecked_access(row) - tmp) / low.unchecked_access(row, row);
	}

	for (row = z.filling() - 1; row >= 0; row--)
	{
		tmp = T{ 0.0f };
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
	for (int row = 0; row < matrix.rows_count(); row++)
		for (int col = 0; col < matrix.cols_count(); col++)
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

	T det = T{ 1.0 };
	for (int rows = 0; rows < low.rows_count(); rows++)
		det *= (low.unchecked_access(rows, rows) * low.unchecked_access(rows, rows));

	if (std::abs(det) < ACCURACY)
		throw std::runtime_error("mathUtils.Matrix is probably singular :: unable to calculate invert matrix");

	numeric_vector_<T> b(matrix.rows_count()), column(matrix.rows_count());

	numeric_matrix_<T> inv = numeric_matrix_<T>::zeros(matrix.rows_count());

	for (int col = 0; col < matrix.cols_count(); col++)
	{
		b.unchecked_access(col) = T{ 1.0 };
		column = numeric_matrix_<T>::linsolve(low, up, b);
		if (column.filling() != b.filling())
			throw std::runtime_error("error :: unable to find matrix inversion");
		b.unchecked_access(col) = T{ 0.0 };
		for (int row = 0; row < matrix.rows_count(); row++)
			inv.unchecked_access(row, col) = column.unchecked_access(row);
	}
	return inv;
}

template<typename T>
inline numeric_matrix_<T> numeric_matrix_<T>::zeros(const int32_t n_rows, const int32_t m_cols)
{
	return numeric_matrix_<T>(n_rows, m_cols);
}

template<typename T>
inline numeric_matrix_<T> numeric_matrix_<T>::identity(const int32_t n_rows, const int32_t n_cols)
{
	numeric_matrix_<T> matrix = numeric_matrix_<T>::zeros(n_rows, m_cols);
	for (int index = 0; index < MIN(n_rows, n_cols); index++)matrix.unchecked_access(index, index) = T{ 1.0 };
	return matrix;
}

template<typename T>
inline numeric_matrix_<T> numeric_matrix_<T>::zeros(const int32_t size)
{
	return numeric_matrix_<T>::zeros(size, size);
}

template<typename T>
inline numeric_matrix_<T> numeric_matrix_<T>::identity(const int32_t size)
{
	return numeric_matrix_<T>::identity(size, size);
}

void numeric_matrix_test();

void numeric_matrix_test()
{	
	double_vector vec({ 1.0, 2.0, 3.0 });

	double_matrix lhs({ 8.0, 1.0, 6.0,
					    3.0, 5.0, 7.0,
					    4.0, 9.0, 2.0 }, 3, 3);

	double_matrix rhs({ 2.0, 9.0, 4.0,
						7.0, 5.0, 3.0 ,
						6.0, 1.0, 8.0}, 3, 3);
	// rhs.push_back(9.0).push_back(8.0).push_back(7.0).push_back(6.0);
	std::cout << "Test matrices\n";
	std::cout << "lhs       :\n" << lhs << "\n";
	std::cout << "rhs       :\n" << rhs << "\n";
	std::cout << "vec       :"   << vec << "\n";
	std::cout << "rhs * vec :\n" << rhs * vec << "\n";
	std::cout << "vec * rhs :\n" << vec * rhs << "\n";

	std::cout << "==========================================\n";
	std::cout << "rhs[:,0]  :\n" << rhs.get_col(0) << "\n";
	std::cout << "rhs[:,1]  :\n" << rhs.get_col(1) << "\n";
	std::cout << "rhs[:,2]  :\n" << rhs.get_col(2) << "\n";
	std::cout << "==========================================\n";
	std::cout << "rhs[0,:]  :\n" << rhs.get_row(0) << "\n";
	std::cout << "rhs[1,:]  :\n" << rhs.get_row(1) << "\n";
	std::cout << "rhs[2,:]  :\n" << rhs.get_row(2) << "\n";
	std::cout << "==========================================\n";
	std::cout << "rhs[:,0]  :\n" << rhs.get_col(0) << "\n";
	std::cout << "rhs[:,1]  :\n" << rhs.get_col(1) << "\n";
	std::cout << "rhs[:,2]  :\n" << rhs.get_col(2) << "\n";
	for (auto v : rhs.cols())
	{
		std::cout << v << " ";
	}
	std::cout << "\n==========================================\n";
	std::cout << "rhs[0,:]  :\n" << rhs.get_row(0) << "\n";
	std::cout << "rhs[1,:]  :\n" << rhs.get_row(1) << "\n";
	std::cout << "rhs[2,:]  :\n" << rhs.get_row(2) << "\n";
	for (auto const& v : rhs.rows())
	{
		std::cout << v << " ";
	}
	// std::cout << "rhs - copy:\n" << double_matrix(rhs) << "\n";
	std::cout << "\nOperations +,-,*,/ for matrix and matrix\n";
	std::cout << "lhs + rhs :\n" << lhs + rhs << "\n";
	std::cout << "lhs - rhs :\n" << lhs - rhs << "\n";
	std::cout << "lhs * rhs :\n" << lhs * rhs << "\n";
	std::cout << "lhs / rhs :\n" << lhs / lhs << "\n";

	double_matrix low(1,1), up(1, 1);
	double_matrix::lu(lhs, low, up);
	std::cout << "LU decomposition\n";
	std::cout << "low(lhs)  :\n" << low << "\n";
	std::cout << "up (lhs)  :\n" << up << "\n";
							
	std::cout << "Operations +,-,*,/ for number and matrix\n";
	// 						
	std::cout << "2 + rhs   :\n" << 2.0 + rhs << "\n";
	std::cout << "2 - rhs   :\n" << 2.0 - rhs << "\n";
	std::cout << "2 * rhs   :\n" << 2.0 * rhs << "\n";
	std::cout << "2 / rhs   :\n" << 2.0 / rhs << "\n";
	
	std::cout << "Operations +,-,*,/ for matrix and number\n";
	// 						
	std::cout << "rhs + 2   :\n" << rhs + 2.0 << "\n";
	std::cout << "rhs - 2   :\n" << rhs - 2.0 << "\n";
	std::cout << "rhs * 2   :\n" << rhs * 2.0 << "\n";
	std::cout << "rhs / 2   :\n" << rhs / 2.0 << "\n";
	// 						
	std::cout << "Operations +=,-=,*=,/= for matrix and matrix\n";
	std::cout << "lhs += rhs:\n" << (lhs += rhs) << "\n";
	std::cout << "lhs -= rhs:\n" << (lhs -= rhs) << "\n";
	std::cout << "lhs *= rhs:\n" << (lhs *= rhs) << "\n";
	std::cout << "lhs /= rhs:\n" << (lhs /= lhs) << "\n";
	// 						
	std::cout << "Operations +=,-=,*=,/= for matrix and number\n";
	std::cout << "lhs += 2.0:\n" << (lhs += 2.0) << "\n";
	std::cout << "lhs -= 2.0:\n" << (lhs -= 2.0) << "\n";
	std::cout << "lhs *= 2.0:\n" << (lhs *= 2.0) << "\n";
	std::cout << "lhs /= 2.0:\n" << (lhs /= 2.0) << "\n";

	std::cout << "lhs == rhs     : " << (lhs == rhs) << "\n";
	std::cout << "lhs != rhs     : " << (lhs != rhs) << "\n";
	std::cout << "lhs > rhs      : " << (lhs > rhs) << "\n";
	std::cout << "lhs < rhs      : " << (lhs < rhs) << "\n";
	std::cout << "lhs >= rhs     : " << (lhs >= rhs) << "\n";
	std::cout << "lhs <= rhs     : " << (lhs <= rhs) << "\n";
	std::cout << "lhs >= 100     : " << (lhs >= 100.0) << "\n";
	std::cout << "lhs <= 100     : " << (lhs <= 100.0) << "\n";
	std::cout << "lhs            :\n" << lhs << "\n";
	lhs[slice(3, 6)] = double_vector({ 13.0, 13.0, 13.0 });
	std::cout << "lhs            :\n" << lhs << "\n";
	double_matrix big_mat = 
	double_matrix({0.8147, 0.4218, 0.2769, 0.7094, 0.8909, 0.3517, 0.5688, 0.2290, 0.2599, 0.4018, 0.7803, 0.6491, 0.4868, 0.6225, 0.9049, 0.0855,
				   0.9058, 0.9157, 0.0462, 0.7547, 0.9593, 0.8308, 0.4694, 0.9133, 0.8001, 0.0760, 0.3897, 0.7317, 0.4359, 0.5870, 0.9797, 0.2625,
				   0.1270, 0.7922, 0.0971, 0.2760, 0.5472, 0.5853, 0.0119, 0.1524, 0.4314, 0.2399, 0.2417, 0.6477, 0.4468, 0.2077, 0.4389, 0.8010,
				   0.9134, 0.9595, 0.8235, 0.6797, 0.1386, 0.5497, 0.3371, 0.8258, 0.9106, 0.1233, 0.4039, 0.4509, 0.3063, 0.3012, 0.1111, 0.0292,
				   0.6324, 0.6557, 0.6948, 0.6551, 0.1493, 0.9172, 0.1622, 0.5383, 0.1818, 0.1839, 0.0965, 0.5470, 0.5085, 0.4709, 0.2581, 0.9289,
				   0.0975, 0.0357, 0.3171, 0.1626, 0.2575, 0.2858, 0.7943, 0.9961, 0.2638, 0.2400, 0.1320, 0.2963, 0.5108, 0.2305, 0.4087, 0.7303,
				   0.2785, 0.8491, 0.9502, 0.1190, 0.8407, 0.7572, 0.3112, 0.0782, 0.1455, 0.4173, 0.9421, 0.7447, 0.8176, 0.8443, 0.5949, 0.4886,
				   0.5469, 0.9340, 0.0344, 0.4984, 0.2543, 0.7537, 0.5285, 0.4427, 0.1361, 0.0497, 0.9561, 0.1890, 0.7948, 0.1948, 0.2622, 0.5785,
				   0.9575, 0.6787, 0.4387, 0.9597, 0.8143, 0.3804, 0.1656, 0.1067, 0.8693, 0.9027, 0.5752, 0.6868, 0.6443, 0.2259, 0.6028, 0.2373,
				   0.9649, 0.7577, 0.3816, 0.3404, 0.2435, 0.5678, 0.6020, 0.9619, 0.5797, 0.9448, 0.0598, 0.1835, 0.3786, 0.1707, 0.7112, 0.4588,
				   0.1576, 0.7431, 0.7655, 0.5853, 0.9293, 0.0759, 0.2630, 0.0046, 0.5499, 0.4909, 0.2348, 0.3685, 0.8116, 0.2277, 0.2217, 0.9631,
				   0.9706, 0.3922, 0.7952, 0.2238, 0.3500, 0.0540, 0.6541, 0.7749, 0.1450, 0.4893, 0.3532, 0.6256, 0.5328, 0.4357, 0.1174, 0.5468,
				   0.9572, 0.6555, 0.1869, 0.7513, 0.1966, 0.5308, 0.6892, 0.8173, 0.8530, 0.3377, 0.8212, 0.7802, 0.3507, 0.3111, 0.2967, 0.5211,
				   0.4854, 0.1712, 0.4898, 0.2551, 0.2511, 0.7792, 0.7482, 0.8687, 0.6221, 0.9001, 0.0154, 0.0811, 0.9390, 0.9234, 0.3188, 0.2316,
				   0.8003, 0.7060, 0.4456, 0.5060, 0.6160, 0.9340, 0.4505, 0.0844, 0.3510, 0.3692, 0.0430, 0.9294, 0.8759, 0.4302, 0.4242, 0.4889,
				   0.1419, 0.0318, 0.6463, 0.6991, 0.4733, 0.1299, 0.0838, 0.3998, 0.5132, 0.1112, 0.1690, 0.7757, 0.5502, 0.1848, 0.5079, 0.6241 }, 16, 16);
	std::cout << "big_mat             :\n" << big_mat << "\n";
	std::cout << "big_mat^-1          :\n" << double_matrix::invert(big_mat) << "\n";
	std::cout << "big_mat * big_mat^-1:\n" << big_mat/ big_mat << "\n";

	std::cout << "rhs + col:\n" << rhs.add_row({ 1, 2, 3 }) << "\n";
	std::cout << "rhs + col:\n" << rhs.add_col({ 100, 100, 100, 100 }) << "\n";
	auto row = rhs.get_row(0);
	// rhs.get_row(0) -= row;
	// std::cout << "rhs - rhs[0:]:\n" << rhs<< "\n";
	rhs.get_row(1) -= row;
	std::cout << "rhs - rhs[1:]:\n" << rhs << "\n";
	rhs.get_row(2) -= row;
	std::cout << "rhs - rhs[2:]:\n" << rhs << "\n";
	rhs.get_row(3) -= row;
	std::cout << "rhs - rhs[3:]:\n" << rhs << "\n";

}



#pragma once
#include "numeric_vector.h"

template<typename T>   class    numeric_matrix_;
typedef numeric_matrix_<double> double_matrix;
typedef numeric_matrix_<float>  float_matrix;
typedef numeric_matrix_<int>    int_matrix;

#define MAT_MUL_SZIE_CHECK 0
#define MAT_SIZE_CHECK 1

template<typename T>
class numeric_matrix_ : public numeric_vector_<T>
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
		// ”наследовано через iterator_
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
		// ”наследовано через iterable_
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
		// ”наследовано через iterator_
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
		// ”наследовано через iterable_
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
	bool  size_check(const numeric_matrix_<T>& other, const uint8_t& comparing_mode = MAT_SIZE_CHECK)const
	{
		switch (comparing_mode)
		{
			case MAT_SIZE_CHECK:
				if (rows_count() != other.rows_count()) return false;
				if (cols_count() != other.cols_count()) return false;
				return true;
			case MAT_MUL_SZIE_CHECK:
				if (rows_count() != other.cols_count()) return false;
				return true;
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
	T trace      ()const;
	T determinant()const;
	numeric_matrix_<T>& transpose();
	numeric_matrix_<T>& invert   ();

	static numeric_vector_<T> linsolve (const numeric_matrix_<T>& matrix, const numeric_vector_<T>& b);
	// static numeric_matrix_<T> mexp     (const numeric_matrix_<T>& matrix);
	// static uint32_t           rank     (const numeric_matrix_<T>& matrix);
	static numeric_matrix_<T> transpose(const numeric_matrix_<T>& matrix);
	static numeric_matrix_<T> invert   (const numeric_matrix_<T>& matrix);
	static numeric_matrix_<T> zeros    (const int& n_rows, const int& m_cols);
	static numeric_matrix_<T> identity (const int& n_rows, const int& m_cols);
	static numeric_matrix_<T> zeros    (const int& size);
	static numeric_matrix_<T> identity (const int& size);
	static void               lu       (const numeric_matrix_<T>& matrix, numeric_matrix_<T>& low, numeric_matrix_<T>& up);
	
	numeric_vector_<T> get_row(const int& index);
	numeric_vector_<T> get_col(const int& index);
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
	template<typename T>friend numeric_matrix_<T> operator*(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs);
	template<typename T>friend numeric_matrix_<T> operator^(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs);
	template<typename T>friend numeric_matrix_<T> operator/(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs);

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

	numeric_matrix_(const uint32_t& rows_count, const uint32_t& cols_count, const bool& fill_by_default_value = true) : 
		numeric_vector_<T>(rows_count * cols_count, fill_by_default_value), m_rows(rows_count), m_cols(cols_count){}

	numeric_matrix_(numeric_matrix_<T>&& src) : numeric_matrix_(src, src.rows_count(), src.cols_count()) {}

	numeric_matrix_(const numeric_matrix_<T>& src) : numeric_matrix_(src, src.rows_count(), src.cols_count()) {}
	
	numeric_matrix_(const numeric_vector_<T>& src) : numeric_matrix_(src, src.rows_count(), src.cols_count()) {}
	
	numeric_matrix_(numeric_vector_<T>&& src, const uint32_t& rows_count, const uint32_t& cols_count)
	{
		assert(rows_count * cols_count == src.filling());
		m_rows = rows_count;
		m_cols = cols_count;
		template_vector_<T>::operator=((template_vector_<T>&&)src);
	}

	numeric_matrix_(const initializer_list<T>& values, const uint32_t& rows_count, const uint32_t& cols_count) : numeric_vector_<T>(values)
	{
		m_rows = rows_count;
		m_cols = cols_count;
	}

	numeric_matrix_(const numeric_vector_<T>& src, const uint32_t& rows_count, const uint32_t& cols_count)
	{
		assert(rows_count * cols_count == src.filling());
		m_rows = rows_count;
		m_cols = cols_count;
		template_vector_<T>::operator=((const template_vector_<T>&)src);
	}
};

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
inline numeric_vector_<T> numeric_matrix_<T>::get_row(const int& index)
{
	if (index < 0 || index >= rows_count())
		throw std::runtime_error("Matrix::get_row::row index: " + std::to_string(index) + " is out of rows indices range...");
	return numeric_vector_<T>::operator[](slice(index * cols_count(), (index + 1) * cols_count()));
}

template<typename T>
inline numeric_vector_<T> numeric_matrix_<T>::get_col(const int& index)
{
	if (index < 0 || index >= cols_count())
		throw std::runtime_error("Matrix::get_col::col index: " + std::to_string(index) + " is out of cols indices range...");
	return numeric_vector_<T>::operator[](slice(index, index + cols_count() * rows_count(), cols_count()));
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
inline numeric_matrix_<T>& numeric_matrix_<T>::operator^=(const numeric_matrix_<T>& rhs)
{
	assert(size_check(rhs), "...");
	numeric_vector_<T>::operator*=(rhs);
	return (*this);
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::operator*=(const numeric_matrix_<T>& rhs)
{
	template_vector_<T>::exchange_data((template_vector_<T>&&)((*this) * rhs));
	return (*this);
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::operator/=(const numeric_matrix_<T>& rhs)
{
	return (*this) *= numeric_matrix_<T>::invert(rhs);
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::operator+=(const T& rhs)
{
	numeric_vector_<T>::operator+=(rhs);
	return (*this);

}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::operator-=(const T& rhs)
{
	numeric_vector_<T>::operator-=(rhs);
	return (*this);
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::operator*=(const T& rhs)
{
	numeric_vector_<T>::operator*=(rhs);
	return (*this);
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::operator/=(const T& rhs)
{
	numeric_vector_<T>::operator/=(rhs);
	return (*this);
}

template<typename T>
inline numeric_matrix_<T> operator+(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs)
{	
	assert(numeric_matrix_<T>::size_check(rhs, lhs), "...");
	return numeric_matrix_<T>((const numeric_vector_<T>&)lhs + (const numeric_vector_<T>&)rhs, rhs.rows_count(), rhs.cols_count());
}

template<typename T>
inline numeric_matrix_<T> operator-(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs)
{
	assert(numeric_matrix_<T>::size_check(rhs, lhs), "...");
	return numeric_matrix_<T>((const numeric_vector_<T>&)lhs - (const numeric_vector_<T>&)rhs, rhs.rows_count(), rhs.cols_count());
}

template<typename T>
inline numeric_matrix_<T> operator^(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs)
{
	assert(numeric_matrix_<T>::size_check(rhs, lhs), "...");
	return numeric_matrix_<T>((const numeric_vector_<T>&)lhs * (const numeric_vector_<T>&)rhs, rhs.rows_count(), rhs.cols_count());
}

template<typename T>
inline numeric_matrix_<T> operator*(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs)
{
	assert(numeric_matrix_<T>::size_check(rhs, lhs), "...");

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
	assert(numeric_matrix_<T>::size_check(rhs, lhs), "...");
	return lhs  * numeric_matrix_<T>::invert(rhs);
}

template<typename T>
inline numeric_matrix_<T> operator+(const T& lhs, const numeric_matrix_<T>& rhs)
{
	return numeric_matrix_<T>(lhs + (const numeric_vector_<T>&)rhs, rhs.rows_count(), rhs.cols_count());
}

template<typename T>
inline numeric_matrix_<T> operator-(const T& lhs, const numeric_matrix_<T>& rhs)
{
	return numeric_matrix_<T>(lhs - (const numeric_vector_<T>&)rhs, rhs.rows_count(), rhs.cols_count());
}

template<typename T>
inline numeric_matrix_<T> operator*(const T& lhs, const numeric_matrix_<T>& rhs)
{
	return numeric_matrix_<T>(lhs * (const numeric_vector_<T>&)rhs, rhs.rows_count(), rhs.cols_count());
}

template<typename T>
inline numeric_matrix_<T> operator/(const T& lhs, const numeric_matrix_<T>& rhs)
{
	return numeric_matrix_<T>(lhs / (const numeric_vector_<T>&)rhs, rhs.rows_count(), rhs.cols_count());
}

template<typename T>
inline numeric_matrix_<T> operator+(const numeric_matrix_<T>& lhs, const T& rhs)
{
	return numeric_matrix_<T>((const numeric_vector_<T>&)lhs + rhs, lhs.rows_count(), lhs.cols_count());
}

template<typename T>
inline numeric_matrix_<T> operator-(const numeric_matrix_<T>& lhs, const T& rhs)
{
	return numeric_matrix_<T>((const numeric_vector_<T>&)lhs - rhs, lhs.rows_count(), lhs.cols_count());
}

template<typename T>
inline numeric_matrix_<T> operator*(const numeric_matrix_<T>& lhs, const T& rhs)
{
	return numeric_matrix_<T>((const numeric_vector_<T>&)lhs * rhs, lhs.rows_count(), lhs.cols_count());
}

template<typename T>
inline numeric_matrix_<T> operator/(const numeric_matrix_<T>& lhs, const T& rhs)
{
	return numeric_matrix_<T>((const numeric_vector_<T>&)lhs / rhs, lhs.rows_count(), lhs.cols_count());
}

template<typename T>
inline bool operator==(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs)
{
	assert(numeric_matrix_<T>::size_check(rhs, lhs), "...");
	return (const numeric_vector_<T>&)lhs == (const numeric_vector_<T>&)rhs;
}

template<typename T>
inline bool operator!=(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs)
{
	assert(numeric_matrix_<T>::size_check(rhs, lhs), "...");
	return (const numeric_vector_<T>&)lhs != (const numeric_vector_<T>&)rhs;
}

template<typename T>
inline bool operator>(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs)
{
	assert(numeric_matrix_<T>::size_check(rhs, lhs), "...");
	return (const numeric_vector_<T>&)lhs > (const numeric_vector_<T>&)rhs;
}

template<typename T>
inline bool operator<(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs)
{
	assert(numeric_matrix_<T>::size_check(rhs, lhs), "...");
	return (const numeric_vector_<T>&)lhs < (const numeric_vector_<T>&)rhs;
}

template<typename T>
inline bool operator>=(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs)
{
	assert(numeric_matrix_<T>::size_check(rhs, lhs), "...");
	return (const numeric_vector_<T>&)lhs >= (const numeric_vector_<T>&)rhs;
}

template<typename T>
inline bool operator<=(const numeric_matrix_<T>& lhs, const numeric_matrix_<T>& rhs)
{
	assert(numeric_matrix_<T>::size_check(rhs, lhs), "...");
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
	const T* data = rhs.data();
	stream << "{\n";
	for (row = 0; row < rhs.rows_count(); row++)
	{
		stream << "\t{";
		for (col = 0; col < rhs.cols_count(); col++)
		{
			stream << std::setw(10) << rational_str(data[row * rhs.cols_count() + col], false);
			if (col < rhs.cols_count() - 1)
				stream << ";";
			else
				stream << "}";
		}
		if (row < rhs.rows_count() - 1)
			stream << ";\n";
		else
			stream << "\n}";
	}
	// TODO: вставьте здесь оператор return
	return stream;
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::add_col(const numeric_vector_<T>& col)
{
	if (rows_count() != col.filling())return(*this);
	int insert_index = cols_count();
	for (const T& item : col)
	{
		template_vector_<T>::insert(insert_index, item);
		insert_index += cols_count();
	}
	this->m_cols += 1;
	return(*this);
}

template<typename T>
inline numeric_matrix_<T>& numeric_matrix_<T>::add_row(const numeric_vector_<T>& row)
{
	if (cols_count() != row.filling())return(*this);
	for (const T& item: row) template_vector_<T>::push_back(item);
	this->m_cols += 1;
	return(*this);
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

	if (std::abs(det) < ACCURACY) throw std::runtime_error("Matrix is probably singular :: unable to solve A^-1 b = x"); // return numeric_vector_<T>({ 0.0 });
	
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
	if (matrix.rows_count() != matrix.cols_count()) throw std::runtime_error("error :: matrix inversion :: non square matrix");
	
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
	if (matrix.rows_count() != matrix.cols_count()) throw std::runtime_error("error :: matrix inversion :: non square matrix");

	numeric_matrix_<T> low(1, 1), up(1, 1); //  inv(1, 1);

	lu(matrix, low, up);

	T det = T{ 1.0 };
	for (int rows = 0; rows < low.rows_count(); rows++)
		det *= (low.unchecked_access(rows, rows) * low.unchecked_access(rows, rows));

	if (std::abs(det) < ACCURACY) throw std::runtime_error("mathUtils.Matrix is probably singular :: unable to calculate invert matrix");

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
inline numeric_matrix_<T> numeric_matrix_<T>::zeros(const int& n_rows, const int& m_cols)
{
	return numeric_matrix_<T>(n_rows, m_cols);
}

template<typename T>
inline numeric_matrix_<T> numeric_matrix_<T>::identity(const int& n_rows, const int& n_cols)
{
	numeric_matrix_<T> matrix = numeric_matrix_<T>::zeros(n_rows, m_cols);
	for (int index = 0; index < MIN(n_rows, n_cols); index++)matrix.unchecked_access(index, index) = T{ 1.0 };
	return matrix;
}

template<typename T>
inline numeric_matrix_<T> numeric_matrix_<T>::zeros(const int& size)
{
	return numeric_matrix_<T>::zeros(size, size);
}

template<typename T>
inline numeric_matrix_<T> numeric_matrix_<T>::identity(const int& size)
{
	return numeric_matrix_<T>::identity(size, size);
}

void numeric_matrix_test();

void numeric_matrix_test()
{
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
	for (auto const& v : rhs.cols())
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

	// std::cout << "mag(lhs)       : " << lhs.magintude() << "\n";
	// std::cout << "dot(lhs, rhs)  : " << double_vector::dot(lhs, rhs) << "\n";
	// std::cout << "dir(lhs, rhs)  : " << double_vector::direction(lhs, rhs) << "\n";
	// std::cout << "normalized(lhs): " << lhs.normalized() << "\n";
	// std::cout << "normalize (lhs): " << lhs.normalize() << "\n";
	// std::cout << "gradient  (lhs): " << double_vector::gradient(test_f, lhs, 1e-9) << "\n";
	// std::cout << "partial2  (lhs): " << double_vector::partial2(test_f, lhs, 0, 0, 1e-9) << "\n";
	// std::cout << "lhs == rhs     : " << (lhs == rhs) << "\n";
	// std::cout << "lhs != rhs     : " << (lhs != rhs) << "\n";
	// std::cout << "lhs > rhs      : " << (lhs > rhs) << "\n";
	// std::cout << "lhs < rhs      : " << (lhs < rhs) << "\n";
	// std::cout << "lhs >= rhs     : " << (lhs >= rhs) << "\n";
	// std::cout << "lhs <= rhs     : " << (lhs <= rhs) << "\n";
	// std::cout << "lhs >= 100     : " << (lhs >= 100.0) << "\n";
	// std::cout << "lhs <= 100     : " << (lhs <= 100.0) << "\n";
	// std::cout << "lhs            : " << lhs << "\n";
	// lhs[slice(3, 6)] = double_vector({ 13.0, 13.0, 13.0 });
	// std::cout << "lhs            : " << lhs << "\n";

}



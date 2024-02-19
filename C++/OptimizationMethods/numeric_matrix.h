#pragma once
#include "numeric_vector.h"

template<typename T>   class    numeric_matrix_;
typedef numeric_matrix_<double> double_matrix;
typedef numeric_matrix_<float>  float_matrix;
typedef numeric_matrix_<int>    int_matrix;

#define matrices_pair pair_<numeric_matrix_<T>, numeric_matrix_<T>>;
#define MAT_MUL_SZIE_CHECK 0
#define MAT_SIZE_CHECK 1

template<typename T>
class numeric_matrix_ : public numeric_vector_<T>
{

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
	uint32_t rows_count() const { return m_rows;};
	uint32_t cols_count() const { return m_cols;};

	numeric_matrix_<T>& add_col(const numeric_vector_<T>& col);
	numeric_matrix_<T>& add_row(const numeric_vector_<T>& row);
	// T trace      ()const;
	// T determinant()const;
	numeric_matrix_<T>& transpose();
	numeric_matrix_<T>& invert   ();

	static numeric_vector_<T> linsolve (const numeric_matrix_<T>& low, const numeric_matrix_<T>& up, const numeric_vector_<T>& b);
	static numeric_vector_<T> linsolve (const numeric_matrix_<T>& matrix, const numeric_vector_<T>& b);
	// static uint32_t           rank     (const numeric_matrix_<T>& matrix);
	static numeric_matrix_<T> transpose(const numeric_matrix_<T>& matrix);
	static numeric_matrix_<T> invert   (const numeric_matrix_<T>& matrix);
	// static numeric_matrix_<T> mexp     (const numeric_matrix_<T>& matrix);
	static numeric_matrix_<T> zeros    (const int& n_rows, const int& m_cols);
	static numeric_matrix_<T> identity (const int& n_rows, const int& m_cols);
	static numeric_matrix_<T> zeros    (const int& size);
	static numeric_matrix_<T> identity (const int& size);
	static void               lu       (const numeric_matrix_<T>& matrix, numeric_matrix_<T>& low, numeric_matrix_<T>& up);

	// vector_indices cols_indices() const {
	// 	return vector_indices(0, this->cols_count(), 1);
	// };

	// vector_indices rows_indices() const {
	// 	return vector_indices(0, this->rows_count(), 1);
	// };

	// vector_values<T> rows_values() const {
	// 	return vector_values<T>(m_data, indices());
	// };
	// static matrices_pair      qr       (const numeric_matrix_<T>& matrix);
	// static numeric_matrix_<T> svd      (const numeric_matrix_<T>& matrix);
	// static numeric_matrix_<T> chol     (const numeric_matrix_<T>& matrix);

	numeric_matrix_<T>& operator=(const numeric_matrix_<T>& rhs);
	numeric_matrix_<T>& operator=(numeric_matrix_<T>&& rhs)noexcept;


	numeric_matrix_<T>& operator+=(const numeric_matrix_<T>& rhs);
	numeric_matrix_<T>& operator-=(const numeric_matrix_<T>& rhs);
	numeric_matrix_<T>& operator*=(const numeric_matrix_<T>& rhs);
	numeric_matrix_<T>& operator/=(const numeric_matrix_<T>& rhs);

	numeric_matrix_<T>& operator+=(const T& rhs);
	numeric_matrix_<T>& operator-=(const T& rhs);
	numeric_matrix_<T>& operator*=(const T& rhs);
	numeric_matrix_<T>& operator/=(const T& rhs);

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

	if (numeric_matrix_<T>::size_check(matrix, low))low = zeros(rows_count, cols_count);
	
	if (numeric_matrix_<T>::size_check(matrix, up))	 up = zeros(rows_count, cols_count);

	int i = 0, j = 0, k = 0;

	for (i = 0; i < rows_count; i++)
	{
		for (j = 0; j < rows_count; j++)
		{
			if (j >= i)
			{
				low.unchecked_access(j, i) = matrix.unchecked_access(j, i);
				for (k = 0; k < i; k++)
					low.unchecked_access(j, i) -= low.unchecked_access(j, k) * up.unchecked_access(j, i);
			}
		}

		for (j = 0; j < cols_count; j++)
		{
			if (j < i) continue;

			if (j == i)
			{
				up.unchecked_access(i, j) = T{ 1.0 };
				continue;
			}
			up.unchecked_access(i, j) = matrix.unchecked_access(i, j) / low.unchecked_access(i, i);
			for (k = 0; k < i; k++) 
				up.unchecked_access(i, j) -= low.unchecked_access(i, k) * up.unchecked_access(k, j) / low.unchecked_access(i, i);
		}
	}
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
	// assert(numeric_matrix_<T>::size_check(rhs, lhs), "...");
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
	int  index = 0;
	int  col = 0;
	int  row = 0;
	const T* data = rhs.data();
	stream << "\n{\n";
	for (row = 0; row < rhs.rows_count(); row++)
	{
		stream << "\t{";
		for (col = 0; col < rhs.cols_count(); col++)
		{
			stream << std::setw(8) << rational_str(data[row * rhs.cols_count() + col]);
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

	for (int i = 0; i < up.rows_count(); i++) det *= (up.unchecked_access(i, i) * up.unchecked_access(i, i));

	if (fabs(det) < N_DIM_ACCURACY) return numeric_vector_<T>({0.0});
	
	numeric_vector_<T> x(b.filling()), z(b.filling());

	T tmp;

	for (int i = 0; i < z.filling(); i++)
	{
		tmp = T{ 0.0f };

		for (int j = 0; j < i; j++)
			tmp += z.unchecked_access(j) * low.unchecked_access(i, j);

		z.unchecked_access(i) = (b.unchecked_access(i) - tmp) / low.unchecked_access(i, i);
	}

	for (int i = z.filling() - 1; i >= 0; i--)
	{
		tmp = 0.0;
		for (int j = i + 1; j < z.filling(); j++)
			tmp += x.unchecked_access(j) * up.unchecked_access(i, j);
		x.unchecked_access(i) = (z.unchecked_access(i) - tmp);
	}
	//  x.unchecked_access(3) = 0.0;
	//std::cout << "x: " << x << "\n";
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

	numeric_vector_<T> b(matrix.rows_count()), col(matrix.rows_count());

	numeric_matrix_<T> inv = numeric_matrix_<T>::zeros(matrix.rows_count());

	for (int i = 0; i < matrix.cols_count(); i++)
	{
		b.unchecked_access(i) = T{ 1.0 };
		col = numeric_matrix_<T>::linsolve(low, up, b);
		if (col.filling() == 1 && col[0] == T{ 0.0 })
			throw std::runtime_error("error :: unable to find matrix inversion");
		b.unchecked_access(i) = T{ 0.0 };
		for (int j = 0; j < matrix.rows_count(); j++)
			inv.unchecked_access(j, i) = col.unchecked_access(j);
	}
	std::cout << "inv:\n" << inv << "\n";

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
	std::cout << "lhs            : " << lhs << "\n";
	std::cout << "rhs            : " << rhs << "\n";
	std::cout << "rhs - copy     : " << double_matrix(rhs) << "\n";
	std::cout << "lhs + rhs      : " << lhs + rhs << "\n";
	std::cout << "lhs - rhs      : " << lhs - rhs << "\n";
	std::cout << "lhs * rhs      : " << lhs * rhs << "\n";
	// std::cout << "lhs / rhs      : " << lhs / lhs << "\n";
	// 
	std::cout << "2 + rhs        : " << 2.0 + rhs << "\n";
	std::cout << "2 - rhs        : " << 2.0 - rhs << "\n";
	std::cout << "2 * rhs        : " << 2.0 * rhs << "\n";
	std::cout << "2 / rhs        : " << 2.0 / rhs << "\n";
	// 
	std::cout << "rhs + 2        : " << rhs + 2.0 << "\n";
	std::cout << "rhs - 2        : " << rhs - 2.0 << "\n";
	std::cout << "rhs * 2        : " << rhs * 2.0 << "\n";
	std::cout << "rhs / 2        : " << rhs / 2.0 << "\n";
	// 
	std::cout << "lhs += rhs     : " << (lhs += rhs) << "\n";
	std::cout << "lhs -= rhs     : " << (lhs -= rhs) << "\n";
	std::cout << "lhs *= rhs     : " << (lhs *= rhs) << "\n";
	// std::cout << "lhs /= rhs     : " << (lhs /= rhs) << "\n";
	// 
	std::cout << "lhs += 2.0     : " << (lhs += 2.0) << "\n";
	std::cout << "lhs -= 2.0     : " << (lhs -= 2.0) << "\n";
	std::cout << "lhs *= 2.0     : " << (lhs *= 2.0) << "\n";
	std::cout << "lhs /= 2.0     : " << (lhs /= 2.0) << "\n";

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



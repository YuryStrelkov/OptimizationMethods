#pragma once
#include "numerics/linalg/numeric_matrix.h"
#include "numerics/linalg/numeric_vector.h"
#include "numerics/rational/rational.h"

////////////////////
/// Lab. work #5 ///
////////////////////
namespace sm
{
	constexpr I32 EQUAL = 0;
	constexpr I32 LESS_EQUAL = -1;
	constexpr I32 MORE_EQUAL = 1;

#define SIMPLEX_MAX  0
#define SIMPLEX_MIN  1

	class simplex
	{
	private:
		/// <summary>
		/// список знаков в неравенств в системе ограничений
		/// </summary>
		vector_i32 m_inequations;

		/// <summary>
		/// список индексов переменных которые войдут в целевую функию, модифицируя ее
		/// </summary>
		vector_i32 m_virtual_args_indices;

		/// <summary>
		///индексы естественных переменных
		/// </summary>
		vector_i32 m_natural_args_indices;

		/// <summary>
		/// список индексов текущих базисных переменных 
		/// </summary>
		vector_i32 m_basis_args_indices;

		/// <summary>
		/// Симплекс таблица
		/// </summary>
		matrix_f64 m_simplex_table;

		/// <summary>
		/// матрица ограничений
		/// </summary>
		matrix_f64 m_bounds_matrix;

		/// <summary>
		/// вектор ограничений
		/// </summary>
		vector_f64 m_bounds_vector;

		/// <summary>
		/// вектор стоимостей
		/// </summary>
		vector_f64 m_prices_vector;

		/// <summary>
		/// режим поиска решения
		/// </summary>
		UI8 m_problem_type = SIMPLEX_MAX;

		/// <summary>
		/// Проверяет оптимальность текущего опорного плана. Исследуется положительность 
		/// симплекс-разностей в последней строке СТ в диапазоне от 1:n-1.
		/// Если целевая функция была модифицирована, то исследует две последних строки.
		/// Если среди элементов от 1:n-1 в последней строке нет отрицательных, то проверяет 
		/// на неотрицательность только те элементы предпоследней строки, которые не являются
		/// искусственными.
		/// </summary>
		/// <param name="A">СМ таблицa</param>
		/// <param name="m_problem_type"></param>
		/// <returns></returns>
		bool is_plan_optimal()const;

		/// <summary>
		/// Определяет ведущий столбец. Среди элементов строки симплекс-разностей ищет максимальны по модулю 
		/// отрицательный элемент. Если целевая функция была модифицирована и среди последней строки нет отрицательных
		/// элементов, то посик таковых будет продолжен среди только тех элементов предпоследней строки, которые не
		/// являются искусственными.
		/// </summary>
		/// <param name="A"></param>
		/// <returns></returns>
		I32 get_main_col()const;

		/// <summary>
		/// Определяет ведущую строку 
		/// </summary>
		/// <param name="simplex_col">ведущий столбец</param>
		/// <param name="A">СМ таблица</param>
		/// <returns></returns>
		I32 get_main_row(const I32 main_col)const;

		/// <summary>
		/// строит виртуальный базисный вектор
		/// </summary>
		/// <param name="inequation_id"></param>
		/// <param name="_ineq"></param>
		/// <param name="col_index"></param>
		/// <param name="col_index_aditional"></param>
		bool build_virtual_basis_column(const I32 inequation_id, const I32 ineq, I32& col_index, I32& col_index_aditional);

		/// <summary>
		/// Строит СМ таблицу для задачи вида:
		/// Маирица системы ограниченй:
		///		|u 0 0|	
		/// A = |0 v 0|
		///		|0 0 w|
		/// Вектор ограничений
		///		|a|	
		/// b = |d|
		///		|f|
		/// с -коэффициенты целевой функции 
		/// f = (x,c)->extr
		///	|u 0 0|   |x| <= |b|
		/// |0 v 0| * |x| >= |f|
		///	|0 0 w|   |x| =  |d|
		/// 
		///  СМ таблицу из A,b,c параметров
		/// </summary>
		/// <param name="A"> Ax <= b   -> (A|I)(x|w) = b </param>
		/// <param name="c"> (c,x) ->((-c|0),(x|w)) </param>
		/// <param name="ineq"> знак неравентсва =, >=, <= </param>
		/// <param name="b"></param>
		///( A|I)  b
		///(-c|0)  F(x,c)
		void build_simplex_table();

		bool exclude_virtual_args();

		bool validate_solution()const;

	public:
		/// <summary>
		/// количество аргументов исходной целевой функции
		/// </summary>
		I32 natural_args_count()const;

		inline const matrix_f64& bounds_matrix()const;

		inline const vector_f64& bounds_vector()const;

		inline const vector_f64& prices_vector()const;

		inline const vector_i32& inequations()const;

		inline const vector_i32& basis_args_indices()const;

		inline const matrix_f64& simplex_table()const;

		inline bool is_target_function_modified()const;
		/// <summary>
		/// Выводит текущее решение СМ таблицы для не искусственных переменных
		/// </summary>
		/// <param name="A">СМ таблица</param>
		/// <param name="basis">список базисных параметров</param>
		/// <param name="n_agrs">количество исходных переменных</param>
		/// <returns></returns>
		vector_f64 current_simplex_solution(const bool only_natural_args = false)const;
		vector_f64 solve(const I32 m_problem_type = SIMPLEX_MAX);
		simplex(const matrix_f64& bounds_matrix, const vector_f64& prices_vector, const vector_i32& inequations, const vector_f64& bounds_vector);
		simplex(const matrix_f64& bounds_matrix, const vector_f64& prices_vector, const vector_f64& bounds_vector);

		simplex(matrix_f64&& bounds_matrix, vector_f64&& prices_vector, vector_i32&& inequations, vector_f64&& bounds_vector);
		simplex(matrix_f64&& bounds_matrix, vector_f64&& prices_vector, vector_f64&& bounds_vector);

		friend std::ostream& operator<<(std::ostream& stream, const simplex& s);
	};

	std::ostream& operator<<(std::ostream& stream, const simplex& s)
	{
		/// <summary>
		///	  (x,w)	
		/// w (A|I)  b
		///   (c|i)  F(x,c)
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <param name="c"></param>

		const  matrix_f64& table = s.simplex_table();

		const bool targeFuncMod = s.is_target_function_modified();

		const vector_i32& m_basis_args_indices = s.basis_args_indices();

		if (table.size() == 0)
		{
			return stream;
		}

		const char separator = ' ';

		const I32 colom_title_w = 6;

		const I32 colom_w = 12;

		stream << std::left << std::setw(colom_title_w) << std::setfill(separator) << "";

		I32 i = 0;

		for (; i < table.cols_count() - 1; i++)
		{
			stream << std::left << std::setw(colom_w) << std::setfill(separator) << "| x " + std::to_string(i + 1);
		}

		stream << std::left << std::setw(colom_w) << std::setfill(separator) << "| b";

		stream << "\n";

		I32 row = 0;

		for (; row < table.rows_count(); row++)
		{

			if (targeFuncMod)
			{
				if (row == table.rows_count() - 2)
				{
					stream << std::left << std::setw(colom_title_w) << std::setfill(separator) << " d0 ";
				}
				else if (row == table.rows_count() - 1)
				{
					stream << std::left << std::setw(colom_title_w) << std::setfill(separator) << " d1 ";
				}
				else
				{
					stream << std::left << std::setw(colom_title_w) << std::setfill(separator) << " x " + std::to_string(m_basis_args_indices[row] + 1);
				}
			}
			else
			{
				if (row == table.rows_count() - 1)
				{
					stream << std::left << std::setw(colom_title_w) << std::setfill(separator) << " d ";
				}
				else
				{
					stream << std::left << std::setw(colom_title_w) << std::setfill(separator) << " x " + std::to_string(m_basis_args_indices[row] + 1);
				}
			}

			for (I32 col = 0; col < table.cols_count(); col++)
			{
				const F64 value = table(row, col);
				if (value >= 0)
				{
					stream << std::left << std::setw(colom_w) << std::setfill(separator) << "| " + rational_str(value);
					continue;
				}
				stream << std::left << std::setw(colom_w) << std::setfill(separator) << "|" + rational_str(value);
			}

			stream << "\n";
		}
		stream << "\n";

		return stream;
	}

	bool simplex::is_plan_optimal()const
	{
		/// <summary>
		/// Проверяем значения последней строки сиплекс-разностей
		/// на положительность. Если все положительны, то план оптимален.
		/// </summary>

		const I32 row_index = m_simplex_table.rows_count() - 1;
		bool optimal = true;
		for (I32 col = 0; col < m_simplex_table.cols_count() - 1; col++)
		{
			optimal = m_simplex_table(row_index, col) < 0;
			if (!optimal) break;
		}

		/// <summary>
		/// если мы модифицировали целевую функцию, то среди списка естественнхых
		/// агументов проверям на положительнность предпослднюю строку симплекс-разностей 
		/// </summary>

		if (is_target_function_modified())
		{
			const I32 row_index = m_simplex_table.rows_count() - 2;
			for (const I32 col : m_natural_args_indices.values())
			{
				optimal &= m_simplex_table(row_index, col) < 0;
				if (!optimal) break;
			}
		}

		return optimal;
	}

	I32 simplex::get_main_col()const
	{
		const int32_t n_rows = m_simplex_table.rows_count();
		const int32_t n_cols = m_simplex_table.cols_count();

		F64 delta = 0;
		F64 value = 0;
		I32    index = -1;

		for (I32 col = 0; col < n_cols - 1; col++)
		{
			value = m_simplex_table(n_rows - 1, col);
			if (value >= delta)continue;
			delta = value;
			index = col;
		}

		if (is_target_function_modified() && index == -1)
		{
			for (const I32 col : m_natural_args_indices.values())
			{
				value = m_simplex_table(n_rows - 2, col);
				if (value >= delta)continue;
				delta = value;
				index = col;
			}
		}
		return index;
	}

	I32 simplex::get_main_row(const I32 simplex_col)const
	{
		I32 index = -1;
		F64 delta = 1e12;
		F64 a_ik;
		I32 b_index = m_simplex_table.cols_count() - 1; // m_simplex_table[0].size() - 1;
		I32 rows_n = is_target_function_modified() ? m_simplex_table.rows_count() - 2 : m_simplex_table.rows_count() - 1;

		for (I32 row = 0; row < rows_n; row++)
		{
			a_ik = m_simplex_table(row, simplex_col);
			if (a_ik < 0)continue;
			if (m_simplex_table(row, b_index) / a_ik > delta) continue;
			delta = m_simplex_table(row, b_index) / a_ik;
			index = row;
		}

		return index;
	}

	bool simplex::build_virtual_basis_column(const I32 inequation_id, const I32 inequation_type, I32& col_index, I32& col_index_aditional)
	{   
		const I32 n_cols = m_simplex_table.cols_count();

		if (inequation_type == EQUAL)
		{
			m_simplex_table.add_col();
			m_simplex_table(inequation_id, n_cols) = 1.0;
			col_index = col_index_aditional = n_cols;
			return true;
		}
		if (inequation_type == MORE_EQUAL)
		{
			m_simplex_table.add_col();
			m_simplex_table.add_col();
			m_simplex_table(inequation_id, n_cols - 1) = -1.0;
			m_simplex_table(inequation_id, n_cols) = 1.0;
			col_index_aditional = n_cols;
			col_index = n_cols - 1;
			return false;
		}
		m_simplex_table.add_col();
		m_simplex_table(inequation_id, n_cols) = 1.0;
		col_index_aditional = -1;
		col_index = n_cols;
		return true;
	}

	void simplex::build_simplex_table()
	{
		m_simplex_table = m_bounds_matrix;
		///
		/// Если среди вектора b есть отрицательные значения, то соответствующие строки
		/// матрицы ограничений умножаем на мину один и меняем знак сравнения
		///
		for (I32 row = 0; row < m_simplex_table.rows_count(); row++)
		{
			if (m_bounds_vector[row] >= 0)continue;
			m_inequations[row] *= -1;
			m_bounds_vector[row] *= -1;
			m_simplex_table.get_row(row) *= -1.0;
		}

		for (I32 index = 0; index < m_prices_vector.size(); index++)
			m_natural_args_indices.push_back(index);

		/// <summary>
		/// построение искуственного базиса
		/// </summary>
		I32 basis_arg_id;
		I32 basis_arg_id_add;
		for (I32 inequality_id = 0; inequality_id < m_inequations.size(); inequality_id++)
		{
			build_virtual_basis_column(inequality_id, m_inequations[inequality_id], basis_arg_id, basis_arg_id_add);

			m_natural_args_indices.push_back(basis_arg_id);

			if (basis_arg_id_add != -1)
			{
				m_basis_args_indices.push_back(basis_arg_id_add);
				m_virtual_args_indices.push_back(basis_arg_id_add);
				continue;
			}
			m_basis_args_indices.push_back(basis_arg_id);
		}

		/// <summary>
		/// добавим столбец ограницений
		/// </summary>
		m_simplex_table.add_col(m_bounds_vector);

		/// <summary>
		/// Построение симплекс разностей
		/// </summary>
		vector_f64 s_deltas(m_simplex_table.cols_count());/// [0] .size());

		s_deltas.apply_enumerate(m_prices_vector, [&](const I32 index, const F64 value) {return index < m_prices_vector.size() ? -value : 0.0; });

		if (m_problem_type == SIMPLEX_MAX)
			s_deltas.apply_enumerate(m_prices_vector, [&](const I32 index, const F64 value) {return index < m_prices_vector.size() ? -value : 0.0; });
		else
			s_deltas.apply_enumerate(m_prices_vector, [&](const I32 index, const F64 value) {return index < m_prices_vector.size() ? value : 0.0; });

		m_simplex_table.add_row(s_deltas);

		/// <summary>
		/// Если целевая функуция не была модифицирована
		/// </summary>
		if (!is_target_function_modified()) return;

		/// <summary>
		/// Если всё же была...
		/// </summary>
		vector_f64 s_deltas_add(m_simplex_table.cols_count()); //  m_simplex_table[0].size());

		for (const I32 index : m_virtual_args_indices.values()) s_deltas_add[index] = 1.0;

		m_simplex_table.add_row(s_deltas_add);
	}

	bool simplex::exclude_virtual_args()
	{
		if (!is_target_function_modified()) return false;

		const I32 last_row_id = m_simplex_table.rows_count() - 1;

		vector_f64 last_row = m_simplex_table.get_row(last_row_id);

		for (const I32 index : m_virtual_args_indices.values())
		{
			for (I32 row = 0; row < m_simplex_table.rows_count(); row++)
			{
				if (m_simplex_table(row, index) == 0) continue;
				const F64 arg = m_simplex_table(last_row_id, index) / m_simplex_table(row, index);
				last_row -= arg * m_simplex_table.get_row(row);
				break;
			}
		}
		return true;
	}

	bool simplex::validate_solution()const
	{
		F64 val = 0;

		I32 n_rows = is_target_function_modified() ? m_simplex_table.rows_count() - 2 : m_simplex_table.rows_count() - 1;

		I32 n_cols = m_simplex_table.cols_count() - 1;

		for (I32 index = 0; index < m_basis_args_indices.size(); index++)
		{
			if (m_basis_args_indices[index] >= natural_args_count())continue;
			val += m_simplex_table(index, n_cols) * m_prices_vector[m_basis_args_indices[index]];
		}
		if (m_problem_type == SIMPLEX_MAX)
		{
			if (abs(val - m_simplex_table(n_rows, n_cols)) < N_DIM_ACCURACY)
			{
				if (is_target_function_modified())
					return (abs(m_simplex_table(m_simplex_table.rows_count() - 1, n_cols)) < N_DIM_ACCURACY);
				return true;
			}
		}
		if (abs(val + m_simplex_table(n_rows, n_cols)) < N_DIM_ACCURACY)
		{
			if (is_target_function_modified())
				return (abs(m_simplex_table(m_simplex_table.rows_count() - 1, n_cols)) < N_DIM_ACCURACY);
			return true;
		}
		return false;
	}

	I32 simplex::natural_args_count()const
	{
		return m_prices_vector.size();
	}

	inline const matrix_f64& simplex::bounds_matrix()const
	{
		return m_bounds_matrix;
	}

	inline const vector_f64& simplex::bounds_vector()const
	{
		return m_bounds_vector;
	}

	inline const vector_f64& simplex::prices_vector()const
	{
		return m_prices_vector;
	}

	inline const vector_i32& simplex::inequations()const
	{
		return m_inequations;
	};

	inline const vector_i32& simplex::basis_args_indices()const
	{
		return m_basis_args_indices;
	};

	inline const matrix_f64& simplex::simplex_table() const { return m_simplex_table; };

	inline bool simplex::is_target_function_modified()const
	{
		return m_virtual_args_indices.size() != 0;
	}

	vector_f64 simplex::current_simplex_solution(const bool only_natural_args)const
	{
		vector_f64 solution(only_natural_args ? natural_args_count() : m_simplex_table.cols_count() - 1);

		for (I32 index = 0; index < m_basis_args_indices.size(); index++)
		{
			if (m_basis_args_indices[index] >= solution.size()) continue;

			solution[m_basis_args_indices[index]] = m_simplex_table(index, m_simplex_table.cols_count() - 1);
		}
		return solution;
	}

	vector_f64 simplex::solve(const I32 problem_type)
	{
		m_problem_type = problem_type;

		std::cout << "Simplex problem type: " << ((m_problem_type == SIMPLEX_MAX) ? "max\n" : "min\n");

		build_simplex_table();

		vector_f64 solution;

		F64 a_ik;

		I32 main_row;

		I32 main_col;

		std::cout << "Start simplex table:" << "\n";

		std::cout << *this;

		if (exclude_virtual_args())
		{
			// второй этап, если задача должна решаться двух проходным(двух этапным) алгоритмом
			std::cout << "Simplex table after args exclude:" << "\n";

			std::cout << *this;
		}

		while (!is_plan_optimal())
		{
			main_col = get_main_col();

			if (main_col == -1) break;

			main_row = get_main_row(main_col);

			if (main_row == -1)
			{
				/// Невозможность определить ведущую строку свидейтельствует о том, что обрасть поиска неограничена
				std::cout << "Unable to get main row. Simplex is probably boundless...\n";
				solution.clear();
				return solution;
			}

			m_basis_args_indices[main_row] = main_col;

			a_ik = m_simplex_table(main_row, main_col);

			vector_f64 main_row_vector = m_simplex_table.get_row(main_row) *= 1.0 / a_ik;
			// m_simplex_table[main_row] = m_simplex_table[main_row] * (1.0 / a_ik);
			for (I32 i = 0; i < m_simplex_table.rows_count(); i++)
			{
				if (i == main_row)continue;
				m_simplex_table.get_row(i) -= (m_simplex_table(i, main_col) * main_row_vector);
			}
			solution = current_simplex_solution();
#if _DEBUG
			std::cout << "a_main { " << main_row + 1 << ", " << main_col + 1 << " } = " << rational_str(a_ik) << "\n";
			std::cout << *this;
			std::cout << "current_solution" << solution << "\n";
			std::cout << "\n";
#endif
		}
		if (validate_solution())
		{
			solution = current_simplex_solution(true);
			/// формирование ответа
			std::cout << "solution : " << solution << "\n";
			return solution;
		}
		std::cout << "Simplex is unresolvable\n";
		/// значение целевой функции не равно ее значению от найденного плана
		solution.clear();
		return solution;
	}

	simplex::simplex(const matrix_f64& bounds_matrix, const vector_f64& prices_vector, const vector_i32& inequations, const vector_f64& bounds_vector)
	{
		if (bounds_vector.size() != inequations.size())
			throw std::runtime_error("Error simplex creation :: b.size() != inequalities.size()");
		if (bounds_matrix.rows_count() != inequations.size())
			throw std::runtime_error("Error simplex creation :: A.rows_number() != inequalities.size()");
		if (bounds_matrix.cols_count() != prices_vector.size())
			throw std::runtime_error("Error simplex creation :: A.cols_number() != price_coeffs.size()");
		m_bounds_vector = bounds_vector;
		m_bounds_matrix = bounds_matrix;
		m_prices_vector = prices_vector;
		m_inequations = inequations;
	}

	simplex::simplex(const matrix_f64& bounds_matrix, const vector_f64& prices_vector, const vector_f64& bounds_vector)
	{
		if (bounds_matrix.rows_count() != bounds_vector.size())
			throw std::runtime_error("Error simplex creation :: A.rows_number() != bouns_coeffs.size()");
		if (bounds_matrix.cols_count() != prices_vector.size())
			throw std::runtime_error("Error simplex creation :: A.cols_number() != price_coeffs.size()");
		m_bounds_vector = bounds_vector;
		m_bounds_matrix = bounds_matrix;
		m_prices_vector = prices_vector;
		m_inequations = vector_i32(bounds_vector.size());
		m_inequations.fill([](const I32) {return LESS_EQUAL; });
	}

	simplex::simplex(matrix_f64&& bounds_matrix, vector_f64&& prices_vector, vector_i32&& inequations, vector_f64&& bounds_vector) 
	{
		if (bounds_vector.size() != inequations.size())
			throw std::runtime_error("Error simplex creation :: b.size() != inequalities.size()");
		if (bounds_matrix.rows_count() != inequations.size())
			throw std::runtime_error("Error simplex creation :: A.rows_number() != inequalities.size()");
		if (bounds_matrix.cols_count() != prices_vector.size())
			throw std::runtime_error("Error simplex creation :: A.cols_number() != price_coeffs.size()");
		m_bounds_vector = std::move(bounds_vector);
		m_bounds_matrix = std::move(bounds_matrix);
		m_prices_vector = std::move(prices_vector);
		m_inequations   = std::move(inequations  );
	}
	simplex::simplex(matrix_f64&& bounds_matrix, vector_f64&& prices_vector, vector_f64&& bounds_vector)
	{
		if (bounds_matrix.rows_count() != bounds_vector.size())
			throw std::runtime_error("Error simplex creation :: A.rows_number() != bouns_coeffs.size()");
		if (bounds_matrix.cols_count() != prices_vector.size())
			throw std::runtime_error("Error simplex creation :: A.cols_number() != price_coeffs.size()");
		m_bounds_vector = std::move(bounds_vector);
		m_bounds_matrix = std::move(bounds_matrix);
		m_prices_vector = std::move(prices_vector);
		m_inequations = vector_i32(bounds_vector.size());
		m_inequations.fill([](const I32) {return LESS_EQUAL; });
	}
}

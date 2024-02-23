#pragma once
#include "vector_utils.h"
#include "matrix_utils.h"
#include "numeric_matrix.h"
#include "numeric_vector.h"
// #include "numeric_utils.h"
#include <iomanip>
#include <string>
#include "rational.h"
// TODO vec_n / mat_mn -> numeric_vector / numeric_matrix
////////////////////
/// Lab. work #5 ///
////////////////////
namespace sm
{
	constexpr int EQUAL = 0;
	constexpr int LESS_EQUAL = -1;
	constexpr int MORE_EQUAL = 1;

#define SIMPLEX_MAX  0
#define SIMPLEX_MIN  1

	class simplex
	{
	private:
		/// <summary>
		/// список знаков в неравенств в системе ограничений
		/// </summary>
		int_vector _inequations;

		/// <summary>
		/// список индексов переменных которые войдут в целевую функию, модифицируя ее
		/// </summary>
		int_vector virtualArgsIds;

		/// <summary>
		///индексы естественных переменных
		/// </summary>
		int_vector naturalArgsIds;

		/// <summary>
		/// список индексов текущих базисных переменных 
		/// </summary>
		int_vector basisArgsIds;

		/// <summary>
		/// Симплекс таблица
		/// </summary>
		double_matrix simplex_t;

		/// <summary>
		/// матрица ограничений
		/// </summary>
		double_matrix bounds_m;

		/// <summary>
		/// вектор ограничений
		/// </summary>
		double_vector bounds_v;

		/// <summary>
		/// вектор стоимостей
		/// </summary>
		double_vector prices_v;

		/// <summary>
		/// режим поиска решения
		/// </summary>
		int mode = SIMPLEX_MAX;

		/// <summary>
		/// Проверяет оптимальность текущего опорного плана. Исследуется положительность 
		/// симплекс-разностей в последней строке СТ в диапазоне от 1:n-1.
		/// Если целевая функция была модифицирована, то исследует две последних строки.
		/// Если среди элементов от 1:n-1 в последней строке нет отрицательных, то проверяет 
		/// на неотрицательность только те элементы предпоследней строки, которые не являются
		/// искусственными.
		/// </summary>
		/// <param name="A">СМ таблицa</param>
		/// <param name="mode"></param>
		/// <returns></returns>
		bool isPlanOptimal       ()const;

		/// <summary>
		/// Определяет ведущий столбец. Среди элементов строки симплекс-разностей ищет максимальны по модулю 
		/// отрицательный элемент. Если целевая функция была модифицирована и среди последней строки нет отрицательных
		/// элементов, то посик таковых будет продолжен среди только тех элементов предпоследней строки, которые не
		/// являются искусственными.
		/// </summary>
		/// <param name="A"></param>
		/// <returns></returns>
		int getMainCol           ()const;

		/// <summary>
		/// Определяет ведущую строку 
		/// </summary>
		/// <param name="simplex_col">ведущий столбец</param>
		/// <param name="A">СМ таблица</param>
		/// <returns></returns>
		int getMainRow           (const int simplex_col)const;
		/// <summary>
		/// строит виртуальный базисный вектор
		/// </summary>
		/// <param name="ineq_id"></param>
		/// <param name="_ineq"></param>
		/// <param name="col_index"></param>
		/// <param name="col_index_aditional"></param>
		bool buildVirtualBasisCol(const int ineq_id, const int ineq, int& col_index, int& col_index_aditional);
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
		void buildSimplexTable   ();

		bool excludeVirtualArgs  ();

		bool validateSolution    ()const;

	public:
		/// <summary>
		/// количество аргументов исходной целевой функции
		/// </summary>
		int naturalArgsN ()const;

		inline const double_matrix& boundsMatrix ()const;

		inline const double_vector& boundsCoeffs ()const;

		inline const double_vector& pricesCoeffs ()const;

		inline const int_vector& inequations ()const;

		inline const int_vector& basisArgumentsIds ()const;

		inline const double_matrix& table ()const;

		inline bool isTargetFunctionModified ()const;
		/// <summary>
		/// Выводит текущее решение СМ таблицы для не искусственных переменных
		/// </summary>
		/// <param name="A">СМ таблица</param>
		/// <param name="basis">список базисных параметров</param>
		/// <param name="n_agrs">количество исходных переменных</param>
		/// <returns></returns>
		vec_n currentSimplexSolution (const bool only_natural_args = false)const;
		vec_n solve (const int mode = SIMPLEX_MAX);
		simplex (const double_matrix& a, const double_vector& c, const int_vector& _ineq, const double_vector& b);
		simplex (const double_matrix& a, const double_vector& c, const double_vector& b);
	
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

		const  double_matrix& table = s.table();

		const bool targeFuncMod = s.isTargetFunctionModified();

		const int_vector& basisArgsIds = s.basisArgumentsIds();

		if (table.size() == 0)
		{
			return stream;
		}

		const char separator = ' ';

		const int colom_title_w = 6;

		const int colom_w = 12;

		stream << std::left << std::setw(colom_title_w) << std::setfill(separator) << "";

		int i = 0;

		for (; i < table.cols_count() - 1; i++)
		{
			stream << std::left << std::setw(colom_w) << std::setfill(separator) << "| x " + std::to_string(i + 1);
		}

		stream << std::left << std::setw(colom_w) << std::setfill(separator) << "| b";

		stream << "\n";

		int row = 0;

		for (;row < table.rows_count(); row++)
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
					stream << std::left << std::setw(colom_title_w) << std::setfill(separator) << " x " + std::to_string(basisArgsIds[row] + 1);
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
					stream << std::left << std::setw(colom_title_w) << std::setfill(separator) << " x " + std::to_string(basisArgsIds[row] + 1);
				}
			}

			for (int col = 0; col < table.cols_count(); col++)
			{
				const double value = table.get(row, col);
				if (value >= 0)
				{
					stream << std::left << std::setw(colom_w) << std::setfill(separator) << "| " + rational::rational_str(value);
					continue;
				}
				stream << std::left << std::setw(colom_w) << std::setfill(separator) << "|" + rational::rational_str(value);
			}

			stream << "\n";
		}
		stream << "\n";

		return stream;
	}

	bool simplex::                              isPlanOptimal()const
	{
		/// <summary>
		/// Проверяем значения последней строки сиплекс-разностей
		/// на положительность. Если все положительны, то план оптимален.
		/// </summary>

		const int row_index = simplex_t.rows_count() - 2;

		bool opt = true; 

		for (int i = 0; i < simplex_t.cols_count() - 1; i++)
		{
			if (simplex_t.get(row_index, i) < 0)
			{
				opt = false;
				break;
			}
		}

		/// <summary>
		/// если мы модифицировали целевую функцию, то среди списка естественнхых
		/// агументов проверям на положительнность предпослднюю строку симплекс-разностей 
		/// </summary>

		if (isTargetFunctionModified())
		{
			if (!opt) return opt;
			
			const int row_index = simplex_t.rows_count() - 2;

			for (const int id : naturalArgsIds.values())
			{
				if (simplex_t.get(row_index, id) < 0)
				{
					opt = false;
					break;
				}
			}
		}

		return opt;
	}

	int simplex::                                  getMainCol()const
	{
		const double_vector& row = simplex_t[simplex_t.size() - 1];

		double delta = 0;

		int    index = -1;

		for (int i = 0; i < row.size() - 1; i++)
		{
			if (row[i] >= delta)continue;
			delta = row[i];
			index = i;
		}

		if (isTargetFunctionModified() && index == -1)
		{
			const double_vector& row_add = simplex_t[simplex_t.size() - 2];

			for (const int id : naturalArgsIds.values())
			{
				if (row_add[id] >= delta)continue;
				delta = row_add[id];
				index = id;
			}
		}
		return index;
	}

	int simplex::                                  getMainRow(const int simplex_col)const
	{
		double delta = 1e12;

		int index = -1;

		double a_ik;

		int b_index = simplex_t.cols_count() - 1; // simplex_t[0].size() - 1;

		int cntr = 0;

		int rows_n = isTargetFunctionModified() ? simplex_t.size() - 2 : simplex_t.size() - 1;

		for (int i = 0; i < rows_n; i++)
		{
			a_ik = simplex_t.get(i, simplex_col);

			if (a_ik < 0)
			{
				cntr++;
				continue;
			}
			if (simplex_t.get(i, b_index) / a_ik > delta)continue;
			delta = simplex_t.get(i, b_index) / a_ik;
			index = i;
		}

		return index;
	}

	bool simplex::buildVirtualBasisCol(const int ineq_id, const int _ineq, int& col_index, int& col_index_aditional)
	{
		if (_ineq == EQUAL)
		{
			simplex_t.add_col();
			simplex_t.get(ineq_id, simplex_t.cols_count() - 1) = 1.0;

			// for (int row = 0; row < simplex_t.size(); row++)
			// {
			// 	if (row == ineq_id)
			// 	{
			// 		simplex_t[row].push_back(1.0);
			// 		continue;
			// 	}
			// 	simplex_t[row].push_back(0.0);
			// }

			col_index = simplex_t.cols_count() - 1;// simplex_t[0].size() - 1;
			col_index_aditional = simplex_t.cols_count() - 1; //simplex_t[0].size() - 1;
			return true;
		}

		if (_ineq == MORE_EQUAL)
		{
			simplex_t.add_col();
			simplex_t.add_col();
			simplex_t.get(ineq_id, simplex_t.cols_count() - 1) = -1.0;
			simplex_t.get(ineq_id, simplex_t.cols_count() - 1) =  1.0;
			//for (int row = 0; row < simplex_t.size(); row++)
			//{
			//	if (row == ineq_id)
			//	{
			//		simplex_t[row].push_back(-1.0);
			//
			//		simplex_t[row].push_back(1.0);
			//
			//		continue;
			//	}
			//
			//	simplex_t[row].push_back(0.0);
			//
			//	simplex_t[row].push_back(0.0);
			//}
			col_index_aditional = simplex_t.cols_count() - 1;
			col_index = simplex_t.cols_count() - 2;
			return false;
		}

		simplex_t.add_col();
		simplex_t.get(ineq_id, simplex_t.cols_count() - 1) = 1.0;

		// for (int row = 0; row < simplex_t.size(); row++)
		// {
		// 	if (row == ineq_id)
		// 	{
		// 		simplex_t[row].push_back(1.0);
		// 		continue;
		// 	}
		// 	simplex_t[row].push_back(0.0);
		// }

		col_index_aditional = -1;
		col_index = simplex_t.cols_count() - 1; //  simplex_t[0].size() - 1;
		return true;
	}

	void simplex::buildSimplexTable()
	{
		simplex_t = bounds_m;
		///
		/// Если среди вектора b есть отрицательные значения, то соответствующие строки
		/// матрицы ограничений умножаем на мину один и меняем знак сравнения
		///
		for (int row = 0; row < simplex_t.rows_count(); row++)
		{
			if (bounds_v[row] >= 0)continue;

			_inequations[row] *= -1;

			bounds_v[row] *= -1;

			simplex_t[row] = simplex_t[row] * (-1.0);
		}


		for (int i = 0; i < prices_v.size(); i++)
		{
			naturalArgsIds.push_back(i);
		}
		/// <summary>
		/// построение искуственного базиса
		/// </summary>
		int basis_arg_id;
		int basis_arg_id_add;
		for (int ineq_id = 0; ineq_id < _inequations.size(); ineq_id++)
		{
			buildVirtualBasisCol(ineq_id, _inequations[ineq_id], basis_arg_id, basis_arg_id_add);

			naturalArgsIds.push_back(basis_arg_id);

			if (basis_arg_id_add != -1)
			{
				basisArgsIds.push_back(basis_arg_id_add);
				virtualArgsIds.push_back(basis_arg_id_add);
				continue;
			}

			basisArgsIds.push_back(basis_arg_id);
		}

		/// <summary>
		/// добавим столбец ограницений
		/// </summary>
		simplex_t.add_col(bounds_v);
		// for (int row = 0; row < simplex_t.size(); row++)
		// {
		// 	simplex_t[row].push_back(bounds_v[row]);
		// }

		/// <summary>
		/// Построение симплекс разностей
		/// </summary>

		double_vector s_deltas(simplex_t.cols_count());/// [0] .size());

		if (mode == SIMPLEX_MAX)
		{
			for (int j = 0; j < s_deltas.size(); j++) s_deltas[j] = j < prices_v.size() ? -prices_v[j] : 0.0;
		}
		else
		{
			for (int j = 0; j < s_deltas.size(); j++) s_deltas[j] = j < prices_v.size() ? prices_v[j] : 0.0;
		}

		simplex_t.add_row(s_deltas);

		/// <summary>
		/// Если целевая функуция не была модифицирована
		/// </summary>

		if (!isTargetFunctionModified()) return;

		/// <summary>
		/// Если всё же была...
		/// </summary>
		double_vector s_deltas_add(simplex_t.cols_count()); //  simplex_t[0].size());

		for (int j = 0; j < virtualArgsIds.size(); j++) s_deltas_add[virtualArgsIds[j]] = 1.0;

		simplex_t.add_row(s_deltas_add);
	}

	bool simplex::excludeVirtualArgs()
	{
		if (!isTargetFunctionModified()) return false;

		int last_row_id = simplex_t.size() - 1;

		for (int i = 0; i < virtualArgsIds.size(); i++)
		{
			for (int row = 0; row < simplex_t.size(); row++)
			{
				if (simplex_t.get(row,virtualArgsIds[i]) != 0)
				{
					double arg = simplex_t.get(last_row_id, virtualArgsIds[i]) / simplex_t.get(row, virtualArgsIds[i]);

					simplex_t[last_row_id] = simplex_t[last_row_id] - arg * simplex_t[row];

					break;
				}
			}
		}

		return true;
	}

	bool simplex::                           validateSolution()const
	{
		double val = 0;

		int n_rows = isTargetFunctionModified() ? simplex_t.size() - 2 : simplex_t.size() - 1;

		int n_cols = simplex_t.cols_count() - 1;

		for (int i = 0; i < basisArgsIds.size(); i++)
		{
			if (basisArgsIds[i] < naturalArgsN())
			{
				val += simplex_t.get(i, n_cols) * prices_v[basisArgsIds[i]];
			}
		}
		if (mode == SIMPLEX_MAX)
		{
			if (abs(val - simplex_t.get(n_rows, n_cols)) < N_DIM_ACCURACY)
			{
				if (isTargetFunctionModified())
				{
					return (abs(simplex_t.get(simplex_t.rows_count() - 1, n_cols)) < N_DIM_ACCURACY);
				}

				return true;
			}
		}
		if (abs(val + simplex_t.get(n_rows, n_cols)) < N_DIM_ACCURACY)
		{
			if (isTargetFunctionModified())
			{
				return (abs(simplex_t.get(simplex_t.rows_count() - 1, n_cols)) < N_DIM_ACCURACY);
			}
			return true;
		}
		return false;
	}

	int simplex::                                naturalArgsN()const
	{
		return prices_v.size();
	}

	inline const double_matrix& simplex::boundsMatrix()const
	{
		return bounds_m;
	}

	inline const double_vector& simplex::boundsCoeffs()const
	{
		return bounds_v;
	}

	inline const double_vector& simplex::pricesCoeffs()const
	{
		return prices_v;
	}

	inline const int_vector & simplex::inequations()const
	{
		return _inequations;
	};

	inline const int_vector& simplex::basisArgumentsIds()const
	{
		return basisArgsIds;
	};

	inline const double_matrix& simplex::table() const { return simplex_t; };

	inline bool simplex::isTargetFunctionModified()const
	{
		return virtualArgsIds.size() != 0;
	}

	vec_n simplex::currentSimplexSolution(const bool only_natural_args)const
	{
		vec_n solution(only_natural_args ? naturalArgsN() : simplex_t.cols_count() - 1);

		for (int i = 0; i < basisArgsIds.size(); i++)
		{
			if (basisArgsIds[i] >= solution.size()) continue;

			solution[basisArgsIds[i]] = simplex_t.get(i, simplex_t.cols_count() - 1);
		}
		return solution;
	}

	vec_n simplex::solve(const int mode)
	{
		this->mode = mode;

		std::cout << "Simplex problem type: " << ((mode == SIMPLEX_MAX) ? "max\n" : "min\n");

		buildSimplexTable();

		vec_n solution;

		double a_ik;

		int main_row;

		int main_col;

		std::cout << "Start simplex table:" << "\n";

		std::cout << *this;

		if (excludeVirtualArgs())
		{
			// второй этап, если задача должна решаться двух проходным(двух этапным) алгоритмом
			std::cout << "Simplex table after args exclude:" << "\n";

			std::cout << *this;
		}

		while (!isPlanOptimal())
		{
			main_col = getMainCol();

			if (main_col == -1) break;

			main_row = getMainRow(main_col);

			if (main_row == -1)
			{
				/// Невозможность определить ведущую строку свидейтельствует о том, что обрасть поиска неограничена
				std::cout << "Unable to get main row. Simplex is probably boundless...\n";
				solution.clear();
				return solution;
			}

			basisArgsIds[main_row] = main_col;

			a_ik = simplex_t.get(main_row, main_col);

			simplex_t[main_row] = simplex_t[main_row] * (1.0 / a_ik);

			for (int i = 0; i < simplex_t.size(); i++)
			{
				if (i == main_row)continue;

				simplex_t[i] = simplex_t[i] - simplex_t.get(i, main_col) * simplex_t[main_row];
			}
			solution = currentSimplexSolution();

#if _DEBUG
			std::cout << "a_main { " << main_row + 1 << ", " << main_col + 1 << " } = " << rational::rational_str(a_ik) << "\n";
			std::cout << *this;
			std::cout << "current_solution" << rational::rational_str(solution) << "\n";
			std::cout << "\n";
#endif
		}
		if (validateSolution())
		{
			solution = currentSimplexSolution(true);
			/// формирование ответа
			std::cout << "solution : " << rational::rational_str(solution) << "\n";
			return solution;
		}
		std::cout << "Simplex is unresolvable\n";
		/// значение целевой функции не равно ее значению от найденного плана
		solution.clear();
		return solution;
	}

	simplex::simplex(const double_matrix& a, const double_vector& c, const int_vector& _ineq, const double_vector& b)
	{
		if (b.size() != _ineq.size())
		{
			throw std::runtime_error("Error simplex creation :: b.size() != inequalities.size()");
		}
		if (a.rows_count() != _ineq.size())
		{
			throw std::runtime_error("Error simplex creation :: A.rows_number() != inequalities.size()");
		}

		if (a.cols_count() != c.size())
		{
			throw std::runtime_error("Error simplex creation :: A.cols_number() != price_coeffs.size()");
		}

		bounds_v = b;

		bounds_m = a;

		prices_v = c;

		_inequations = int_vector(_ineq);
	}

	simplex::simplex(const double_matrix& a, const double_vector& c, const double_vector& b)
	{
		if (a.rows_count() != b.size())
		{
			throw std::runtime_error("Error simplex creation :: A.rows_number() != bouns_coeffs.size()");
		}

		if (a.cols_count() != c.size())
		{
			throw std::runtime_error("Error simplex creation :: A.cols_number() != price_coeffs.size()");
		}

		int_vector _ineq;

		for (int i = 0; i < b.size(); i++)
		{
			_ineq.push_back(LESS_EQUAL);
		}

		bounds_v = b;

		bounds_m = a;

		prices_v = c;

		_inequations = _ineq;
	}

}

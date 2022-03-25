#pragma once
#include "vector_utils.h"
#include "matrix_utils.h"
#include "numeric_utils.h"
#include <iomanip>
#include <string>

///
#define EQUAL       0
#define LESS_EQUAL -1
#define MORE_EQUAL  1
////////////////////
/// Lab. work #5 ///
////////////////////
static std::string str_rational(const double val, const bool full_rational =  true)
{
	int r_part;
	int nom;
	int denom;

	decimal_to_rational(val, r_part, nom, denom);
	if (nom == 0)
	{
		return std::to_string(r_part);
	}
	if (r_part == 0)
	{
		return std::to_string(nom) + "/" + std::to_string(denom);
	}

	if (full_rational)
	{
		return std::to_string((nom + abs(r_part) * denom) * (r_part >= 0 ? 1 : -1)) + "/" + std::to_string(denom);
	}
	return std::to_string(denom) + " " + std::to_string(nom) + "/" + std::to_string(denom);
}

static std::string str_rational(const vec_n& val, const bool full_rational = true)
{
	std::string str = "{ ";
	for (int i = 0; i < val.size() - 1; i++) 
	{
		str += str_rational(val[i], full_rational);
		str += ", ";
	}
	str += str_rational(val[val.size() - 1], full_rational);

	str += " }";
	return str;
}

static void write_symplex(const  mat_mn& A, const std::vector<int>& basis_args)
{
	/// <summary>
	///	  (x,w)	
	/// w (A|I)  b
	///   (c|i)  F(x,c)
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="c"></param>

	if (A.size() == 0 )
	{
		return;
	}

	const char separator    = ' ';
	const int colom_title_w = 6;
	const int colom_w       = 12;

	std::cout << std::left << std::setw(colom_title_w) << std::setfill(separator) << "";

	int i = 0;
	for (;i < A[0].size() - 1; i++)
	{
		std::cout << std::left << std::setw(colom_w) << std::setfill(separator) << "| x " + std::to_string(i + 1);
	}

	std::cout << std::left << std::setw(colom_w) << std::setfill(separator) << "| b";
	std::cout << std::endl;

	int n_row = -1;

	for (auto const& row : A) 
	{
		n_row++;

		if(n_row == A.size() - 1)
		{
			std::cout << std::left << std::setw(colom_title_w) << std::setfill(separator) << " d ";
		}
		else 
		{
			std::cout << std::left << std::setw(colom_title_w) << std::setfill(separator) << " x " + std::to_string(basis_args[n_row] + 1);
		}

		for (int col = 0; col < row.size(); col++)
		{
			if (row[col] >= 0)
			{
				std::cout << std::left << std::setw(colom_w) << std::setfill(separator) << "| " + str_rational(row[col]);
				continue;
			}
			std::cout << std::left << std::setw(colom_w) << std::setfill(separator) << "|" + str_rational(row[col]);

		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

/// <summary>
/// Проверяет совместность СЛАУ вида Ax = b. Используется теорема Кронекера-Капелли 
/// </summary>
/// <param name="A"></param>
/// <param name="b"></param>
/// <returns>0 - нет решений, 1 - одно решение, 2 - бесконечное множествое решений</returns>
int check_system(const  mat_mn&A, const vec_n&b)
{
	int rank_a   = rank(A);
	
	mat_mn ab = A;
	
	int rank_a_b = rank(add_col(ab,b));

#if _DEBUG
	std::cout << "rank ( A ) " << rank_a   << std::endl;
	std::cout << "rank (A|b) " << rank_a_b << std::endl;
	if (rank_a == rank_a_b)
	{
		std::cout << "one solution"<< std::endl;
	}
	if (rank_a < rank_a_b)
	{
		std::cout << "infinite amount of solutions" << std::endl;
	}
	if (rank_a > rank_a_b)
	{
		std::cout << "no solutions" << std::endl;
	}
#endif

	if (rank_a == rank_a_b) 
	{
		return 1;
	}
	if (rank_a < rank_a_b)
	{
		return 2;
	}
	if (rank_a > rank_a_b)
	{
		return 0;
	}
	throw std::runtime_error("error :: check_system");
}
//mode = 
#define SYMPLEX_MAX  0
#define SYMPLEX_MIN  1
/// <summary>
/// Проверяет оптимальность плана в соответсвии с тем типом экстремума, которыей требуется найти.
/// Провеяются элменты от 1:n-1 полсдней строки СМ таблицы 
/// </summary>
/// <param name="A">СМ таблицa</param>
/// <param name="mode"></param>
/// <returns></returns>
bool is_plan_optimal(const mat_mn& A, const int mode = SYMPLEX_MAX)
{
	const vec_n& deltas = A[A.size() - 1];

	if (mode == SYMPLEX_MAX)
	{
		for (int i = 0; i < deltas.size() - 1; i++)
		{
			if (deltas[i] < 0)
			{
				return false;
			}
		}
		return true;
	}

	for (int i = 0; i < deltas.size() - 1; i++)
	{
		if (deltas[i] > 0)
		{
			return false;
		}
	}
	return true;
}
/// <summary>
/// Определяет ведущий столбец в соответсвии с тем типом экстремума, который требуется найти.
/// Исследуются элменты от 1:n-1 полсдней строки СМ таблицы 
/// </summary>
/// <param name="A"></param>
/// <returns></returns>
int get_main_col(const mat_mn& A, const int mode = SYMPLEX_MAX)
{
	double delta = 0;

	int index = -1;

	const vec_n& c = A[A.size() - 1];

	if (SYMPLEX_MIN == mode)
	{
		for (int i = 0; i < c.size() - 1; i++)
		{
			if (c[i] <= 0)
			{
				continue;
			}
			if (c[i] > delta)
			{
				continue;
			}

			delta = c[i];

			index = i;
		}
		return index;
	}

	for (int i = 0; i < c.size() - 1; i++) 
	{
		if (c[i]>=0)
		{
			continue;
		}
		if (abs(c[i]) < delta)
		{
			continue;
		}

		delta = abs(c[i]);

		index = i;
	}
	
	return index;
}
/// <summary>
/// Определяет ведущую строку 
/// </summary>
/// <param name="symplex_col">ведущий столбец</param>
/// <param name="A">СМ таблица</param>
/// <returns></returns>
int get_main_row(const int symplex_col, const  mat_mn& A) 
{
	double delta = 1e12;

	int index = -1;
	
	double a_ik;
	
	int b_index = A[0].size() - 1;
	
	for (int i = 0; i < A.size(); i++)
	{
		a_ik = A[i][symplex_col];

		if (a_ik < 0)
		{
			continue;
		}
		if (A[i][b_index] / a_ik > delta)
		{
			continue;
		}
		delta = A[i][b_index] / a_ik;
		index = i;
	}
	return index;
}

/// <summary>
/// Выводит текущее решение СМ таблицы для не искусственных переменных
/// </summary>
/// <param name="A">СМ таблица</param>
/// <param name="basis">список базисных параметров</param>
/// <param name="n_agrs">количество исходных переменных</param>
/// <returns></returns>
vec_n current_symplex_solution(const mat_mn& A,const std::vector<int> basis, const int n_agrs)
{
	vec_n solution(n_agrs);
	for (int i = 0; i < basis.size(); i++)
	{
		if (basis[i] >= n_agrs)
		{
			continue;
		}
		solution[basis[i]] = A[i][A[0].size() - 1];
	}
	return solution;
}

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
std::vector<int> build_symplex_table(mat_mn& A, const mat_mn& a, const vec_n& c, const vec_n& b, std::vector<int> ineq)
{
	int cntr = 0;
	
	A = a;
	
	std::vector<int> basis;

	for(auto& row : A)
	{
		basis.push_back(-1);
		
		for (int j = 0; j < b.size(); j++)
		{
		    if (ineq[j] == EQUAL)
			{
				basis[basis.size() - 1]  = j;
				continue;
			}
			if (ineq[j] == MORE_EQUAL)
			{
				if (cntr == j) 
				{
					basis[basis.size() - 1] = c.size() + j;
					row.push_back(-1.0);
					continue;
				}
				row.push_back(0.0);
				continue;
			}
			if (ineq[j] == LESS_EQUAL)
			{

				if (cntr == j)
				{
					basis[basis.size() - 1] = c.size() + j;
					row.push_back(1.0);
					continue;
				}
				row.push_back(0.0);
				continue;
			}
		}
		row.push_back(b[cntr]);

		if (b[cntr] < 0)
		{
			row = row * (-1);
		}

		cntr++;
	}

	vec_n C(A[0].size());

	for (int j = 0; j < C.size(); j++)
	{
		C[j] = j < c.size()? -c[j] : 0.0;
	}
	A.push_back(C);
	
	return basis;
}

vec_n symplex_solve(const mat_mn& a, const vec_n& c, const vec_n& b, const std::vector<int> ineq, const int mode = SYMPLEX_MAX)
{
	vec_n solution;
	
	int system_condition = check_system(a, b);

	if (system_condition == 0)
	{
		return solution;
	}
	
	if (system_condition == 1)
	{
		return linsolve(a, b);
	}
	
	double a_ik;
	
	int main_row;
	
	int main_col;

	mat_mn A ;
	
	std::vector<int> basis = build_symplex_table(A, a, c, b, ineq);
#if _DEBUG
	std::cout << "Start symplex table"<<std::endl;
	write_symplex(A, basis);
#endif

	while (!is_plan_optimal(A, mode))
	{
		main_col = get_main_col(A, mode);

		if (main_col == -1) 
		{
			break;
		}

		main_row = get_main_row(main_col, A);

		if (main_row == -1)
		{
			break;
		}

		basis[main_row] = main_col;

		a_ik =  A[main_row][main_col];

		A[main_row] = A[main_row] * (1.0 / a_ik);

		for (int i = 0; i < A.size(); i++) 
		{
			if (i == main_row) 
			{
				continue;
			}
			A[i] = A[i] - A[i][main_col] * A[main_row];
		}
#if _DEBUG
		std::cout << "a_main { " << main_row + 1 << ", " << main_col + 1 << " } = " << str_rational(a_ik) << std::endl;
		write_symplex(A, basis);
		std::cout << "current_solution" << str_rational(current_symplex_solution(A, basis, c.size()))<<std::endl;
		std::cout << std::endl;

#endif
	}
	/// формирование ответа
	return current_symplex_solution(A, basis, c.size());
}



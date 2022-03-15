#pragma once
#include "vector_utils.h"
#include "matrix_utils.h"
/// <summary>
/// Проверяет совместность СЛАУ вида ax = b. Используется теорема Кронекера-Капелли 
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns>0 - нет решений, 1 - одно решение, 2 - бесконечное множествое решений</returns>
int check_system(const  mat_mn&a, const vec_n&b)
{
	int rank_a   = rank(a);
	
	mat_mn ab = a;
	
	int rank_a_b = rank(add_col(ab,b));

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

bool is_plane_optimal(const vec_n& deltas, const int mode = SYMPLEX_MAX)
{
	for (auto& const delta : deltas)
	{
		if (delta < 0)
		{
			return false;
		}
	}
	return true;
}
int get_main_col(const vec_n& c)
{
	double delta = 0;
	int index = -1;
	for (int i = 0; i < c.size(); i++) 
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
int get_main_row(const int symplex_col, const  mat_mn& a, const vec_n& b) 
{
	double delta = 1e12;
	int index = -1;
	float a_ik;
	for (int i = 0; i < b.size(); i++)
	{
		a_ik = a[i][symplex_col];

		if (a_ik < 0)
		{
			continue;
		}
		if (b[i] / a_ik > delta)
		{
			continue;
		}
		delta = b[i] / a_ik;
		index = i;
	}
	return index;
}

void expand_problem(mat_mn& a, vec_n& c, const vec_n& b)
{
	int cntr = 0;
	for(auto& row : a)
	{
		for (int j = 0; j < b.size(); j++)
		{
			row.push_back(cntr == j ? 1.0 : 0.0);
		}
		cntr++;
	}
	for (int j = 0; j < b.size(); j++)
	{
		c.push_back(0.0);
	}
}

vec_n symplex_solve( mat_mn& a, vec_n& c, vec_n& b, const int mode = SYMPLEX_MAX)
{
	vec_n solution = b;
	
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

	expand_problem(a, c, b);

	while (!is_plane_optimal(c))
	{
		main_col = get_main_col(c);
		if (main_col == -1) 
		{
			break;
		}
		main_row = get_main_row(main_col,a,b);
		if (main_row == -1)
		{
			break;
		}
		
		a_ik = 1.0 / a[main_row][main_col];

		a[main_row] = a[main_row] * a_ik;

		b[main_row] = b[main_row] * a_ik;

		for (int i = 0; i < a.size(); i++) 
		{
			if (i == main_row) 
			{
				continue;
			}
			a[i] = a[i] - a[i][main_col] * a[main_row];
			
			b[i] = b[i] - a[i][main_col] * b[main_row];

		}
		c = c - c[main_col] * a[main_row];
	}

	return b;
}

#pragma once
#include "one_dimensional.h"
#include "vector_utils.h"
#include "matrix_utils.h"

#include <vector>
#include <cassert>
//Параметры по умолчанию


// Методы n-мерной дихотомии, золотого сечения и Фибоначчи определяют минимум строго вдоль направления из  x_0 в x_1
// т.е., если истинный минимум функции на этом направлении не лежит, метод всё равно найдёт минимальное значение, но оно 
// будет отличаться от истинного минимума
static vec_n bisect(func_nd f, const vec_n& x_0, const vec_n& x_1, const double& eps = N_DIM_ACCURACY, const int& max_iters = N_DIM_ITERS_MAX)
{
	vec_n x_l = x_0, x_r = x_1, x_c, dir;

	dir = direction(x_0, x_1) * eps;

	int cntr = 0;

	for (; cntr != max_iters; cntr++)
	{
		if (magnitude(x_r - x_l) < eps) break;
		
		x_c = (x_r + x_l) * 0.5;

		if (f(x_c + dir) > f(x_c - dir))
		{
			x_r = x_c;
			continue;
		}
		x_l = x_c;
	}
#ifdef _DEBUG 
	std::cout << "dihotomia iterations number : " << cntr << "\n";
#endif
	return x_c;
}

static vec_n goldenRatio        (func_nd f, const vec_n& x_0, const vec_n& x_1, const double& eps = N_DIM_ACCURACY, const int& max_iters = N_DIM_ITERS_MAX)
{
	vec_n  a = x_0, b = x_1;

	vec_n x_l(a), x_r(b), dx;

	int cntr = 0;

	double one_div_phi = 1.0 / phi;

	for (; cntr != max_iters; cntr++)
	{
		if (magnitude(x_r - x_l) < eps) break;
		
		dx   = (b - a) * one_div_phi;

		x_l =  b - dx;
		x_r =  a + dx;

		if (f(x_l) >= f(x_r))
		{
			a = x_l;
			continue;
		}
		b = x_r;
	}
#if _DEBUG
	std::cout <<"golden ratio iterations number : " << cntr << "\n";
#endif
	return (a + b) * 0.5;
}

static vec_n fibonacci          (func_nd f, const vec_n& x_0, const vec_n& x_1, const double& eps = N_DIM_ACCURACY)
{
	vec_n a(x_0), b(x_1);
	
	vec_n x_l(x_0), x_r(x_1), dx;
	
	int f_n, f_n_1, f_tmp, cntr = 0;

	get_closeset_fibonacci_pair(magnitude(b - a) / eps, f_n, f_n_1);

	while (f_n != f_n_1)
	{
		if (magnitude(x_r - x_l) < eps) break;
		
		dx = (b - a);
		f_tmp = f_n_1 - f_n;
		x_l = a + dx * ((double)f_tmp / f_n_1);
		x_r = a + dx * ((double)f_n   / f_n_1);
		f_n_1 = f_n;
		f_n = f_tmp;
		if (f(x_l) < f(x_r))
		{
			b = x_r;
			continue;
		}
		a = x_l;
	}
#if _DEBUG
	std::cout << "fibonacchi iterations number : " << cntr << "\n";
#endif
	return (x_r + x_l) * 0.5;
}

// Покоординатный спуск, градиентный спуск и спуск с помощью сопряжённых градиентов, определяют
// минимальное значение функции только по одной начальной точке x_start.
// Поэтому не зависят от выбора направления.
static vec_n perCoordDescend    (func_nd f, const vec_n& x_start, const double& eps = N_DIM_ACCURACY, const int& max_iters = N_DIM_ITERS_MAX)
{
	vec_n x_0(x_start);
	
	vec_n x_1(x_start);

	double step = 1.0;
	
	double x_i, y_1, y_0;

	int opt_coord_n = 0, coord_id;

	for (int i = 0; i < max_iters; i++)
	{
		coord_id = i % x_0.size();

		x_1[coord_id] -= eps;

		y_0 = f(x_1);

		x_1[coord_id] += 2.0 * eps;

		y_1 = f(x_1);

		x_1[coord_id] -= eps;

		x_1[coord_id] = y_0 > y_1 ? x_1[coord_id] += step : x_1[coord_id] -= step;

		x_i = x_0[coord_id];

		x_1 = bisect(f, x_0, x_1, eps, max_iters);

		x_0 = x_1;

		if (abs(x_1[coord_id] - x_i) < eps)
		{
			opt_coord_n++;

			if (opt_coord_n == x_1.size())
			{
#if _DEBUG
				std::cout << "per coord descend iterations number : " << i << "\n";
#endif
				return x_0;
			}
			continue;
		}
		opt_coord_n = 0;
	}
#if _DEBUG
	std::cout << "per coord descend iterations number : " << max_iters << "\n";
#endif
	return x_0;
}
//
static vec_n gradientDescend    (func_nd f, const vec_n& x_start, const double& eps = N_DIM_ACCURACY, const int& max_iters = N_DIM_ITERS_MAX)
{
	vec_n x_i(x_start);
	vec_n x_i_1; 
	vec_n grad;
	int cntr = 0;
	for(; cntr <= max_iters; cntr++)
	{
		grad  = gradient(f, x_i, eps);

		x_i_1 = x_i - grad;
		
		x_i_1 = bisect(f, x_i, x_i_1, eps, max_iters);
		
		if (magnitude(x_i_1 - x_i) < eps) break;
	
		x_i = x_i_1;
	}
#if _DEBUG
	std::cout << "gradient descend iterations number : " << cntr << "\n";
#endif
	return (x_i_1 + x_i) * 0.5;
}

static vec_n conjGradientDescend(func_nd f, const vec_n& x_start, const double& eps = N_DIM_ACCURACY, const int& max_iters = N_DIM_ITERS_MAX)
{
	vec_n x_i(x_start);
	vec_n x_i_1;
	vec_n s_i = gradient(f, x_start, eps)*(-1.0), s_i_1;
	double omega;
	int cntr = 0;
	for (; cntr <= max_iters; cntr++)
	{
		x_i_1 = x_i + s_i;

		if (magnitude(x_i_1 - x_i) < eps) break;

		x_i_1 = bisect(f, x_i, x_i_1, eps, max_iters);

		s_i_1 = gradient(f, x_i_1, eps);

		omega = pow(magnitude(s_i_1), 2) / pow(magnitude(s_i), 2);

		s_i = s_i * omega - s_i_1;

		x_i = x_i_1;
	}
#if _DEBUG
	std::cout << "conj gradient descend iterations number : " << cntr << "\n";
#endif
	return (x_i_1 + x_i) * 0.5;
}

static vec_n newtoneRaphson     (func_nd f, const vec_n& x_start, const double& eps = N_DIM_ACCURACY, const int& max_iters = N_DIM_ITERS_MAX)
{
	vec_n x_i(x_start);
	vec_n x_i_1;
	vec_n grad;
	mat_mn hess;
	int cntr = 0;
	for (; cntr <= max_iters; cntr++)
	{
		grad = gradient(f, x_i, eps);
	
		hess = invert(hessian(f, x_i, eps));
		
		x_i_1 = x_i - hess * grad;
		
		if (magnitude(x_i_1 - x_i) < eps) break;

		x_i = x_i_1;
	}
#if _DEBUG
	std::cout << "Newtone-Raphson method iterations number : " << cntr << "\n";
#endif
	return (x_i_1 + x_i) * 0.5;
}

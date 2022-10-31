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
static vec_n dihotomia          (func_n f, const vec_n& x_0, const vec_n& x_1, const double eps = N_DIM_ACCURACY, const int max_iters = N_DIM_ITERS_MAX)
{
	vec_n x_0_ = x_0, x_1_ = x_1, x_c, dir;

	dir = direction(x_0, x_1) * eps;

	int cntr = 0;

	for (; cntr != max_iters; cntr++)
	{
		if (magnitude(x_1_ - x_0_) < eps)
		{
			break;
		}
		x_c = (x_1_ + x_0_) * 0.5;

		if (f(x_c + dir) > f(x_c - dir))
		{
			x_1_ = x_c;
			continue;
		}
		x_0_ = x_c;
	}
#ifdef _DEBUG 
	std::cout << "dihotomia iterations number : " << cntr << "\n";
#endif
	return x_c;
}

static vec_n goldenRatio        (func_n f, const vec_n& x_0, const vec_n& x_1, const double eps = N_DIM_ACCURACY, const int max_iters = N_DIM_ITERS_MAX)
{
	vec_n  a = x_0, b = x_1;

	vec_n x_0_(a), x_1_(b), dx;

	int cntr = 0;

	double one_div_phi = 1.0 / phi;

	for (; cntr != max_iters; cntr++)
	{
		if (magnitude(x_1_ - x_0_) < eps)
		{
			break;
		}
		dx   = (b - a) * one_div_phi;

		x_0_ =  b - dx;
		x_1_ =  a + dx;

		if (f(x_0_) >= f(x_1_))
		{
			a = x_0_;
			continue;
		}
		b = x_1_;
	}
#if _DEBUG
	std::cout <<"golden ratio iterations number : " << cntr << "\n";
#endif
	return (a + b) * 0.5;
}

static vec_n fibonacci          (func_n f, const vec_n& x_0, const vec_n& x_1, const double eps = N_DIM_ACCURACY)
{
	vec_n a(x_0), b(x_1);
	
	vec_n x_0_(x_0), x_1_(x_1), dx;
	
	int max_iters = closestFibonacci(magnitude(x_1 - x_0) / eps);
	
	int cntr = max_iters - 1;

	std::vector<double> f_n_s = fibonacciNumbers<double>(max_iters);

	for (; cntr >= 2; cntr--)
	{
		if (magnitude(x_1_ - x_0_) < eps)
		{
			break;
		}
		dx   = b - a;
		x_0_ = a + dx * ((double)f_n_s[cntr - 2] / f_n_s[cntr]);
		x_1_ = a + dx * ((double)f_n_s[cntr - 1] / f_n_s[cntr]);

		if (f(x_0_) < f(x_1_))
		{
			b = x_1_;
			continue;
		}
		a = x_0_;
	}
#if _DEBUG
	std::cout << "fibonacchi iterations number : " << max_iters << "\n";
#endif
	return (x_1_ + x_0_) * 0.5;
}
// Покоординатный спуск, градиентный спуск и спуск с помощью сопряжённых градиентов, определяют
// минимальное значение функции только по одной начальной точке x_start.
// Поэтому не зависят от выбора направления.
static vec_n perCoordDescend    (func_n f, const vec_n& x_start, const double eps = N_DIM_ACCURACY, const int max_iters = N_DIM_ITERS_MAX)
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

		x_1[coord_id] += 2 * eps;

		y_1 = f(x_1);

		x_1[coord_id] -= eps;

		x_1[coord_id] = y_0 > y_1 ? x_1[coord_id] += step : x_1[coord_id] -= step;

		x_i = x_0[coord_id];

		x_1 = dihotomia(f, x_0, x_1, eps, max_iters);

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
static vec_n gradientDescend    (func_n f, const vec_n& x_start, const double eps = N_DIM_ACCURACY, const int max_iters = N_DIM_ITERS_MAX)
{
	vec_n x_i(x_start);
	vec_n x_i_1; 
	vec_n grad;
	int cntr = 0;
	while (true)
	{
		cntr++;
		if (cntr == max_iters)
		{
			break;
		}
		grad  = gradient(f, x_i, eps);
		x_i_1 = x_i - grad;
		x_i_1 = dihotomia(f, x_i, x_i_1, eps, max_iters);
		
		if (magnitude(x_i_1 - x_i) < eps) 
		{
			break;
		}

		x_i = x_i_1;
	}
#if _DEBUG
	std::cout << "gradient descend iterations number : " << cntr << "\n";
#endif
	return (x_i_1 + x_i) * 0.5;
}

static vec_n conjGradientDescend(func_n f, const vec_n& x_start, const double eps = N_DIM_ACCURACY, const int max_iters = N_DIM_ITERS_MAX)
{
	vec_n x_i(x_start);
	vec_n x_i_1;
	vec_n s_i = gradient(f, x_start, eps)*(-1.0), s_i_1;
	double omega;
	int cntr = 0;
	while (true)
	{
		cntr++;
	
		if (cntr == max_iters)
		{
			break;
		}

		x_i_1 = x_i + s_i;
	
		x_i_1 = dihotomia(f, x_i, x_i_1, eps, max_iters);

		if (magnitude(x_i_1 - x_i) < eps)
		{
			break;
		}

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

static vec_n newtoneRaphson     (func_n f, const vec_n& x_start, const double eps = N_DIM_ACCURACY, const int max_iters = N_DIM_ITERS_MAX)
{
	vec_n x_i(x_start);
	vec_n x_i_1;
	vec_n grad;
	mat_mn hess;
	int cntr = 0;
	while (true)
	{
		cntr++;
		if (cntr == max_iters)
		{
			break;
		}
		
		grad = gradient(f, x_i, eps);
	
		hess = invert(hessian(f, x_i, eps));
		
		x_i_1 = x_i - hess * grad;

		if (magnitude(x_i_1 - x_i) < eps)
		{
			break;
		}
		x_i = x_i_1;
	}
#if _DEBUG
	std::cout << "Newtone-Raphson method iterations number : " << cntr << "\n";
#endif
	return (x_i_1 + x_i) * 0.5;
}

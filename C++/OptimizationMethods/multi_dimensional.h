#pragma once
#include "numeric_vector.h"
#include "numeric_matrix.h"
#include "numeric_utils.h"

typedef F64(*function_nd)(const vector_f64&);

// Методы n-мерной дихотомии, золотого сечения и Фибоначчи определяют минимум строго вдоль направления из  x_0 в x_1
// т.е., если истинный минимум функции на этом направлении не лежит, метод всё равно найдёт минимальное значение, но оно 
// будет отличаться от истинного минимума
static vector_f64 bisect(function_nd function, const vector_f64& x_0, const vector_f64& x_1, const F64 eps = N_DIM_ACCURACY, const I32 max_iters = N_DIM_ITERS_MAX)
{
	vector_f64 x_l = x_0, x_r = x_1, x_c, dir;
	dir = vector_f64::direction(x_0, x_1) * eps;
	I32 cntr = 0;
	for (; cntr != max_iters; cntr++)
	{
		if ((x_r - x_l).magnitude() < eps) break;
		x_c = (x_r + x_l) * 0.5;
		if (function(x_c + dir) > function(x_c - dir))
			x_r = x_c;
		else
			x_l = x_c;
	}
	return x_c;
}

static vector_f64 golden_ratio(function_nd function, const vector_f64& x_0, const vector_f64& x_1, const F64 eps = N_DIM_ACCURACY, const I32 max_iters = N_DIM_ITERS_MAX)
{
	vector_f64  a = x_0, b = x_1;
	vector_f64 x_l(a), x_r(b), dx;
	I32 cntr = 0;
	for (; cntr != max_iters; cntr++)
	{
		if ((x_r - x_l).magnitude() < eps) break;
		dx  = (b - a) * ONE_OVER_PHI;
		x_l =  b - dx;
		x_r =  a + dx;
		if (function(x_l) >= function(x_r))
			a = x_l;
		else
			b = x_r;
	}
	return (a + b) * 0.5;
}

static vector_f64 fibonacci(function_nd function, const vector_f64& x_0, const vector_f64& x_1, const F64 eps = N_DIM_ACCURACY)
{
	vector_f64 a(x_0), b(x_1);
	vector_f64 x_l(x_0), x_r(x_1), dx;
	I32 f_n, f_n_1, f_tmp, cntr = 0;
	closest_fibonacci_pair((b - a).magnitude() / eps, f_n, f_n_1);
	while (f_n != f_n_1)
	{
		if ((x_r - x_l).magnitude() < eps) break;
		dx = (b - a);
		f_tmp = f_n_1 - f_n;
		x_l = a + dx * ((F64)f_tmp / f_n_1);
		x_r = a + dx * ((F64)f_n   / f_n_1);
		f_n_1 = f_n;
		f_n = f_tmp;
		if (function(x_l) < function(x_r))
			b = x_r;
		else
			a = x_l;
	}
	return (x_r + x_l) * 0.5;
}

// Покоординатный спуск, градиентный спуск и спуск с помощью сопряжённых градиентов, определяют
// минимальное значение функции только по одной начальной точке x_start.
// Поэтому не зависят от выбора направления.
static vector_f64 per_coord_descend(function_nd function, const vector_f64& x_start, const F64 eps = N_DIM_ACCURACY, const I32 max_iters = N_DIM_ITERS_MAX)
{
	vector_f64 x_0(x_start);
	vector_f64 x_1(x_start);
	F64 step = 1.0;
	F64 x_i, y_1, y_0;
	I32 opt_coord_n = 0, coord_id;
	for (I32 i = 0; i < max_iters; i++)
	{
		coord_id = i % x_0.size();
		x_1[coord_id] -= eps;
		y_0 = function(x_1);
		x_1[coord_id] += 2.0 * eps;
		y_1 = function(x_1);
		x_1[coord_id] -= eps;
		x_1[coord_id] = y_0 > y_1 ? x_1[coord_id] += step : x_1[coord_id] -= step;
		x_i = x_0[coord_id];
		x_1 = bisect(function, x_0, x_1, eps, max_iters);
		x_0 = x_1;
		if (abs(x_1[coord_id] - x_i) < eps)
		{
			opt_coord_n++;

			if (opt_coord_n == x_1.size())
			{
#if DISPLAY_PROGRES
				std::cout << "\nper coord descend iterations number : " << i << "\n";
#endif
				return x_0;
			}
			continue;
		}
		opt_coord_n = 0;
	}
#if DISPLAY_PROGRES
	std::cout << "per coord descend iterations number : " << max_iters << "\n";
#endif
	return x_0;
}
//
static vector_f64 gradient_descend(function_nd function, const vector_f64& x_start, const F64 eps = N_DIM_ACCURACY, const I32 max_iters = N_DIM_ITERS_MAX)
{
	vector_f64 x_i(x_start);
	vector_f64 x_i_1;
	vector_f64 grad;
	I32 cntr = 0;
	for(; cntr <= max_iters; cntr++)
	{
		grad  = vector_f64::gradient(function, x_i, eps);
		x_i_1 = x_i - grad;
		x_i_1 = bisect(function, x_i, x_i_1, eps, max_iters);
		if ((x_i_1 - x_i).magnitude() < eps) break;
		x_i = x_i_1;
	}
#if DISPLAY_PROGRES
	std::cout << "\ngradient descend iterations number : " << cntr << "\n";
#endif
	return (x_i_1 + x_i) * 0.5;
}

static vector_f64 conj_gradient_descend(function_nd function, const vector_f64& x_start, const F64 eps = N_DIM_ACCURACY, const I32 max_iters = N_DIM_ITERS_MAX)
{
	vector_f64 x_i(x_start);
	vector_f64 x_i_1;
	vector_f64 s_i = vector_f64::gradient(function, x_i, eps)*(-1.0), s_i_1;
	F64 omega;
	I32 cntr = 0;
	for (; cntr <= max_iters; cntr++)
	{
		x_i_1 = x_i + s_i;
		if ((x_i_1 - x_i).magnitude() < eps) break;
		x_i_1 = bisect(function, x_i, x_i_1, eps, max_iters);
		s_i_1 = vector_f64::gradient(function, x_i_1, eps);
		omega = pow(s_i_1.magnitude(), 2) / pow(s_i.magnitude(), 2);
		s_i = s_i * omega - s_i_1;
		x_i = x_i_1;
	}
#if DISPLAY_PROGRES
	std::cout << "\nconj gradient descend iterations number : " << cntr << "\n";
#endif
	return (x_i_1 + x_i) * 0.5;
}

static vector_f64 newtone_raphson (function_nd function, const vector_f64& x_start, const F64 eps = N_DIM_ACCURACY, const I32 max_iters = N_DIM_ITERS_MAX)
{
	vector_f64 x_i(x_start);
	vector_f64 x_i_1;
	vector_f64 grad ;
	matrix_f64 hess(1, 1);
	I32 cntr = 0;
	for (; cntr <= max_iters; cntr++)
	{
		grad = vector_f64::gradient(function, x_i, eps);
		hess = matrix_f64::invert(matrix_f64::hessian(function, x_i, eps));
		x_i_1 = x_i - (hess * grad);
		if ((x_i_1 - x_i).magnitude() < eps) break;
		x_i = x_i_1;
	}
#if DISPLAY_PROGRES
	std::cout << "\nNewtone-Raphson method iterations number : " << cntr << "\n";
#endif
	return (x_i_1 + x_i) * 0.5;
}

#pragma once
#include "numerics/linalg/numeric_vector.h"
#include "numerics/linalg/numeric_matrix.h"
#include "numerics/numeric_utils.h"

typedef F64(*function_nd)(const vector_f64&);
static vector_f64 bisect      (function_nd function, const vector_f64& left, const vector_f64& right, const F64 eps = N_DIM_ACCURACY, const I32 max_iterations = N_DIM_ITERS_MAX);
static vector_f64 golden_ratio(function_nd function, const vector_f64& left, const vector_f64& right, const F64 eps = N_DIM_ACCURACY, const I32 max_iters = N_DIM_ITERS_MAX);
static vector_f64 fibonacci   (function_nd function, const vector_f64& left, const vector_f64& right, const F64 eps = N_DIM_ACCURACY);

static vector_f64 per_coord_descend    (function_nd function, const vector_f64& x_start, const F64 eps = N_DIM_ACCURACY, const I32 max_iters = N_DIM_ITERS_MAX);
static vector_f64 gradient_descend     (function_nd function, const vector_f64& x_start, const F64 eps = N_DIM_ACCURACY, const I32 max_iters = N_DIM_ITERS_MAX);
static vector_f64 conj_gradient_descend(function_nd function, const vector_f64& x_start, const F64 eps = N_DIM_ACCURACY, const I32 max_iters = N_DIM_ITERS_MAX);
static vector_f64 newtone_raphson      (function_nd function, const vector_f64& x_start, const F64 eps = N_DIM_ACCURACY, const I32 max_iters = N_DIM_ITERS_MAX);


// Методы n-мерной дихотомии, золотого сечения и Фибоначчи определяют минимум строго вдоль направления из  x_0 в x_1
// т.е., если истинный минимум функции на этом направлении не лежит, метод всё равно найдёт минимальное значение, но оно 
// будет отличаться от истинного минимума
static vector_f64 bisect(function_nd function, const vector_f64& left, const vector_f64& right, const F64 eps, const I32 max_iterations)
{
	vector_f64 x_c, dir, lhs(left), rhs(right);
	dir = vector_f64::direction(lhs, rhs) * eps;
	I32 iteration = 0;
	for (; iteration != max_iterations; iteration++)
	{
		if ((lhs - rhs).magnitude() < 2 * eps) break;
		x_c = (lhs + rhs) * 0.5;
		if (function(x_c - dir) > function(x_c + dir))
			lhs = x_c;
		else
			rhs = x_c;
	}
#if _DEBUG
	std::cout << "bisect::function arg range    : " << (rhs - lhs).magnitude() << "\n";
	std::cout << "bisect::function probes count : " << iteration * 2 << "\n";
#endif
	return (lhs + rhs) * 0.5;
}

static vector_f64 golden_ratio(function_nd function, const vector_f64& left, const vector_f64& right, const F64 eps, const I32 max_iterations)
{
	vector_f64 lhs(left), rhs(right);
	vector_f64 x_l, x_r;
	F64 f_l, f_r;
	I32 iteration = 0;
	x_l = rhs - (rhs - lhs) * PSI;
	x_r = lhs + (rhs - lhs) * PSI;
	f_l = function(x_l);
	f_r = function(x_r);
	for (; iteration != max_iterations; iteration++)
	{
		if ((rhs - lhs).magnitude() < 2 * eps) break;
		if (f_l > f_r)
		{
			lhs = x_l;
			x_l = x_r;
			f_l = f_r;
			x_r = lhs + (rhs - lhs) * PSI;
			f_r = function(x_r);
		}
		else
		{
			rhs = x_r;
			x_r = x_l;
			f_r = f_l;
			x_l = rhs - (rhs - lhs) * PSI;
			f_l = function(x_l);
		}
	}
#if _DEBUG
	std::cout << "golden_ratio::function arg range    : " << (rhs - lhs).magnitude() << "\n";
	std::cout << "golden_ratio::function probes count : " << iteration + 2 << "\n";
#endif
	return (right + left) * 0.5;
}

static vector_f64 fibonacci(function_nd function, const vector_f64& left, const vector_f64& right, const F64 eps)
{
	vector_f64 x_l, x_r;
	vector_f64 lhs(left), rhs(right);
	F64 f_l, f_r, value, fib_t{ 0.0 }, fib_1{ 1.0 }, fib_2{ 1.0 };
	I32 iterations{ 0 };
	value = (right - left).magnitude() / eps;
	while (fib_2 < value)
	{
		iterations++;
		fib_t = fib_1;
		fib_1 = fib_2;
		fib_2 += fib_t;
	}
	x_l = lhs + (rhs - lhs) * ((fib_2 - fib_1) / fib_2);
	x_r = lhs + (rhs - lhs) * (fib_1 / fib_2);

	f_l = function(x_l);
	f_r = function(x_r);

	fib_t = fib_2 - fib_1;
	fib_2 = fib_1;
	fib_1 = fib_t;

	for (I32 index = iterations; index > 0; index--)
	{
		if (f_l > f_r)
		{
			lhs = x_l;
			f_l = f_r;
			x_l = x_r;
			x_r = lhs + (rhs - lhs) * (fib_1 / fib_2);
			f_r = function(x_r);
		}
		else
		{
			rhs = x_r;
			x_r = x_l;
			f_r = f_l;
			x_l = lhs + (rhs - lhs) * ((fib_2 - fib_1) / fib_2);
			f_l = function(x_l);
		}
		fib_t = fib_2 - fib_1;
		fib_2 = fib_1;
		fib_1 = fib_t;
		// #if _DEBUG
		// 	   std::cout << "\nfibonacchi [a, b] range: " << (right - left) << "\n";
		// #endif
	}
#if _DEBUG
	std::cout << "fibonacci::function arg range    : " << (rhs - lhs).magnitude() << "\n";
	std::cout << "fibonacci::function probes count : " << iterations + 2 << "\n";
#endif
	return (rhs + lhs) * 0.5;
}

// Покоординатный спуск, градиентный спуск и спуск с помощью сопряжённых градиентов, определяют
// минимальное значение функции только по одной начальной точке x_start.
// Поэтому не зависят от выбора направления.
static vector_f64 per_coord_descend(function_nd function, const vector_f64& x_start, const F64 eps, const I32 max_iters)
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

static vector_f64 gradient_descend(function_nd function, const vector_f64& x_start, const F64 eps, const I32 max_iters)
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

static vector_f64 conj_gradient_descend(function_nd function, const vector_f64& x_start, const F64 eps, const I32 max_iters)
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

static vector_f64 newtone_raphson (function_nd function, const vector_f64& x_start, const F64 eps, const I32 max_iters)
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

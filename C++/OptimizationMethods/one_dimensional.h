#pragma once
#include "numeric_utils.h"
#include "common.h"

typedef F64(*function_1d)(const F64);

static F64 bisect(function_1d function, const F64 x_0, const F64 x_1, const F64 eps = ACCURACY, const int max_iters = ITERS_MAX)
{
	F64 x_l = x_0, x_r = x_1, x_c = 0.0;
	if (x_l > x_r) std::swap(x_l, x_r);
	int cntr = 0;
	for (; cntr != max_iters; cntr++)
	{
		if (abs(x_r - x_l) < 2 * eps) break;
		x_c = (x_r + x_l) * 0.5;
		if (function(x_c + eps * 1e-1) > function(x_c - eps * 1e-1))
			x_r = x_c;
		else
			x_l = x_c;
	}
#if _DEBUG
	std::cout << "\ndihotomia iterations number : " << cntr * 2 << "\n";
#endif
	return x_c;
}

static F64 golden_ratio(function_1d function, const F64 x_0, const F64 x_1, const F64 eps = ACCURACY, const int max_iters = ITERS_MAX)
{
	F64 a = x_0, b = x_1;
	if (a > b) std::swap(a, b);
	F64 x_l = a, x_r = b;
	F64 f_l, f_r;
	int cntr = 2;
	x_l = b - (b - a) * PSI;
	x_r = a + (b - a) * PSI;
	f_l = function(x_l);
	f_r = function(x_r);
	for (; cntr != max_iters; cntr++)
	{
		if (abs(b - a) < 2 * eps) break;
		if (f_l > f_r)
		{
			a = x_l;
			x_l = x_r;
			f_l = f_r;
			x_r = a + (b - a) * PSI;
			f_r = function(x_r);
		}
		else
		{
			b = x_r;
			x_r = x_l;
			f_r = f_l;
			x_l = b - (b - a) * PSI;
			f_l = function(x_l);
		}
	}
#if _DEBUG
	std::cout << "\ngolden ratio iterations number : " << cntr << "\n";
	// std::cout << "interaval range : " << ((b - a) * 0.5) << "\n";
#endif
	return (x_r + x_l) * 0.5;
}

static F64 fibonacci(function_1d function, const F64 x_0, const F64 x_1, const F64 eps = ACCURACY)
{
	F64 a = x_0, b = x_1;
	if (a > b) std::swap(a, b);
	F64 x_l = a, x_r = b, f_l, f_r;
	I32 f_n, f_n_1, f_tmp, cntr = 2;
	
	closest_fibonacci_pair((b - a) / eps, f_n, f_n_1);
	
	f_tmp = f_n_1 - f_n;
	
	x_l = a + (b - a) * (static_cast<F64>(f_tmp) / f_n_1);
	x_r = a + (b - a) * (static_cast<F64>(f_n  ) / f_n_1);
	
	f_l = function(x_l);
	f_r = function(x_r);

	f_tmp = f_n_1 - f_n;
	f_n_1 = f_n;
	f_n = f_tmp;
	
	while (f_n != f_n_1)
	{
		if (abs(b - a) < eps)break;
		cntr++;
		if (f_l > f_r)
		{
			a   = x_l;
			f_l = f_r;
			x_l = x_r;
			x_r = a + (b - a) * (static_cast<F64>(f_n) / f_n_1);
			f_r = function(x_r);
		}
		else
		{
			b   = x_r;
			x_r = x_l;
			f_r = f_l;
			x_l = a + (b - a) * (static_cast<F64>(f_n_1 - f_n) / f_n_1);
			f_l = function(x_l);
		}
		f_tmp = f_n_1 - f_n;
		f_n_1 = f_n;
		f_n = f_tmp;
#if _DEBUG
		// std::cout << "\nfibonacchi [a, b] range: " << (b - a) << "\n";
		//std::swap(dx_prew, dx_curr);
#endif

	}
#if _DEBUG
	std::cout << "\nfibonacchi iterations number : " << cntr << "\n";
#endif
	return (x_r + x_l) * 0.5;
}

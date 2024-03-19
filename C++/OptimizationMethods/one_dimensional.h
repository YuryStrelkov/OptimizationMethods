#pragma once
#include "numeric_utils.h"
#include "common.h"

typedef double(*function_1d)(const double);

static double bisect(function_1d function, const double x_0, const double x_1, const double eps = ACCURACY, const int max_iters = ITERS_MAX)
{
	double x_l = x_0 , x_r = x_1, x_c = 0.0;
	if (x_l > x_r) std::swap(x_l, x_r);
	int cntr = 0;
	for (;cntr != max_iters; cntr++)
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

static double golden_ratio(function_1d function, const double x_0, const double x_1, const double eps = ACCURACY, const int max_iters = ITERS_MAX)
{
	double a = x_0, b = x_1;
	if (a > b) std::swap(a, b);
	double x_l = a, x_r = b, f_l, f_r;
	int cntr = 2;
	x_l = b - (b - a) * ONE_OVER_PHI;
	x_r = a + (b - a) * ONE_OVER_PHI;
	f_l = function(x_l);
	f_r = function(x_r);
	for (; cntr != max_iters; cntr++)
	{
		if (abs(x_r - x_l) < 2 * eps) break;
		if (f_l > f_r) 
		{
			a   = x_l;
			x_l = x_r;
			f_l = f_r;
			x_r = a + (b - a) * ONE_OVER_PHI;
			f_r = function(x_r);
		}
		else 
		{
			b = x_r;
			x_r = x_l;
			f_r = f_l;
			x_l = b - (b - a) * ONE_OVER_PHI;
			f_l = function(x_l);
		}
	}
#if _DEBUG
	std::cout << "\ngolden ratio iterations number : " << cntr << "\n";
#endif
	return (x_r + x_l) * 0.5;
}

static double fibonacci(function_1d function, const double x_0, const double x_1, const double eps = ACCURACY)
{
	double a = x_0, b = x_1;
	if (a > b) std::swap(a, b);
	double x_l = a, x_r = b,  dx;
	int f_n, f_n_1, f_tmp, cntr = 0;
	closest_fibonacci_pair((b - a) / eps, f_n, f_n_1);
	while(f_n != f_n_1)
	{
		if (x_r - x_l < eps)break;
		cntr++;
		dx    = (b - a);
		f_tmp = f_n_1 - f_n;
		x_l   = a + dx * ((double)f_tmp / f_n_1);
		x_r   = a + dx * ((double)f_n   / f_n_1);
		f_n_1 = f_n;
		f_n   = f_tmp;
		if(function(x_l) < function(x_r))
			b = x_r;
		else
			a = x_l;
	}
#if _DEBUG
	std::cout << "\nfibonacchi iterations number : " << cntr << "\n";
#endif
	return (x_l + x_r) * 0.5;
}

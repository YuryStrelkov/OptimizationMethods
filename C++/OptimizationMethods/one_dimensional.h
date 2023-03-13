#pragma once
#include <cmath>
#include <iostream>
#include <vector>
#include "common.h"


static double         bisect       (func_1d f, const double& x_0, const double& x_1, const double& eps = ACCURACY, const int& max_iters = ITERS_MAX)
{
	double x_l = x_0 , x_r = x_1, x_c = 0.0;
	
	if (x_l > x_r) std::swap(x_l, x_r);

	int cntr = 0;

	for (;cntr != max_iters; cntr++)
	{
#if _DEBUG
		progresBar((float)cntr / (float)max_iters);
#endif

		if (x_r - x_l < eps) break;

		x_c = (x_r + x_l) * 0.5;

		if (f(x_c + eps) > f(x_c - eps))
		{
			x_r = x_c;
			continue;
		}
		x_l = x_c;
	}
#if _DEBUG
	std::cout << "\ndihotomia iterations number : " << cntr << "\n";
#endif
	return x_c;
}

static double         goldenRatio     (func_1d f, const double& x_0, const double& x_1, const double& eps = ACCURACY, const int& max_iters = ITERS_MAX)
{
	double a = x_0, b = x_1;
	
	if (a > b) std::swap(a, b);

	double x_l = a, x_r = b, dx;

	int cntr = 0;

	for (; cntr != max_iters; cntr++)
	{
#if _DEBUG
		progresBar((float)cntr / (float)max_iters);
#endif

		if (x_r - x_l < eps) break;
		
		dx = b - a;

		x_l = b - dx / phi;
		x_r = a + dx / phi;

		if (f(x_l) >= f(x_r))
		{
			a = x_l;
			continue;
		}
		b = x_r;
	}
#if _DEBUG
	std::cout << "\ngolden ratio iterations number : " << cntr << "\n";
#endif
	return (x_r + x_l) * 0.5;
}

static int            closestFibonacci(const double& value)
{
	int f_1 = 1;
	if (value <= 1) 
	{
		return f_1;
	}
	int f_2 = 2;
	if (value <= 2)
	{
		return f_2;
	}
	int f_3 = 3;
	if (value <= 3)
	{
		return f_3;
	}
	int cntr = 3;
	while (true)
	{
		f_1 = f_2;
		f_2 = f_3;
		f_3 = f_1 + f_2;
		if (f_3 > value)
		{
			return cntr;
		}
		cntr++;
	}
}

template<typename T> static std::vector<T> fibonacciNumbers(const int& index)
{
	if (index < 1)
		return { (T)0 };
	if (index < 2)
		return { (T)1 };

	std::vector<T> res(index);
	
	res.at(0) = (T)0;
	
	res.at(1) = (T)1;

	for (int i = 2; i < index; i++)res.at(i) = res.at(i - 2) + res.at(i - 1);

	return res;
}

template<typename T> static void get_closeset_fibonacci_pair(const T& value, int& f_n, int& f_n_1)
{
	f_n = 0;
	f_n_1 = 0;
	
	if (value < 1) return;
	
	f_n_1 = 1;	   

	if (value < 2) return;
	int f_tmp;

	while (f_n < value)
	{
		f_tmp = f_n;
		f_n = f_n_1;
		f_n_1 += f_tmp;
	}
}

static double         fibonacci       (func_1d f, const double& x_0, const double& x_1, const double& eps = ACCURACY)
{
	double a = x_0, b = x_1;

	if (a > b) std::swap(a, b);

	double x_l = a, x_r = b,  dx;

	int f_n, f_n_1, f_tmp, cntr = 0;

	get_closeset_fibonacci_pair((b - a) / eps, f_n, f_n_1);

	while(f_n != f_n_1)
	{
#if _DEBUG
		progresBar((float)cntr / (float)ITERS_MAX);
#endif
		if (x_r - x_l < eps)break;
		cntr++;
		dx    = (b - a);
		f_tmp = f_n_1 - f_n;
		x_l   = a + dx * ((double)f_tmp / f_n_1);
		x_r   = a + dx * ((double)f_n   / f_n_1);
		f_n_1 = f_n;
		f_n   = f_tmp;
		if(f(x_l) < f(x_r))
		{
			b = x_r;
			continue;
		}
		a = x_l;
	}
#if _DEBUG
	std::cout << "\nfibonacchi iterations number : " << cntr << "\n";
#endif
	return (x_l + x_r) * 0.5;
}

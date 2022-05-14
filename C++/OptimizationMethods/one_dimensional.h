#pragma once
#include <cmath>
#include <iostream>
#include <vector>
#define phi 1.61803398874989484820

typedef double(*func)(const double);

static double         dihotomia       (func f, const double x_0, const double x_1, const double eps = 1e-6, const int max_iters = 1000)
{
	double x_0_ = x_0 , x_1_ = x_1, x_c = 0.0;
	
	if (x_0_ > x_1_)
	{
		std::swap(x_0_ , x_1_);
	}

	int cntr = 0;

	for (;cntr != max_iters; cntr++)
	{
		if (fabs(x_1_ - x_0_) < eps)
		{
			break;
		}
		x_c = (x_1_ + x_0_) * 0.5;

		if (f(x_c + eps) > f(x_c - eps))
		{
			x_1_ = x_c;
			continue;
		}
		x_0_ = x_c;
	}
#if _DEBUG
	std::cout << "dihotomia iterations number : " << cntr << "\n";
#endif
	return x_c;
}

static double         goldenRatio     (func f, const double x_0, const double x_1, const double eps = 1e-6, const int max_iters = 1000)
{
	double a = x_0, b = x_1;
	
	if (a > b)
	{
		std::swap(a, b);
	}

	double x_0_ = a, x_1_ = b, dx;

	int cntr = 0;

	for (; cntr != max_iters; cntr++)
	{
		if (fabs(x_1_ - x_0_) < eps)
		{
			break;
		}
		dx = b - a;

		x_0_ = b - dx / phi;
		x_1_ = a + dx / phi;

		if (f(x_0_) >= f(x_1_))
		{
			a = x_0_;
			continue;
		}
		b = x_1_;
	}
#if _DEBUG
	std::cout << "golden ratio iterations number : " << cntr << "\n";
#endif
	return (x_1_ + x_0_) * 0.5;
}

static int            closestFibonacci(double value)
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

template<typename T>
static std::vector<T> fibonacciNumbers(int index)
{
	if (index < 0)
	{
		return {(T)0};
	}
	if (index == 0 || index == 1)
	{
		return {(T)1};
	}
	
	std::vector<T> res(index);
	
	res.at(0) = (T)1;
	
	res.at(1) = (T)1;

	for (int i = 2; i < index; i++)
	{
		res.at(i) = res.at(i - 2) + res.at(i - 1);
	}

	return res;
}

static double         fibonacci       (func f, const double x_0, const double x_1, const double eps = 1e-6)
{
	double a = x_0, b = x_1;

	if (a > b)
	{
		std::swap(a, b);
	}

	double x_0_ = a, x_1_ = b, dx;
	
	int max_iters = closestFibonacci((b - a) / eps);

	int cntr = max_iters - 1;

	std::vector<double> f_n_s = fibonacciNumbers<double>(max_iters);

	for (; cntr >= 2; cntr--)
	{
		if (fabs(x_1_ - x_0_) < eps)
		{
			break;
		}
		dx = (b - a);
		x_0_ = a + dx * f_n_s[cntr - 2] / f_n_s[cntr];
		x_1_ = a + dx * f_n_s[cntr - 1] / f_n_s[cntr];

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



#pragma once
#include <cmath>
#include <iostream>
#define phi (1.0f + sqrt(5.0f))*0.5f

typedef double(*func)(const double);

static double test_f(const double arg) 
{
	return arg * (arg - 5);
}

static double dihotomia(func f, const double x_0, const double x_1, const double eps = 1e-6f, const int max_iters = 1000)
{
	double x_0_ = x_0 , x_1_ = x_1, x_c = 0.0f;
	
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
		x_c = (x_1_ + x_0_) * 0.5f;

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

static double golden_ratio(func f, const double x_0, const double x_1, const double eps = 1e-6f, const int max_iters = 1000)
{
	double a = x_0, b = x_1;
	
	if (a > b)
	{
		std::swap(a, b);
	}

	double x_0_ = a, x_1_ = b,dx_;

	int cntr = 0;

	for (; cntr != max_iters; cntr++)
	{
		if (fabs(x_1_ - x_0_) < eps)
		{
			break;
		}
		dx_ = (b - a) / phi;

		x_0_ = b - dx_;
		x_1_ = a + dx_;

		if (f(x_0_) > f(x_1_))
		{
			b = x_0_;
			continue;
		}
		a = x_1_;
	}
#if _DEBUG
	std::cout << "golden ratio iterations number : " << cntr << "\n";
#endif
	return (x_1_ + x_0_) * 0.5f;
}

static int   closest_fibonacci(double value)
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

static double fibonacci(func f, const double x_0, const double x_1, const double eps = 1e-6f)
{
	double a = x_0, b = x_1;

	if (a > b)
	{
		std::swap(a, b);
	}

	double x_0_ = a, x_1_ = b, dx;
	double f_1 = 1.0f, f_2 = 2.0f, f_3 = 3.0f;
	int cntr = 0;

	int max_iters = closest_fibonacci((b - a) / eps);

	for (; cntr != max_iters; cntr++)
	{
		if (fabs(x_1_ - x_0_) < eps)
		{
			break;
		}
		dx = (b - a);
		x_0_ = b - dx * f_1 / f_3;
		x_1_ = b + dx * f_2 / f_3;

		f_1 = f_2;
		f_2 = f_3;
		f_3 = f_1 + f_2;

		if (f(x_0_) < f(x_1_))
		{
			b = x_0_;
			continue;
		}
		a = x_1_;
	}
#if _DEBUG
	std::cout << "fibonacchi iterations number : " << cntr << "\n";
#endif
	return (x_1_ + x_0_) * 0.5f;
}
/// <summary>
/// Âûחûגאועסÿ ג main
/// </summary>
static void  one_dimensional_methods_test()
{
	double x_0 =  10;
	double x_1 = -1;
	std::cout << "\n";
	std::cout << "x = agrmin(x * (x - 5))\n";
	std::cout << "x_0 = " << x_0 << ", x_1 = " << x_1 << "\n";
	std::cout << "dihotomia   : " << dihotomia   (test_f, x_0, x_1, 1e-3f) << "\n";
	std::cout << "golden_ratio: " << golden_ratio(test_f, x_0, x_1, 1e-3f) << "\n";
	std::cout << "fibonacci   : " << fibonacci   (test_f, x_0, x_1, 1e-3f) << "\n";
}



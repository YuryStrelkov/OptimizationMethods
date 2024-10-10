#pragma once
#include "numerics/common.h"

typedef F64(*function_1d)(const F64);
static F64 bisect      (function_1d function, F64 left, F64 right, const F64 eps = ACCURACY, const I32 max_iterations = ITERS_MAX);
static F64 golden_ratio(function_1d function, F64 left, F64 right, const F64 eps = ACCURACY, const I32 max_iterations = ITERS_MAX);
static F64 fibonacci   (function_1d function, F64 left, F64 right, const F64 eps = ACCURACY);

static F64 bisect(function_1d function, F64 left, F64 right, const F64 eps, const I32 max_iterations)
{
	if (left > right) std::swap(left, right);
	F64 x_c = 0.0;
	I32 iteration = 0;
	for (; iteration != max_iterations && abs(right - left) > 2 * eps; iteration++)
	{
		x_c = (right + left) * 0.5;
		if (function(x_c - eps * 1e-1) > function(x_c + eps * 1e-1))
			left = x_c;
		else
			right = x_c;
	}
#if _DEBUG
	std::cout << "bisect::function arg range    : " << right - left << "\n";
	std::cout << "bisect::function probes count : " << iteration * 2  << "\n";
#endif
	return (right + left) * 0.5;
}

static F64 golden_ratio(function_1d function, F64 left, F64 right, const F64 eps, const I32 max_iterations)
{
	if (left > right) std::swap(left, right);
	I32 iteration = 0;
	F64 x_l = right - (right - left) * PSI;
	F64 x_r = left  + (right - left) * PSI;
	F64 f_l = function(x_l);
	F64 f_r = function(x_r);
	for (; iteration != max_iterations && abs(right - left) > 2 * eps; iteration++)
	{
		if (f_l > f_r)
		{
			left = x_l;
			x_l = x_r;
			f_l = f_r;
			x_r = left + (right - left) * PSI;
			f_r = function(x_r);
		}
		else
		{
			right = x_r;
			x_r = x_l;
			f_r = f_l;
			x_l = right - (right - left) * PSI;
			f_l = function(x_l);
		}
	}
#if _DEBUG
	std::cout << "golden_ratio::function arg range    : " << right - left << "\n";
	std::cout << "golden_ratio::function probes count : " << iteration + 2 << "\n";
#endif
	return (right + left) * 0.5;
}

static F64 fibonacci(function_1d function, F64 left, F64 right, const F64 eps)
{
	if (left > right) std::swap(left, right);
	F64 condition = (right - left) / eps;
	F64 fib_t{ 0.0 }, fib_1{ 1.0 }, fib_2{ 1.0 };
	I32 iterations{ 0 };
	while (fib_2 < condition) 
	{
		iterations++;
		fib_t = fib_1;
		fib_1 = fib_2;
		fib_2 += fib_t;
	}
	F64 x_l = left + (right - left) * ((fib_2 - fib_1) / fib_2);
	F64 x_r = left + (right - left) * (          fib_1 / fib_2);
	
	F64 f_l = function(x_l);
	F64 f_r = function(x_r);

	for(I32 index = iterations; index; index--)
	{
		fib_t = fib_2 - fib_1;
		fib_2 = fib_1;
		fib_1 = fib_t;
		if (f_l > f_r)
		{
			left = x_l;
			f_l = f_r;
			x_l = x_r;
			x_r = left + (right - left) * (fib_1 / fib_2);
			f_r = function(x_r);
		}
		else
		{
			right = x_r;
			x_r = x_l;
			f_r = f_l;
			x_l = left + (right - left) * ((fib_2 - fib_1) / fib_2);
			f_l = function(x_l);
		}
#if _DEBUG
	   std::cout << "\nfibonacchi [a, b] range: " << (right - left) << "\n";
#endif
	}
#if _DEBUG
	std::cout << "fibonacci::function arg range    : " << right - left << "\n";
	std::cout << "fibonacci::function probes count : " << iterations + 2 << "\n";
#endif
	return (right + left) * 0.5;
}

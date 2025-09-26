#pragma once
#include "numerics/common.h"
enum class search_method_type {
	bisection,
	golden_ratio,
	fibonacchi,
	none
};

struct search_result {
public:
	search_method_type type;
	UI64               iterations;
	UI64               function_probes;
	F64                accuracy;
	F64                result;
	search_result()
		: type           (search_method_type::none)
		, iterations     (                       0)
		, function_probes(                       0)
		, accuracy       (                     0.0)
		, result         (                     0.0)
	{}
	void clear() 
	{
		type            = search_method_type::none;
		iterations      =                        0;
		function_probes =                        0;
		accuracy        =                      0.0;
		result          =                      0.0;
	}
	search_result& operator=(const search_result& other) = delete;
	search_result& operator=(search_result&& other) = delete;
	friend std::ostream& operator<<(std::ostream&, const search_result&);
};

std::ostream& operator<<(std::ostream& stream, const search_result& result)
{
	switch (result.type)
	{
	case search_method_type::bisection:
		stream << fstring("method:         \"bisection\"\n");
		break;
	case search_method_type::golden_ratio:
		stream << fstring("method:         \"goldenRatio\"\n");
		break;
	case search_method_type::fibonacchi:
		stream << fstring("method:         \"fibonacchi\"\n");
		break;
	case search_method_type::none:
		stream << fstring("method:         \"none\"\n");
		return stream;
	};
	stream<<fstring("iterations:     %lld\n", result.iterations);
	stream<<fstring("functionProbes: %lld\n", result.function_probes);
	stream<<fstring("accuracy:       %le\n",  result.accuracy);
	stream<<fstring("result:         %le\n",  result.result);
	return stream;
}

typedef F64(*function_1d)(const F64);
static F64 bisect      (function_1d function, F64 left, F64 right, const F64 eps = ACCURACY, const I32 max_iterations = ITERS_MAX);
static F64 golden_ratio(function_1d function, F64 left, F64 right, const F64 eps = ACCURACY, const I32 max_iterations = ITERS_MAX);
static F64 fibonacci   (function_1d function, F64 left, F64 right, const F64 eps = ACCURACY);

static void bisect      (search_result & result, function_1d function, F64 left, F64 right, const F64 eps = ACCURACY, const I32 max_iterations = ITERS_MAX);
static void golden_ratio(search_result & result, function_1d function, F64 left, F64 right, const F64 eps = ACCURACY, const I32 max_iterations = ITERS_MAX);
static void fibonacci   (search_result & result, function_1d function, F64 left, F64 right, const F64 eps = ACCURACY);

static F64 bisect      (function_1d function, F64 left, F64 right, const F64 eps, const I32 max_iterations)
{
	search_result result;
	bisect(result, function, left, right, eps, max_iterations);
#ifdef _DEBUG 
	std::cout << result;
#endif // _DEBUG 
	return result.result;
}
static F64 golden_ratio(function_1d function, F64 left, F64 right, const F64 eps, const I32 max_iterations)
{
	search_result result;
	golden_ratio(result, function, left, right, eps, max_iterations);
#ifdef _DEBUG 
	std::cout << result;
#endif // _DEBUG 
	return result.result;
}
static F64 fibonacci   (function_1d function, F64 left, F64 right, const F64 eps)
{
	search_result result;
	fibonacci(result, function, left, right, eps);
#ifdef _DEBUG 
	std::cout << result;
#endif // _DEBUG 
	return result.result;
}

static void bisect      (search_result & result, function_1d function, F64 left, F64 right, const F64 eps, const I32 max_iterations)
{
	result.clear();
	result.type = search_method_type::bisection;
	if (left > right)
		std::swap(left, right);
	for (; result.iterations != max_iterations && (result.accuracy = abs(right - left)) > 2 * eps; result.iterations++, result.function_probes += 2)
	{
		result.result = (right + left) * 0.5;
		if (function(result.result - eps * 1e-1) > function(result.result + eps * 1e-1))
			left = result.result;
		else
			right = result.result;
	}
}
static void golden_ratio(search_result & result, function_1d function, F64 left, F64 right, const F64 eps, const I32 max_iterations)
{
	result.clear();
	result.type = search_method_type::golden_ratio;
	if (left > right)
		std::swap(left, right);
	I32 iteration = 0;
	F64 x_l = right - (right - left) * PSI;
	F64 x_r = left  + (right - left) * PSI;
	F64 f_l = function(x_l);
	F64 f_r = function(x_r);
	for (; result.iterations != max_iterations && (result.accuracy = abs(right - left)) > 2 * eps; result.iterations++)
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
	result.result          = (right + left) * 0.5;
	result.function_probes = result.iterations + 2;
}
static void fibonacci   (search_result & result, function_1d function, F64 left, F64 right, const F64 eps)
{
	result.clear();
	result.type = search_method_type::fibonacchi;
	if (left > right)
		std::swap(left, right);

	F64 condition = (right - left) / eps;
	F64 fib_t{ 0.0 }, fib_1{ 1.0 }, fib_2{ 1.0 };

	while (fib_2 < condition)
	{
		result.iterations++;
		fib_t = fib_1;
		fib_1 = fib_2;
		fib_2 += fib_t;
	}
	result.function_probes = result.iterations + 2;

	F64 x_l = left + (right - left) * ((fib_2 - fib_1) / fib_2);
	F64 x_r = left + (right - left) * (fib_1 / fib_2);

	F64 f_l = function(x_l);
	F64 f_r = function(x_r);

	for (I32 index = result.iterations; index; index--)
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
// #if _DEBUG
// 		std::cout << "\nfibonacchi [a, b] range: " << (right - left) << "\n";
// #endif
	}
	result.result   = (right + left) * 0.5;
	result.accuracy = (right - left);
}

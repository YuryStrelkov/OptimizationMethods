#pragma once
#include "numerics/numerics.h"

enum class search_method_type_nd {
	bisection,
	golden_ratio,
	fibonacchi,
	per_coordinate_descend,
	gradient_descend,
	conj_gradient_descend,
	newton,
	none
};

struct search_result_nd {
public:
	search_method_type_nd type;
	UI64                  iterations;
	UI64                  function_probes;
	F64                   accuracy;
	numerics::vector_f64  result;
	search_result_nd()
		: type           (search_method_type_nd::none)
		, iterations     (                       0)
		, function_probes(                       0)
		, accuracy       (                     0.0)
		, result         (                        )
	{}
	void clear() 
	{
		type            = search_method_type_nd::none;
		iterations      =                        0;
		function_probes =                        0;
		accuracy        =                      0.0;
		result.clear();
	}
	search_result_nd& operator=(const search_result_nd& other) = delete;
	search_result_nd& operator=(search_result_nd&& other) = delete;
	friend std::ostream& operator<<(std::ostream&, const search_result_nd&);
};

std::ostream& operator<<(std::ostream& stream, const search_result_nd& result)
{
	switch (result.type)
	{
	case search_method_type_nd::bisection:
		printf("method:         \"bisection\"\n");
		break;
	case search_method_type_nd::golden_ratio:
		printf("method:         \"goldenRatio\"\n");
		break;
	case search_method_type_nd::fibonacchi:
		printf("method:         \"fibonacchi\"\n");
		break;
	case search_method_type_nd::per_coordinate_descend:
		printf("method:         \"perCoordinateDescend\"\n");
		break;
	case search_method_type_nd::gradient_descend:
		printf("method:         \"gradientDescend\"\n");
		break;
	case search_method_type_nd::conj_gradient_descend:
		printf("method:         \"conjGradientDescend\"\n");
		break;
	case search_method_type_nd::newton:
		printf("method:         \"Newton\"\n");
		break;
	case search_method_type_nd::none:
		printf("method:         \"none\"\n");
		return stream;
	};
	stream << fstring("iterations:     %lld\n", result.iterations);
	stream << fstring("functionProbes: %lld\n", result.function_probes);
	stream << fstring("accuracy:       %le\n",  result.accuracy);
	stream << fstring("result:         ") << result.result;
	return stream;
}


typedef F64(*function_nd)(const numerics::vector_f64&);
static numerics::vector_f64 bisect      (function_nd function, const numerics::vector_f64& left, const numerics::vector_f64& right, const F64 eps = N_DIM_ACCURACY, const I32 max_iterations = N_DIM_ITERS_MAX);
static numerics::vector_f64 golden_ratio(function_nd function, const numerics::vector_f64& left, const numerics::vector_f64& right, const F64 eps = N_DIM_ACCURACY, const I32 max_iters = N_DIM_ITERS_MAX);
static numerics::vector_f64 fibonacci   (function_nd function, const numerics::vector_f64& left, const numerics::vector_f64& right, const F64 eps = N_DIM_ACCURACY);

static numerics::vector_f64 per_coord_descend    (function_nd function, const numerics::vector_f64& x_start, const F64 eps = N_DIM_ACCURACY, const I32 max_iters = N_DIM_ITERS_MAX);
static numerics::vector_f64 gradient_descend     (function_nd function, const numerics::vector_f64& x_start, const F64 eps = N_DIM_ACCURACY, const I32 max_iters = N_DIM_ITERS_MAX);
static numerics::vector_f64 conj_gradient_descend(function_nd function, const numerics::vector_f64& x_start, const F64 eps = N_DIM_ACCURACY, const I32 max_iters = N_DIM_ITERS_MAX);
static numerics::vector_f64 newtone_raphson      (function_nd function, const numerics::vector_f64& x_start, const F64 eps = N_DIM_ACCURACY, const I32 max_iters = N_DIM_ITERS_MAX);

static void bisect      (search_result_nd& result, function_nd function, const numerics::vector_f64& left, const numerics::vector_f64& right, const F64 eps = N_DIM_ACCURACY, const I32 max_iterations = N_DIM_ITERS_MAX);
static void golden_ratio(search_result_nd& result, function_nd function, const numerics::vector_f64& left, const numerics::vector_f64& right, const F64 eps = N_DIM_ACCURACY, const I32 max_iters = N_DIM_ITERS_MAX);
static void fibonacci   (search_result_nd& result, function_nd function, const numerics::vector_f64& left, const numerics::vector_f64& right, const F64 eps = N_DIM_ACCURACY);

static void per_coord_descend    (search_result_nd& result, function_nd function, const numerics::vector_f64& x_start, const F64 eps = N_DIM_ACCURACY, const I32 max_iters = N_DIM_ITERS_MAX);
// static void gradient_descend     (search_result_nd& result, function_nd function, const numerics::vector_f64& x_start, const F64 eps = N_DIM_ACCURACY, const I32 max_iters = N_DIM_ITERS_MAX);
// static void conj_gradient_descend(search_result_nd& result, function_nd function, const numerics::vector_f64& x_start, const F64 eps = N_DIM_ACCURACY, const I32 max_iters = N_DIM_ITERS_MAX);
// static void newtone_raphson      (search_result_nd& result, function_nd function, const numerics::vector_f64& x_start, const F64 eps = N_DIM_ACCURACY, const I32 max_iters = N_DIM_ITERS_MAX);

static numerics::vector_f64 bisect      (function_nd function, const numerics::vector_f64& left, const numerics::vector_f64& right, const F64 eps, const I32 max_iterations)
{
	search_result_nd result;
	bisect(result, function, left, right, eps, max_iterations);
#ifdef _DEBUG
	std::cout << result;
#endif // _DEBUG
	return result.result;
}
static numerics::vector_f64 golden_ratio(function_nd function, const numerics::vector_f64& left, const numerics::vector_f64& right, const F64 eps, const I32 max_iterations)
{
	search_result_nd result;
	golden_ratio(result, function, left, right, eps, max_iterations);
#ifdef _DEBUG
	std::cout << result;
#endif // _DEBUG
	return result.result;
}
static numerics::vector_f64 fibonacci   (function_nd function, const numerics::vector_f64& left, const numerics::vector_f64& right, const F64 eps)
{
	search_result_nd result;
	fibonacci(result, function, left, right, eps);
#ifdef _DEBUG
	std::cout << result;
#endif // _DEBUG
	return result.result;
}

static void bisect      (search_result_nd& result, function_nd function, const numerics::vector_f64& left, const numerics::vector_f64& right, const F64 eps, const I32 max_iterations)
{
	result.clear();
	result.type = search_method_type_nd::bisection;
	numerics::vector_f64 dir, lhs(left), rhs(right);
	dir = numerics::vector_f64::direction(lhs, rhs) * eps;
	for (; result.iterations != max_iterations && (result.accuracy = numerics::vector_f64::distance(lhs, rhs)) > 2 * eps; result.iterations++, result.function_probes+=2)
	{
		result.result = (lhs + rhs) * 0.5;
		if (function(result.result - dir) > function(result.result + dir))
			lhs = result.result;
		else
			rhs = result.result;
	}
}
static void golden_ratio(search_result_nd& result, function_nd function, const numerics::vector_f64& left, const numerics::vector_f64& right, const F64 eps, const I32 max_iterations)
{
	result.clear();
	result.type = search_method_type_nd::golden_ratio;
	numerics::vector_f64 lhs(left), rhs(right);
	numerics::vector_f64 x_l = rhs - (rhs - lhs) * PSI;
	numerics::vector_f64 x_r = lhs + (rhs - lhs) * PSI;
	F64 f_l = function(x_l);
	F64 f_r = function(x_r);
	result.function_probes = 2;
	for (; result.iterations != max_iterations && (result.accuracy = numerics::vector_f64::distance(lhs, rhs)) > 2 * eps; result.iterations++, result.function_probes++)
	{
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
	result.result = (right + left) * 0.5;
}
static void fibonacci   (search_result_nd& result, function_nd function, const numerics::vector_f64& left, const numerics::vector_f64& right, const F64 eps)
{
	result.clear();
	result.type = search_method_type_nd::fibonacchi;
	numerics::vector_f64 lhs(left), rhs(right);
	F64 condition = numerics::vector_f64::distance(lhs, rhs) / eps;
	F64 fib_t{ 0.0 }, fib_1{ 1.0 }, fib_2{ 1.0 };
	result.function_probes = 2;
	while (fib_2 < condition)
	{
		result.iterations++;
		result.function_probes++;
		fib_t = fib_1;
		fib_1 = fib_2;
		fib_2 += fib_t;
	}
	numerics::vector_f64 x_l = lhs + (rhs - lhs) * ((fib_2 - fib_1) / fib_2);
	numerics::vector_f64 x_r = lhs + (rhs - lhs) * (fib_1 / fib_2);
	F64 f_l = function(x_l);
	F64 f_r = function(x_r);
	for (I32 index = result.iterations; index > 0; index--)
	{
		fib_t = fib_2 - fib_1;
		fib_2 = fib_1;
		fib_1 = fib_t;
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
		// #if _DEBUG
		// 	   std::cout << "\nfibonacchi [a, b] range: " << (right - left) << "\n";
		// #endif
	}
	result.result   = (rhs + lhs) * 0.5;
	result.accuracy = numerics::vector_f64::distance(rhs, lhs);
}

static numerics::vector_f64 per_coord_descend(function_nd function, const numerics::vector_f64& x_start, const F64 eps, const I32 max_iters)
{
	search_result_nd result;
	per_coord_descend(result, function, x_start, eps, max_iters);
#ifdef _DEBUG
	std::cout << result;
#endif // _DEBUG
	return result.result;
}
static void per_coord_descend(search_result_nd& result, function_nd function, const numerics::vector_f64& x_start, const F64 eps, const I32 max_iters)
{
	result.clear();
	UI64 total_probes = 0;
	numerics::vector_f64 x_0(x_start);
	numerics::vector_f64 x_1(x_start);
	F64 step = 1.0;
	F64 x_i, y_1, y_0;
	I32 opt_coord_n = 0, coord_id, iterations;
	for (iterations = 0; iterations < max_iters; ++iterations)
	{
		coord_id = iterations % x_0.size();
		x_1[coord_id]-= eps;
		y_0 = function(x_1);
		x_1[coord_id]+= 2.0 * eps;
		y_1 = function(x_1);
		x_1[coord_id] -= eps;
		x_1[coord_id] = y_0 > y_1 ? x_1[coord_id] += step : x_1[coord_id] -= step;
		x_i = x_0[coord_id];
		fibonacci(result, function, x_0, x_1, eps);
		x_0 = result.result;
		total_probes += result.function_probes + 2;
		if (std::abs(x_1[coord_id] - x_i) < 2 * eps)
		{
			opt_coord_n++;
			if (opt_coord_n == x_1.size())
			{
// #if DISPLAY_PROGRES
// 				std::cout << "\nper coord descend iterations number : " << iterations << "\n";
// #endif
				break;
			}
			continue;
		}
		opt_coord_n = 0;
	}
// #if DISPLAY_PROGRES
// 	std::cout << "per coord descend iterations number : " << max_iters << "\n";
// #endif
	result.type            = search_method_type_nd::per_coordinate_descend;
	result.iterations      = iterations;
	result.function_probes = total_probes;
	// return x_0;
}


static numerics::vector_f64 gradient_descend(function_nd function, const numerics::vector_f64& x_start, const F64 eps, const I32 max_iters)
{
	numerics::vector_f64 x_i(x_start);
	numerics::vector_f64 x_i_1;
	numerics::vector_f64 grad;
	I32 cntr = 0;
	for(; cntr <= max_iters; cntr++)
	{
		grad  = numerics::vector_f64::gradient(function, x_i, eps);
		x_i_1 = x_i - grad;
		x_i_1 = fibonacci(function, x_i, x_i_1, eps);
		if (numerics::vector_f64::distance(x_i_1, x_i) < 2 * eps) break;
		x_i = x_i_1;
	}
#if DISPLAY_PROGRES
	std::cout << "\ngradient descend iterations number : " << cntr << "\n";
#endif
	return (x_i_1 + x_i) * 0.5;
}
static numerics::vector_f64 conj_gradient_descend(function_nd function, const numerics::vector_f64& x_start, const F64 eps, const I32 max_iters)
{
	numerics::vector_f64 x_i(x_start);
	numerics::vector_f64 x_i_1;
	numerics::vector_f64 s_i = numerics::vector_f64::gradient(function, x_i, eps)*(-1.0), s_i_1;
	F64 omega;
	I32 cntr = 0;
	for (; cntr <= max_iters; cntr++)
	{
		x_i_1 = x_i + s_i;
		if (numerics::vector_f64::distance(x_i_1, x_i) < 2 * eps) break;
		x_i_1 = fibonacci(function, x_i, x_i_1, eps);
		s_i_1 = numerics::vector_f64::gradient(function, x_i_1, eps);
		omega = std::pow(s_i_1.magnitude(), 2) / std::pow(s_i.magnitude(), 2);
		s_i = s_i * omega - s_i_1;
		x_i = x_i_1;
	}
#if DISPLAY_PROGRES
	std::cout << "\nconj gradient descend iterations number : " << cntr << "\n";
#endif
	return (x_i_1 + x_i) * 0.5;
}
static numerics::vector_f64 newtone_raphson (function_nd function, const numerics::vector_f64& x_start, const F64 eps, const I32 max_iters)
{
	numerics::vector_f64 x_i(x_start);
	numerics::vector_f64 x_i_1;
	numerics::vector_f64 grad ;
	numerics::matrix_f64 hess(1, 1);
	I32 cntr = 0;
	for (; cntr <= max_iters; cntr++)
	{
		grad = numerics::vector_f64::gradient(function, x_i, eps);
		hess = numerics::matrix_f64::invert(numerics::matrix_f64::hessian(function, x_i, eps));
		x_i_1 = x_i - (hess * grad);
		if (numerics::vector_f64::distance(x_i_1, x_i) < 2 * eps) break;
		x_i = x_i_1;
	}
#if DISPLAY_PROGRES
	std::cout << "\nNewtone-Raphson method iterations number : " << cntr << "\n";
#endif
	return (x_i_1 + x_i) * 0.5;
}

#pragma once
#include "numerics/numerics.h"

typedef F64(*function_nd)(const numerics::vector_f64&);
static numerics::vector_f64 bisect      (function_nd function, const numerics::vector_f64& left, const numerics::vector_f64& right, const F64 eps = N_DIM_ACCURACY, const I32 max_iterations = N_DIM_ITERS_MAX);
static numerics::vector_f64 golden_ratio(function_nd function, const numerics::vector_f64& left, const numerics::vector_f64& right, const F64 eps = N_DIM_ACCURACY, const I32 max_iters = N_DIM_ITERS_MAX);
static numerics::vector_f64 fibonacci   (function_nd function, const numerics::vector_f64& left, const numerics::vector_f64& right, const F64 eps = N_DIM_ACCURACY);

static numerics::vector_f64 per_coord_descend    (function_nd function, const numerics::vector_f64& x_start, const F64 eps = N_DIM_ACCURACY, const I32 max_iters = N_DIM_ITERS_MAX);
static numerics::vector_f64 gradient_descend     (function_nd function, const numerics::vector_f64& x_start, const F64 eps = N_DIM_ACCURACY, const I32 max_iters = N_DIM_ITERS_MAX);
static numerics::vector_f64 conj_gradient_descend(function_nd function, const numerics::vector_f64& x_start, const F64 eps = N_DIM_ACCURACY, const I32 max_iters = N_DIM_ITERS_MAX);
static numerics::vector_f64 newtone_raphson      (function_nd function, const numerics::vector_f64& x_start, const F64 eps = N_DIM_ACCURACY, const I32 max_iters = N_DIM_ITERS_MAX);


// Ìåòîäû n-ìåðíîé äèõîòîìèè, çîëîòîãî ñå÷åíèÿ è Ôèáîíà÷÷è îïðåäåëÿþò ìèíèìóì ñòðîãî âäîëü íàïðàâëåíèÿ èç  x_0 â x_1
// ò.å., åñëè èñòèííûé ìèíèìóì ôóíêöèè íà ýòîì íàïðàâëåíèè íå ëåæèò, ìåòîä âñ¸ ðàâíî íàéä¸ò ìèíèìàëüíîå çíà÷åíèå, íî îíî 
// áóäåò îòëè÷àòüñÿ îò èñòèííîãî ìèíèìóìà
static numerics::vector_f64 bisect(function_nd function, const numerics::vector_f64& left, const numerics::vector_f64& right, const F64 eps, const I32 max_iterations)
{
	numerics::vector_f64 x_c, dir, lhs(left), rhs(right);
	dir = numerics::vector_f64::direction(lhs, rhs) * eps;
	I32 iteration = 0;
	for (; iteration != max_iterations && numerics::vector_f64::distance(lhs, rhs) > 2 * eps; iteration++)
	{
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

static numerics::vector_f64 golden_ratio(function_nd function, const numerics::vector_f64& left, const numerics::vector_f64& right, const F64 eps, const I32 max_iterations)
{
	numerics::vector_f64 lhs(left), rhs(right);
	numerics::vector_f64 x_l, x_r;
	F64 f_l, f_r;
	I32 iteration = 0;
	x_l = rhs - (rhs - lhs) * PSI;
	x_r = lhs + (rhs - lhs) * PSI;
	f_l = function(x_l);
	f_r = function(x_r);
	for (; iteration != max_iterations && numerics::vector_f64::distance(lhs, rhs) > 2 * eps; iteration++)
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
#if _DEBUG
	std::cout << "golden_ratio::function arg range    : " << (rhs - lhs).magnitude() << "\n";
	std::cout << "golden_ratio::function probes count : " << iteration + 2 << "\n";
#endif
	return (right + left) * 0.5;
}

static numerics::vector_f64 fibonacci(function_nd function, const numerics::vector_f64& left, const numerics::vector_f64& right, const F64 eps)
{
	numerics::vector_f64 lhs(left), rhs(right);
	F64 condition = numerics::vector_f64::distance(lhs, rhs) / eps;
	F64 fib_t{ 0.0 }, fib_1{ 1.0 }, fib_2{ 1.0 };
	I32 iterations{ 0 };
	while (fib_2 < condition)
	{
		iterations++;
		fib_t = fib_1;
		fib_1 = fib_2;
		fib_2 += fib_t;
	}
	numerics::vector_f64 x_l = lhs + (rhs - lhs) * ((fib_2 - fib_1) / fib_2);
	numerics::vector_f64 x_r = lhs + (rhs - lhs) * (fib_1 / fib_2);
	F64 f_l = function(x_l);
	F64 f_r = function(x_r);
	for (I32 index = iterations; index > 0; index--)
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
#if _DEBUG
	std::cout << "fibonacci::function arg range    : " << (rhs - lhs).magnitude() << "\n";
	std::cout << "fibonacci::function probes count : " << iterations + 2 << "\n";
#endif
	return (rhs + lhs) * 0.5;
}

// Ïîêîîðäèíàòíûé ñïóñê, ãðàäèåíòíûé ñïóñê è ñïóñê ñ ïîìîùüþ ñîïðÿæ¸ííûõ ãðàäèåíòîâ, îïðåäåëÿþò
// ìèíèìàëüíîå çíà÷åíèå ôóíêöèè òîëüêî ïî îäíîé íà÷àëüíîé òî÷êå x_start.
// Ïîýòîìó íå çàâèñÿò îò âûáîðà íàïðàâëåíèÿ.
static numerics::vector_f64 per_coord_descend(function_nd function, const numerics::vector_f64& x_start, const F64 eps, const I32 max_iters)
{
	numerics::vector_f64 x_0(x_start);
	numerics::vector_f64 x_1(x_start);
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
		x_1 = fibonacci(function, x_0, x_1, eps);
		x_0 = x_1;
		if (std::abs(x_1[coord_id] - x_i) < 2 * eps)
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

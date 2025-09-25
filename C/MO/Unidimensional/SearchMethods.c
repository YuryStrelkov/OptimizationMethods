#include "../Math/Constatnts.h"
#include "SearchMethods.h"
#include <malloc.h>
#include <math.h>
#include <stdio.h>

void setupEmptyResult(struct SearchResult** result)
{
	(*result)->type           = (enum SearchMethodType)3;
	(*result)->functionProbes = 0;
	(*result)->iterations     = 0;
	(*result)->accuracy       = 1e32;
	(*result)->result         = 0.0;
}

uint8_t createResult(struct SearchResult** result)
{
	if (*result)
	{
		setupEmptyResult(result);
		return 1;
	}
	if (*result = (struct SearchResult*)malloc(sizeof(struct SearchResult)))
	{
		setupEmptyResult(result);
		return 1;
	}
	return 0;
};

void printResult(const struct SearchResult** result)
{
	if (*result)
	{
		switch ((*result)->type)
		{
		case m_bisection:
			printf("method:         \"bisection\"\n");
			break;
		case m_goldenRatio:
			printf("method:         \"goldenRatio\"\n");
			break;
		case m_fibonacchi:
			printf("method:         \"fibonacchi\"\n");
			break;
		case m_none:
			printf("method:         \"none\"\n");
			return;
		};
		printf("iterations:     %lld\n", (*result)->iterations);
		printf("functionProbes: %lld\n", (*result)->functionProbes);
		printf("accuracy:       %le\n",  (*result)->accuracy);
		printf("result:         %le\n",  (*result)->result);
	}
};

void destroyResult(struct  SearchResult** result)
{
	if (*result)
	{
		free(*result);
		*result = NULL;
	}
};

void bisection(struct SearchResult** result, function1d target, double lhs, double rhs, double accuracy, uint64_t maxIters)
{
	if (!createResult(result))
		return;
	if (lhs > rhs)
		dSwap(&lhs, &rhs);
	double xc;
	for (; (*result)->iterations < maxIters && fabs(rhs - lhs) > 2.0 * accuracy; ++((*result)->iterations), (*result)->functionProbes += 2)
	{
		xc = (lhs + rhs) * 0.5;
		if (target(xc - accuracy) > target(xc + accuracy))
			lhs = xc;
		else
			rhs = xc;
	}
	(*result)->type     = (enum SearchMethodType)0;
	(*result)->accuracy = fabs(rhs - lhs);
	(*result)->result   = 0.5 * (rhs + lhs);
};

void goldenRatio(struct SearchResult** result, function1d target, double lhs, double rhs, double accuracy, uint64_t maxIters)
{
	if (!createResult(result))
		return;
	if (lhs > rhs)
		dSwap(&lhs, &rhs);
	double xl = rhs - (rhs - lhs) * psi;
	double xr = lhs + (rhs - lhs) * psi;
	double fl = target(xl);
	double fr = target(xr);
	for (; (*result)->iterations < maxIters && fabs(rhs - lhs) > 2.0 * accuracy; ++((*result)->iterations))
	{
		if (fl > fr)
		{
			lhs = xl;
			xl  = xr;
			fl  = fr;
			xr  = lhs + (rhs - lhs) * psi;
			fr  = target(xr);
		}
		else
		{
			rhs = xr;
			xr  = xl;
			fr  = fl;
			xl  = rhs - (rhs - lhs) * psi;
			fl  = target(xl);
		}
	}
	(*result)->functionProbes = (*result)->iterations + 2;
	(*result)->type           = (enum SearchMethodType)1;
	(*result)->accuracy       = fabs(rhs - lhs);
	(*result)->result         = 0.5 * (rhs + lhs);
};

void fibonacci(struct SearchResult** result, function1d target, double lhs, double rhs, double accuracy)
{
	if (!createResult(result))
		return;
	if (lhs > rhs)
		dSwap(&lhs, &rhs);
	double condition = (rhs - lhs) / accuracy;
	double fib_t = 0.0, fib_1 = 1.0, fib_2 = 1.0;
	int64_t iterations = 0;
	while (fib_2 < condition)
	{
		iterations++;
		fib_t = fib_1;
		fib_1 = fib_2;
		fib_2 += fib_t;
	}
	double x_l = lhs + (rhs - lhs) * ((fib_2 - fib_1) / fib_2);
	double x_r = lhs + (rhs - lhs) * (fib_1 / fib_2);

	double f_l = target(x_l);
	double f_r = target(x_r);

	for (int64_t index = iterations; index; index--, ++((*result)->iterations))
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
			f_r = target(x_r);
		}
		else
		{
			rhs = x_r;
			x_r = x_l;
			f_r = f_l;
			x_l = lhs + (rhs - lhs) * ((fib_2 - fib_1) / fib_2);
			f_l = target(x_l);
		}
	};
	(*result)->functionProbes = (*result)->iterations + 2;
	(*result)->type           = (enum SearchMethodType)2;
	(*result)->accuracy       = fabs(rhs - lhs);
	(*result)->result         = 0.5 * (rhs + lhs);
}
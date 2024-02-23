#pragma once
#include "numeric_vector.h"

static int get_factorial(const int32_t number) {
	int result = 1;
	for (int i = 1; i <= number; i++) result = result * i;
	return result;
};

static int_vector calculate_factorials()
{
	int_vector factorials(128);
	factorials.fill([&](const int32_t index) {return get_factorial(index); });
	return factorials;
};

static int_vector fibonacci_numbers(const int32_t index) {
	if (index < 1) return int_vector({ 0 });
	if (index < 2) return int_vector({ 0, 1 });
	int_vector res = int_vector(index);
	res[0] = 0;
	res[1] = 1;
	for (int i = 2; i < index; i++) res[i] = res[i - 1] + res[i - 2];
	return res;
};

static void closest_fibonacci_pair(const double value, int32_t& fib_n, int32_t& fib_n_1) {
	fib_n = 0;
	fib_n_1 = 0;
	if (value < 1) return;
	fib_n_1 = 1;
	if (value < 2) return;
	int f_tmp;
	while (fib_n < value) {
		f_tmp = fib_n;
		fib_n = fib_n_1;
		fib_n_1 += f_tmp;
	}
};

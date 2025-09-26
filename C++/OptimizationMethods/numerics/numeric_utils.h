#pragma once
#include "linalg/numeric_vector.h"

static int get_factorial(const I32 number) {
	int result = 1;
	for (int i = 1; i <= number; i++) result = result * i;
	return result;
};

static numerics::vector_i32 calculate_factorials(const I32 number = 128)
{
	numerics::vector_i32 factorials(number);
	factorials.fill([](const I32 index) {return get_factorial(index); });
	return factorials;
};

static numerics::vector_i32 fibonacci_numbers(const I32 index) {
	if (index < 1) return numerics::vector_i32({ 0 });
	if (index < 2) return numerics::vector_i32({ 0, 1 });
	numerics::vector_i32 res = numerics::vector_i32(index);
	res[0] = 0;
	res[1] = 1;
	for (int i = 2; i < index; i++) res[i] = res[i - 1] + res[i - 2];
	return res;
};

static void closest_fibonacci_pair(const F64 value, I32& fib_n, I32& fib_n_1) {
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

#pragma once
#include <functional>
#include <algorithm>
#include <string>
#include <iterator>
#include <initializer_list>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cassert>
class matrix_type {};
class vector_type {};
#define PHI 1.61803398874989484820
#define ONE_OVER_PHI 0.6180339887498948
typedef std::vector<double>   vec_n;
typedef std::vector<vec_n>    mat_mn;
#define DOUBLE_PRECISION      1
#define N_DIM_ACCURACY        1e-5
#define N_DIM_DERIVATIVE_STEP 1e-6
#define N_DIM_ITERS_MAX       1000
#define ACCURACY              1e-5
#define ITERS_MAX             50
#define MAX_DENOMINATOR       1000
#define NUMBER_CHARS_COUNT    11
#define DECIMAL_NUMBERS_REPRESNTATION 0
#define RATIONAL_NUMBERS_REPRESNTATION (!DECIMAL_NUMBERS_REPRESNTATION)
#define MAX(A, B) (A > B ? A : B)
#define MIN(A, B) (A < B ? A : B)
#define DISPLAY_PROGRES (_DEBUG? 1 : 0)

auto sum_f = [&](const auto& lhs, const auto& rhs) { return lhs + rhs; };
auto dif_f = [&](const auto& lhs, const auto& rhs) { return lhs - rhs; };
auto div_f = [&](const auto& lhs, const auto& rhs) { return lhs / rhs; };
auto mul_f = [&](const auto& lhs, const auto& rhs) { return lhs * rhs; };

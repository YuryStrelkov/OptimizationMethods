#pragma once
#include <vector>
#define phi 1.61803398874989484820
typedef std::vector<double>   vec_n;
typedef std::vector<vec_n>    mat_mn;
typedef double(*func_1d)(const double&);
typedef double(*func_nd)(const vec_n&);
#define N_DIM_ACCURACY        1e-6
#define N_DIM_DERIVATIVE_STEP 1e-6
#define N_DIM_ITERS_MAX       1000
#define ACCURACY              1e-6
#define ITERS_MAX             1000
#define MAX_DENOMINATOR       1000





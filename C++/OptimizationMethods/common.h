#pragma once
#include <vector>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>

#define phi 1.61803398874989484820
typedef std::vector<double>   vec_n;
typedef std::vector<vec_n>    mat_mn;
typedef double(*func_1d)(const double&);
typedef double(*func_nd)(const vec_n&);
#define DOUBLE_PRECISION      1
#define N_DIM_ACCURACY        1e-6
#define N_DIM_DERIVATIVE_STEP 1e-6
#define N_DIM_ITERS_MAX       1000
#define ACCURACY              1e-6
#define ITERS_MAX             50
#define MAX_DENOMINATOR       1000
#define MAX(A, B) (A > B ? A : B)
#define MIN(A, B) (A < B ? A : B)
#define DISPLAY_PROGRES _DEBUG? 1 : 0
const char* LINE_UP = "\033[1A";
const char* LINE_CLEAR = "\x1b[2K\r";


void progresBar(const float& progress,
			    const int& prgress_steps = 50,
				const char& progress_char = ':',
				const char& stand_by_char = '.', 
				std::ostream& stream = std::cout);

void progresBar(const float& progress, const int& prgress_steps, const char& progress_char, const char& stand_by_char, std::ostream& stream)
{
	int filler_l = int(MIN(MAX(0.0f, progress), 1.0f) * prgress_steps);
	int filler_r = prgress_steps - filler_l;
	stream << '\r';
	stream << '|';
	int index = 0;
	for (index = 0; index <= filler_l; index++) stream << progress_char;
	for (index = 0; index <= filler_r; index++) stream << stand_by_char;
	stream << '|'<<"[" << int((float)filler_l / prgress_steps * 100.0f) <<"%]";
#ifdef _DEBUG
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
#endif
}



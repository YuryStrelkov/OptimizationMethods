#pragma once
#include "multi_dimensional.h"
#include "one_dimensional.h"
#include "numerics/numeric_utils.h"
#include "simplex.h"

// тестовая унимодальная одномерная функция с минимумом в точке {2} 
static F64 test_function_1d(const F64 x)
{
	return  x * (x - 5);
}

// тестовая унимодальная двумерная функция с минимумом в точке {2,2} 
static F64 test_function_2d(const numerics::vector_f64& args)
{
	return (args[0] - 5) * args[0] + (args[1] - 3) * args[1];
}

// тестовая унимодальная n - мерная функция с минимумом в точке {2,...,2} 
static F64 test_function_nd(const numerics::vector_f64& args)
{
	F64  val = 0;
	for (const F64& x : args.values()) 
	{
		val += test_function_1d(x);
	}
	return val;
}

static void one_dimensional_methods_test()
{
	std::cout << "\n///////////////////////////////////////////"   << "\n";
	std::cout <<   "//////// OneDimensionalMethodsTest ////////"   << "\n";
	std::cout <<   "///////////////////////////////////////////\n" << "\n";
	F64 x_0 = 125;
	F64 x_1 = -5;
	std::cout << "\n";
	std::cout << "x = agrmin(x * (x - 5))\n";
	std::cout << "x_0 = " << x_0 << ",\nx_1 = " << x_1 << "\n";
	// std::cout << "bisect      : " << rational::rational_number(bisect      (test_function_1d, x_0, x_1, ACCURACY)) << "\n";
	// std::cout << "golden_ratio: " << rational::rational_number(golden_ratio(test_function_1d, x_0, x_1, ACCURACY)) << "\n";
	// std::cout << "fibonacci   : " << rational::rational_number(fibonacci   (test_function_1d, x_0, x_1, ACCURACY)) << "\n";
	std::cout << bisect      (test_function_1d, x_0, x_1, ACCURACY) << "\n";
	std::cout << golden_ratio(test_function_1d, x_0, x_1, ACCURACY * 0.7) << "\n";
	std::cout << fibonacci   (test_function_1d, x_0, x_1, ACCURACY) << "\n";
}

static void multi_dimensional_methods_test()
{
	std::cout << "\n/////////////////////////////////////////////"  << "\n";
	std::cout <<   "//////// MultiDimensionalMethodsTest ////////"  << "\n";
	std::cout <<   "/////////////////////////////////////////////\n"<< "\n";

	numerics::vector_f64 x_0 = { 0,0 };
	numerics::vector_f64 x_1 = { 5,3 };

	std::cout << "\n";
	std::cout << "x_0 = " << x_0 << ",\nx_1 = " << x_1 << "\n";
	std::cout << "bisect                : " << bisect	   (test_function_2d, x_1, x_0, ACCURACY) << "\n";
	std::cout << "golden_ratio          : " << golden_ratio(test_function_2d, x_1, x_0, ACCURACY) << "\n";
	std::cout << "fibonacci             : " << fibonacci   (test_function_2d, x_1, x_0, ACCURACY) << "\n";
	std::cout << "\n";
	numerics::vector_f64 x_start = { -0,0 };
	std::cout << "per_coord_descend     : " << per_coord_descend    (test_function_2d, x_start, ACCURACY) << "\n";
	std::cout << "gradient_descend      : " << gradient_descend     (test_function_2d, x_start, ACCURACY) << "\n";
	std::cout << "conj_gradient_descend : " << conj_gradient_descend(test_function_2d, x_start, ACCURACY) << "\n";
	std::cout << "newtone_raphson       : " << newtone_raphson      (test_function_2d, x_start, ACCURACY) << "\n";
}

static void simplex_method_test()
{
	std::cout << "\n/////////////////////////////\n";
	std::cout <<   "//////// SimplexTest ////////\n";
	std::cout <<   "/////////////////////////////\n";
	std::cout << " f(x,c) =  2x1 + 3x2;\n arg_max = {4, 8}, f(arg_max) = 32\n";
	std::cout << " |-2x1 + 6x2 <= 40\n";
	std::cout << " | 3x1 + 2x2 <= 28\n";
	std::cout << " | 2x1 -  x2 <= 14\n\n";
	
	sm::simplex sym_0(numerics::matrix_f64({ -2, 6, 3, 2, 2, -1 }, 3, 2), { 2, 3 }, { sm::LESS_EQUAL, sm::LESS_EQUAL, sm::LESS_EQUAL }, { 40, 28, 14 });
	sym_0.solve(SIMPLEX_MAX); 
	std::cout << "\n";
	std::cout << " f(x,c) = -2x1 + 3x2;\n arg_min = {7, 0}, f(arg_min) =-14\n\n";

	sm::simplex sym_1(numerics::matrix_f64({-2, 6, 3, 2, 2, -1}, 3, 2), { -2, 3 }, { sm::LESS_EQUAL, sm::LESS_EQUAL, sm::LESS_EQUAL }, { 40, 28, 14 });
	sym_1.solve(SIMPLEX_MIN);
	std::cout << "\n";

	std::cout << "/////////////////////////////\n" << std::endl;
	std::cout << " f(x,c) =  2x1 + 3x2;\n arg_min = {62/5, 54/5}, f(arg_max) = 57 1/5\n";
	std::cout << " |-2x1 + 6x2 >= 40\n";
	std::cout << " | 3x1 + 2x2 >= 28\n";
	std::cout << " | 2x1 -  x2 >= 14\n\n";
	sm::simplex sym_2(numerics::matrix_f64({ -2, 6, 3, 2, 2, -1}, 3, 2), {2, 1}, {sm::MORE_EQUAL, sm::MORE_EQUAL, sm::MORE_EQUAL}, {40, 28, 14});
	sym_2.solve(SIMPLEX_MIN);
	std::cout << " f(x,c) =  -2x1 - x2;\n arg_min = {62/5, 54/5}, f(arg_max) = -35 3/5\n";
	std::cout << "\n";
	sm::simplex sym_3(numerics::matrix_f64({ -2, 6, 3, 2, 2, -1 }, 3, 2), {-2, -1}, {sm::MORE_EQUAL, sm::MORE_EQUAL, sm::MORE_EQUAL}, {40, 28, 14});
	sym_3.solve(SIMPLEX_MAX);
	std::cout << " f(x,c) =  2x1 + 3x2;\n arg_min = {none, none}, f(arg_max) = none\n";
	std::cout << "\n"; 
	sm::simplex sym_4(numerics::matrix_f64({ -2, 6, 3, 2, 2,-1 },3, 2), {2, 3}, {sm::MORE_EQUAL, sm::MORE_EQUAL, sm::MORE_EQUAL}, {40, 28, 14});
	sym_4.solve(SIMPLEX_MAX);
	std::cout << "/////////////////////////////\n" << std::endl;
	// f(x,c) = 2x1 + 3x2 -> arg_min = {62/5, 54/5}, f(arg_min) = 57 1/5 , arg_max = {-, -}, f(arg_max) = - (не работает учёт неограниченности) 
	//
	//         -2x1 + 6x2 = 40 
	//          3x1 + 2x2 = 28
	//          2x1 -  x2 = 14
	//
	sm::simplex sym_5(numerics::matrix_f64({ -2, 6, 3, 2, 2,-1 }, 3, 2), {2, 3}, {sm::EQUAL, sm::EQUAL, sm::EQUAL}, {40, 28, 14});

	sym_5.solve();

	std::cout << "/////////////////////////////\n";
}

static void numeric_tests()
{
	std::cout << "\n//////////////////////////////\n";
	std::cout <<   "//////// NumericTests ////////\n";
	std::cout <<   "//////////////////////////////\n";
	std::cout << "1.6666       = " << numerics::rational_number(1.6666     ) << "\n";
	std::cout << "0.6666       = " << numerics::rational_number(0.6666     ) << "\n";
	std::cout << "-3.0         = " << numerics::rational_number(-3.0       ) << "\n";
	std::cout << "-0.125       = " << numerics::rational_number(-0.125     ) << "\n";
	std::cout << "3.769230769  = " << numerics::rational_number(3.769230769) << "\n";

	std::cout << "-1.6666      = " << numerics::rational_number(-1.6666     ) << "\n";
	std::cout << "-0.6666      = " << numerics::rational_number(-0.6666     ) << "\n";
	std::cout << "3.0          = " << numerics::rational_number(3.0         ) << "\n";
	std::cout << "0.125        = " << numerics::rational_number(0.125       ) << "\n";
	std::cout << "-3.769230769 = " << numerics::rational_number(-3.769230769) << "\n";
}

static int test_all()
{
	// numerics::vector_f32 a = { 1, 2, 3, 4 };
	// numerics::vector_f32 b = { 2, 3, 4, 5 };
	// std::cout << "numerics::vector_f32::distance(a, b)" << numerics::vector_f32::distance(a, b);;
	numerics::template_vector_test();
	numerics::numeric_vector_test();
	numerics::numeric_matrix_test();
	numeric_tests();
	one_dimensional_methods_test();
	multi_dimensional_methods_test();
	simplex_method_test();
	return 0;
}
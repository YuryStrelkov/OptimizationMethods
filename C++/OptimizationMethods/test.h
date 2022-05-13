#pragma once
#include "one_dimensional.h";
#include "multi_dimensional.h";
#include "matrix_utils.h"
#include "simplex.h"
#include "numeric_utils.h"
// тестовая унимодальная одномерная функция с минимумом в точке {2} 
static double test_function_1d(const double x)
{
	return  (x - 2) * (x - 5);
}

// тестовая унимодальная двумерная функция с минимумом в точке {2,2} 
static double test_function_2d(const vec_n& args)
{
	return (args[0] - 5) * args[0] + (args[1] - 3) * args[1];
}

// тестовая унимодальная n - мерная функция с минимумом в точке {2,...,2} 
static double test_function_nd(const vec_n& args)
{
	double  val = 0;
	for (auto const& x : args) 
	{
		val += test_function_1d(x);
	}
	return val;
}

static void  one_dimensional_methods_test()
{
	std::cout << "\n///////////////////////////////////////////" << std::endl;
	std::cout <<   "//////// OneDimensionalMethodsTest ////////" << std::endl;
	std::cout <<   "///////////////////////////////////////////\n" << std::endl;
	double x_0 = 125;
	double x_1 = -5;
	std::cout << "\n";
	std::cout << "x = agrmin(x * (x - 5))\n";
	std::cout << "x_0 = " << x_0 << ",\nx_1 = " << x_1 << "\n";
	std::cout << "dihotomia   : " << dihotomia   (test_function_1d, x_0, x_1, 1e-4) << "\n";
	std::cout << "golden_ratio: " << golden_ratio(test_function_1d, x_0, x_1, 1e-4) << "\n";
	std::cout << "fibonacci   : " << fibonacci   (test_function_1d, x_0, x_1, 1e-4) << "\n";
}

static void multi_dimensional_methods_test()
{
	std::cout << "\n/////////////////////////////////////////////" << std::endl;
	std::cout <<   "//////// MultiDimensionalMethodsTest ////////" << std::endl;
	std::cout <<   "/////////////////////////////////////////////\n" << std::endl;

	vec_n x_0 = { 0,0 };
	vec_n x_1 = { 5,3 };

	std::cout << "\n";
	std::cout << "x_0 = " << x_0 << ",\nx_1 = " << x_1 << "\n";
	std::cout << "dihotomia             : " << dihotomia   (test_function_2d, x_1, x_0, 1e-5) << "\n";
	std::cout << "golden_ratio          : " << golden_ratio(test_function_2d, x_1, x_0, 1e-5) << "\n";
	std::cout << "fibonacci             : " << fibonacci   (test_function_2d, x_1, x_0, 1e-5) << "\n";
	std::cout << "\n";
	vec_n x_start = { -0,0 };
	std::cout << "per_coord_descend     : " << per_coord_descend    (test_function_2d, x_start, 1e-5) << "\n";
	std::cout << "gradient_descend      : " << gradient_descend     (test_function_2d, x_start, 1e-5) << "\n";
	std::cout << "conj_gradient_descend : " << conj_gradient_descend(test_function_2d, x_start, 1e-5) << "\n";
	std::cout << "newtone_raphson       : " << newtone_raphson      (test_function_2d, x_start, 1e-5) << "\n";
}

static void matrix_test()
{
	std::cout << "\n////////////////////////////" << std::endl;
	std::cout <<   "//////// MatrixTest ////////" << std::endl;
	std::cout <<   "////////////////////////////\n" << std::endl;

	vec_n b{ 1,2,3 };
	/*
	0.05
	0.3
	0.05
	*/
	mat_mn matrix{ {8,1,6}, {3,5,7}, {4,9,2} };
	std::cout << matrix << "\n";
	std::cout << "matrix summ \n";
	std::cout << matrix + matrix << "\n";
	std::cout << "matrix diff \n";
	std::cout << matrix - matrix << "\n";
	std::cout << "matrix mult \n";
	std::cout << matrix * matrix << "\n";
	mat_mn l, u;
	lu(matrix, l, u);
	std::cout << "matrix l: \n";
	std::cout << l << "\n";
	std::cout << "matrix u: \n";
	std::cout << u << "\n";
	std::cout << "matrix lu: \n";
	std::cout << l * u << "\n";
	vec_n x = linsolve(matrix, b);

	std::cout << "x : \n";
	std::cout << x << "\n";
	std::cout << "Ax - b: \n";
	std::cout << matrix * x - b << "\n";
	std::cout << "A*inv(A): \n";
	std::cout << matrix * invert(matrix) << "\n";
	std::cout << "Hessian: \n";
	vec_n x_ = { 1,2,3 };
	std::cout << hessian(test_function_nd, x_) << "\n";
}

static void simplex_method_test()
{
	std::cout << "\n/////////////////////////////"  << std::endl;
	std::cout <<   "//////// SimplexTest ////////"  << std::endl;
	std::cout <<   "/////////////////////////////\n"<< std::endl;
	std::cout << " f(x,c) =  2x1 + 3x2;\n arg_max = {4, 8}, f(arg_max) = 32\n";
	std::cout << " |-2x1 + 6x2 <= 40\n";
	std::cout << " | 3x1 + 2x2 <= 28\n";
	std::cout << " | 2x1 -  x2 <= 14\n\n";
	
	sm::simplex sym_0({{-2, 6}, { 3, 2}, { 2,-1} }, { 2, 3 }, { sm::LESS_EQUAL, sm::LESS_EQUAL, sm::LESS_EQUAL }, { 40, 28, 14 });
	sym_0.solve(SIMPLEX_MAX); 
	std::cout << "\n";

	std::cout << " f(x,c) = -2x1 + 3x2;\n arg_min = {7, 0}, f(arg_min) =-14\n\n";

	sm::simplex sym_1({ {-2, 6}, { 3, 2}, { 2,-1} }, { -2, 3 }, { sm::LESS_EQUAL, sm::LESS_EQUAL, sm::LESS_EQUAL }, { 40, 28, 14 });
	sym_1.solve(SIMPLEX_MIN);
	std::cout << "\n";

	std::cout << "/////////////////////////////\n" << std::endl;
	std::cout << " f(x,c) =  2x1 + 3x2;\n arg_min = {62/5, 54/5}, f(arg_max) = 57 1/5\n";
	std::cout << " |-2x1 + 6x2 >= 40\n";
	std::cout << " | 3x1 + 2x2 >= 28\n";
	std::cout << " | 2x1 -  x2 >= 14\n\n";
	sm::simplex sym_2({{-2, 6},{ 3, 2},{ 2,-1} }, { 2, 1 }, { sm::MORE_EQUAL, sm::MORE_EQUAL, sm::MORE_EQUAL }, { 40, 28, 14 });
	sym_2.solve(SIMPLEX_MIN);
	std::cout << " f(x,c) =  -2x1 - x2;\n arg_min = {62/5, 54/5}, f(arg_max) = -35 3/5\n";
	std::cout << "\n";
	sm::simplex sym_3({{-2, 6},{ 3, 2},{ 2,-1} }, { -2, -1 }, { sm::MORE_EQUAL, sm::MORE_EQUAL, sm::MORE_EQUAL }, { 40, 28, 14 });
	sym_3.solve(SIMPLEX_MAX);
	std::cout << " f(x,c) =  2x1 + 3x2;\n arg_min = {none, none}, f(arg_max) = none\n";
	std::cout << "\n"; 
	sm::simplex sym_4({{-2, 6},{ 3, 2},{ 2,-1} }, { 2, 3 }, { sm::MORE_EQUAL, sm::MORE_EQUAL, sm::MORE_EQUAL }, { 40, 28, 14 });
	sym_4.solve(SIMPLEX_MAX);
	std::cout << "/////////////////////////////\n" << std::endl;

	// f(x,c) = 2x1 + 3x2 -> arg_min = {62/5, 54/5}, f(arg_min) = 57 1/5 , arg_max = {-, -}, f(arg_max) = - (не работает учёт неограниченности) 
	//
	//         -2x1 + 6x2 = 40 
	//          3x1 + 2x2 = 28
	//          2x1 -  x2 = 14
	//
	sm::simplex sym_5({ {-2, 6},{ 3, 2},{ 2,-1} }, { 2, 3 }, { sm::EQUAL, sm::EQUAL, sm::EQUAL }, { 40, 28, 14 });

	sym_5.solve();

	std::cout << "/////////////////////////////\n" << std::endl;

}


static void numeric_tests()
{
	std::cout << "\n//////////////////////////////"  << std::endl;
	std::cout <<   "//////// NumericTests ////////"  << std::endl;
	std::cout <<   "//////////////////////////////\n"<< std::endl;

	int rational;
	int numerator;
	int denominator;
	decimal_to_rational(1.6666, rational, numerator, denominator);
	std::cout << rational << " " << numerator << "/" << denominator << std::endl;
	decimal_to_rational(0.6666, rational, numerator, denominator);
	std::cout << rational << " " << numerator << "/" << denominator << std::endl;
	decimal_to_rational(-3, rational, numerator, denominator);
	std::cout << rational << " " << numerator << "/" << denominator << std::endl;
	decimal_to_rational(-0.125, rational, numerator, denominator);
	std::cout << rational << " " << numerator << "/" << denominator << std::endl;
	decimal_to_rational(3.769230769230769230769, rational, numerator, denominator);
	std::cout << rational << " " << numerator	 << "/" << denominator << std::endl;
}

static int test_all()
{
	//one_dimensional_methods_test();
	//multi_dimensional_methods_test();
	//matrix_test();
	simplex_method_test();
	//numeric_tests();
	return 0;
}
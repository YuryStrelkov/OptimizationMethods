#pragma once
#include "one_dimensional.h";
#include "multi_dimensional.h";
#include "matrix_utils.h"
#include "symplex.h"
#include "numeric_utils.h"
// тестовая унимодальная одномерная функция с минимумом в точке {2} 
static double test_function_1d(const double x)
{
	return  (x - 2) * (x - 2);
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
	double x_0 = 10;
	double x_1 = -1;
	std::cout << "\n";
	std::cout << "x = agrmin(x * (x - 5))\n";
	std::cout << "x_0 = " << x_0 << ", x_1 = " << x_1 << "\n";
	std::cout << "dihotomia   : " << dihotomia   (test_function_1d, x_0, x_1, 1e-3) << "\n";
	std::cout << "golden_ratio: " << golden_ratio(test_function_1d, x_0, x_1, 1e-3) << "\n";
	std::cout << "fibonacci   : " << fibonacci   (test_function_1d, x_0, x_1, 1e-3) << "\n";
}

static void multi_dimensional_methods_test()
{
	vec_n x_0 = { 0,0 };
	vec_n x_1 = { 5,3 };

	std::cout << "\n";
	std::cout << "x_0 = " << x_0 << ", x_1 = " << x_1 << "\n";
	std::cout << "dihotomia             : " << dihotomia   (test_function_2d, x_1, x_0, 1e-5) << "\n";
	std::cout << "golden_ratio          : " << golden_ratio(test_function_2d, x_1, x_0, 1e-5) << "\n";
	std::cout << "fibonacci             : " << fibonacci   (test_function_2d, x_1, x_0, 1e-5) << "\n";
	std::cout << "\n";
	vec_n x_start = { -0,0 };

	std::cout << "per_coord_descend     : " << per_coord_descend    (test_function_2d, x_start,       1e-5) << "\n";
	std::cout << "gradient_descend      : " << gradient_descend     (test_function_2d, x_start,       1e-5) << "\n";
	std::cout << "conj_gradient_descend : " << conj_gradient_descend(test_function_2d, x_start,       1e-5) << "\n";
	std::cout << "newtone_raphson       : " << newtone_raphson      (test_function_2d, x_start, 1e-5) << "\n";
}

static void matrix_test()
{
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

static void symplex_method_test()
{
	mat_mn A = 
	{ {-2, 6},
	  { 3, 2},
	  { 2,-1}};
	vec_n  b = {40, 28, 14};
	vec_n  c = {2, 3};
	///write_symplex(A, b, c);
	symplex_solve(A, c, b, {-1, -1, -1});

}


static void numeric_tests()
{
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
	std::cout << rational << " " << numerator << "/" << denominator << std::endl;
}

static int test_all()
{
	//  one_dimensional_methods_test();
		multi_dimensional_methods_test();
	//	matrix_test();
	//	symplex_method_test();
	//numeric_tests();
	return 0;
}
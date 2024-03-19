#include "test.h"

////////////////////
/// Lab. work #1 ///
////////////////////

static F64 test_func(const F64 x)
{
	return (x - 5) * x; // min at point x = 2.5
}

static void  lab_1(function_1d function)
{
	std::cout << "\n////////////////////\n";
	std::cout <<   "/// Lab. work #1 ///\n";
	std::cout <<   "////////////////////\n\n";
	
	F64 x_0 = 10;
	F64 x_1 = -1;
	std::cout << "x_0 = " << x_0 << ", x_1 = " << x_1 << "\n";
	std::cout << "bisect      : " << bisect	     (function, x_0, x_1, ACCURACY) << "\n";
	std::cout << "golden_ratio: " << golden_ratio(function, x_0, x_1, ACCURACY) << "\n";
	std::cout << "fibonacci   : " << fibonacci   (function, x_0, x_1, ACCURACY) << "\n";
}

////////////////////
/// Lab. work #2 ///
////////////////////

static F64 test_func_2(const vector_f64& x)
{
	return (x[0] - 5) * x[0] + (x[1] - 3) * x[1]; // min at point x = 2.5, y = 1.5
}

static void  lab_2(function_nd function)
{
	std::cout << "\n////////////////////\n";
	std::cout <<   "/// Lab. work #2 ///\n";
	std::cout <<   "////////////////////\n\n";
	
	vector_f64 x_0 = { 0,0 };
	vector_f64 x_1 = { 5,3 };

	std::cout << "{ x, y } = agrmin((x - 2) * (x - 2) + (y - 2) * (y - 2))\n";
	std::cout << "x_0 = " << x_0 << ", x_1 = " << x_1 << "\n";
	///  Для реализации по-координтаного спуска необходимо реализовать один из следующих трех методов для работы с vec_n
	std::cout << "bisect                : " << bisect      (function, x_1, x_0, ACCURACY) << "\n";
	std::cout << "golden_ratio          : " << golden_ratio(function, x_1, x_0, ACCURACY) << "\n";
	std::cout << "fibonacci             : " << fibonacci   (function, x_1, x_0, ACCURACY) << "\n";
	std::cout << "\n";

	vector_f64 x_start = { -14, -33.98 };
	std::cout << "x_start = " << x_start << "\n";
	std::cout << "per_coord_descend     : " << per_coord_descend(function, x_start, ACCURACY) << "\n";
}

////////////////////
/// Lab. work #3 ///
////////////////////

static void  lab_3(function_nd function)
{
	std::cout << "\n////////////////////\n";
	std::cout <<   "/// Lab. work #3 ///\n";
	std::cout <<   "////////////////////\n\n";
	vector_f64 x_start = { -14, -33.98 };
	std::cout << "x_start = " << x_start << "\n";
	std::cout << "gradient_descend      : " << gradient_descend     (function, x_start, ACCURACY) << "\n";
	std::cout << "conj_gradient_descend : " << conj_gradient_descend(function, x_start, ACCURACY) << "\n";
}

////////////////////
/// Lab. work #4 ///
////////////////////

/// Пример применения функций штрафа
/// Уловие 1
static F64 psi1(const vector_f64& args)
{
	return 1 / (5 - args[0] * 2 + args[1] * 3);
}
/// Уловие 1
static F64 psi2(const vector_f64& args)
{
	return 1 / (6 + args[0] * 3 - args[1]);
}
///Ишем минимум функции  Testf2 при условии Psi1 и Psi2(Это внутренний штраф)
static F64 bouded_func(const vector_f64& args)
{
	return test_func_2(args) + psi2(args) + psi1(args);
}
static void  lab_4(function_nd function)
{
	std::cout << "\n////////////////////\n";
	std::cout <<   "/// Lab. work #4 ///\n";
	std::cout <<   "////////////////////\n\n";
	vector_f64 x_start = { -12.0, -15.0 };
	std::cout << "newtone_raphson       : " << newtone_raphson(function, x_start, ACCURACY) << "\n";
	//Поиск минимума функции при наличии функций штрафа
	std::cout << "newtone_raphson       : " << newtone_raphson(bouded_func, x_start, ACCURACY) << "\n";
}


int main()
{
	test_all();
	// lab_1(  test_func);
	// lab_2(test_func_2);
	// lab_3(test_func_2);
	// lab_4(test_func_2);
	return 0;
}
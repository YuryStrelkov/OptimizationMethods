#include "test.h";

////////////////////
/// Lab. work #1 ///
////////////////////

static double test_func(const double x)
{
	return (x - 5) * x; // min at point x = 2.5
}

static void  lab_1(func f)
{
	std::cout << "\n////////////////////\n";
	std::cout <<   "/// Lab. work #1 ///\n";
	std::cout <<   "////////////////////\n\n";
	
	double x_0 = 10;
	double x_1 = -1;
	std::cout << "x_0 = " << x_0 << ", x_1 = " << x_1 << "\n";
	std::cout << "dihotomia   : " << dihotomia   (f, x_0, x_1, 1e-4) << "\n";
	std::cout << "golden_ratio: " << goldenRatio(f, x_0, x_1, 1e-4) << "\n";
	std::cout << "fibonacci   : " << fibonacci   (f, x_0, x_1, 1e-4) << "\n";
}

////////////////////
/// Lab. work #2 ///
////////////////////

static double test_func_2(const vec_n& x)
{
	return (x[0] - 5) * x[0] + (x[1] - 3) * x[1]; // min at point x = 2.5, y = 1.5
}

static void  lab_2(func_n f)
{
	std::cout << "\n////////////////////\n";
	std::cout <<   "/// Lab. work #2 ///\n";
	std::cout <<   "////////////////////\n\n";
	
	vec_n x_0 = { 0,0 };
	vec_n x_1 = { 5,3 };

	std::cout << "{ x, y } = agrmin((x - 2) * (x - 2) + (y - 2) * (y - 2))\n";
	std::cout << "x_0 = " << x_0 << ", x_1 = " << x_1 << "\n";
	///  Для реализации по-координтаного спуска необходимо реализовать один из следующих трех методов для работы с vec_n
	std::cout << "dihotomia             : " << dihotomia  (f, x_1, x_0, 1e-4) << "\n";
	std::cout << "golden_ratio          : " << goldenRatio(f, x_1, x_0, 1e-4) << "\n";
	std::cout << "fibonacci             : " << fibonacci  (f, x_1, x_0, 1e-4) << "\n";
	std::cout << "\n";

	vec_n x_start = { -14, -33.98 };
	std::cout << "x_start = " << x_start << "\n";
	std::cout << "per_coord_descend     : " << perCoordDescend(f, x_start, 1e-4) << "\n";
}

////////////////////
/// Lab. work #3 ///
////////////////////

static void  lab_3(func_n f)
{
	std::cout << "\n////////////////////\n";
	std::cout <<   "/// Lab. work #3 ///\n";
	std::cout <<   "////////////////////\n\n";
	vec_n x_start = { -14, -33.98 };
	std::cout << "x_start = " << x_start << "\n";
	std::cout << "gradient_descend      : " << gradientDescend     (f, x_start, 1e-4) << "\n";
	std::cout << "conj_gradient_descend : " << conjGradientDescend(f, x_start, 1e-4) << "\n";
}

////////////////////
/// Lab. work #4 ///
////////////////////

/// Пример применения функций штрафа
/// Уловие 1
static double psi1(const vec_n& args)
{
	return 1 / (5 - args[0] * 2 + args[1] * 3);
}
/// Уловие 1
static double psi2(const vec_n& args)
{
	return 1 / (6 + args[0] * 3 - args[1]);
}
///Ишем минимум функции  Testf2 при условии Psi1 и Psi2(Это внутренний штраф)
static double bouded_func(const vec_n& args)
{
	return test_func_2(args) + psi2(args) + psi1(args);
}
static void  lab_4(func_n f)
{
	std::cout << "\n////////////////////\n";
	std::cout <<   "/// Lab. work #4 ///\n";
	std::cout <<   "////////////////////\n\n";
	vec_n x_start = { -14, -33.98 };
	std::cout << "newtone_raphson       : " << newtoneRaphson(f, x_start, 1e-5) << "\n";
	//Поиск минимума функции при наличии функций штрафа
	std::cout << "newtone_raphson       : " << newtoneRaphson(bouded_func, x_start, 1e-5) << "\n";
}

int main()
{
	//lab_1(  test_func);
	//lab_2(test_func_2);
	/*lab_3(test_func_2);
	lab_4(test_func_2);*/
	testAll();
	return 0;
}
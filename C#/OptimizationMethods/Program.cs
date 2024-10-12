using MathUtils;
using System;

namespace OptimizationMethods
{
    class Program
    {
        ////////////////////
        /// Lab. work #1 ///
        ////////////////////
        static double Testf1(double arg) => arg * (arg - 5.0);
        static void Lab1()
        {
            Console.WriteLine("\n////////////////////\n");
            Console.WriteLine(  "/// Lab. work #1 ///\n");
            Console.WriteLine(  "////////////////////\n\n");

            double x_0 = 10.0;
            double x_1 = -1.0;
            Console.WriteLine($"x_0 = {x_0}, x_1 = {x_1}\n");
            Console.WriteLine($"BiSect      : {OneDimensional.BiSect     (Testf1, x_0, x_1, 1e-3)}");
            Console.WriteLine($"GoldenRatio : {OneDimensional.GoldenRatio(Testf1, x_0, x_1, 1e-3)}");
            Console.WriteLine($"Fibonacchi  : {OneDimensional.Fibonacci  (Testf1, x_0, x_1, 1e-3)}\n");
        }
        ////////////////////
        /// Lab. work #2 ///
        ////////////////////
        static double Testf2(DoubleVector args) => (args[0] - 2.0) * (args[0] - 2.0) + (args[1] - 2.0) * (args[1] - 2.0);
        static void Lab2()
        {
            Console.WriteLine("\n////////////////////\n");
            Console.WriteLine(  "/// Lab. work #2 ///\n");
            Console.WriteLine(  "////////////////////\n\n");

            DoubleVector x_1 = new DoubleVector(0.0, 0.0);
            DoubleVector x_0 = new DoubleVector(5.0, 5.0);
            Console.WriteLine($"x_0 = {x_0}, x_1 = {x_1}\n");
            Console.WriteLine($"BiSect                 : {MultiDimensional.BiSect             (Testf2, x_1, x_0)}");
            Console.WriteLine($"GoldenRatio            : {MultiDimensional.GoldenRatio        (Testf2, x_1, x_0)}" );
            Console.WriteLine($"Fibonacci              : {MultiDimensional.Fibonacci          (Testf2, x_1, x_0)}");
            Console.WriteLine($"PerCoordDescend        : {MultiDimensional.PerCoordDescend    (Testf2, x_1)}");
            Console.WriteLine($"GradientDescend        : {MultiDimensional.GradientDescend    (Testf2, x_1)}" );
            Console.WriteLine($"СonjGradientDescend    : {MultiDimensional.СonjGradientDescend(Testf2, x_1)}" );
            Console.WriteLine($"NewtoneRaphson         : {MultiDimensional.NewtoneRaphson     (Testf2, x_1)}\n");
        }

        ////////////////////
        /// Lab. work #3 ///
        ////////////////////
        static void Lab3()
        {
            Console.WriteLine("\n////////////////////\n");
            Console.WriteLine(  "/// Lab. work #3 ///\n");
            Console.WriteLine(  "////////////////////\n\n");

            DoubleVector x_1 = new DoubleVector(0.0, 0.0);
            DoubleVector x_0 = new DoubleVector(5.0, 5.0);
            Console.WriteLine($"x_0 = {x_0}, x_1 = {x_1}\n");
            Console.WriteLine($"GradientDescend        : {MultiDimensional.GradientDescend    (Testf2, x_1)}");
            Console.WriteLine($"СonjGradientDescend    : {MultiDimensional.СonjGradientDescend(Testf2, x_1)}");
            Console.WriteLine($"NewtoneRaphson         : {MultiDimensional.NewtoneRaphson     (Testf2, x_1)}\n");
        }

        ////////////////////
        /// Lab. work #4 ///
        ////////////////////
        /// Пример применения функций штрафа
        /// Уловие 1
        static double Psi1(DoubleVector args) => 1.0 / (5.0 - args[0] * 2.0 + args[1] * 3.0);
        /// Уловие 1
        static double Psi2(DoubleVector args) => 1.0 / (6.0 + args[0] * 3.0 - args[1]);
        ///Ишем минимум функции  Testf2 при условии Psi1 и Psi2(Это внутренний штраф)
        static double Func(DoubleVector args) => Testf2(args) + Psi2(args) + Psi1(args);
        static void Lab4()
        {
            Console.WriteLine("\n////////////////////\n");
            Console.WriteLine(  "/// Lab. work #4 ///\n");
            Console.WriteLine(  "////////////////////\n\n");
            DoubleVector x_1 = new DoubleVector(0.0, 0.0);
            DoubleVector x_0 = new DoubleVector(5.0, 5.0);
            Console.WriteLine($"x_0 = {x_0}, x_1 = {x_1}\n");
            Console.WriteLine($"NewtoneRaphson         : {MultiDimensional.NewtoneRaphson(Testf2, x_1)}");
            Console.WriteLine($"NewtoneRaphson         : {MultiDimensional.NewtoneRaphson(Func, x_1)}\n");
        }
        static void Main(string[] args)
        {
                Tests.TestAll();
        }
    }
}

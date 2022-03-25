using System;

namespace OptimizationMethods
{
    class Program
    {
        ////////////////////
        /// Lab. work #1 ///
        ////////////////////
        static double Testf1(double arg)
        {
            return arg * (arg - 5);
        }
        static void Lab1()
        {
            Console.WriteLine("\n////////////////////\n");
            Console.WriteLine(  "/// Lab. work #1 ///\n");
            Console.WriteLine(  "////////////////////\n\n");

            double x_0 = 10;
            double x_1 = -1;
            Console.WriteLine("x_0 = " + x_0 + ", x_1 = " + x_1 + "\n");
            Console.WriteLine("Dihotomia   : " + OneDimensional.Dihotomia(Testf1, x_0, x_1, 1e-3f));
            Console.WriteLine("GoldenRatio : " + OneDimensional.GoldenRatio(Testf1, x_0, x_1, 1e-3f));
            Console.WriteLine("Fibonacchi  : " + OneDimensional.Fibonacci(Testf1, x_0, x_1, 1e-3f) + "\n");
        }
        ////////////////////
        /// Lab. work #2 ///
        ////////////////////
        static double Testf2(Vector args)
        {
            return (args[0] - 2) *(args[0] - 2) + (args[1] - 2) * (args[1] - 2);
        }
        static void Lab2()
        {
            Console.WriteLine("\n////////////////////\n");
            Console.WriteLine(  "/// Lab. work #2 ///\n");
            Console.WriteLine(  "////////////////////\n\n");

            Vector x_1 = new double[] { 0, 0 };
            Vector x_0 = new double[] { 5, 5 };
            Console.WriteLine("x_0 = " + x_0 + ", x_1 = " + x_1 + "\n");
            Console.WriteLine("Dihotomia              : " + MultiDimensional.Dihotomia(Testf2, x_1, x_0).ToString());
            Console.WriteLine("GoldenRatio            : " + MultiDimensional.GoldenRatio(Testf2, x_1, x_0).ToString());
            Console.WriteLine("Fibonacci              : " + MultiDimensional.Fibonacci(Testf2, x_1, x_0).ToString());
            Console.WriteLine("PerCoordDescend        : " + MultiDimensional.PerCoordDescend(Testf2, x_1).ToString());
            Console.WriteLine("GradientDescend        : " + MultiDimensional.GradientDescend(Testf2, x_1).ToString());
            Console.WriteLine("СonjGradientDescend    : " + MultiDimensional.СonjGradientDescend(Testf2, x_1).ToString());
            Console.WriteLine("NewtoneRaphson         : " + MultiDimensional.NewtoneRaphson(Testf2, x_1).ToString());
        }

        ////////////////////
        /// Lab. work #3 ///
        ////////////////////
        static void Lab3()
        {
            Console.WriteLine("\n////////////////////\n");
            Console.WriteLine(  "/// Lab. work #3 ///\n");
            Console.WriteLine(  "////////////////////\n\n");

            Vector x_1 = new double[] { 0, 0 };
            Vector x_0 = new double[] { 5, 5 };
            Console.WriteLine("x_0 = " + x_0 + ", x_1 = " + x_1 + "\n");
            Console.WriteLine("GradientDescend        : " + MultiDimensional.GradientDescend(Testf2, x_1).ToString());
            Console.WriteLine("СonjGradientDescend    : " + MultiDimensional.СonjGradientDescend(Testf2, x_1).ToString());
            Console.WriteLine("NewtoneRaphson         : " + MultiDimensional.NewtoneRaphson(Testf2, x_1).ToString());
        }

        ////////////////////
        /// Lab. work #4 ///
        ////////////////////
        /// Пример применения функций штрафа
        /// Уловие 1
        static double Psi1(Vector args)
        {
            return 1 / (5 - args[0] * 2 + args[1] * 3 );
        }
        /// Уловие 1
        static double Psi2(Vector args)
        {
            return 1 / (6 + args[0] * 3 - args[1]);
        }
        ///Ишем минимум функции  Testf2 при условии Psi1 и Psi2(Это внутренний штраф)
        static double Func(Vector args)
        {
            return Testf2(args) + Psi2(args) + Psi1(args);
        }
        static void Lab4()
        {
            Console.WriteLine("\n////////////////////\n");
            Console.WriteLine(  "/// Lab. work #4 ///\n");
            Console.WriteLine(  "////////////////////\n\n");

            Vector x_1 = new double[] { 0, 0 };
            Vector x_0 = new double[] { 5, 5 };
            Console.WriteLine("x_0 = " + x_0 + ", x_1 = " + x_1 + "\n");
            Console.WriteLine("NewtoneRaphson         : " + MultiDimensional.NewtoneRaphson(Testf2, x_1).ToString());
            Console.WriteLine("NewtoneRaphson         : " + MultiDimensional.NewtoneRaphson(Func, x_1).ToString());
        }


        static void Main(string[] args)
        {
           Tests.TestAll();
        }
    }
}

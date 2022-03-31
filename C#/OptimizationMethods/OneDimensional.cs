using System;
using System.Collections.Generic;

namespace OptimizationMethods
{
    public delegate double func_1(double x);
    public static class OneDimensional
    {
        ////////////////////
        /// Lab. work #1 ///
        ////////////////////
        public static double TestFunc(double x)
        {
	        return (x - 5) * (x - 2); // min at point x = 3.5
        }

        public static readonly double Phi = 1.61803398874989484820;
        static void Swap<T>(ref T lhs, ref T rhs)
        {
            T temp;
            temp = lhs;
            lhs = rhs;
            rhs = temp;
        }
        public static double Dihotomia  (func_1 f, double x_0, double x_1, double eps = 1e-6, int max_iters = 1000)
        {
            double x_c = 0.0;

            if (x_0 > x_1)
            {
                Swap(ref x_0, ref x_1);
            }

            int cntr = 0;

            for (; cntr != max_iters; cntr++)
            {
                if (Math.Abs(x_1 - x_0) < eps)
                {
                    break;
                }
                x_c = (x_1 + x_0) * 0.5;

                if (f(x_c + eps) > f(x_c - eps))
                {
                    x_1 = x_c;
                    continue;
                }
                x_0 = x_c;
            }
#if DEBUG
            Console.WriteLine("dihotomia iterations number : " + cntr + "\n");
#endif
            return x_c;
        }
        public static double GoldenRatio(func_1 f, double x_0, double x_1, double eps = 1e-6, int max_iters = 1000)
        {
            if (x_0 > x_1)
            {
                Swap(ref x_0, ref x_1);
            }
            double a = x_0, b = x_1, dx;
            
            int cntr = 0;

            for (; cntr != max_iters; cntr++)
            {
                if (Math.Abs(x_1 - x_0) < eps)
                {
                    break;
                }
                dx  = b - a;
                x_0 = b - dx / Phi;
                x_1 = a + dx / Phi;

                if (f(x_0) >= f(x_1))
                {
                    a = x_0;
                    continue;
                }
                b = x_1;
            }
#if DEBUG
            Console.WriteLine("golden ratio iterations number : " + cntr + "\n");
#endif
            return (x_1 + x_0) * 0.5;
        }
        public static int[] FibonacchiNumbers(int index)
        {
            if (index < 0)
            {
                return new int[]{ 0 };
            }
            if (index == 0 || index == 1)
            {
                return new int[]{ 1 };
            }

            int[] res = new int[index];

            res[0] = 1;

            res[1] = 1;

            for (int i = 2; i < index; i++)
            {
                res[i] = res[i - 1] + res[i - 2];
            }

            return res;
        }
        public static int ClosestFibonacci(double value)
        {
            int f_1 = 1;
            if (value <= 1)
            {
                return f_1;
            }
            int f_2 = 2;
            if (value <= 2)
            {
                return f_2;
            }
            int f_3 = 3;
            if (value <= 3)
            {
                return f_3;
            }
            int cntr = 3;
            while (true)
            {
                f_1 = f_2;
                f_2 = f_3;
                f_3 = f_1 + f_2;
                if (f_3 > value)
                {
                    return cntr;
                }
                cntr++;
            }
        }
        public static double Fibonacci(func_1 f, double x_0, double x_1, double eps = 1e-6)
        {
            if (x_0 > x_1)
            {
                Swap(ref x_0, ref x_1);
            }
            
            double a = x_0, b = x_1, dx;

            int max_iters = ClosestFibonacci((b - a) / eps);

            int cntr = max_iters - 1;

            int[] f_n_s = FibonacchiNumbers(max_iters);

            for (; cntr >= 2; cntr--)
            {
                if (Math.Abs(x_1 - x_0) < eps)
                {
                    break;
                }

                dx = (b - a);
                x_0 = a + dx * f_n_s[cntr - 2] / f_n_s[cntr];
                x_1 = a + dx * f_n_s[cntr - 1] / f_n_s[cntr];
               
                if (f(x_0) < f(x_1))
                {
                    b = x_1;
                    continue;
                }
                a = x_0;
            }
#if DEBUG
            Console.WriteLine("fibonacchi iterations number : " + max_iters + "\n");
#endif
            return (x_1 + x_0) * 0.5;
        }
    }
}

using System;

namespace OptimizationMethods
{
    public delegate double Function1D(double x);
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

        public static double BiSect (Function1D f, double x_0, double x_1, double eps = 1e-6, int max_iters = 1000)
        {
            double x_c = 0.0;

            if (x_0 > x_1) Swap(ref x_0, ref x_1);

            int cntr = 0;

            for (; cntr != max_iters; cntr++)
            {
                if (x_1 - x_0 < eps) break;
                
                x_c = (x_1 + x_0) * 0.5;

                if (f(x_c + eps) > f(x_c - eps))
                {
                    x_1 = x_c;
                    continue;
                }
                x_0 = x_c;
            }
#if DEBUG
            Console.WriteLine($"dihotomia iterations number : {cntr}");
#endif
            return x_c;
        }
        public static double GoldenRatio(Function1D f, double x_0, double x_1, double eps = 1e-6, int max_iters = 1000)
        {
            if (x_0 > x_1) Swap(ref x_0, ref x_1);
            
            double a = x_0, b = x_1, dx;
            
            int cntr = 0;

            for (; cntr != max_iters; cntr++)
            {
                if (x_1 - x_0 < eps) break;
               
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
            Console.WriteLine($"golden ratio iterations number : {cntr}");
#endif
            return (x_1 + x_0) * 0.5;
        }
        public static int[] FibonacchiNumbers(int index)
        {
            if (index < 1) return new int[] { 0 };
            
            if (index < 2) return new int[] { 0, 1 };

            int[] res = new int[index];

            res[0] = 0;

            res[1] = 1;

            for (int i = 2; i < index; i++) res[i] = res[i - 1] + res[i - 2];

            return res;
        }
        public static void ClosestFibonacciPair(double value, ref int f_n, ref int f_n_1)
        {
            f_n   = 0;
            f_n_1 = 0;

            if (value < 1) return;

            f_n_1 = 1;

            if (value < 2) return;
            int f_tmp;

            while (f_n < value)
            {
                f_tmp = f_n;
                f_n = f_n_1;
                f_n_1 += f_tmp;
            }
        }
        public static double Fibonacci(Function1D f, double x_0, double x_1, double eps = 1e-6)
        {
            if (x_0 > x_1) Swap(ref x_0, ref x_1);
            
            double a = x_0, b = x_1, dx;

            int f_n = 0, f_n_1 = 0, f_tmp, cntr = 0;

            ClosestFibonacciPair((b - a) / eps, ref f_n, ref f_n_1);

            while(f_n != f_n_1)
            {
                if (x_1 - x_0 < eps) break;
                cntr++;
                dx = (b - a);
                f_tmp = f_n_1 - f_n;
                x_0 = a + dx * ((double)f_tmp / f_n_1);
                x_1 = a + dx * ((double)f_n   / f_n_1);
                f_n_1 = f_n;
                f_n = f_tmp;
                if (f(x_0) < f(x_1))
                {
                    b = x_1;
                    continue;
                }
                a = x_0;
            }
#if DEBUG
            Console.WriteLine($"fibonacci iterations number : {cntr}");
#endif
            return (x_1 + x_0) * 0.5;
        }
    }
}

using System;
using MathUtils;

namespace OptimizationMethods
{
    public delegate double Function1D(double x);
    public static class OneDimensional
    {
        ////////////////////
        /// Lab. work #1 ///
        ////////////////////
        public static double BiSect(Function1D f, double left, double right, double accuracy, int iterations)
        {
            if (left > right)  NumericUtils.Swap(ref left, ref right);
            
            double x_c = 0.0;

            int cntr = 0;

            for (; cntr != iterations; cntr++)
            {
                if (right - left < accuracy) break;
                
                x_c = (right + left) * 0.5;

                if (f(x_c + accuracy) > f(x_c - accuracy))
                {
                    right = x_c;
                    continue;
                }
                left = x_c;
            }
#if DEBUG
            Console.WriteLine($"dihotomia iterations number : {cntr}");
#endif
            return x_c;
        }
        public static double BiSect(Function1D f, double left, double right, double accuracy) => BiSect(f, left, right, accuracy, NumericCommon.ITERATIONS_COUNT_HIGH);
        public static double BiSect(Function1D f, double left, double right) => BiSect(f, left, right, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);

        public static double GoldenRatio(Function1D f, double left, double right, double accuracy, int iterations)
        {
            if (left > right) NumericUtils.Swap(ref left, ref right);
            
            double a = left, b = right, dx;
            
            int cntr = 0;

            for (; cntr != iterations; cntr++)
            {
                if (right - left < accuracy) break;
               
                dx  = b - a;
                left = b - dx * NumericCommon.ONE_OVER_PHI;
                right = a + dx * NumericCommon.ONE_OVER_PHI;

                if (f(left) >= f(right))
                {
                    a = left;
                    continue;
                }
                b = right;
            }
#if DEBUG
            Console.WriteLine($"golden ratio iterations number : {cntr}");
#endif
            return (right + left) * 0.5;
        }
        public static double GoldenRatio(Function1D f, double left, double right, double accuracy) => GoldenRatio(f, left, right, accuracy, NumericCommon.ITERATIONS_COUNT_HIGH);
        public static double GoldenRatio(Function1D f, double left, double right) => GoldenRatio(f, left, right, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);
 
        public static double Fibonacci(Function1D f, double left, double right, double accuracy)
        {
            if (left > right) NumericUtils.Swap(ref left, ref right);
            
            double a = left, b = right, dx;

            int f_n = 0, f_n_1 = 0, f_tmp, cntr = 0;

            NumericUtils.ClosestFibonacciPair((b - a) / accuracy, ref f_n, ref f_n_1);

            while(f_n != f_n_1)
            {
                if (right - left < accuracy) break;
                cntr++;
                dx = (b - a);
                f_tmp = f_n_1 - f_n;
                left = a + dx * ((double)f_tmp / f_n_1);
                right = a + dx * ((double)f_n   / f_n_1);
                f_n_1 = f_n;
                f_n = f_tmp;
                if (f(left) < f(right))
                {
                    b = right;
                    continue;
                }
                a = left;
            }
#if DEBUG
            Console.WriteLine($"fibonacci iterations number : {cntr}");
#endif
            return (right + left) * 0.5;
        }
        public static double Fibonacci(Function1D f, double left, double right) => Fibonacci(f, left, right, NumericCommon.NUMERIC_ACCURACY_MIDDLE);
    }
}

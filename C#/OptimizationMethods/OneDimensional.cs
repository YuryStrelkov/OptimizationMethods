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
        public static double BiSect(Function1D targetFunction, double lhs, double rhs, double accuracy, int iterations)
        {
            if (lhs > rhs)  NumericUtils.Swap(ref lhs, ref rhs);
            int cntr = 0;
            for (; cntr != iterations && rhs - lhs > 2 * accuracy; cntr++)
            {
                double x_c = (rhs + lhs) * 0.5;
                if (targetFunction(x_c + accuracy) > targetFunction(x_c - accuracy))
                    rhs = x_c;
                else
                    lhs = x_c;
            }
#if DEBUG
            Console.WriteLine($"BiSect iterations number : {cntr}");
            Console.WriteLine($"BiSect argument range    : {rhs - lhs}");
#endif
            return (rhs + lhs) * 0.5;
        }
        public static double BiSect(Function1D targetFunction, double lhs, double rhs, double accuracy) => BiSect(targetFunction, lhs, rhs, accuracy, NumericCommon.ITERATIONS_COUNT_HIGH);
        public static double BiSect(Function1D targetFunction, double lhs, double rhs) => BiSect(targetFunction, lhs, rhs, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);
        // INCORRECT!!!
        public static double GoldenRatio(Function1D targetFunction, double lhs, double rhs, double accuracy, int iterations)
        {
            if (lhs > rhs) NumericUtils.Swap(ref lhs, ref rhs);
            int iteration = 0;
            double x_l = rhs - (rhs - lhs) * NumericCommon.PSI;
            double x_r = lhs + (rhs - lhs) * NumericCommon.PSI;
            double f_l = targetFunction(x_l);
            double f_r = targetFunction(x_r);
            for (; iteration != iterations && rhs - lhs > 2 * accuracy; iteration++)
            {
                if (f_l > f_r)
                {
                    lhs = x_l;
                    x_l = x_r;
                    f_l = f_r;
                    x_r = lhs + (rhs - lhs) * NumericCommon.PSI;
                    f_r = targetFunction(x_r);
                }
                else
                {
                    rhs = x_r;
                    x_r = x_l;
                    f_r = f_l;
                    x_l = rhs - (rhs - lhs) * NumericCommon.PSI;
                    f_l = targetFunction(x_l);
                }
            }
#if DEBUG
            Console.WriteLine($"GoldenRatio iterations number : {cntr}");
            Console.WriteLine($"GoldenRatio argument range    : {rhs - lhs}");
#endif
            return (rhs + lhs) * 0.5;
        }
        public static double GoldenRatio(Function1D targetFunction, double lhs, double rhs, double accuracy) => GoldenRatio(targetFunction, lhs, rhs, accuracy, NumericCommon.ITERATIONS_COUNT_HIGH);
        public static double GoldenRatio(Function1D targetFunction, double lhs, double rhs) => GoldenRatio(targetFunction, lhs, rhs, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);
        // INCORRECT!!!
        public static double Fibonacci(Function1D targetFunction, double lhs, double rhs, double accuracy)
        {
            if (lhs > rhs) NumericUtils.Swap(ref lhs, ref rhs);
            double fib_t = 0.0, fib_1 = 1.0, fib_2 = 1.0;
            double condition = (rhs - lhs) / accuracy;
            int iterations = 0;
            while (fib_2 < condition)
            {
                iterations++;
                fib_t = fib_1;
                fib_1 = fib_2;
                fib_2 += fib_t;
            }
            double x_l = lhs + (rhs - lhs) * ((fib_2 - fib_1) / fib_2);
            double x_r = lhs + (rhs - lhs) * (fib_1 / fib_2);
            double f_l = targetFunction(x_l);
            double f_r = targetFunction(x_r);
            for (int index = iterations; index > 0; index--)
            {
                fib_t = fib_2 - fib_1;
                fib_2 = fib_1;
                fib_1 = fib_t;

                if (f_l > f_r)
                {
                    lhs = x_l;
                    f_l = f_r;
                    x_l = x_r;
                    x_r = lhs + (rhs - lhs) * (fib_1 / fib_2);
                    f_r = targetFunction(x_r);
                }
                else
                {
                    rhs = x_r;
                    x_r = x_l;
                    f_r = f_l;
                    x_l = lhs + (rhs - lhs) * ((fib_2 - fib_1) / fib_2);
                    f_l = targetFunction(x_l);
                }
            }
#if DEBUG
            Console.WriteLine($"Fibonacci iterations number : {cntr}");
            Console.WriteLine($"Fibonacci argument range    : {rhs - lhs}");
#endif
            return (rhs + lhs) * 0.5;
        }
        public static double Fibonacci(Function1D targetFunction, double lhs, double rhs) => Fibonacci(targetFunction, lhs, rhs, NumericCommon.NUMERIC_ACCURACY_MIDDLE);
    }
}

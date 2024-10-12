using MathUtils;
using System;

namespace OptimizationMethods
{
    public static class MultiDimensional
    {
        ////////////////////
        /// Lab. work #2 ///
        ////////////////////
        public static DoubleVector BiSect(FunctionND targetFunction, DoubleVector lhs, DoubleVector rhs, double accuracy, int iterations)
        {
            DoubleVector x_c, dir;
            dir = DoubleVector.Direction(lhs, rhs) * accuracy;
            int cntr = 0;
            for (; cntr != iterations && DoubleVector.Distance(rhs, lhs) > 2 * accuracy; cntr++)
            {
                x_c = (rhs + lhs) * 0.5;
                if (targetFunction(x_c + dir) > targetFunction(x_c - dir))
                    rhs = x_c;
                else
                    lhs = x_c;
            }
#if DEBUG
            Console.WriteLine($"BiSect iterations number : {cntr}");
            Console.WriteLine($"BiSect argument range    : {DoubleVector.Distance(rhs, lhs)}");
#endif
            return (rhs + lhs) * 0.5;
        }
        public static DoubleVector BiSect(FunctionND targetFunction, DoubleVector lhs, DoubleVector rhs, double accuracy) => BiSect(targetFunction, lhs, rhs, accuracy, NumericCommon.ITERATIONS_COUNT_HIGH);
        public static DoubleVector BiSect(FunctionND targetFunction, DoubleVector lhs, DoubleVector rhs) => BiSect(targetFunction, lhs, rhs, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);
        public static DoubleVector GoldenRatio(FunctionND targetFunction, DoubleVector _lhs, DoubleVector _rhs, double accuracy, int iterations)
        {
            DoubleVector lhs = new DoubleVector(_lhs);
            DoubleVector rhs = new DoubleVector(_rhs);
            DoubleVector x_l = rhs - (rhs - lhs) * NumericCommon.PSI;
            DoubleVector x_r = lhs + (rhs - lhs) * NumericCommon.PSI;
            double f_l = targetFunction(x_l);
            double f_r = targetFunction(x_r);
            int iteration = 0;
            for (; iteration != iterations && DoubleVector.Distance(rhs, lhs) > 2 * accuracy; iteration++)
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
            Console.WriteLine($"GoldenRatio iterations number : {iteration + 2}");
            Console.WriteLine($"GoldenRatio argument range    : {DoubleVector.Distance(rhs, lhs)}");
#endif
            return (rhs + lhs) * 0.5;
        }
        public static DoubleVector GoldenRatio(FunctionND targetFunction, DoubleVector lhs, DoubleVector rhs, double accuracy) => GoldenRatio(targetFunction, lhs, rhs, accuracy, NumericCommon.ITERATIONS_COUNT_HIGH);
        public static DoubleVector GoldenRatio(FunctionND targetFunction, DoubleVector lhs, DoubleVector rhs) => GoldenRatio(targetFunction, lhs, rhs, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);
        public static DoubleVector Fibonacci(FunctionND targetFunction, DoubleVector _lhs, DoubleVector _rhs, double accuracy)
        {
            DoubleVector lhs = new DoubleVector(_lhs);
            DoubleVector rhs = new DoubleVector(_rhs);
            double fib_t = 0.0, fib_1 = 1.0, fib_2 = 1.0;
            double condition = DoubleVector.Distance(rhs, lhs) / accuracy;
            int iterations = 0;
            while (fib_2 < condition)
            {
                iterations++;
                fib_t = fib_1;
                fib_1 = fib_2;
                fib_2 += fib_t;
            }
            DoubleVector x_l = lhs + (rhs - lhs) * ((fib_2 - fib_1) / fib_2);
            DoubleVector x_r = lhs + (rhs - lhs) * (fib_1 / fib_2);

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
            Console.WriteLine($"Fibonacci iterations number : {iterations + 2}");
            Console.WriteLine($"Fibonacci argument range    : {DoubleVector.Distance(rhs, lhs)}");
#endif
            return (rhs + lhs) * 0.5;
        }
        public static DoubleVector Fibonacci(FunctionND targetFunction, DoubleVector lhs, DoubleVector rhs) => Fibonacci(targetFunction, lhs, rhs, NumericCommon.NUMERIC_ACCURACY_MIDDLE);

        public static DoubleVector PerCoordDescend(FunctionND targetFunction, DoubleVector xStart, double accuracy, int iterations)
        {
            DoubleVector lhs = new DoubleVector(xStart);

            DoubleVector rhs = new DoubleVector(xStart);

            double step = 1.0;

            double x_i, y_1, y_0;

            int opt_coord_n = 0, coord_id;
            
            int i = 0;

            for (i = 0; i < iterations; i++)
            {
                coord_id = i % lhs.Count;

                rhs[coord_id] -= accuracy;

                y_0 = targetFunction(rhs);

                rhs[coord_id] += 2 * accuracy;

                y_1 = targetFunction(rhs);

                rhs[coord_id] -= accuracy;

                rhs[coord_id] = y_0 > y_1 ? rhs[coord_id] += step : rhs[coord_id] -= step;

                x_i = lhs[coord_id];

                rhs = BiSect(targetFunction, lhs, rhs, accuracy, iterations);

                lhs = new DoubleVector(rhs);

                if (Math.Abs(rhs[coord_id] - x_i) < accuracy)
                {
                    opt_coord_n++;

                    if (opt_coord_n == rhs.Count)
                    {
#if DEBUG
                        Console.WriteLine($"per coord descend iterations number : {i}");
#endif
                        return lhs;
                    }
                    continue;
                }
                opt_coord_n = 0;
            }
#if DEBUG
            Console.WriteLine($"per coord descend iterations number : {iterations}");
#endif
            return lhs;
        }
        public static DoubleVector PerCoordDescend(FunctionND targetFunction, DoubleVector xStart, double accuracy) => PerCoordDescend(targetFunction, xStart, accuracy, NumericCommon.ITERATIONS_COUNT_HIGH);
        public static DoubleVector PerCoordDescend(FunctionND targetFunction, DoubleVector xStart) => PerCoordDescend(targetFunction, xStart, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);

        ////////////////////
        /// Lab. work #3 ///
        ////////////////////
        public static DoubleVector GradientDescend(FunctionND targetFunction, DoubleVector xStart, double accuracy, int iterations)
        {
            DoubleVector x_i = new DoubleVector(xStart);

            DoubleVector x_i_1 = new DoubleVector(xStart);

            int cntr = 0;

            for (; cntr <= iterations; cntr++)
            {
                x_i_1 = x_i - DoubleVector.Gradient(targetFunction, x_i, accuracy);

                x_i_1 = BiSect(targetFunction, x_i, x_i_1, accuracy, iterations);

                if ((x_i_1 - x_i).Magnitude < accuracy) break;

                x_i = x_i_1;
            }
#if DEBUG
            Console.WriteLine($"gradient descend iterations number : {cntr}");
#endif
            return (x_i_1 + x_i) * 0.5;
        }
        public static DoubleVector GradientDescend(FunctionND targetFunction, DoubleVector xStart, double accuracy) => GradientDescend(targetFunction, xStart, accuracy, NumericCommon.ITERATIONS_COUNT_HIGH);
        public static DoubleVector GradientDescend(FunctionND targetFunction, DoubleVector xStart) => GradientDescend(targetFunction, xStart, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);
        
        public static DoubleVector СonjGradientDescend(FunctionND targetFunction, DoubleVector xStart, double accuracy, int iterations)
        {
            DoubleVector x_i = new DoubleVector(xStart);

            DoubleVector x_i_1 = new DoubleVector(xStart);

            DoubleVector s_i = DoubleVector.Gradient(targetFunction, xStart, accuracy) * (-1.0), s_i_1;

            double omega;

            int cntr = 0;

            for (; cntr <= iterations; cntr++)
            {
                x_i_1 = x_i + s_i;

                x_i_1 = BiSect(targetFunction, x_i, x_i_1, accuracy, iterations);

                if ((x_i_1 - x_i).Magnitude < accuracy) break;

                s_i_1 = DoubleVector.Gradient(targetFunction, x_i_1, accuracy);

                omega = Math.Pow((s_i_1).Magnitude, 2) / Math.Pow((s_i).Magnitude, 2);

                s_i = s_i * omega - s_i_1;

                x_i = x_i_1;
            }
#if DEBUG
            Console.WriteLine($"conj gradient descend iterations number : {cntr}");
#endif
            return (x_i_1 + x_i) * 0.5;
        }
        public static DoubleVector СonjGradientDescend(FunctionND targetFunction, DoubleVector xStart, double accuracy) => СonjGradientDescend(targetFunction, xStart, accuracy, NumericCommon.ITERATIONS_COUNT_HIGH);
        public static DoubleVector СonjGradientDescend(FunctionND targetFunction, DoubleVector xStart) => СonjGradientDescend(targetFunction, xStart, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);
    
        ////////////////////
        /// Lab. work #4 ///
        ////////////////////
        public static DoubleVector NewtoneRaphson(FunctionND targetFunction, DoubleVector xStart, double accuracy, int iterations)
        {
            DoubleVector x_i   = new DoubleVector(xStart);

            DoubleVector x_i_1 = new DoubleVector(xStart);

            int cntr = 0;

            for (; cntr <= iterations; cntr++)
            { 
                x_i_1 = x_i - DoubleMatrix.Invert(DoubleMatrix.Hessian(targetFunction, x_i, accuracy)) * DoubleVector.Gradient(targetFunction, x_i, accuracy);

                if ((x_i_1 - x_i).Magnitude < accuracy) break;

                x_i = x_i_1;
            }
#if DEBUG
            Console.WriteLine($"Newtone - Raphson iterations number : {cntr}");
#endif
            return (x_i_1 + x_i) * 0.5;
        }
        public static DoubleVector NewtoneRaphson(FunctionND targetFunction, DoubleVector xStart, double accuracy) => NewtoneRaphson(targetFunction, xStart, accuracy, NumericCommon.ITERATIONS_COUNT_HIGH);
        public static DoubleVector NewtoneRaphson(FunctionND targetFunction, DoubleVector xStart) => NewtoneRaphson(targetFunction, xStart, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);

    }
}

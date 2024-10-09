using MathUtils;
using System;

namespace OptimizationMethods
{
    public static class MultiDimensional
    {
        ////////////////////
        /// Lab. work #2 ///
        ////////////////////
        public static Vector BiSect(FunctionND targetFunction, Vector lhs, Vector rhs, double accuracy, int iterations)
        {
            Vector x_c, dir;
            dir = Vector.Direction(lhs, rhs) * accuracy;
            int cntr = 0;
            for (; cntr != iterations && Vector.Distance(rhs, lhs) > 2 * accuracy; cntr++)
            {
                x_c = (rhs + lhs) * 0.5;
                if (targetFunction(x_c + dir) > targetFunction(x_c - dir))
                    rhs = x_c;
                else
                    lhs = x_c;
            }
#if DEBUG
            Console.WriteLine($"BiSect iterations number : {cntr}");
            Console.WriteLine($"BiSect argument range    : {Vector.Distance(rhs, lhs)}");
#endif
            return (rhs + lhs) * 0.5;
        }
        public static Vector BiSect(FunctionND targetFunction, Vector lhs, Vector rhs, double accuracy) => BiSect(targetFunction, lhs, rhs, accuracy, NumericCommon.ITERATIONS_COUNT_HIGH);
        public static Vector BiSect(FunctionND targetFunction, Vector lhs, Vector rhs) => BiSect(targetFunction, lhs, rhs, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);
        public static Vector GoldenRatio(FunctionND targetFunction, Vector _lhs, Vector _rhs, double accuracy, int iterations)
        {
            Vector lhs = new Vector(_lhs);
            Vector rhs = new Vector(_rhs);
            Vector x_l, x_r;
            double f_l, f_r;
            int iteration = 0;
            x_l = rhs - (rhs - lhs) * NumericCommon.PSI;
            x_r = lhs + (rhs - lhs) * NumericCommon.PSI;
            f_l = targetFunction(x_l);
            f_r = targetFunction(x_r);
            for (; iteration != iterations && Vector.Distance(rhs, lhs) > 2 * accuracy; iteration++)
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
            Console.WriteLine($"GoldenRatio argument range    : {Vector.Distance(rhs, lhs)}");
#endif
            return (rhs + lhs) * 0.5;
        }
        public static Vector GoldenRatio(FunctionND targetFunction, Vector lhs, Vector rhs, double accuracy) => GoldenRatio(targetFunction, lhs, rhs, accuracy, NumericCommon.ITERATIONS_COUNT_HIGH);
        public static Vector GoldenRatio(FunctionND targetFunction, Vector lhs, Vector rhs) => GoldenRatio(targetFunction, lhs, rhs, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);
        public static Vector Fibonacci(FunctionND targetFunction, Vector _lhs, Vector _rhs, double accuracy)
        {
            Vector lhs = new Vector(_lhs);
            Vector rhs = new Vector(_rhs);
            Vector x_l, x_r;
            double f_l, f_r, fib_t = 0.0, fib_1 = 1.0, fib_2 = 1.0;
            double condition = Vector.Distance(rhs, lhs) / accuracy;
            int iterations = 0;
            while (fib_2 < condition)
            {
                iterations++;
                fib_t = fib_1;
                fib_1 = fib_2;
                fib_2 += fib_t;
            }
            x_l = lhs + (rhs - lhs) * ((fib_2 - fib_1) / fib_2);
            x_r = lhs + (rhs - lhs) * (fib_1 / fib_2);

            f_l = targetFunction(x_l);
            f_r = targetFunction(x_r);
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
            Console.WriteLine($"Fibonacci argument range    : {Vector.Distance(rhs, lhs)}");
#endif
            return (rhs + lhs) * 0.5;
        }
        public static Vector Fibonacci(FunctionND targetFunction, Vector lhs, Vector rhs) => Fibonacci(targetFunction, lhs, rhs, NumericCommon.NUMERIC_ACCURACY_MIDDLE);

        public static Vector PerCoordDescend(FunctionND targetFunction, Vector xStart, double accuracy, int iterations)
        {
            Vector lhs = new Vector(xStart);

            Vector rhs = new Vector(xStart);

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

                lhs = new Vector(rhs);

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
        public static Vector PerCoordDescend(FunctionND targetFunction, Vector xStart, double accuracy) => PerCoordDescend(targetFunction, xStart, accuracy, NumericCommon.ITERATIONS_COUNT_HIGH);
        public static Vector PerCoordDescend(FunctionND targetFunction, Vector xStart) => PerCoordDescend(targetFunction, xStart, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);

        ////////////////////
        /// Lab. work #3 ///
        ////////////////////
        public static Vector GradientDescend(FunctionND targetFunction, Vector xStart, double accuracy, int iterations)
        {
            Vector x_i = new Vector(xStart);

            Vector x_i_1 = new Vector(xStart);

            int cntr = 0;

            for (; cntr <= iterations; cntr++)
            {
                x_i_1 = x_i - Vector.Gradient(targetFunction, x_i, accuracy);

                x_i_1 = BiSect(targetFunction, x_i, x_i_1, accuracy, iterations);

                if ((x_i_1 - x_i).Magnitude < accuracy) break;

                x_i = x_i_1;
            }
#if DEBUG
            Console.WriteLine($"gradient descend iterations number : {cntr}");
#endif
            return (x_i_1 + x_i) * 0.5;
        }
        public static Vector GradientDescend(FunctionND targetFunction, Vector xStart, double accuracy) => GradientDescend(targetFunction, xStart, accuracy, NumericCommon.ITERATIONS_COUNT_HIGH);
        public static Vector GradientDescend(FunctionND targetFunction, Vector xStart) => GradientDescend(targetFunction, xStart, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);
        
        public static Vector СonjGradientDescend(FunctionND targetFunction, Vector xStart, double accuracy, int iterations)
        {
            Vector x_i = new Vector(xStart);

            Vector x_i_1 = new Vector(xStart);

            Vector s_i = Vector.Gradient(targetFunction, xStart, accuracy) * (-1.0), s_i_1;

            double omega;

            int cntr = 0;

            for (; cntr <= iterations; cntr++)
            {
                x_i_1 = x_i + s_i;

                x_i_1 = BiSect(targetFunction, x_i, x_i_1, accuracy, iterations);

                if ((x_i_1 - x_i).Magnitude < accuracy) break;

                s_i_1 = Vector.Gradient(targetFunction, x_i_1, accuracy);

                omega = Math.Pow((s_i_1).Magnitude, 2) / Math.Pow((s_i).Magnitude, 2);

                s_i = s_i * omega - s_i_1;

                x_i = x_i_1;
            }
#if DEBUG
            Console.WriteLine($"conj gradient descend iterations number : {cntr}");
#endif
            return (x_i_1 + x_i) * 0.5;
        }
        public static Vector СonjGradientDescend(FunctionND targetFunction, Vector xStart, double accuracy) => СonjGradientDescend(targetFunction, xStart, accuracy, NumericCommon.ITERATIONS_COUNT_HIGH);
        public static Vector СonjGradientDescend(FunctionND targetFunction, Vector xStart) => СonjGradientDescend(targetFunction, xStart, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);
    
        ////////////////////
        /// Lab. work #4 ///
        ////////////////////
        public static Vector NewtoneRaphson(FunctionND targetFunction, Vector xStart, double accuracy, int iterations)
        {
            Vector x_i   = new Vector(xStart);

            Vector x_i_1 = new Vector(xStart);

            int cntr = 0;

            for (; cntr <= iterations; cntr++)
            { 
                x_i_1 = x_i - Matrix.Invert(Matrix.Hessian(targetFunction, x_i, accuracy)) * Vector.Gradient(targetFunction, x_i, accuracy);

                if ((x_i_1 - x_i).Magnitude < accuracy) break;

                x_i = x_i_1;
            }
#if DEBUG
            Console.WriteLine($"Newtone - Raphson iterations number : {cntr}");
#endif
            return (x_i_1 + x_i) * 0.5;
        }
        public static Vector NewtoneRaphson(FunctionND targetFunction, Vector xStart, double accuracy) => NewtoneRaphson(targetFunction, xStart, accuracy, NumericCommon.ITERATIONS_COUNT_HIGH);
        public static Vector NewtoneRaphson(FunctionND targetFunction, Vector xStart) => NewtoneRaphson(targetFunction, xStart, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);

    }
}

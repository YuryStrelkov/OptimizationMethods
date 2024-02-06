using MathUtils;
using System;

namespace OptimizationMethods
{
    public static class MultiDimensional
    {
        ////////////////////
        /// Lab. work #2 ///
        ////////////////////
        public static Vector BiSect(FunctionND f, Vector left, Vector right, double accuracy, int iterations)
        {
            Vector x_c, dir;

            dir = Vector.Direction(left, right) * accuracy;

            int cntr = 0;

            for (; cntr != iterations; cntr++)
            {
                if ((right - left).Magnitude < accuracy) break;

                x_c = (right + left) * (0.5);

                if (f(x_c + dir) > f(x_c - dir))
                {
                    right = x_c;
                    continue;
                }
                left = x_c;
            }
#if DEBUG
            Console.WriteLine($"dihotomia iterations number : {cntr}");
#endif
            return (right + left) * 0.5;
        }
        public static Vector BiSect(FunctionND f, Vector left, Vector right, double accuracy) => BiSect(f, left, right, accuracy, NumericCommon.ITERATIONS_COUNT_HIGH);
        public static Vector BiSect(FunctionND f, Vector left, Vector right) => BiSect(f, left, right, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);

        public static Vector GoldenRatio(FunctionND f, Vector left, Vector right, double accuracy, int iterations)
        {
            Vector a = new Vector(left);

            Vector b = new Vector(right);

            Vector dx;

            int cntr = 0;

            for (; cntr != iterations; cntr++)
            {
                if ((right - left).Magnitude < accuracy) break;

                dx = (b - a) * NumericCommon.ONE_OVER_PHI;
                left = b - dx;
                right = a + dx;

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
            return (a + b) * 0.5;
        }
        public static Vector GoldenRatio(FunctionND f, Vector left, Vector right, double accuracy) => GoldenRatio(f, left, right, accuracy, NumericCommon.ITERATIONS_COUNT_HIGH);
        public static Vector GoldenRatio(FunctionND f, Vector left, Vector right) => GoldenRatio(f, left, right, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);

        public static Vector Fibonacci(FunctionND f, Vector left, Vector right, double accuracy)
        {
            int f_n = 0, f_n_1 = 0, f_tmp, cntr = 0;

            NumericUtils.ClosestFibonacciPair((right - left).Magnitude / accuracy, ref f_n, ref f_n_1);

            Vector a = new Vector(left);

            Vector b = new Vector(right);

            Vector dx;

            while (f_n != f_n_1)
            {
                if ((right - left).Magnitude < accuracy) break;

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
            return (a + b) * 0.5;
        }
        public static Vector Fibonacci(FunctionND f, Vector left, Vector right) => Fibonacci(f, left, right, NumericCommon.NUMERIC_ACCURACY_MIDDLE);

        public static Vector PerCoordDescend(FunctionND f, Vector xStart, double accuracy, int iterations)
        {
            Vector left = new Vector(xStart);

            Vector right = new Vector(xStart);

            double step = 1.0;

            double x_i, y_1, y_0;

            int opt_coord_n = 0, coord_id;
            
            int i = 0;

            for (i = 0; i < iterations; i++)
            {
                coord_id = i % left.Count;

                right[coord_id] -= accuracy;

                y_0 = f(right);

                right[coord_id] += 2 * accuracy;

                y_1 = f(right);

                right[coord_id] -= accuracy;

                right[coord_id] = y_0 > y_1 ? right[coord_id] += step : right[coord_id] -= step;

                x_i = left[coord_id];

                right = BiSect(f, left, right, accuracy, iterations);

                left = new Vector(right);

                if (Math.Abs(right[coord_id] - x_i) < accuracy)
                {
                    opt_coord_n++;

                    if (opt_coord_n == right.Count)
                    {
#if DEBUG
                        Console.WriteLine($"per coord descend iterations number : {i}");
#endif
                        return left;
                    }
                    continue;
                }
                opt_coord_n = 0;
            }
#if DEBUG
            Console.WriteLine($"per coord descend iterations number : {iterations}");
#endif
            return left;
        }
        public static Vector PerCoordDescend(FunctionND f, Vector xStart, double accuracy) => PerCoordDescend(f, xStart, accuracy, NumericCommon.ITERATIONS_COUNT_HIGH);
        public static Vector PerCoordDescend(FunctionND f, Vector xStart) => PerCoordDescend(f, xStart, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);

        ////////////////////
        /// Lab. work #3 ///
        ////////////////////
        public static Vector GradientDescend(FunctionND f, Vector xStart, double accuracy, int iterations)
        {
            Vector x_i = new Vector(xStart);

            Vector x_i_1 = new Vector(xStart);

            int cntr = 0;

            for (; cntr <= iterations; cntr++)
            {
                x_i_1 = x_i - Vector.Gradient(f, x_i, accuracy);

                x_i_1 = BiSect(f, x_i, x_i_1, accuracy, iterations);

                if ((x_i_1 - x_i).Magnitude < accuracy) break;

                x_i = x_i_1;
            }
#if DEBUG
            Console.WriteLine($"gradient descend iterations number : {cntr}");
#endif
            return (x_i_1 + x_i) * 0.5;
        }
        public static Vector GradientDescend(FunctionND f, Vector xStart, double accuracy) => GradientDescend(f, xStart, accuracy, NumericCommon.ITERATIONS_COUNT_HIGH);
        public static Vector GradientDescend(FunctionND f, Vector xStart) => GradientDescend(f, xStart, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);
        
        public static Vector СonjGradientDescend(FunctionND f, Vector xStart, double accuracy, int iterations)
        {
            Vector x_i = new Vector(xStart);

            Vector x_i_1 = new Vector(xStart);

            Vector s_i = Vector.Gradient(f, xStart, accuracy) * (-1.0), s_i_1;

            double omega;

            int cntr = 0;

            for (; cntr <= iterations; cntr++)
            {
                x_i_1 = x_i + s_i;

                x_i_1 = BiSect(f, x_i, x_i_1, accuracy, iterations);

                if ((x_i_1 - x_i).Magnitude < accuracy) break;

                s_i_1 = Vector.Gradient(f, x_i_1, accuracy);

                omega = Math.Pow((s_i_1).Magnitude, 2) / Math.Pow((s_i).Magnitude, 2);

                s_i = s_i * omega - s_i_1;

                x_i = x_i_1;
            }
#if DEBUG
            Console.WriteLine($"conj gradient descend iterations number : {cntr}");
#endif
            return (x_i_1 + x_i) * 0.5;
        }
        public static Vector СonjGradientDescend(FunctionND f, Vector xStart, double accuracy) => СonjGradientDescend(f, xStart, accuracy, NumericCommon.ITERATIONS_COUNT_HIGH);
        public static Vector СonjGradientDescend(FunctionND f, Vector xStart) => СonjGradientDescend(f, xStart, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);
    
        ////////////////////
        /// Lab. work #4 ///
        ////////////////////
        public static Vector NewtoneRaphson(FunctionND f, Vector xStart, double accuracy, int iterations)
        {
            Vector x_i   = new Vector(xStart);

            Vector x_i_1 = new Vector(xStart);

            int cntr = 0;

            for (; cntr <= iterations; cntr++)
            { 
                x_i_1 = x_i - Matrix.Invert(Matrix.Hessian(f, x_i, accuracy)) * Vector.Gradient(f, x_i, accuracy);

                if ((x_i_1 - x_i).Magnitude < accuracy) break;

                x_i = x_i_1;
            }
#if DEBUG
            Console.WriteLine($"Newtone - Raphson iterations number : {cntr}");
#endif
            return (x_i_1 + x_i) * 0.5;
        }
        public static Vector NewtoneRaphson(FunctionND f, Vector xStart, double accuracy) => NewtoneRaphson(f, xStart, accuracy, NumericCommon.ITERATIONS_COUNT_HIGH);
        public static Vector NewtoneRaphson(FunctionND f, Vector xStart) => NewtoneRaphson(f, xStart, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);

    }
}

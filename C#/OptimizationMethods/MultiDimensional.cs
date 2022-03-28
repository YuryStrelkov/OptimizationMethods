using System;

namespace OptimizationMethods
{

    public static class MultiDimensional
    {
        public static double TestFunc2D(Vector x)
        {
            return (x[0] - 5) * x[0] + (x[1] - 3) * x[1]; // min at point x = 2.5, y = 1.5
        }

        public static double TestFuncND(Vector x)
        {

            double val = 0.0;
            for (int i=0; i < x.Size;i++) 
            {
                val += (x[i] - i) * x[i];
            }

            return val; // min at point x_i = i/2, i from 0 to x.Size-1
        }
        ////////////////////
        /// Lab. work #2 ///
        ////////////////////
        public static Vector Dihotomia      (func_n f, Vector x_0, Vector x_1, double eps = 1e-4f, int max_iters = 1000)
        {
            Vector x_c, dir;

            dir = Vector.Direction(x_0, x_1);

            int cntr = 0;

            for (; cntr != max_iters; cntr++)
            {
                if ((x_1 - x_0).Magnitude < eps)
                {
                    break;
                }
                x_c = (x_1 + x_0) * (0.5f);

                if (f(x_c + dir * eps) > f(x_c - dir * eps))
                {
                    x_1 = x_c;
                    continue;
                }
                x_0 = x_c;
            }
#if DEBUG
            Console.WriteLine("dihotomia iterations number : " + cntr);
#endif
            return (x_1 + x_0) * 0.5f;
        }
        public static Vector GoldenRatio    (func_n f, Vector x_0, Vector x_1, double eps = 1e-4f, int max_iters = 1000)
        {
            Vector a = new Vector(x_0);

            Vector b = new Vector(x_1);

            Vector dx;

            int cntr = 0;

            double one_div_phi = 1.0f / OneDimensional.Phi;

            for (; cntr != max_iters; cntr++)
            {
                if ((x_1 - x_0).Magnitude < eps)
                {
                    break;
                }

                dx = (b - a) * one_div_phi;
                x_0 = b - dx;
                x_1 = a + dx;

                if (f(x_0) >= f(x_1))
                {
                    a = x_0;
                    continue;
                }
                b = x_1;
            }
#if DEBUG
            Console.WriteLine("golden ratio iterations number : " + cntr);
#endif
            return (a + b) * 0.5f;
        }
        public static Vector Fibonacci      (func_n f, Vector x_0, Vector x_1, double eps = 1e-4f)
        {
            int max_iters = OneDimensional.ClosestFibonacci((x_1 - x_0).Magnitude / eps);

            int cntr = max_iters - 1;

            int[] f_n_s = OneDimensional.FibonacchiNumbers(max_iters);

            Vector a = new Vector(x_0);

            Vector b = new Vector(x_1);

            Vector dx;

            for (; cntr >= 2; cntr--)
            {
                if ((x_1 - x_0).Magnitude < eps)
                {
                    break;
                }
                dx = (b - a);
                x_0 = a + dx * ((double)f_n_s[cntr - 2] / f_n_s[cntr]);
                x_1 = a + dx * ((double)f_n_s[cntr - 1] / f_n_s[cntr]);

                if (f(x_0) < f(x_1))
                {
                    b = x_1;
                    continue;
                }
                a = x_0;
            }
#if DEBUG
            Console.WriteLine("fibonacci iterations number : " + max_iters);
#endif
            return (a + b) * 0.5f;
        }
        public static Vector PerCoordDescend(func_n f, Vector x_start, double eps = 1e-4f, int max_iters = 1000)
        {
            int cntr = 0;

            Vector x_0 = new Vector(x_start);

            Vector x_1 = new Vector(x_start);

            double step = 1.0f;

            double x_i, y_1, y_0;

            int opt_coord_n = 0, coord_id;

            for (int i = 0; i < max_iters; i++)
            {
                coord_id = i % x_0.Size;

                x_1[coord_id] -= eps;

                y_0 = f(x_1);

                x_1[coord_id] += 2 * eps;

                y_1 = f(x_1);

                x_1[coord_id] -= eps;

                x_1[coord_id] = y_0 > y_1 ? x_1[coord_id] += step : x_1[coord_id] -= step;

                x_i = x_0[coord_id];

                x_1 = Dihotomia(f, x_0, x_1, eps, max_iters);

                x_0 = new Vector(x_1);

                if (Math.Abs(x_1[coord_id] - x_i) < eps)
                {
                    opt_coord_n++;

                    if (opt_coord_n == x_1.Size)
                    {
#if DEBUG
                        Console.WriteLine($"per coord descend iterations number : {cntr}\n");
#endif
                        return x_0;
                    }
                    continue;
                }
                opt_coord_n = 0;
            }
#if DEBUG
            Console.WriteLine($"per coord descend iterations number : {cntr}\n");
#endif
            return x_0;
        }
        ////////////////////
        /// Lab. work #3 ///
        ////////////////////
        public static Vector GradientDescend(func_n f, Vector x_start, double eps = 1e-4f, int max_iters = 1000)
        {
            Vector x_i = new Vector(x_start);

            Vector x_i_1 = new Vector(x_start); ;

            int cntr = 0;

            while (true)
            {
                cntr++;

                if (cntr == max_iters)
                {
                    break;
                }

                x_i_1 = x_i - Vector.Gradient(f, x_i, eps);

                x_i_1 = Dihotomia(f, x_i, x_i_1, eps, max_iters);

                if ((x_i_1 - x_i).Magnitude < eps)
                {
                    break;
                }

                x_i = x_i_1;
            }
#if DEBUG
            Console.WriteLine("gradient descend iterations number : " + cntr);
#endif
            return (x_i_1 + x_i) * 0.5f;
        }
        public static Vector СonjGradientDescend(func_n f, Vector x_start, double eps = 1e-4f, int max_iters = 1000)
        {
            Vector x_i = new Vector(x_start);

            Vector x_i_1 = new Vector(x_start);

            Vector s_i = Vector.Gradient(f, x_start, eps) * (-1.0f), s_i_1;

            double omega;

            int cntr = 0;

            while (true)
            {
                cntr++;

                if (cntr == max_iters)
                {
                    break;
                }

                x_i_1 = x_i + s_i;

                x_i_1 = Dihotomia(f, x_i, x_i_1, eps, max_iters);

                if ((x_i_1 - x_i).Magnitude < eps)
                {
                    break;
                }

                s_i_1 = Vector.Gradient(f, x_i_1, eps);

                omega = Math.Pow((s_i_1).Magnitude, 2) / Math.Pow((s_i).Magnitude, 2);

                s_i = s_i * omega - s_i_1;

                x_i = x_i_1;
            }
#if DEBUG
            Console.WriteLine("conj gradient descend iterations number : " + cntr);
#endif
            return (x_i_1 + x_i) * 0.5f;
        }
        ////////////////////
        /// Lab. work #4 ///
        ////////////////////
        public static Vector NewtoneRaphson(func_n f, Vector x_start, double eps = 1e-4f, int max_iters = 1000)
        {
            Vector x_i = new Vector(x_start);

            Vector x_i_1 = new Vector(x_start);

            int cntr = 0;

            while (true)
            {
                cntr++;

                if (cntr == max_iters)
                {
                    break;
                }

                x_i_1 = x_i - Matrix.Invert(Matrix.Hessian(f, x_i, eps)) * Vector.Gradient(f, x_i, eps);

                if ((x_i_1 - x_i).Magnitude < eps)
                {
                    break;
                }

                x_i = x_i_1;
            }
#if DEBUG
            Console.WriteLine("gradient descend iterations number : " + cntr);
#endif
            return (x_i_1 + x_i) * 0.5f;
        }
    }
}

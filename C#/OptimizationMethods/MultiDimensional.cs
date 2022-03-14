using System;

namespace OptimizationMethods
{

    public static class MultiDimensional
    {
        public static Vector Dihotomia(func_n f, Vector x_0, Vector x_1, double eps = 1e-6f, int max_iters = 1000)
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
        public static Vector GoldenRatio(func_n f, Vector x_0, Vector x_1, double eps = 1e-6f, int max_iters = 1000)
        {
            Vector a = new Vector(x_0);
            Vector b = new Vector(x_1);
            Vector dx;

            int cntr = 0;

            double one_div_phi = 1.0f / (double)OneDimensional.Phi;

            for (; cntr != max_iters; cntr++)
            {
                dx = (b - a) * one_div_phi;
                x_1 = b - dx;
                x_0 = a + dx;

                if ((x_1 - x_0).Magnitude < eps)
                {
                    break;
                }

                if (f(x_0) > f(x_1))
                {
                    b = x_0;
                    continue;
                }
                a = x_1;
            }
#if DEBUG
            Console.WriteLine("golden ratio iterations number : " + cntr);
#endif
            return (a + b) * 0.5f;
        }
        public static Vector Fibonacci(func_n f, Vector x_0, Vector x_1, double eps = 1e-6f)
        {
            int max_iters = OneDimensional.ClosestFibonacci((x_1 - x_0).Magnitude / eps);

            Vector a = new Vector(x_0);
            Vector b = new Vector(x_1);
            Vector dx;

            double f_1 = 1.0f, f_2 = 2.0f, f_3 = 3.0f;

            int cntr = 0;

            for (; cntr != max_iters; cntr++)
            {
                dx = b - a;
                x_0 = b - dx * (f_1 / f_3);
                x_1 = b + dx * (f_2 / f_3);

                f_1 = f_2;
                f_2 = f_3;
                f_3 = f_1 + f_2;

                if ((x_1 - x_0).Magnitude < eps)
                {
                    break;
                }

                if (f(x_0) < f(x_1))
                {
                    b = x_0;
                    continue;
                }
                a = x_1;
            }
#if DEBUG
            Console.WriteLine("fibonacci iterations number : " + cntr);
#endif
            return (a + b) * 0.5f;
        }
        public static Vector PerCoordDescend(func_n f, Vector x_start, double eps = 1e-6f, int max_iters = 1000)
        {
            int cntr = 0;
            Vector x_0 = new Vector(x_start);
            Vector x_1 = new Vector(x_start);
            double step = 1.0f;
            double y_1, y_0;
            while (true)
            {
                for (int i = 0; i < x_0.Size; i++)
                {
                    cntr++;
                    if (cntr == max_iters)
                    {
#if DEBUG
                        Console.WriteLine("per coord descend iterations number : " + cntr);
#endif
                        return x_0;
                    }
                    x_1[i] -= eps;
                    y_0 = f(x_1);
                    x_1[i] += 2 * eps;
                    y_1 = f(x_1);
                    x_1[i] -= eps;

                    if (y_0 > y_1)
                    {
                        x_1[i] += step;
                    }

                    else
                    {
                        x_1[i] -= step;
                    }
                    x_1 = Dihotomia(f, x_0, x_1, eps, max_iters);
                    if ((x_1 - x_0).Magnitude < eps)
                    {
#if DEBUG
                        Console.WriteLine("per coord descend iterations number : " + cntr);
#endif
                        return x_0;
                    }
                    x_0 = new Vector(x_1);
                }
            }
        }
        public static Vector GradientDescend(func_n f, Vector x_start, double eps = 1e-6f, int max_iters = 1000)
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
        public static Vector СonjGradientDescend(func_n f, Vector x_start, double eps = 1e-6f, int max_iters = 1000)
        {
            Vector x_i   = new Vector(x_start);
            Vector x_i_1 = new Vector(x_start);
            Vector s_i   = Vector.Gradient(f, x_start, eps) * (-1.0f), s_i_1;
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

        public static Vector NewtoneRaphson(func_n f, Vector x_start, double eps = 1e-6f, int max_iters = 1000)
        {
            Vector x_i  = new Vector(x_start);
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
        public static void MultiDimensionalMethodsTest(func_n f)
        {
            Vector x_1 = new double[] { 0, 0 };
            Vector x_0 = new double[] { 5, 5 };
            Console.WriteLine("{ x, y } = agrmin((x - 2) * (x - 2) + (y - 2) * (y - 2))\n");
            Console.WriteLine("x_0 = " + x_0 + ", x_1 = " + x_1 + "\n");
            Console.WriteLine("Dihotomia              : " + Dihotomia(f, x_1, x_0).ToString());
            Console.WriteLine("GoldenRatio            : " + GoldenRatio(f, x_1, x_0).ToString());
            Console.WriteLine("Fibonacci              : " + Fibonacci(f, x_1, x_0).ToString());
            Console.WriteLine("PerCoordDescend        : " + PerCoordDescend(f, x_1).ToString());
            Console.WriteLine("GradientDescend        : " + GradientDescend(f, x_1).ToString());
            Console.WriteLine("СonjGradientDescend    : " + СonjGradientDescend(f, x_1).ToString());
            Console.WriteLine("NewtoneRaphson         : " + NewtoneRaphson(f, x_1).ToString());
        }
    }
}

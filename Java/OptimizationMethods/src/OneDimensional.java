public final class OneDimensional
{
    public static  final IFunction1D testFunc = new IFunction1D() {
        @Override
        public double execute(double arg) {
            return OneDimensional.testFunc(arg);
        }
    };
    ////////////////////
    /// Lab. work #1 ///
    ////////////////////
    public static double testFunc(double x)
    {
        return (x - 5) * (x - 2); // min at point x = 3.5
    }

    public static final double Phi = 1.61803398874989484820;

    public static double dihotomia  (IFunction1D f, double x_0, double x_1, double eps, int max_iters)
    {
        double x_c = 0.0;

        if (x_0 > x_1)
        {
            double tmp = x_0;
            x_0 = x_1;
            x_1 = tmp;
        }

        int cntr = 0;

        for (; cntr != max_iters; cntr++)
        {
            if (Math.abs(x_1 - x_0) < eps)
            {
                break;
            }
            x_c = (x_1 + x_0) * 0.5;

            if (f.execute(x_c + eps) > f.execute(x_c - eps))
            {
                x_1 = x_c;
                continue;
            }
            x_0 = x_c;
        }

        System.out.println("Dihotomia iterations number : " + cntr);

        return x_c;
    }

    public static double dihotomia  (IFunction1D f, double x_0, double x_1, double eps)
    {
       return dihotomia(f,x_0,x_1,eps, 1000);
    }
    public static double dihotomia  (IFunction1D f, double x_0, double x_1)
    {
        return dihotomia(f,x_0,x_1,1e-6, 1000);
    }

    public static double goldenRatio(IFunction1D f, double x_0, double x_1, double eps, int max_iters)
    {
        if (x_0 > x_1)
        {
            double tmp = x_0;
            x_0 = x_1;
            x_1 = tmp;
        }
        double a = x_0, b = x_1, dx;

        int cntr = 0;

        for (; cntr != max_iters; cntr++)
        {
            if (Math.abs(x_1 - x_0) < eps)
            {
                break;
            }
            dx  = b - a;
            x_0 = b - dx / Phi;
            x_1 = a + dx / Phi;

            if (f.execute(x_0) >= f.execute(x_1))
            {
                a = x_0;
                continue;
            }
            b = x_1;
        }

        System.out.println("Golden ratio iterations number : " + cntr);
        return (x_1 + x_0) * 0.5;
    }

    public static double goldenRatio  (IFunction1D f, double x_0, double x_1, double eps)
    {
        return goldenRatio(f,x_0,x_1,eps, 1000);
    }
    public static double goldenRatio  (IFunction1D f, double x_0, double x_1)
    {
        return goldenRatio(f,x_0,x_1,1e-6, 1000);
    }

    public static int[] fibonacchiNumbers(int index)
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
    public static int closestFibonacci(double value)
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
    public static double fibonacci(IFunction1D f, double x_0, double x_1, double eps)
    {
        if (x_0 > x_1)
        {
            double tmp = x_0;
            x_0 = x_1;
            x_1 = tmp;
        }

        double a = x_0, b = x_1, dx;

        int max_iters = closestFibonacci((b - a) / eps);

        int cntr = max_iters - 1;

        int[] f_n_s = fibonacchiNumbers(max_iters);

        for (; cntr >= 2; cntr--)
        {
            if (Math.abs(x_1 - x_0) < eps)
            {
                break;
            }

            dx = (b - a);
            x_0 = a + dx * f_n_s[cntr - 2] / f_n_s[cntr];
            x_1 = a + dx * f_n_s[cntr - 1] / f_n_s[cntr];

            if (f.execute(x_0) < f.execute(x_1))
            {
                b = x_1;
                continue;
            }
            a = x_0;
        }
        System.out.println("Fibonacchi iterations number : " + max_iters);

        return (x_1 + x_0) * 0.5;
    }
    public static double fibonacci  (IFunction1D f, double x_0, double x_1)
    {
        return fibonacci(f,x_0,x_1,1e-6);
    }
}

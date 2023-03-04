public final class OneDimensional
{
    public static final IFunction1D testFunc = OneDimensional::testFunc;
    ////////////////////
    /// Lab. work #1 ///
    ////////////////////
    public static double testFunc(double x)
    {
        return (x - 5) * (x - 2); // min at point x = 3.5
    }

    public static  boolean ShowDebugLog = false;

    public static final double Phi = 1.61803398874989484820;

    public static double biSect  (IFunction1D f, double x_0, double x_1, double eps, int max_iters)
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
            if (x_1 - x_0 < eps) break;

            x_c = (x_1 + x_0) * 0.5;

            if (f.call(x_c + eps) > f.call(x_c - eps))
            {
                x_1 = x_c;
                continue;
            }
            x_0 = x_c;
        }

        if(ShowDebugLog)System.out.println("BiSect iterations number : " + cntr);

        return x_c;
    }

    public static double biSect  (IFunction1D f, double x_0, double x_1, double eps)
    {
       return biSect(f, x_0, x_1, eps, 1000);
    }
    public static double biSect  (IFunction1D f, double x_0, double x_1)
    {
        return biSect(f, x_0, x_1, 1e-6, 1000);
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
            if (Math.abs(x_1 - x_0) < eps) break;

            dx  = b - a;
            x_0 = b - dx / Phi;
            x_1 = a + dx / Phi;

            if (f.call(x_0) >= f.call(x_1))
            {
                a = x_0;
                continue;
            }
            b = x_1;
        }

        if(ShowDebugLog)System.out.println("Golden ratio iterations number : " + cntr);

        return (x_1 + x_0) * 0.5;
    }

    public static double goldenRatio  (IFunction1D f, double x_0, double x_1, double eps)
    {
        return goldenRatio(f, x_0, x_1, eps, 1000);
    }
    public static double goldenRatio  (IFunction1D f, double x_0, double x_1)
    {
        return goldenRatio(f, x_0, x_1, 1e-6, 1000);
    }
    public static int[] fibonacciNumbers(int index)
    {
        if (index < 1) return new int[]{ 0 };

        if (index < 2) return new int[]{ 0, 1 };

        int[] res = new int[index];

        res[0] = 0;

        res[1] = 1;

        for (int i = 2; i < index; i++) res[i] = res[i - 1] + res[i - 2];

        return res;
    }
    public static int[] closestFibonacciPair(double value)
    {
        int f_n   = 0;
        int f_n_1 = 0;
        if (value < 1) return new int[]{ 0 };
        f_n_1 = 1;
        if (value < 2) return new int[]{ 0 , 1};
        int f_tmp;

        while (f_n < value)
        {
            f_tmp = f_n;
            f_n = f_n_1;
            f_n_1 += f_tmp;
        }
        return new int[]{f_n , f_n_1};
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

        int[] fib_pair = closestFibonacciPair((b - a) / eps);

        int f_n = fib_pair[0], f_n_1 = fib_pair[1], f_tmp, cntr = 0;

        while(f_n != f_n_1)
        {
            if (x_1 - x_0 < eps) break;
            cntr++;
            dx = (b - a);
            f_tmp = f_n_1 - f_n;
            x_0 = a + dx * ((double)f_tmp / f_n_1);
            x_1 = a + dx * ((double)f_n   / f_n_1);
            f_n_1 = f_n;
            f_n   = f_tmp;
            if (f.call(x_0) < f.call(x_1))
            {
                b = x_1;
                continue;
            }
            a = x_0;
        }
        if(ShowDebugLog)System.out.println("Fibonacci iterations number : " + cntr);

        return (x_1 + x_0) * 0.5;
    }
    public static double fibonacci  (IFunction1D f, double x_0, double x_1)
    {
        return fibonacci(f, x_0, x_1, 1e-6);
    }
}

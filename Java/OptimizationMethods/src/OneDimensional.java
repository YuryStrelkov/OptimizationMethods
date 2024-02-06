import functionalInterfaces.IFunction1D;
import mathUtils.NumericCommon;
import mathUtils.NumericUtils;

public final class OneDimensional {
    ////////////////////
    /// Lab. work #1 ///
    ////////////////////
    public static double biSect(IFunction1D f, double x1, double x2, double eps, int maxIterations) {
        double x_c = 0.0;
        if (x1 > x2) {
            double tmp = x1;
            x1 = x2;
            x2 = tmp;
        }
        int counter = 0;
        for (; counter != maxIterations; counter++) {
            if (x2 - x1 < eps) break;
            x_c = (x2 + x1) * 0.5;
            if (f.call(x_c + eps) > f.call(x_c - eps)) {
                x2 = x_c;
                continue;
            }
            x1 = x_c;
        }
        if (NumericCommon.SHOW_ZERO_ORDER_METHODS_DEBUG_LOG) System.out.printf("BiSect iterations number : %s\n", counter);
        return x_c;
    }

    public static double biSect(IFunction1D f, double x1, double x2, double eps) {
        return biSect(f, x1, x2, eps, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    public static double biSect(IFunction1D f, double x1, double x2) {
        return biSect(f, x1, x2, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    public static double goldenRatio(IFunction1D f, double x1, double x2, double eps, int maxIterations) {
        if (x1 > x2) {
            double tmp = x1;
            x1 = x2;
            x2 = tmp;
        }
        double a = x1, b = x2, dx;
        int counter = 0;
        for (; counter != maxIterations; counter++) {
            if (Math.abs(x2 - x1) < eps) break;
            dx = b - a;
            x1 = b - dx * NumericCommon.ONE_OVER_PHI;
            x2 = a + dx * NumericCommon.ONE_OVER_PHI;
            if (f.call(x1) >= f.call(x2)) {
                a = x1;
                continue;
            }
            b = x2;
        }

        if (NumericCommon.SHOW_ZERO_ORDER_METHODS_DEBUG_LOG)
            System.out.printf("Golden ratio iterations number : %s\n", counter);
        return (x2 + x1) * 0.5;
    }

    public static double goldenRatio(IFunction1D f, double x1, double x2, double eps) {
        return goldenRatio(f, x1, x2, eps, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    public static double goldenRatio(IFunction1D f, double x1, double x2) {
        return goldenRatio(f, x1, x2, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    public static double fibonacci(IFunction1D f, double x1, double x2, double eps) {
        if (x1 > x2) {
            double tmp = x1;
            x1 = x2;
            x2 = tmp;
        }
        double a = x1, b = x2, dx;
        int[] fib_pair = NumericUtils.closestFibonacciPair((b - a) / eps);
        int f_n = fib_pair[0], f_n_1 = fib_pair[1], f_tmp, counter = 0;
        while (f_n != f_n_1) {
            if (x2 - x1 < eps) break;
            counter++;
            dx = (b - a);
            f_tmp = f_n_1 - f_n;
            x1 = a + dx * ((double) f_tmp / f_n_1);
            x2 = a + dx * ((double) f_n / f_n_1);
            f_n_1 = f_n;
            f_n = f_tmp;
            if (f.call(x1) < f.call(x2)) {
                b = x2;
                continue;
            }
            a = x1;
        }
        if (NumericCommon.SHOW_ZERO_ORDER_METHODS_DEBUG_LOG) System.out.printf("Fibonacci iterations number : %s\n", counter);
        return (x2 + x1) * 0.5;
    }

    public static double fibonacci(IFunction1D f, double x1, double x2) {
        return fibonacci(f, x1, x2, NumericCommon.NUMERIC_ACCURACY_MIDDLE);
    }
}

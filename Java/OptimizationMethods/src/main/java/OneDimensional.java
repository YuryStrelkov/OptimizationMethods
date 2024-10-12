import mathUtils.functionalInterfaces.IFunction1D;
import mathUtils.NumericCommon;

public final class OneDimensional {
    ////////////////////
    /// Lab. work #1 ///
    ////////////////////
    public static double biSect(IFunction1D function, double left, double right, final double eps, final int maxIterations) {
        if (left > right) {
            final double tmp = left;
            left = right;
            right = tmp;
        }
        double center;
        int iteration = 0;
        for (; iteration != maxIterations && Math.abs(right - left) > 2 * eps; iteration++) {
            center = (right + left) * 0.5;
            if (function.call(center + 1e-1 * eps) > function.call(center - 1e-1 * eps))
                right = center;
            else
                left = center;
        }
        if (NumericCommon.SHOW_ZERO_ORDER_METHODS_DEBUG_LOG) {
            System.out.printf("BiSect::function probes count : %s\n", 2 * iteration);
            System.out.printf("BiSect::function arg range    : %s\n", right - left);
        }
        return (right + left) * 0.5;
    }

    public static double biSect(IFunction1D function, double left, double right, double eps) {
        return biSect(function, left, right, eps, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    public static double biSect(IFunction1D function, double left, double right) {
        return biSect(function, left, right, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    public static double goldenRatio(IFunction1D function, double left, double right, double eps, int maxIterations) {
        if (left > right) {
            final double tmp = left;
            left = right;
            right = tmp;
        }
        double x_l, x_r, f_l, f_r;
        int iteration = 0;
        x_l = right - (right - left) * NumericCommon.PSI;
        x_r = left + (right - left) * NumericCommon.PSI;
        f_l = function.call(x_l);
        f_r = function.call(x_r);
        for (; iteration != maxIterations && Math.abs(right - left) > 2 * eps; iteration++) {
            if (f_l > f_r) {
                left = x_l;
                x_l = x_r;
                f_l = f_r;
                x_r = left + (right - left) * NumericCommon.PSI;
                f_r = function.call(x_r);
            } else {
                right = x_r;
                x_r = x_l;
                f_r = f_l;
                x_l = right - (right - left) * NumericCommon.PSI;
                f_l = function.call(x_l);
            }
        }

        if (NumericCommon.SHOW_ZERO_ORDER_METHODS_DEBUG_LOG) {
            System.out.printf("goldenRatio::function probes count : %s\n", 2 + iteration);
            System.out.printf("goldenRatio::function arg range    : %s\n", right - left);
        }
        return (right + left) * 0.5;
    }

    public static double goldenRatio(IFunction1D function, double left, double right, double eps) {
        return goldenRatio(function, left, right, eps, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    public static double goldenRatio(IFunction1D function, double left, double right) {
        return goldenRatio(function, left, right, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    public static double fibonacci(IFunction1D function, double left, double right, double eps) {
        if (left > right) {
            final double tmp = left;
            left = right;
            right = tmp;
        }
        double x_l, x_r, f_l, f_r, condition, fib_t, fib_1 = 1.0, fib_2 = 1.0;
        int iterations = 0;
        condition = (right - left) / eps;
        while (fib_2 < condition) {
            iterations++;
            fib_t = fib_1;
            fib_1 = fib_2;
            fib_2 += fib_t;
        }
        x_l = left + (right - left) * ((fib_2 - fib_1) / fib_2);
        x_r = left + (right - left) * (fib_1 / fib_2);
        f_l = function.call(x_l);
        f_r = function.call(x_r);
        for (int index = iterations; index > 0; index--) {
            fib_t = fib_2 - fib_1;
            fib_2 = fib_1;
            fib_1 = fib_t;
            if (f_l > f_r) {
                left = x_l;
                f_l = f_r;
                x_l = x_r;
                x_r = left + (right - left) * (fib_1 / fib_2);
                f_r = function.call(x_r);
            } else {
                right = x_r;
                x_r = x_l;
                f_r = f_l;
                x_l = left + (right - left) * ((fib_2 - fib_1) / fib_2);
                f_l = function.call(x_l);
            }
        }
        if (NumericCommon.SHOW_ZERO_ORDER_METHODS_DEBUG_LOG) {
            System.out.printf("fibonacci::function probes count : %s\n", 2 + iterations);
            System.out.printf("fibonacci::function arg range    : %s\n", right - left);
        }
        return (right + left) * 0.5;
    }

    public static double fibonacci(IFunction1D function, double left, double right) {
        return fibonacci(function, left, right, NumericCommon.NUMERIC_ACCURACY_MIDDLE);
    }
}

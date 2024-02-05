import functionalInterfaces.IFunctionND;
import mathUtils.Common;
import mathUtils.DoubleVector;
import mathUtils.DoubleMatrix;

import java.util.Objects;

public class MultiDimensional {

    public static final IFunctionND testFunc2d = MultiDimensional::_testFunc2D;

    public static final IFunctionND testFuncNd = MultiDimensional::_testFuncND;

    private static double _testFunc2D(DoubleVector x) {
        return (x.get(0) - 5) * x.get(0) + (x.get(1) - 3) * x.get(1); // min at point x = 2.5, y = 1.5
    }

    private static double _testFuncND(DoubleVector x) {
        double val = 0.0;

        for (int i = 0; i < x.size(); i++) {
            val += (x.get(i) - i) * x.get(i);
        }
        return val; // min at point x_i = i/2, i from 0 to x.Size-1
    }

    ////////////////////
    /// Lab. work #2 ///
    ////////////////////
    public static DoubleVector biSect(IFunctionND f, DoubleVector x0, DoubleVector x1, double eps, int max_iters) {
        DoubleVector x_0 = new DoubleVector(x0), x_1 = new DoubleVector(x1), x_c, dir;

        dir = DoubleVector.direction(x_0, x_1).mul(eps);

        int cntr = 0;

        for (; cntr != max_iters; cntr++) {
            if (DoubleVector.sub(x_1, x_0).magnitude() < eps) break;

            x_c = DoubleVector.add(x_1, x_0).mul(0.5);

            if (f.call(DoubleVector.add(x_c, dir)) > f.call(DoubleVector.sub(x_c, dir))) {
                x_1 = x_c;
                continue;
            }
            x_0 = x_c;
        }

        if (Common.SHOW_ZERO_ORDER_METHODS_DEBUG_LOG) System.out.println("BiSect iterations number : " + cntr);

        return DoubleVector.add(x_1, x_0).mul(0.5);
    }

    public static DoubleVector biSect(IFunctionND f, DoubleVector x_0, DoubleVector x_1, double eps) {
        return biSect(f, x_0, x_1, eps, Common.ITERATIONS_COUNT_HIGH);
    }

    public static DoubleVector biSect(IFunctionND f, DoubleVector x_0, DoubleVector x_1) {
        return biSect(f, x_0, x_1, Common.NUMERIC_ACCURACY_MIDDLE, Common.ITERATIONS_COUNT_HIGH);
    }

    public static DoubleVector goldenRatio(IFunctionND f, DoubleVector x_0, DoubleVector x_1, double eps, int max_iters) {
        DoubleVector a = new DoubleVector(x_0);

        DoubleVector b = new DoubleVector(x_1);

        DoubleVector dx;

        int cntr = 0;

        for (; cntr != max_iters; cntr++) {
            if (DoubleVector.sub(x_1, x_0).magnitude() < eps) break;

            dx = DoubleVector.sub(b, a).mul(Common.ONE_OVER_PHI);
            x_0 = DoubleVector.sub(b, dx);
            x_1 = DoubleVector.add(a, dx);

            if (f.call(x_0) >= f.call(x_1)) {
                a = x_0;
                continue;
            }
            b = x_1;
        }

        if (Common.SHOW_ZERO_ORDER_METHODS_DEBUG_LOG) System.out.println("golden ratio iterations number : " + cntr);

        return DoubleVector.add(x_1, x_0).mul(0.5);
    }

    public static DoubleVector goldenRatio(IFunctionND f, DoubleVector x_0, DoubleVector x_1, double eps) {
        return goldenRatio(f, x_0, x_1, eps, Common.ITERATIONS_COUNT_HIGH);
    }

    public static DoubleVector goldenRatio(IFunctionND f, DoubleVector x_0, DoubleVector x_1) {
        return goldenRatio(f, x_0, x_1, Common.NUMERIC_ACCURACY_MIDDLE, Common.ITERATIONS_COUNT_HIGH);
    }

    public static DoubleVector fibonacci(IFunctionND f, DoubleVector x_0, DoubleVector x_1, double eps) {
        DoubleVector a = new DoubleVector(x_0);

        DoubleVector b = new DoubleVector(x_1);

        DoubleVector dx;

        int[] fib_pair = OneDimensional.closestFibonacciPair(DoubleVector.sub(b, a).magnitude() / eps);

        int f_n = fib_pair[0], f_n_1 = fib_pair[1], f_tmp, cntr = 0;

        while (f_n != f_n_1) {
            if (DoubleVector.sub(x_1, x_0).magnitude() < eps) break;
            cntr++;
            f_tmp = f_n_1 - f_n;
            dx = DoubleVector.sub(b, a);
            x_0 = DoubleVector.add(a, DoubleVector.mul(dx, (double) f_tmp / f_n_1));
            x_1 = DoubleVector.add(a, DoubleVector.mul(dx, (double) f_n / f_n_1));
            f_n_1 = f_n;
            f_n = f_tmp;
            if (f.call(x_0) < f.call(x_1)) {
                b = x_1;
                continue;
            }
            a = x_0;
        }

        if (Common.SHOW_ZERO_ORDER_METHODS_DEBUG_LOG) System.out.println("fibonacci iterations number : " + cntr);

        return DoubleVector.add(x_1, x_0).mul(0.5);
    }

    public static DoubleVector fibonacci(IFunctionND f, DoubleVector x_0, DoubleVector x_1) {
        return fibonacci(f, x_0, x_1, Common.NUMERIC_ACCURACY_MIDDLE);
    }

    public static DoubleVector perCordDescend(IFunctionND f, DoubleVector x_start, double eps, int max_iters) {
        DoubleVector x_0 = new DoubleVector(x_start);

        DoubleVector x_1 = new DoubleVector(x_start);

        double step = 1.0;

        double x_i, y_1, y_0;

        int opt_coord_n = 0, coord_id;

        for (int i = 0; i < max_iters; i++) {
            coord_id = i % x_0.size();

            x_1.set(coord_id, x_1.get(coord_id) - eps);

            y_0 = f.call(x_1);

            x_1.set(coord_id, x_1.get(coord_id) + 2 * eps);

            y_1 = f.call(x_1);

            x_1.set(coord_id, x_1.get(coord_id) - eps);

            x_1.set(coord_id, y_0 > y_1 ? x_1.get(coord_id) + step : x_1.get(coord_id) - step);

            x_i = x_0.get(coord_id);

            x_1 = biSect(f, x_0, x_1, eps, max_iters);

            x_0 = new DoubleVector(x_1);

            if (Math.abs(x_1.get(coord_id) - x_i) < eps) {
                opt_coord_n++;

                if (opt_coord_n == x_1.size()) {
                    if (Common.SHOW_DEBUG_LOG) System.out.println("per cord descend iterations number : " + (i + 1));

                    return x_0;
                }
                continue;
            }
            opt_coord_n = 0;
        }

        if (Common.SHOW_DEBUG_LOG) System.out.println("per cord descend iterations number : " + max_iters);

        return x_0;
    }

    public static DoubleVector perCordDescend(IFunctionND f, DoubleVector x_start, double eps) {
        return perCordDescend(f, x_start, eps, Common.ITERATIONS_COUNT_HIGH);
    }

    public static DoubleVector perCordDescend(IFunctionND f, DoubleVector x_start) {
        return perCordDescend(f, x_start, Common.NUMERIC_ACCURACY_MIDDLE, Common.ITERATIONS_COUNT_HIGH);
    }

    ////////////////////
    /// Lab. work #3 ///
    ////////////////////
    public static DoubleVector gradientDescend(IFunctionND f, DoubleVector x_start, double eps, int max_iters) {
        DoubleVector x_i = new DoubleVector(x_start);

        DoubleVector x_i_1 = new DoubleVector(x_start);
        ;

        int cntr = 0;

        for (; cntr != max_iters; cntr++) {
            x_i_1 = DoubleVector.sub(x_i, DoubleVector.gradient(f, x_i, eps));

            x_i_1 = biSect(f, x_i, x_i_1, eps, max_iters);

            if (DoubleVector.sub(x_i_1, x_i).magnitude() < eps) break;

            x_i = x_i_1;
        }

        if (Common.SHOW_DEBUG_LOG) System.out.println("gradient descend iterations number : " + (cntr + 1));

        return DoubleVector.add(x_i_1, x_i).mul(0.5);
    }

    public static DoubleVector gradientDescend(IFunctionND f, DoubleVector x_start, double eps) {
        return gradientDescend(f, x_start, eps, Common.ITERATIONS_COUNT_HIGH);
    }

    public static DoubleVector gradientDescend(IFunctionND f, DoubleVector x_start) {
        return gradientDescend(f, x_start, Common.NUMERIC_ACCURACY_MIDDLE, Common.ITERATIONS_COUNT_HIGH);
    }

    public static DoubleVector conjGradientDescend(IFunctionND f, DoubleVector x_start, double eps, int max_iters) {
        DoubleVector x_i = new DoubleVector(x_start);

        DoubleVector x_i_1 = new DoubleVector(x_start);

        DoubleVector s_i = DoubleVector.gradient(f, x_start, eps).mul(-1.0), s_i_1;

        double omega;

        int cntr = 0;

        for (; cntr != max_iters; cntr++) {
            x_i_1 = DoubleVector.add(x_i, s_i);

            x_i_1 = biSect(f, x_i, x_i_1, eps, max_iters);

            if (DoubleVector.sub(x_i_1, x_i).magnitude() < eps) break;

            s_i_1 = DoubleVector.gradient(f, x_i_1, eps);

            omega = Math.pow((s_i_1).magnitude(), 2) / Math.pow((s_i).magnitude(), 2);

            s_i.mul(omega).sub(s_i_1);

            x_i = x_i_1;
        }

        if (Common.SHOW_DEBUG_LOG) System.out.println("Conj gradient descend iterations number : " + (cntr + 1));

        return DoubleVector.add(x_i_1, x_i).mul(0.5);
    }

    public static DoubleVector conjGradientDescend(IFunctionND f, DoubleVector x_start, double eps) {
        return conjGradientDescend(f, x_start, eps, Common.ITERATIONS_COUNT_HIGH);
    }

    public static DoubleVector conjGradientDescend(IFunctionND f, DoubleVector x_start) {
        return conjGradientDescend(f, x_start, Common.NUMERIC_ACCURACY_MIDDLE, Common.ITERATIONS_COUNT_HIGH);
    }

    ////////////////////
    /// Lab. work #4 ///
    ////////////////////
    public static DoubleVector newtoneRaphson(IFunctionND f, DoubleVector x_start, double eps, int max_iters) {
        DoubleVector x_i = new DoubleVector(x_start);

        DoubleVector x_i_1 = new DoubleVector(x_start);

        int cntr = -1;

        for (; cntr != max_iters; cntr++) {
            DoubleMatrix invHessian = Objects.requireNonNull(DoubleMatrix.invert(DoubleMatrix.hessian(f, x_i, eps)));

            x_i_1 = DoubleVector.sub(x_i, DoubleMatrix.mul(invHessian, DoubleVector.gradient(f, x_i, eps)));

            ///Метод работает, но условие снизу не отрабатывает
            if (DoubleVector.sub(x_i_1, x_i).magnitude() < eps) break;

            x_i = x_i_1;
        }

        if (Common.SHOW_DEBUG_LOG) System.out.println("Newtone - Raphson iterations number : " + (cntr + 1));

        return DoubleVector.add(x_i_1, x_i).mul(0.5);
    }

    public static DoubleVector newtoneRaphson(IFunctionND f, DoubleVector x_start, double eps) {
        return newtoneRaphson(f, x_start, eps, Common.ITERATIONS_COUNT_LOW);
    }

    public static DoubleVector newtoneRaphson(IFunctionND f, DoubleVector x_start) {
        return newtoneRaphson(f, x_start, Common.NUMERIC_ACCURACY_MIDDLE, Common.ITERATIONS_COUNT_LOW);
    }
}

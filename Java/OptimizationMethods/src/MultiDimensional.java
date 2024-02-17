import functionalInterfaces.IFunctionND;
import mathUtils.NumericCommon;
import mathUtils.DoubleVector;
import mathUtils.DoubleMatrix;
import mathUtils.NumericUtils;

import java.util.Objects;

@SuppressWarnings("all")
public class MultiDimensional {
    ////////////////////
    /// Lab. work #2 ///
    ////////////////////
    public static DoubleVector biSect(IFunctionND f, DoubleVector x0, DoubleVector x1,
                                      double eps, int maxIterations) {
        DoubleVector x_0 = new DoubleVector(x0), x_1 = new DoubleVector(x1), x_c, dir;
        dir = DoubleVector.direction(x_0, x_1).mul(eps);
        int iteration = 0;
        for (; iteration != maxIterations; iteration++) {
            if (DoubleVector.sub(x_1, x_0).magnitude() < eps) break;
            x_c = DoubleVector.add(x_1, x_0).mul(0.5);
            if (f.call(DoubleVector.add(x_c, dir)) > f.call(DoubleVector.sub(x_c, dir))) {
                x_1 = x_c;
                continue;
            }
            x_0 = x_c;
        }
        if (NumericCommon.SHOW_ZERO_ORDER_METHODS_DEBUG_LOG) System.out.printf("BiSect iterations number : %s\n", iteration);
        return DoubleVector.add(x_1, x_0).mul(0.5);
    }

    public static DoubleVector biSect(IFunctionND f, DoubleVector x1, DoubleVector x2, double eps) {
        return biSect(f, x1, x2, eps, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    public static DoubleVector biSect(IFunctionND f, DoubleVector x1, DoubleVector x2) {
        return biSect(f, x1, x2, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    public static DoubleVector goldenRatio(IFunctionND f, DoubleVector x1, DoubleVector x2,
                                           double eps, int maxIterations) {
        DoubleVector a = new DoubleVector(x1);
        DoubleVector b = new DoubleVector(x2);
        DoubleVector dx;
        int iteration = 0;
        for (; iteration != maxIterations; iteration++) {
            if (DoubleVector.sub(x2, x1).magnitude() < eps) break;
            dx = DoubleVector.sub(b, a).mul(NumericCommon.ONE_OVER_PHI);
            x1 = DoubleVector.sub(b, dx);
            x2 = DoubleVector.add(a, dx);
            if (f.call(x1) >= f.call(x2)) {
                a = x1;
                continue;
            }
            b = x2;
        }

        if (NumericCommon.SHOW_ZERO_ORDER_METHODS_DEBUG_LOG)
            System.out.printf("golden ratio iterations number : %s\n", iteration);

        return DoubleVector.add(x2, x1).mul(0.5);
    }

    public static DoubleVector goldenRatio(IFunctionND f, DoubleVector x1, DoubleVector x2, double eps) {
        return goldenRatio(f, x1, x2, eps, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    public static DoubleVector goldenRatio(IFunctionND f, DoubleVector x1, DoubleVector x2) {
        return goldenRatio(f, x1, x2, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    public static DoubleVector fibonacci(IFunctionND f, DoubleVector x0, DoubleVector x1, double eps) {
        DoubleVector a = new DoubleVector(x0);
        DoubleVector b = new DoubleVector(x1);
        DoubleVector dx;
        int[] fib_pair = NumericUtils.closestFibonacciPair(DoubleVector.sub(b, a).magnitude() / eps);
        int f_n = fib_pair[0], f_n_1 = fib_pair[1], f_tmp, iteration = 0;
        while (f_n != f_n_1) {
            if (DoubleVector.sub(x1, x0).magnitude() < eps) break;
            iteration++;
            f_tmp = f_n_1 - f_n;
            dx = DoubleVector.sub(b, a);
            x0 = DoubleVector.add(a, DoubleVector.mul(dx, (double) f_tmp / f_n_1));
            x1 = DoubleVector.add(a, DoubleVector.mul(dx, (double) f_n / f_n_1));
            f_n_1 = f_n;
            f_n = f_tmp;
            if (f.call(x0) < f.call(x1)) {
                b = x1;
                continue;
            }
            a = x0;
        }
        if (NumericCommon.SHOW_ZERO_ORDER_METHODS_DEBUG_LOG)
            System.out.printf("fibonacci iterations number : %s\n", iteration);
        return DoubleVector.add(x1, x0).mul(0.5);
    }

    public static DoubleVector fibonacci(IFunctionND f, DoubleVector x0, DoubleVector x1) {
        return fibonacci(f, x0, x1, NumericCommon.NUMERIC_ACCURACY_MIDDLE);
    }

    public static DoubleVector perCordDescend(IFunctionND f, DoubleVector xStart, double eps, int maxIterations) {
        DoubleVector x_0 = new DoubleVector(xStart);
        DoubleVector x_1 = new DoubleVector(xStart);
        double step = 1.0;
        double x_i, y_1, y_0;
        int optCoordinatesCount = 0, coordinateId;
        for (int iteration = 0; iteration < maxIterations; iteration++) {
            coordinateId = iteration % x_0.size();
            x_1.set(coordinateId, x_1.get(coordinateId) - eps);
            y_0 = f.call(x_1);
            x_1.set(coordinateId, x_1.get(coordinateId) + 2 * eps);
            y_1 = f.call(x_1);
            x_1.set(coordinateId, x_1.get(coordinateId) - eps);
            x_1.set(coordinateId, y_0 > y_1 ? x_1.get(coordinateId) + step : x_1.get(coordinateId) - step);
            x_i = x_0.get(coordinateId);
            x_1 = biSect(f, x_0, x_1, eps, maxIterations);
            x_0 = new DoubleVector(x_1);
            if (Math.abs(x_1.get(coordinateId) - x_i) < eps) {
                optCoordinatesCount++;
                if (optCoordinatesCount == x_1.size()) {
                    if (NumericCommon.SHOW_DEBUG_LOG)
                        System.out.printf("per cord descend iterations number : %s\n", iteration + 1);
                    return x_0;
                }
                continue;
            }
            optCoordinatesCount = 0;
        }
        if (NumericCommon.SHOW_DEBUG_LOG) System.out.printf("per cord descend iterations number : %s\n", maxIterations);
        return x_0;
    }

    public static DoubleVector perCordDescend(IFunctionND f, DoubleVector xStart, double eps) {
        return perCordDescend(f, xStart, eps, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    public static DoubleVector perCordDescend(IFunctionND f, DoubleVector xStart) {
        return perCordDescend(f, xStart, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    ////////////////////
    /// Lab. work #3 ///
    ////////////////////
    public static DoubleVector gradientDescend(IFunctionND f, DoubleVector xStart, double eps, int maxIterations) {
        DoubleVector x_i = new DoubleVector(xStart);
        DoubleVector x_i_1 = new DoubleVector(xStart);
        int cntr = 0;
        for (; cntr != maxIterations; cntr++) {
            x_i_1 = DoubleVector.sub(x_i, DoubleVector.gradient(f, x_i, eps));
            x_i_1 = biSect(f, x_i, x_i_1, eps, maxIterations);
            if (DoubleVector.sub(x_i_1, x_i).magnitude() < eps) break;
            x_i = x_i_1;
        }

        if (NumericCommon.SHOW_DEBUG_LOG) System.out.printf("gradient descend iterations number : %s\n", cntr + 1);

        return DoubleVector.add(x_i_1, x_i).mul(0.5);
    }

    public static DoubleVector gradientDescend(IFunctionND f, DoubleVector xStart, double eps) {
        return gradientDescend(f, xStart, eps, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    public static DoubleVector gradientDescend(IFunctionND f, DoubleVector xStart) {
        return gradientDescend(f, xStart, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    public static DoubleVector conjGradientDescend(IFunctionND f, DoubleVector xStart, double eps, int maxIterations) {
        DoubleVector x_i = new DoubleVector(xStart);
        DoubleVector x_i_1 = new DoubleVector(xStart);
        DoubleVector s_i = DoubleVector.gradient(f, xStart, eps).mul(-1.0), s_i_1;
        double omega;
        int iteration = 0;
        for (; iteration != maxIterations; iteration++) {
            x_i_1 = DoubleVector.add(x_i, s_i);
            x_i_1 = biSect(f, x_i, x_i_1, eps, maxIterations);
            if (DoubleVector.sub(x_i_1, x_i).magnitude() < eps) break;
            s_i_1 = DoubleVector.gradient(f, x_i_1, eps);
            omega = Math.pow((s_i_1).magnitude(), 2) / Math.pow((s_i).magnitude(), 2);
            s_i.mul(omega).sub(s_i_1);
            x_i = x_i_1;
        }

        if (NumericCommon.SHOW_DEBUG_LOG) System.out.printf("Conj gradient descend iterations number : %s\n", iteration + 1);
        return DoubleVector.add(x_i_1, x_i).mul(0.5);
    }

    public static DoubleVector conjGradientDescend(IFunctionND f, DoubleVector xStart, double eps) {
        return conjGradientDescend(f, xStart, eps, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    public static DoubleVector conjGradientDescend(IFunctionND f, DoubleVector xStart) {
        return conjGradientDescend(f, xStart, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    ////////////////////
    /// Lab. work #4 ///
    ////////////////////
    public static DoubleVector newtoneRaphson(IFunctionND f, DoubleVector xStart, double eps, int maxIterations) {
        DoubleVector x_i = new DoubleVector(xStart);
        DoubleVector x_i_1 = new DoubleVector(xStart);
        int iteration = -1;
        for (; iteration != maxIterations; iteration++) {
            DoubleMatrix invHessian = Objects.requireNonNull(DoubleMatrix.invert(DoubleMatrix.hessian(f, x_i, eps)));
            x_i_1 = DoubleVector.sub(x_i, DoubleMatrix.mul(invHessian, DoubleVector.gradient(f, x_i, eps)));
            ///Метод работает, но условие снизу не отрабатывает
            if (DoubleVector.sub(x_i_1, x_i).magnitude() < eps) break;
            x_i = x_i_1;
        }

        if (NumericCommon.SHOW_DEBUG_LOG) System.out.printf("Newtone - Raphson iterations number : %s\n", iteration + 1);
        return DoubleVector.add(x_i_1, x_i).mul(0.5);
    }

    public static DoubleVector newtoneRaphson(IFunctionND f, DoubleVector xStart, double eps) {
        return newtoneRaphson(f, xStart, eps, NumericCommon.ITERATIONS_COUNT_LOW);
    }

    public static DoubleVector newtoneRaphson(IFunctionND f, DoubleVector xStart) {
        return newtoneRaphson(f, xStart, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_LOW);
    }
}

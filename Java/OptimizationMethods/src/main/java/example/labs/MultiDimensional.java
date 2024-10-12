package example.labs;

import example.functionalInterfaces.IFunctionND;
import lombok.extern.slf4j.Slf4j;
import example.mathUtils.*;

import java.util.Objects;

@SuppressWarnings("all")
@Slf4j
public class MultiDimensional {
    ////////////////////
    /// Lab. work #2 ///
    ////////////////////
    public static DoubleVector biSect(IFunctionND function, DoubleVector left, DoubleVector right,
                                      double eps, int maxIterations) {
        DoubleVector lhs = new DoubleVector(left);
        DoubleVector rhs = new DoubleVector(right);
        DoubleVector x_c, dir = DoubleVectorUtils.direction(lhs, rhs).mul(eps);
        int iteration = 0;
        for (; iteration != maxIterations && DoubleVectorUtils.distance(rhs, lhs) > 2 * eps; iteration++) {
            x_c = DoubleVectorUtils.add(rhs, lhs).mul(0.5);
            if (function.call(DoubleVectorUtils.add(x_c, dir)) > function.call(DoubleVectorUtils.sub(x_c, dir)))
                rhs = x_c;
            else
                lhs = x_c;
        }
        if (Boolean.parseBoolean(System.getenv("SHOW_ZERO_ORDER_METHODS_DEBUG_LOG"))) {
            System.out.printf("biSect::function arg range    : %s\n", DoubleVectorUtils.sub(rhs, lhs).magnitude());
            System.out.printf("biSect::function probes count : %s\n", 2 * iteration);
        }
        return DoubleVectorUtils.add(rhs, lhs).mul(0.5);
    }

    public static DoubleVector biSect(IFunctionND function, DoubleVector left, DoubleVector right, double eps) {
        return biSect(function, left, right, eps, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    public static DoubleVector biSect(IFunctionND function, DoubleVector left, DoubleVector right) {
        return biSect(function, left, right, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    public static DoubleVector goldenRatio(IFunctionND function, DoubleVector left, DoubleVector right,
                                           double eps, int maxIterations) {
        DoubleVector lhs = new DoubleVector(left);
        DoubleVector rhs = new DoubleVector(right);
        DoubleVector x_l = DoubleVectorUtils.sub(rhs, DoubleVectorUtils.mul(DoubleVectorUtils.sub(rhs, lhs), NumericCommon.PSI));
        DoubleVector x_r = DoubleVectorUtils.add(lhs, DoubleVectorUtils.mul(DoubleVectorUtils.sub(rhs, lhs), NumericCommon.PSI));
        double f_l = function.call(x_l);
        double f_r = function.call(x_r);
        int iteration = 0;
        for (; iteration != maxIterations && DoubleVectorUtils.distance(rhs, lhs) > 2 * eps; iteration++) {
            if (f_l > f_r) {
                lhs = x_l;
                x_l = x_r;
                f_l = f_r;
                x_r = DoubleVectorUtils.add(lhs, DoubleVectorUtils.mul(DoubleVectorUtils.sub(rhs, lhs), NumericCommon.PSI));
                f_r = function.call(x_r);
            } else {
                rhs = x_r;
                x_r = x_l;
                f_r = f_l;
                x_l = DoubleVectorUtils.sub(rhs, DoubleVectorUtils.mul(DoubleVectorUtils.sub(rhs, lhs), NumericCommon.PSI));
                f_l = function.call(x_l);
            }
        }
        if (NumericCommon.SHOW_ZERO_ORDER_METHODS_DEBUG_LOG) {
            System.out.printf("goldenRatio::function arg range    : %s\n", DoubleVectorUtils.sub(rhs, lhs).magnitude());
            System.out.printf("goldenRatio::function probes count : %s\n", 2 + iteration);
        }
        return DoubleVectorUtils.add(rhs, lhs).mul(0.5);
    }

    public static DoubleVector goldenRatio(IFunctionND function, DoubleVector left, DoubleVector right, double eps) {
        return goldenRatio(function, left, right, eps, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    public static DoubleVector goldenRatio(IFunctionND f, DoubleVector left, DoubleVector right) {
        return goldenRatio(f, left, right, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    public static DoubleVector fibonacci(IFunctionND function, DoubleVector left, DoubleVector right, double eps) {
        DoubleVector lhs = new DoubleVector(left);
        DoubleVector rhs = new DoubleVector(right);
        double condition, fib_t, fib_1 = 1.0, fib_2 = 1.0;
        int iterations = 0;
        condition = DoubleVectorUtils.distance(rhs, lhs) / eps;
        while (fib_2 < condition) {
            iterations++;
            fib_t = fib_1;
            fib_1 = fib_2;
            fib_2 += fib_t;
        }
        DoubleVector x_l = DoubleVectorUtils.add(lhs, DoubleVectorUtils.mul(DoubleVectorUtils.sub(rhs, lhs), (fib_2 - fib_1) / fib_2));
        DoubleVector x_r = DoubleVectorUtils.add(lhs, DoubleVectorUtils.mul(DoubleVectorUtils.sub(rhs, lhs), fib_1 / fib_2));
        double f_l = function.call(x_l);
        double f_r = function.call(x_r);
        for (int index = iterations; index > 0; index--) {
            fib_t = fib_2 - fib_1;
            fib_2 = fib_1;
            fib_1 = fib_t;
            if (f_l > f_r) {
                lhs = x_l;
                x_l = x_r;
                f_l = f_r;
                x_r = DoubleVectorUtils.add(lhs, DoubleVectorUtils.mul(DoubleVectorUtils.sub(rhs, lhs), fib_1 / fib_2));
                f_r = function.call(x_r);
            } else {
                rhs = x_r;
                x_r = x_l;
                f_r = f_l;
                x_l = DoubleVectorUtils.add(lhs, DoubleVectorUtils.mul(DoubleVectorUtils.sub(rhs, lhs), (fib_2 - fib_1) / fib_2));
                f_l = function.call(x_l);
            }
        }
        log.debug("goldenRatio::function arg range    : {}\n", DoubleVectorUtils.sub(rhs, lhs).magnitude());
        log.debug("goldenRatio::function probes count : {}\n", 2 + iterations);

        return DoubleVectorUtils.add(rhs, lhs).mul(0.5);
    }

    public static DoubleVector fibonacci(IFunctionND function, DoubleVector left, DoubleVector right) {
        return fibonacci(function, left, right, NumericCommon.NUMERIC_ACCURACY_MIDDLE);
    }

    public static DoubleVector perCordDescend(IFunctionND function, DoubleVector xStart, double eps, int maxIterations) {
        DoubleVector x_0 = new DoubleVector(xStart);
        DoubleVector x_1 = new DoubleVector(xStart);
        double step = 1.0;
        double x_i, y_1, y_0;
        int optCoordinatesCount = 0, coordinateId;
        for (int iteration = 0; iteration < maxIterations; iteration++) {
            coordinateId = iteration % x_0.size();
            x_1.set(coordinateId, x_1.get(coordinateId) - eps);
            y_0 = function.call(x_1);
            x_1.set(coordinateId, x_1.get(coordinateId) + 2 * eps);
            y_1 = function.call(x_1);
            x_1.set(coordinateId, x_1.get(coordinateId) - eps);
            x_1.set(coordinateId, y_0 > y_1 ? x_1.get(coordinateId) + step : x_1.get(coordinateId) - step);
            x_i = x_0.get(coordinateId);
            x_1 = biSect(function, x_0, x_1, eps, maxIterations);
            x_0 = new DoubleVector(x_1);
            if (Math.abs(x_1.get(coordinateId) - x_i) < 2 * eps) {
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

    public static DoubleVector perCordDescend(IFunctionND function, DoubleVector xStart, double eps) {
        return perCordDescend(function, xStart, eps, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    public static DoubleVector perCordDescend(IFunctionND function, DoubleVector xStart) {
        return perCordDescend(function, xStart, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    ////////////////////
    /// Lab. work #3 ///
    ////////////////////
    public static DoubleVector gradientDescend(IFunctionND function, DoubleVector xStart, double eps, int maxIterations) {
        DoubleVector x_i = new DoubleVector(xStart);
        DoubleVector x_i_1 = new DoubleVector(xStart);
        int cntr = 0;
        for (; cntr != maxIterations; cntr++) {
            x_i_1 = DoubleVectorUtils.sub(x_i, DoubleVectorUtils.gradient(function, x_i, eps));
            x_i_1 = biSect(function, x_i, x_i_1, eps, maxIterations);
            if (DoubleVectorUtils.distance(x_i_1, x_i) < 2 * eps) break;
            x_i = x_i_1;
        }

        if (NumericCommon.SHOW_DEBUG_LOG) System.out.printf("gradient descend iterations number : %s\n", cntr + 1);

        return DoubleVectorUtils.add(x_i_1, x_i).mul(0.5);
    }

    public static DoubleVector gradientDescend(IFunctionND function, DoubleVector xStart, double eps) {
        return gradientDescend(function, xStart, eps, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    public static DoubleVector gradientDescend(IFunctionND function, DoubleVector xStart) {
        return gradientDescend(function, xStart, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    public static DoubleVector conjGradientDescend(IFunctionND function, DoubleVector xStart, double eps, int maxIterations) {
        DoubleVector x_i = new DoubleVector(xStart);
        DoubleVector x_i_1 = new DoubleVector(xStart);
        DoubleVector s_i = DoubleVectorUtils.gradient(function, xStart, eps).mul(-1.0), s_i_1;
        double omega;
        int iteration = 0;
        for (; iteration != maxIterations; iteration++) {
            x_i_1 = DoubleVectorUtils.add(x_i, s_i);
            x_i_1 = biSect(function, x_i, x_i_1, eps, maxIterations);
            if (DoubleVectorUtils.distance(x_i_1, x_i) < 2 * eps) break;
            s_i_1 = DoubleVectorUtils.gradient(function, x_i_1, eps);
            omega = Math.pow((s_i_1).magnitude(), 2) / Math.pow((s_i).magnitude(), 2);
            s_i.mul(omega).sub(s_i_1);
            x_i = x_i_1;
        }

        if (NumericCommon.SHOW_DEBUG_LOG)
            System.out.printf("Conj gradient descend iterations number : %s\n", iteration + 1);
        return DoubleVectorUtils.add(x_i_1, x_i).mul(0.5);
    }

    public static DoubleVector conjGradientDescend(IFunctionND function, DoubleVector xStart, double eps) {
        return conjGradientDescend(function, xStart, eps, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    public static DoubleVector conjGradientDescend(IFunctionND function, DoubleVector xStart) {
        return conjGradientDescend(function, xStart, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    ////////////////////
    /// Lab. work #4 ///
    ////////////////////
    public static DoubleVector newtoneRaphson(IFunctionND function, DoubleVector xStart, double eps, int maxIterations) {
        DoubleVector x_i = new DoubleVector(xStart);
        DoubleVector x_i_1 = new DoubleVector(xStart);
        int iteration = -1;
        for (; iteration != maxIterations; iteration++) {
            DoubleMatrix invHessian = Objects.requireNonNull(DoubleMatrixUtils.invert(DoubleMatrixUtils.hessian(function, x_i, eps)));
            x_i_1 = DoubleVectorUtils.sub(x_i, DoubleMatrixUtils.mul(invHessian, DoubleVectorUtils.gradient(function, x_i, eps)));
            // Метод работает, но условие снизу не отрабатывает
            if (DoubleVectorUtils.distance(x_i_1, x_i) < 2 * eps) break;
            x_i = x_i_1;
        }

        if (NumericCommon.SHOW_DEBUG_LOG)
            System.out.printf("Newtone - Raphson iterations number : %s\n", iteration + 1);
        return DoubleVectorUtils.add(x_i_1, x_i).mul(0.5);
    }

    public static DoubleVector newtoneRaphson(IFunctionND function, DoubleVector xStart, double eps) {
        return newtoneRaphson(function, xStart, eps, NumericCommon.ITERATIONS_COUNT_LOW);
    }

    public static DoubleVector newtoneRaphson(IFunctionND function, DoubleVector xStart) {
        return newtoneRaphson(function, xStart, NumericCommon.NUMERIC_ACCURACY_MIDDLE, NumericCommon.ITERATIONS_COUNT_LOW);
    }
}

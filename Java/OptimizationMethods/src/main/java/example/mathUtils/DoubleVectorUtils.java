package example.mathUtils;

import example.functionalInterfaces.IFunctionND;

public class DoubleVectorUtils {
    /**
     * Эквивалент left + right, где left и right могут иметь типы double или DoubleVector
     */
    public static DoubleVector add(final DoubleVector left, double right) {
        return new DoubleVector(TemplateVector.combine(left, right, Double::sum));
    }

    public static DoubleVector add(double left, final DoubleVector right) {
        return new DoubleVector(TemplateVector.combine(left, right, Double::sum));
    }

    public static DoubleVector add(final DoubleVector left, final DoubleVector right) {
        if (left.size() != right.size()) throw new RuntimeException("Vectors sum :: this.Size()!= other.Size()");
        return new DoubleVector(TemplateVector.combine(left, right, Double::sum));
    }

    /**
     * Эквивалент left - right, где left и right могут иметь типы double или DoubleVector
     */
    public static DoubleVector sub(final DoubleVector left, double right) {
        return new DoubleVector(TemplateVector.combine(left, right, (l, r) -> l - r));
    }

    public static DoubleVector sub(double left, final DoubleVector right) {
        return new DoubleVector(TemplateVector.combine(left, right, (l, r) -> l - r));
    }

    public static DoubleVector sub(final DoubleVector left, final DoubleVector right) {
        if (left.size() != right.size()) throw new RuntimeException("Vectors sub :: this.Size()!= other.Size()");
        return new DoubleVector(TemplateVector.combine(left, right, (l, r) -> l - r));
    }

    /**
     * Эквивалент left * right, где left и right могут иметь типы double или DoubleVector
     */
    public static DoubleVector mul(final DoubleVector left, double right) {
        return new DoubleVector(TemplateVector.combine(left, right, (l, r) -> l * r));
    }

    public static DoubleVector mul(double left, final DoubleVector right) {
        return new DoubleVector(TemplateVector.combine(left, right, (l, r) -> l * r));
    }

    public static DoubleVector mul(final DoubleVector left, final DoubleVector right) {
        if (left.size() != right.size()) throw new RuntimeException("Vectors sub :: this.Size()!= other.Size()");
        return new DoubleVector(TemplateVector.combine(left, right, (l, r) -> l * r));
    }

    /**
     * Эквивалент left / right, где left и right могут иметь типы double или DoubleVector
     */
    public static DoubleVector div(final DoubleVector left, double right) {
        return new DoubleVector(TemplateVector.combine(left, right, (l, r) -> l / r));
    }

    public static DoubleVector div(double left, final DoubleVector right) {
        return new DoubleVector(TemplateVector.combine(left, right, (l, r) -> l / r));
    }

    public static DoubleVector div(final DoubleVector left, final DoubleVector right) {
        if (left.size() != right.size()) throw new RuntimeException("Vectors sub :: this.Size()!= other.Size()");
        return new DoubleVector(TemplateVector.combine(left, right, (l, r) -> l / r));
    }

    public static DoubleVector direction(final DoubleVector left, final DoubleVector right) {
        if (left.size() != right.size()) throw new RuntimeException("Vectors direction :: a.Size()!= b.Size()");
        return sub(right, left).normalize();
    }

    public static double dot(final DoubleVector left, final DoubleVector right) {
        return left.dot(right);
    }

    public static double partial(IFunctionND func, DoubleVector x, int index, double eps) {
        if (x.notInRange(index)) throw new RuntimeException("Partial derivative index out of bounds!");
        x.uncheckedSet(index, x.uncheckedGet(index) + eps);
        double f_r = func.call(x);
        x.uncheckedSet(index, x.uncheckedGet(index) - 2.0 * eps);
        double f_l = func.call(x);
        x.uncheckedSet(index, x.uncheckedGet(index) + eps);
        return (f_r - f_l) / eps * 0.5;
    }

    public static double partial(IFunctionND func, DoubleVector x, int index) {
        return partial(func, x, index, NumericCommon.NUMERIC_ACCURACY_MIDDLE);
    }

    public static double partial2(IFunctionND func, DoubleVector x, int index_1, int index_2, double eps) {
       return partial((arg)->partial(func, arg, index_1, eps), x, index_2, eps);
    }

    public static DoubleVector gradient(IFunctionND func, final DoubleVector x, double eps) {
        DoubleVector df = new DoubleVector(x.size());
        df.applyEnumerate((i, v) -> partial(func, x, i, eps));
        return df;
    }

    public static double partial2(IFunctionND func, final DoubleVector x, int index_1, int index_2) {
        return partial2(func, x, index_1, index_2, NumericCommon.NUMERIC_ACCURACY_MIDDLE);
    }

    public static DoubleVector gradient(IFunctionND func, final DoubleVector x) {
        return  gradient(func, x, NumericCommon.NUMERIC_ACCURACY_MIDDLE);
    }

    public static double distance(final DoubleVector lhs, final DoubleVector rhs) {
        if (lhs.size() != rhs.size()) throw new RuntimeException("Distance :: this.Size()!= other.Size()");
        return Math.sqrt(TemplateVector.reduce(DoubleVector.combine(lhs, rhs, (l, r) -> (l - r) * (l - r)), Double::sum, 0.0));
    }
}

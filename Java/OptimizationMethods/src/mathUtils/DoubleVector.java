package mathUtils;

import functionalInterfaces.IFunctionND;


public class DoubleVector extends TemplateVector<Double>{
    public DoubleVector(int cap, double fill_value) {
        super(cap);
        fill(i -> fill_value);
    }

    public DoubleVector(int cap) {
        this(cap, 0.0);
    }

    public DoubleVector(DoubleVector other) {
        super(other);
    }

    public DoubleVector(Double... args) {
        super(args);
    }

    public DoubleVector(Iterable<Double> other) {
        super(other);
    }

    protected DoubleVector(Slice slice, DoubleVector other) {
        super(slice, other);
    }

    public DoubleVector get(Slice slice)
    {
        return new DoubleVector(slice, this);
    }

    public double magnitude() {
        return Math.sqrt(reduce(this, (acc, val) -> acc + val * val, 0.0));
    }

    public DoubleVector normalize() {
        double inv_mag = 1.0 / magnitude();
        apply(v -> v * inv_mag);
        return this;
    }

    public DoubleVector normalized() {
        return new DoubleVector(this).normalize();
    }

    public double dot(DoubleVector other) {
        if (size() != other.size()) throw new RuntimeException("Dot product :: this.Size()!= other.Size()");
        return reduce(combine(this, other, (l, r) -> l * r), Double::sum, 0.0);
    }

    @Override
    public String toString() {
        if(NumericCommon.NUMBER_RATIONAL_FORMAT)
        {
            return String.format("{%s}", String.join("; ",
                    DoubleVector.map(this, v -> String.format("%8s",
                            NumericUtils.toRationalStr(v, false)))));
        }
        return String.format("{%s}", String.join("; ",
                DoubleVector.map(this, v -> String.format("%8s", String.format("%.4f", v)))));
    }

    /**
     * Эквивалент this += other, где other может иметь типы double или DoubleVector
     */
    public DoubleVector add(double other) {
        apply(v -> v + other);
        return this;
    }

    public DoubleVector add(DoubleVector other) {
        if (size() != other.size()) throw new RuntimeException("Vectors add :: this.Size()!= other.Size()");
        combine(other, Double::sum);
        return this;
    }

    /**
     * Эквивалент this -= other, где other может иметь типы double или DoubleVector
     */
    public DoubleVector sub(double other) {
        apply(v -> v - other);
        return this;
    }

    public DoubleVector sub(DoubleVector other) {
        if (size() != other.size()) throw new RuntimeException("Vectors sub :: this.Size()!= other.Size()");
        combine(other, (l, r) -> l - r);
        return this;
    }

    /**
     * Эквивалент this *= other, где other может иметь типы double или DoubleVector
     */
    public DoubleVector mul(double other) {
        apply(v -> v * other);
        return this;
    }

    public DoubleVector mul(DoubleVector other) {
        if (size() != other.size()) throw new RuntimeException("Vectors add :: this.Size()!= other.Size()");
        combine(other, (l, r) -> l * r);
        return this;
    }

    /**
     * Эквивалент this /= other, где other может иметь типы double или DoubleVector
     */
    public DoubleVector div(double other) {
        apply(v -> v / other);
        return this;
    }

    public DoubleVector div(DoubleVector other) {
        if (size() != other.size()) throw new RuntimeException("Vectors sub :: this.Size()!= other.Size()");
        combine(other, (l, r) -> l / r);
        return this;
    }

    /**
     * Эквивалент left + right, где left и right могут иметь типы double или DoubleVector
     */
    public static DoubleVector add(DoubleVector left, double right) {
        return new DoubleVector(combine(left, right, Double::sum));
    }

    public static DoubleVector add(double left, DoubleVector right) {
        return new DoubleVector(combine(left, right, Double::sum));
    }

    public static DoubleVector add(DoubleVector left, DoubleVector right) {
        if (left.size() != right.size()) throw new RuntimeException("Vectors sum :: this.Size()!= other.Size()");
        return new DoubleVector(combine(left, right, Double::sum));
    }

    /**
     * Эквивалент left - right, где left и right могут иметь типы double или DoubleVector
     */
    public static DoubleVector sub(DoubleVector left, double right) {
        return new DoubleVector(combine(left, right, (l, r) -> l - r));
    }

    public static DoubleVector sub(double left, DoubleVector right) {
        return new DoubleVector(combine(left, right, (l, r) -> l - r));
    }

    public static DoubleVector sub(DoubleVector left, DoubleVector right) {
        if (left.size() != right.size()) throw new RuntimeException("Vectors sub :: this.Size()!= other.Size()");
        return new DoubleVector(combine(left, right, (l, r) -> l - r));
    }

    /**
     * Эквивалент left * right, где left и right могут иметь типы double или DoubleVector
     */
    public static DoubleVector mul(DoubleVector left, double right) {
        return new DoubleVector(combine(left, right, (l, r) -> l * r));
    }

    public static DoubleVector mul(double left, DoubleVector right) {
        return new DoubleVector(combine(left, right, (l, r) -> l * r));
    }

    public static DoubleVector mul(DoubleVector left, DoubleVector right) {
        if (left.size() != right.size()) throw new RuntimeException("Vectors sub :: this.Size()!= other.Size()");
        return new DoubleVector(combine(left, right, (l, r) -> l * r));
    }

    /**
     * Эквивалент left / right, где left и right могут иметь типы double или DoubleVector
     */
    public static DoubleVector div(DoubleVector left, double right) {
        return new DoubleVector(combine(left, right, (l, r) -> l / r));
    }

    public static DoubleVector div(double left, DoubleVector right) {
        return new DoubleVector(combine(left, right, (l, r) -> l / r));
    }

    public static DoubleVector div(DoubleVector left, DoubleVector right) {
        if (left.size() != right.size()) throw new RuntimeException("Vectors sub :: this.Size()!= other.Size()");
        return new DoubleVector(combine(left, right, (l, r) -> l / r));
    }

    public static DoubleVector direction(DoubleVector left, DoubleVector right) {
        if (left.size() != right.size()) throw new RuntimeException("Vectors direction :: a.Size()!= b.Size()");
        return sub(right, left).normalize();
    }

    public static double dot(DoubleVector left, DoubleVector right) {
        return left.dot(right);
    }

    public static double partial(IFunctionND func, DoubleVector x, int index, double eps) {
        if (x.notInRange(index)) throw new RuntimeException("Partial derivative index out of bounds!");
        x.unchecked_set(index, x.unchecked_get(index) + eps);
        double f_r = func.call(x);
        x.unchecked_set(index, x.unchecked_get(index) - 2.0 * eps);
        double f_l = func.call(x);
        x.unchecked_set(index, x.unchecked_get(index) + eps);
        return (f_r - f_l) / eps * 0.5;
    }

    public static double partial(IFunctionND func, DoubleVector x, int index) {
        return partial(func, x, index, NumericCommon.NUMERIC_ACCURACY_MIDDLE);
    }

    public static double partial2(IFunctionND func, DoubleVector x, int index_1, int index_2, double eps) {
        if (x.notInRange(index_2)) throw new RuntimeException("Partial derivative index out of bounds!");

        x.unchecked_set(index_2, x.unchecked_get(index_2) - eps);
        double f_l = partial(func, x, index_1, eps);
        x.unchecked_set(index_2, x.unchecked_get(index_2) + 2.0 * eps);
        double f_r = partial(func, x, index_1, eps);
        x.unchecked_set(index_2, x.unchecked_get(index_2) - eps);
        return (f_r - f_l) / eps * 0.5;
    }

    public static DoubleVector gradient(IFunctionND func, DoubleVector x, double eps) {
        DoubleVector df = new DoubleVector(x.size());
        df.applyEnumerate((i, v) -> partial(func, x, i, eps));
        return df;
    }

    public static double partial2(IFunctionND func, DoubleVector x, int index_1, int index_2) {
        return partial2(func, x, index_1, index_2, NumericCommon.NUMERIC_ACCURACY_MIDDLE);
    }

    public static DoubleVector gradient(IFunctionND func, DoubleVector x) {
        return  gradient(func, x, NumericCommon.NUMERIC_ACCURACY_MIDDLE);
    }
}

package example.mathUtils;


@SuppressWarnings("all")
public final class DoubleVector extends TemplateVector<Double> {
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

    public DoubleVector get(Slice slice) {
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

    public double dot(final DoubleVector other) {
        if (size() != other.size()) throw new RuntimeException("Dot product :: this.Size()!= other.Size()");
        return reduce(combine(this, other, (l, r) -> l * r), Double::sum, 0.0);
    }

    @Override
    public String toString() {
        if (NumericCommon.NUMBER_RATIONAL_FORMAT) {
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

    public DoubleVector add(final DoubleVector other) {
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

    public DoubleVector sub(final DoubleVector other) {
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

    public DoubleVector mul(final DoubleVector other) {
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

    public DoubleVector div(final DoubleVector other) {
        if (size() != other.size()) throw new RuntimeException("Vectors sub :: this.Size()!= other.Size()");
        combine(other, (l, r) -> l / r);
        return this;
    }

}

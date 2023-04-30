public class DoubleVector extends TemplateVector<Double>
{
    public DoubleVector(int cap)
    {
        super(cap);
        for (int i =0; i < cap; i++) set(i, 0.0);
    }

    public DoubleVector(DoubleVector other)
    {
        super(other);
    }

    public DoubleVector(Double... args)
    {
        super(args);
    }

    public DoubleVector(Iterable<Double> other)
    {
        super(other);
    }

    @SuppressWarnings("all")
    public double magnitude()
    {
        double mag = 0.0;

        for (double element : this) mag += (element * element);

        return Math.sqrt(mag);
    }

    @SuppressWarnings("all")
    public DoubleVector normalize()
    {
        double inv_mag = 1.0 / magnitude();

        for (int i = 0; i < size(); i++) this.set(i, this.get(i) * inv_mag);

        return this;
    }

    @SuppressWarnings("all")
    public DoubleVector normalized()
    {
        DoubleVector copy = new DoubleVector(this);
        return copy.normalize();
    }

    @SuppressWarnings("all")
    public double dot(DoubleVector other)
    {
        if(size()!= other.size()) throw new RuntimeException("Dot product :: this.Size()!= other.Size()");

        double dot = 0.0;

        for(Pair<Double> p : zip(this, other)) dot += p.First * p.Second;

        return dot;
    }

    @Override
    public String toString()
    {
        StringBuilder sb = new StringBuilder();
        sb.append("{ ");
        for (double v: this) sb.append(String.format("%8s; ", String.format("%.4f", v)));
        sb.delete(sb.length() - 2, sb.length());
        sb.append("}");
        return sb.toString();
    }

    @SuppressWarnings("notNull")
    public DoubleVector add(DoubleVector other)
    {
        if(size()!= other.size()) throw new RuntimeException("Vectors add :: this.Size()!= other.Size()");

        for (int i = 0; i < other.size(); i++) set(i, get(i) + other.get(i));

        return this;
    }

    public DoubleVector add(double other)
    {
        for (int i = 0; i < size(); i++) set(i, get(i) + other);

        return this;
    }

    public DoubleVector sub(DoubleVector other)
    {
        if(size()!= other.size())  throw new RuntimeException("Vectors sub :: this.Size()!= other.Size()");

        for (int i = 0; i < other.size(); i++)  set(i, get(i) - other.get(i));

        return this;
    }

    public DoubleVector sub(double other)
    {
        for (int i = 0; i < size(); i++) set(i, get(i) - other);

        return this;
    }

    public DoubleVector mul(double other)
    {
        for (int i = 0; i < size(); i++) set(i, get(i) * other);

        return this;
    }

    public DoubleVector div(double other)
    {
        for (int i = 0; i < size(); i++)  set(i, get(i) / other);

        return this;
    }

    @SuppressWarnings("all")
    public static DoubleVector add(DoubleVector a, DoubleVector b)
    {
        if(a.size()!= b.size()) throw new RuntimeException("Vectors sum :: this.Size()!= other.Size()");
        DoubleVector  res = new DoubleVector(a);
        return  res.add(b);
    }

    @SuppressWarnings("all")
    public static DoubleVector add(DoubleVector a, double b)
    {
        DoubleVector res = new DoubleVector(a);
        return res.add(b);
    }

    @SuppressWarnings("all")
    public static DoubleVector add(double b,DoubleVector a)
    {
        return  add(a,b);
    }

    @SuppressWarnings("all")
    public static DoubleVector sub(DoubleVector a, DoubleVector b)
    {
        if(a.size()!= b.size()) throw new RuntimeException("Vectors sub :: this.Size()!= other.Size()");
        DoubleVector res = new DoubleVector(a);
        return res.sub(b);
    }

    @SuppressWarnings("all")
    public static DoubleVector sub(DoubleVector a, double b)
    {
        DoubleVector res = new DoubleVector(a);
        return res.sub(b);
    }

    @SuppressWarnings("all")
    public static DoubleVector sub(double b, DoubleVector vector)
    {
        DoubleVector res = new DoubleVector(vector.size());
        for(Double v: vector)res.pushBack(b - v);
        return  res;
    }

    @SuppressWarnings("all")
    public static DoubleVector mul(DoubleVector a,double b)
    {
        DoubleVector res = new DoubleVector(a);
        return  res.mul(b);
    }

    @SuppressWarnings("all")
    public static DoubleVector mul(double b,DoubleVector a)
    {
        return  mul(a,b);
    }

    @SuppressWarnings("all")
    public static DoubleVector div(DoubleVector a, double b)
    {
        DoubleVector res = new DoubleVector(a);
        return res.div(b);
    }

    @SuppressWarnings("all")
    public static DoubleVector div(double b, DoubleVector vector)
    {
        DoubleVector res = new DoubleVector(vector.size());
        for(Double v: vector)res.pushBack(b / v);
        return  res;
    }

    @SuppressWarnings("all")
    public static DoubleVector direction(DoubleVector a, DoubleVector b)
    {
        if (a.size() != b.size()) throw new RuntimeException("Vectors direction :: a.Size()!= b.Size()");
        return sub(b , a).normalize();
    }

    @SuppressWarnings("all")
    public static double dot(DoubleVector a, DoubleVector b)
    {
        return a.dot(b);
    }

    @SuppressWarnings("all")
    public static DoubleVector gradient(IFunctionND func, DoubleVector x, double eps)
    {
        DoubleVector df = new DoubleVector(x.size());
        for (int i = 0; i < x.size(); i++)  df.set(i, partial(func, x, i, eps));
        return df;
    }

    @SuppressWarnings("all")
    public static double partial (IFunctionND func, DoubleVector x, int index, double eps )
    {
        if (x.notInRange(index))  throw new RuntimeException("Partial derivative index out of bounds!");

        x.set(index,  x.get(index) + eps);
        double f_r = func.call(x);
        x.set(index,  x.get(index) - 2.0 * eps);
        double f_l = func.call(x);
        x.set(index,  x.get(index) +  eps);
        return (f_r - f_l) / eps * 0.5;
    }

    @SuppressWarnings("all")
    public static double partial2(IFunctionND func, DoubleVector x, int index_1, int index_2, double eps)
    {
        if (x.notInRange(index_2)) throw new RuntimeException("Partial derivative index out of bounds!");

        x.set(index_2,  x.get(index_2) - eps);
        double f_l = partial(func, x, index_1, eps);
        x.set(index_2,  x.get(index_2) + 2.0 * eps);
        double f_r = partial(func, x, index_1, eps);
        x.set(index_2,  x.get(index_2) - eps);
        return (f_r - f_l) / eps * 0.5;
    }

}

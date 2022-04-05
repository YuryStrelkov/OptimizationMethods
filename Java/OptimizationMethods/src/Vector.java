import java.util.ArrayList;
import java.util.Objects;

public class Vector
{
    private ArrayList<Double> data;

    private Vector(){}

    public Vector(int cap)
    {
        data = new ArrayList(cap);

        for(int i = 0; i < cap; i++)
        {
            data.add(0.0);
        }
    }

    private boolean isInRange(int index)
    {
        if (index < 0 || index >= data.size())
        {
            return  false;
        }
        return  true;
    }

    public Vector(Vector other)
    {
        data = new ArrayList(other.data);
    }

    public Vector(double[] other)
    {
        data = new ArrayList<>(other.length);

        for(int i = 0; i < other.length; i++)
        {
            data.add(other[i]);
        }
    }

    public double get(int index)
    {
        return data.get(index);
    }

    public void set(int index, double value)
    {
        data.set(index, value);
    }

    public  void pushBack(double value)
    {
        data.add(value);
    }

    public int size()
    {
        return  data.size();
    }

    public double magnitude()
    {
        double mag = 0.0;

        for (double element : data)
        {
            mag += (element * element);
        }
        return Math.sqrt(mag);
    }

    public Vector normalize()
    {
        double inv_mag = 1.0 / magnitude();

        for (int i = 0; i < size(); i++)
        {
            this.set(i,  this.get(i) * inv_mag);
        }
        return this;
    }

    public Vector normalized()
    {
        Vector copy =  new Vector(this);
        return copy.normalize();
    }

    public double dot(Vector other) throws Exception
    {
        if(size()!= other.size())
        {
            throw new Exception("Dot product :: this.Size()!= other.Size()");
        }
        double dot = 0.0;

        for (int i = 0; i < other.size(); i++)
        {
            dot += this.get(i) * other.get(i);
        }
        return dot;
    }

    @Override
    public String toString()
    {
        String s = "{ ";
        for (int i = 0; i < data.size() - 1; i++)
        {
           if(get(i) >= 0)
           {
               s += String.format("%-8s; ",  String.format("%.4f",get(i)));
               continue;
           }
            s += String.format("%-8s; ", String.format("%.4f",get(i)));
        }
        if(get(data.size() - 1) >= 0)
        {
            s += String.format("%-8s ", String.format("%.4f",get(data.size() - 1)));
            s += " }";
            return s;
        }
        s += String.format("%-8s ",String.format("%.4f",get(data.size() - 1)));
        s += " }";
        return s;
    }

    @Override
    public boolean equals(Object o)
    {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Vector vector = (Vector) o;
        return data.equals(vector.data);
    }

    @Override
    public int hashCode() {
        return Objects.hash(data);
    }

    public  Vector add(Vector other)throws Exception
    {
        if(size()!= other.size())
        {
            throw new Exception("Vectors add :: this.Size()!= other.Size()");
        }
        for (int i = 0; i < other.size(); i++)
        {
            set(i, get(i) + other.get(i));
        }
        return this;
    }

    public  Vector add(double other)
    {
        for (int i = 0; i < size(); i++)
        {
            set(i, get(i) + other);
        }
        return this;
    }

    public  Vector sub(Vector other)throws Exception
    {
        if(size()!= other.size())
        {
            throw new Exception("Vectors sub :: this.Size()!= other.Size()");
        }
        for (int i = 0; i < other.size(); i++)
        {
            set(i, get(i) - other.get(i));
        }
        return this;
    }

    public  Vector sub(double other)
    {
        for (int i = 0; i < size(); i++)
        {
            set(i, get(i) - other);
        }
        return this;
    }

    public  Vector mul(double other)
    {
        for (int i = 0; i < size(); i++)
        {
            set(i, get(i) * other);
        }
        return this;
    }

    public  Vector div(double other)
    {
        for (int i = 0; i < size(); i++)
        {
            set(i, get(i) / other);
        }
        return this;
    }

    public static Vector add(Vector a, Vector b)throws Exception
    {
        if(a.size()!= b.size())
        {
            throw new Exception("Vectors sum :: this.Size()!= other.Size()");
        }
        Vector  res = new Vector(a);
        return  res.add(b);
    }

    public static Vector add(Vector a, double b)
    {
        Vector res = new Vector(a);
        return res.add(b);
    }

    public static Vector add(double b,Vector a)
    {
        return  add(a,b);
    }

    public static Vector sub(Vector a, Vector b)throws Exception
    {
        if(a.size()!= b.size())
        {
            throw new Exception("Vectors sub :: this.Size()!= other.Size()");
        }
        Vector res = new Vector(a);
        return res.sub(b);
    }

    public static Vector sub(Vector a, double b)
    {
        Vector res = new Vector(a);
        return res.sub(b);
    }

    public static Vector sub(double b, Vector a)
    {
        Vector res = new Vector(a);

        for(int i = 0; i < a.size(); i++)
        {
            res.set(i, b - a.get(i));
        }
        return  res;
    }

    public static Vector mul(Vector a,double b)
    {
        Vector res = new Vector(a);
        return  res.mul(b);
    }

    public static Vector mul(double b,Vector a)
    {
        return  mul(a,b);
    }

    public static Vector div(Vector a, double b)
    {
        Vector res = new Vector(a);
        return res.div(b);
    }

    public static Vector div(double b, Vector a)
    {
        Vector res = new Vector(a);

        for(int i = 0; i < a.size(); i++)
        {
            res.set(i, b / a.get(i));
        }
        return  res;
    }

    public static Vector direction(Vector a, Vector b)throws Exception
    {
        if (a.size() != b.size())
        {
            throw new Exception("Vectors direction :: a.Size()!= b.Size()");
        }
        return sub(b , a).normalize();
    }

    public static double dot(Vector a, Vector b)throws Exception
    {
        return a.dot(b);
    }

    public static Vector gradient(IFunctionND func, Vector x, double eps)
    {
        Vector x_l = new Vector(x);
        Vector x_r = new Vector(x);
        Vector df = new Vector(x.size());
        for (int i = 0; i < x.size(); i++)
        {
            x_l.set(i, x_l.get(i) - eps);//   [i] -= eps;
            x_r.set(i, x_r.get(i) + eps);

            df.set(i,(func.execute(x_r) - func.execute(x_l)) * (0.5 / eps));

            x_l.set(i, x_l.get(i) + eps);//   [i] -= eps;
            x_r.set(i, x_r.get(i) - eps);
        }
        return df;
    }

    public static double partial (IFunctionND func, Vector x, int index, double eps )throws Exception
    {
        if (!x.isInRange(index))
        {
            throw new Exception("Partial derivative index out of bounds!");
        }
        x.set(index,  x.get(index) + eps);//[index] += eps;
        double f_r = func.execute(x);
        x.set(index,  x.get(index) - 2.0 * eps);
        double f_l = func.execute(x);
        x.set(index,  x.get(index) +  eps);
        return (f_r - f_l) / eps * 0.5;
    }

    public static double partial2(IFunctionND func, Vector x, int index_1, int index_2, double eps)throws Exception
    {
        if (!x.isInRange(index_2))
        {
            throw new Exception("Partial derivative index out of bounds!");
        }
        x.set(index_2,  x.get(index_2) - eps);
        double f_l = partial(func, x, index_1, eps);
        x.set(index_2,  x.get(index_2) + 2.0 * eps);
        double f_r = partial(func, x, index_1, eps);
        x.set(index_2,  x.get(index_2) - eps);
        return (f_r - f_l) / eps * 0.5;
    }

}

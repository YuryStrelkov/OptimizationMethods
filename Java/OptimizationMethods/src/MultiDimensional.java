public class MultiDimensional {

    public static final IFunctionND testFunc2d = new IFunctionND() {
        @Override
        public double execute(Vector arg) {
            return _testFunc2D(arg);
        }
    };
    public static final IFunctionND testFuncNd = new IFunctionND() {
        @Override
        public double execute(Vector arg) {
            return _testFuncND(arg);
        }
    };

    public static  boolean ShowDebugLog = false;

    public static  boolean ShowZeroOrderMethodsDebugLog = false;

    private static double _testFunc2D(Vector x)
    {
        return (x.get(0) - 5) * x.get(0) + (x.get(1) - 3) * x.get(1); // min at point x = 2.5, y = 1.5
    }
    private static double _testFuncND(Vector x)
    {
        double val = 0.0;

        for (int i=0; i < x.size();i++)
        {
            val += (x.get(i) - i) * x.get(i);
        }

        return val; // min at point x_i = i/2, i from 0 to x.Size-1
    }
    ////////////////////
    /// Lab. work #2 ///
    ////////////////////
    public static Vector dihotomia      (IFunctionND f, Vector x0, Vector x1, double eps, int max_iters)
    {
        Vector x_0  = new Vector(x0), x_1 = new Vector(x1), x_c, dir;

        dir = Vector.direction(x_0, x_1).mul(eps);

        int cntr = 0;

        for (; cntr != max_iters; cntr++)
        {
            if (Vector.sub(x_1 , x_0).magnitude() < eps)
            {
                break;
            }
            x_c = Vector.add(x_1 , x_0).mul(0.5);

            if (f.execute(Vector.add(x_c, dir)) > f.execute(Vector.sub(x_c, dir)))
            {
                x_1 = x_c;
                continue;
            }
            x_0 =  x_c;
        }

        if(ShowZeroOrderMethodsDebugLog)System.out.println("Dihotomia iterations number : " + cntr);

        return  Vector.add(x_1 , x_0).mul(0.5);
    }
    public static Vector dihotomia      (IFunctionND f, Vector x_0, Vector x_1, double eps)
    {
      return  dihotomia(f,  x_0,  x_1,  eps,1000);
    }
    public static Vector dihotomia      (IFunctionND f, Vector x_0, Vector x_1)
    {
        return  dihotomia(f,  x_0,  x_1,  1e-5,1000);
    }
    public static Vector goldenRatio    (IFunctionND f, Vector x_0, Vector x_1, double eps, int max_iters)
    {
        Vector a = new Vector(x_0);

        Vector b = new Vector(x_1);

        Vector dx;

        int cntr = 0;

        double one_div_phi = 1.0 / OneDimensional.Phi;

        for (; cntr != max_iters; cntr++)
        {
            if (Vector.sub(x_1 , x_0).magnitude() < eps)
            {
                break;
            }

            dx  = Vector.sub(b, a).mul(one_div_phi);
            x_0 = Vector.sub(b, dx);
            x_1 = Vector.add(a, dx);

            if (f.execute(x_0) >= f.execute(x_1))
            {
                a = x_0;
                continue;
            }
            b = x_1;
        }

        if(ShowZeroOrderMethodsDebugLog)System.out.println("golden ratio iterations number : " + cntr);

        return  Vector.add(x_1 , x_0).mul(0.5);
    }
    public static Vector goldenRatio    (IFunctionND f, Vector x_0, Vector x_1, double eps)
    {
      return goldenRatio(f, x_0,  x_1, eps, 1000);
    }
    public static Vector goldenRatio    (IFunctionND f, Vector x_0, Vector x_1)
    {
        return goldenRatio(f, x_0,  x_1, 1e-5, 1000);
    }
    public static Vector fibonacci      (IFunctionND f, Vector x_0, Vector x_1, double eps)
    {
        int max_iters = OneDimensional.closestFibonacci(Vector.sub(x_1, x_0).magnitude() / eps);

        int cntr = max_iters - 1;

        int[] f_n_s = OneDimensional.fibonacchiNumbers(max_iters);

        Vector a = new Vector(x_0);

        Vector b = new Vector(x_1);

        Vector dx;

        for (; cntr >= 2; cntr--)
        {
            if (Vector.sub(x_1 , x_0).magnitude() < eps)
            {
                break;
            }
            dx  = Vector.sub(b , a);
            x_0 = Vector.add(a, Vector.mul(dx,((double)f_n_s[cntr - 2] / f_n_s[cntr])));
            x_1 = Vector.add(a, Vector.mul(dx,((double)f_n_s[cntr -1]  / f_n_s[cntr])));

            if (f.execute(x_0) < f.execute(x_1))
            {
                b = x_1;
                continue;
            }
            a = x_0;
        }

        if(ShowZeroOrderMethodsDebugLog)System.out.println("fibonacci iterations number : "+max_iters);

        return  Vector.add(x_1 , x_0).mul(0.5);
    }
    public static Vector fibonacci      (IFunctionND f, Vector x_0, Vector x_1)
    {
        return  fibonacci( f,  x_0,  x_1, 1e-5);
    }
    public static Vector perCoordDescend(IFunctionND f, Vector x_start, double eps, int max_iters)
    {
        Vector x_0 = new Vector(x_start);

        Vector x_1 = new Vector(x_start);

        double step = 1.0;

        double x_i, y_1, y_0;

        int opt_coord_n = 0, coord_id;

        int i = 0;

        for (i = 0; i < max_iters; i++)
        {
            coord_id = i % x_0.size();

            x_1.set(coord_id, x_1.get(coord_id) - eps);

            y_0 = f.execute(x_1);

            x_1.set(coord_id, x_1.get(coord_id) + 2 * eps);

            y_1 = f.execute(x_1);

            x_1.set(coord_id, x_1.get(coord_id) - eps);

            x_1.set(coord_id,y_0 > y_1 ? x_1.get(coord_id) + step : x_1.get(coord_id) - step);

            x_i = x_0.get(coord_id);

            x_1 = dihotomia(f, x_0, x_1, eps, max_iters);

            x_0 = new Vector(x_1);

            if (Math.abs(x_1.get(coord_id) - x_i) < eps)
            {
                opt_coord_n++;

                if (opt_coord_n == x_1.size())
                {
                    if(ShowDebugLog)System.out.println("per coord descend iterations number : "+i);

                    return x_0;
                }
                continue;
            }
            opt_coord_n = 0;
        }

        if(ShowDebugLog)System.out.println("per coord descend iterations number : "+max_iters);

        return x_0;
    }
    public static Vector perCoordDescend(IFunctionND f, Vector x_start, double eps)
    {
        return     perCoordDescend( f,  x_start,  eps,1000);
    }
    public static Vector perCoordDescend(IFunctionND f, Vector x_start)
    {
        return     perCoordDescend( f,  x_start,  1e-5,1000);
    }
    ////////////////////
    /// Lab. work #3 ///
    ////////////////////
    public static Vector gradientDescend(IFunctionND f, Vector x_start, double eps, int max_iters)
    {
        Vector x_i = new Vector(x_start);

        Vector x_i_1 = new Vector(x_start); ;

        int cntr = 0;

        while (true)
        {
            cntr++;

            if (cntr == max_iters)
            {
                break;
            }

            x_i_1 = Vector.sub(x_i , Vector.gradient(f, x_i, eps));

            x_i_1 = dihotomia(f, x_i, x_i_1, eps, max_iters);

            if (Vector.sub(x_i_1, x_i).magnitude() < eps)
            {
                break;
            }

            x_i = x_i_1;
        }

        if(ShowDebugLog)System.out.println("gradient descend iterations number : " + cntr);

        return Vector.add(x_i_1, x_i).mul(0.5);
    }
    public static Vector gradientDescend(IFunctionND f, Vector x_start, double eps)
    {
        return gradientDescend( f,  x_start,  eps, 1000);
    }
    public static Vector gradientDescend(IFunctionND f, Vector x_start)
    {
        return gradientDescend( f,  x_start,  1e-5, 1000);
    }
    public static Vector conjGradientDescend(IFunctionND f, Vector x_start, double eps , int max_iters)
    {
        Vector x_i = new Vector(x_start);

        Vector x_i_1 = new Vector(x_start);

        Vector s_i = Vector.gradient(f, x_start, eps).mul(-1.0), s_i_1;

        double omega;

        int cntr = 0;

        while (true)
        {
            cntr++;

            if (cntr == max_iters)
            {
                break;
            }

            x_i_1 = Vector.add(x_i, s_i);

            x_i_1 = dihotomia(f, x_i, x_i_1, eps, max_iters);

            if (Vector.sub(x_i_1, x_i).magnitude() < eps)
            {
                break;
            }

            s_i_1 = Vector.gradient(f, x_i_1, eps);

            omega = Math.pow((s_i_1).magnitude(), 2) / Math.pow((s_i).magnitude(), 2);

            s_i.mul(omega).sub(s_i_1);

            x_i = x_i_1;
        }

        if(ShowDebugLog)System.out.println("Conj gradient descend iterations number : "+cntr);

        return Vector.add(x_i_1, x_i).mul(0.5);
    }
    public static Vector conjGradientDescend(IFunctionND f, Vector x_start, double eps )
    {
         return conjGradientDescend( f,  x_start,  eps, 1000);
    }
    public static Vector conjGradientDescend(IFunctionND f, Vector x_start)
    {
        return conjGradientDescend( f,  x_start,  1e-5, 1000);
    }

    ////////////////////
    /// Lab. work #4 ///
    ////////////////////
    public static Vector newtoneRaphson(IFunctionND f, Vector x_start, double eps, int max_iters)
    {
        Vector x_i = new Vector(x_start);

        Vector x_i_1 = new Vector(x_start);

        int cntr = -1;

        while (true)
        {
            cntr++;

            if (cntr == max_iters)
            {
                break;
            }

            x_i_1 = Vector.sub(x_i, Matrix.mul(Matrix.invert(Matrix.hessian(f, x_i, eps)),Vector.gradient(f, x_i, eps))) ;

            ///Метод работает, но условие снизу не отрабатывает
            if (Vector.sub(x_i_1, x_i).magnitude() < eps)
            {
                break;
            }
            x_i = x_i_1;
        }

        if(ShowDebugLog)System.out.println("Newtone - Raphson iterations number : " + cntr);

        return Vector.add(x_i_1 ,x_i).mul(0.5);
    }
    public static Vector newtoneRaphson(IFunctionND f, Vector x_start, double eps)
    {
        return newtoneRaphson(f, x_start,  eps, 100);
    }
    public static Vector newtoneRaphson(IFunctionND f, Vector x_start)
    {
        return newtoneRaphson(f, x_start,  1e-6, 100);
    }
}

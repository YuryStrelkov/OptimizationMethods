using System;

namespace OptimizationMethods
{
    class Program
    {
        static double test_f(double arg)
        {
            return arg * (arg - 5);
        }
        static double test_n(Vector args)
        {
            return (args[0] - 2) *(args[0] - 2) + (args[1] - 2) * (args[1] - 2);
        }
        static double test_3(Vector args)
        {
            return  (args[0] - 2) * (args[0] - 2) + (args[1] - 2) * (args[1] - 2) + (args[2] - 3) * (args[2] - 3);
        }
        static void Main(string[] args)
        {
            OneDimensional.OneDimensionalMethodsTest(test_f);
            MultiDimensional.MultiDimensionalMethodsTest(test_n);
            Matrix.MatrixTest();
            Matrix m = Matrix.Hessian(test_3, new double[] { 0, 0, 0 });
            Console.WriteLine(m.ToString());
        }
    }
}

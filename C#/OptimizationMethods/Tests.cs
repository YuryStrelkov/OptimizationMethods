using System;
using System.Collections.Generic;
using System.Text;

namespace OptimizationMethods
{
   public static class Tests
    {
        public static void OneDimensionalMethodsTest()
        {
            const double x_0 = 10;
            const double x_1 = -1;
            Console.WriteLine("x_0 = " + x_0 + ", x_1 = " + x_1 + "\n");
            Console.WriteLine("Dihotomia   : " + OneDimensional.Dihotomia(OneDimensional.TestFunc, x_0, x_1, 1e-3f));
            Console.WriteLine("GoldenRatio : " + OneDimensional.GoldenRatio(OneDimensional.TestFunc, x_0, x_1, 1e-3f));
            Console.WriteLine("Fibonacchi  : " + OneDimensional.Fibonacci(OneDimensional.TestFunc, x_0, x_1, 1e-3f) + "\n");
        }
        public static void MultiDimensionalMethodsTest()
        {
            Vector x_1 = new double[] { 0, 0 };
            Vector x_0 = new double[] { 5, 3 };
            Console.WriteLine("x_0 = " + x_0 + ", x_1 = " + x_1 + "\n");
            Console.WriteLine("Dihotomia              : " + MultiDimensional.Dihotomia(MultiDimensional.TestFunc2D, x_1, x_0).ToString());
            Console.WriteLine("GoldenRatio            : " + MultiDimensional.GoldenRatio(MultiDimensional.TestFunc2D, x_1, x_0).ToString());
            Console.WriteLine("Fibonacci              : " + MultiDimensional.Fibonacci(MultiDimensional.TestFunc2D, x_1, x_0).ToString());
            Console.WriteLine("PerCoordDescend        : " + MultiDimensional.PerCoordDescend(MultiDimensional.TestFunc2D, x_1).ToString());
            Console.WriteLine("GradientDescend        : " + MultiDimensional.GradientDescend(MultiDimensional.TestFunc2D, x_1).ToString());
            Console.WriteLine("СonjGradientDescend    : " + MultiDimensional.СonjGradientDescend(MultiDimensional.TestFunc2D, x_1).ToString());
            Console.WriteLine("NewtoneRaphson         : " + MultiDimensional.NewtoneRaphson(MultiDimensional.TestFunc2D, x_1).ToString());
        }
        /// <summary>
        /// проверка работоспособности класса
        /// </summary>
        public static void MatrixTest()
        {
            ///У меня всё работает, у вас мой класс тоже заработает
            Matrix matrix = new Vector[] { new double[] { 8, 1, 6 }, new double[] { 3, 5, 7 }, new double[] { 4, 9, 2 } };
            Console.WriteLine(matrix);
            Console.WriteLine("\nmatrix + matrix:\n" + (matrix + matrix));
            Console.WriteLine("\nmatrix - matrix:\n" + (matrix - matrix));
            Console.WriteLine("\nmatrix * matrix:\n" + (matrix * matrix));
            Matrix l, u;
            Matrix.LU(ref matrix, out l, out u);
            Console.WriteLine("\nL  matrix:\n" + l);
            Console.WriteLine("\nU  matrix:\n" + u);
            Console.WriteLine("\nL * U - matrix:\n" + (l * u - matrix));
            Vector b = new double[] { 1, 2, 3 };
            /// x = {0.05,0.3,0.05};
            Vector x = Matrix.Linsolve(matrix, b);
            Console.WriteLine("\nx vector:\n" + x);
            Console.WriteLine("\nAx - b:\n" + (matrix * x - b));
            Console.WriteLine("\nA*inv(A):\n" + matrix * Matrix.Invert(matrix));
            Matrix matrix_ = new Vector[] { new double[] { 8, 1, 6 }, new double[] { 3, 5, 7 } };
            Console.WriteLine("\nnon rect mat:\n" + matrix_);
            Console.WriteLine("\nnon rect mat mul by transposed it self :\n" + matrix_ * Matrix.Transpose(matrix_));
            Matrix m = Matrix.Hessian(MultiDimensional.TestFuncND, new double[] { 0, 0, 0 });
            Console.WriteLine(m.ToString());
        }

        public static void NumericTests()
        {
            int rational;
            int nominator;
            int denominator;
            NumericUtils.DecimalToRational(1.6666,out rational, out nominator, out denominator);
            Console.WriteLine($"{rational} {nominator} / {denominator}\n");
            NumericUtils.DecimalToRational(0.6666, out rational, out nominator, out denominator);
            Console.WriteLine($"{rational} {nominator} / {denominator}\n");
            NumericUtils.DecimalToRational(-3, out rational, out nominator, out denominator);
            Console.WriteLine($"{rational} {nominator} / {denominator}\n");
            NumericUtils.DecimalToRational(-0.125, out rational, out nominator, out denominator);
            Console.WriteLine($"{rational} {nominator} / {denominator}\n");
            NumericUtils.DecimalToRational(3.769230769230769230769, out rational, out nominator, out denominator);
            Console.WriteLine($"{rational} {nominator} / {denominator}\n");
        }


        public static void SympexTest() 
        {
            Matrix A = new Vector[]
            {
              new double[]{-2, 6},
              new double[]{ 3, 2},
              new double[]{ 2,-1}
            };
            Vector b = new double[] { 40, 28, 14 };
            Vector c = new double[] { 2, 3 };
            ///write_symplex(A, b, c);
/*            Matrix table = null;
            List<int> basis = Symplex.BuildSymplexTable(out table, A, c, b, new List<Sign>(){ Sign.Less, Sign.Less, Sign.Less });
            Console.WriteLine(Symplex.SymplexToString(table, basis));*/
            
            Symplex.SymplexSolve(A, c, b, new List<Sign>() { Sign.Less, Sign.Less, Sign.Less });
        }

        public static void TestAll() 
        {
            OneDimensionalMethodsTest();
            MultiDimensionalMethodsTest();
           // MatrixTest();
            SympexTest();
            NumericTests();
        }
    }
}

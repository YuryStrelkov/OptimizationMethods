﻿using System;
using System.Collections.Generic;
using System.Text;

namespace OptimizationMethods
{
   public static class Tests
    {
        public static void OneDimensionalMethodsTest()
        {
            Console.WriteLine("\n///////////////////////////////////////////");
            Console.WriteLine(  "//////// OneDimensionalMethodsTest ////////");
            Console.WriteLine(  "///////////////////////////////////////////\n");

            const double x_0 = 10;
            const double x_1 = -1;
            Console.WriteLine($"x_0 = {x_0},\nx_1 = {x_1}\n");
            Console.WriteLine($"Dihotomia   : {OneDimensional.Dihotomia  (OneDimensional.TestFunc, x_0, x_1, 1e-4)}");
            Console.WriteLine($"GoldenRatio : {OneDimensional.GoldenRatio(OneDimensional.TestFunc, x_0, x_1, 1e-4)}");
            Console.WriteLine($"Fibonacchi  : {OneDimensional.Fibonacci  (OneDimensional.TestFunc, x_0, x_1, 1e-4)}\n");
        }
        public static void MultiDimensionalMethodsTest()
        {
            Console.WriteLine("\n/////////////////////////////////////////////");
            Console.WriteLine(  "//////// MultiDimensionalMethodsTest ////////");
            Console.WriteLine(  "/////////////////////////////////////////////\n");

            Vector x_1 = new double[] { 0, 0 };
            Vector x_0 = new double[] { 5, 3 };

            Console.WriteLine($"x_0 = {x_0},\nx_1 = {x_1}\n") ;
            Console.WriteLine($"Dihotomia              : {MultiDimensional.Dihotomia          (MultiDimensional.TestFunc2D, x_1, x_0)}");
            Console.WriteLine($"GoldenRatio            : {MultiDimensional.GoldenRatio        (MultiDimensional.TestFunc2D, x_1, x_0)}");
            Console.WriteLine($"Fibonacci              : {MultiDimensional.Fibonacci          (MultiDimensional.TestFunc2D, x_1, x_0)}");
            Console.WriteLine($"PerCoordDescend        : {MultiDimensional.PerCoordDescend    (MultiDimensional.TestFunc2D, x_1)}");
            Console.WriteLine($"GradientDescend        : {MultiDimensional.GradientDescend    (MultiDimensional.TestFunc2D, x_1)}");
            Console.WriteLine($"СonjGradientDescend    : {MultiDimensional.СonjGradientDescend(MultiDimensional.TestFunc2D, x_1)}");
            Console.WriteLine($"NewtoneRaphson         : {MultiDimensional.NewtoneRaphson     (MultiDimensional.TestFunc2D, x_1)}\n");
        }
        /// <summary>
        /// проверка работоспособности класса
        /// </summary>
        public static void MatrixTest()
        {
            Console.WriteLine("\n////////////////////////////");
            Console.WriteLine(  "//////// MatrixTest ////////");
            Console.WriteLine(  "////////////////////////////\n");

            ///У меня всё работает, у вас мой класс тоже заработает
            Matrix matrix = new Vector[] { new double[] { 8, 1, 6 }, new double[] { 3, 5, 7 }, new double[] { 4, 9, 2 } };
            Console.WriteLine(matrix);
            Console.WriteLine($"\nmatrix + matrix:\n{ matrix + matrix }\n");
            Console.WriteLine($"\nmatrix - matrix:\n{ matrix - matrix }\n");
            Console.WriteLine($"\nmatrix * matrix:\n{ matrix * matrix }\n");
            Matrix l, u;
            Matrix.LU(ref matrix, out l, out u);
            Console.WriteLine($"\nL  matrix:\n{l}");
            Console.WriteLine($"\nU  matrix:\n{u}");
            Console.WriteLine($"\nL * U - matrix:\n{l * u - matrix}");
            Vector b = new double[] { 1, 2, 3 };
            /// x = {0.05,0.3,0.05};
            Vector x = Matrix.Linsolve(matrix, b);
            Console.WriteLine($"\nx vector:\n{x}");
            Console.WriteLine($"\nAx - b:\n{matrix * x - b}");
            Console.WriteLine($"\nA*inv(A):\n{matrix * Matrix.Invert(matrix)}");
            Matrix matrix_ = new Vector[] { new double[] { 8, 1, 6 }, new double[] { 3, 5, 7 } };
            Console.WriteLine($"\nnon rect mat:\n{matrix_}");
            Console.WriteLine($"\nnon rect mat mul by transposed it self :\n{matrix_ * Matrix.Transpose(matrix_)}");
            Matrix m = Matrix.Hessian(MultiDimensional.TestFuncND, new double[] { 0, 0, 0 });
            Console.WriteLine($"\nHessian(MultiDimensional.TestFuncND):\n{m}");
        }

        public static void NumericTests()
        {
            Console.WriteLine("\n//////////////////////////////");
            Console.WriteLine(  "//////// NumericTests ////////");
            Console.WriteLine(  "//////////////////////////////\n");

            int rational;
            int numerator;
            int denominator;
            NumericUtils.DecimalToRational(1.6666,out rational, out numerator, out denominator);
            Console.WriteLine($"{rational} {numerator} / {denominator}\n");
            NumericUtils.DecimalToRational(0.6666, out rational, out numerator, out denominator);
            Console.WriteLine($"{rational} {numerator} / {denominator}\n");
            NumericUtils.DecimalToRational(-3, out rational, out numerator, out denominator);
            Console.WriteLine($"{rational} {numerator} / {denominator}\n");
            NumericUtils.DecimalToRational(-0.125, out rational, out numerator, out denominator);
            Console.WriteLine($"{rational} {numerator} / {denominator}\n");
            NumericUtils.DecimalToRational(3.769230769230769230769, out rational, out numerator, out denominator);
            Console.WriteLine($"{rational} {numerator} / {denominator}\n");
        }

        public static void SympexTest() 
        {
            Console.WriteLine("\n/////////////////////////////");
            Console.WriteLine(  "//////// SymplexTest ////////");
            Console.WriteLine(  "/////////////////////////////\n");

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
            //OneDimensionalMethodsTest();
            // MultiDimensionalMethodsTest();
            //MatrixTest();
            SympexTest();
            NumericTests();
        }
    }
}

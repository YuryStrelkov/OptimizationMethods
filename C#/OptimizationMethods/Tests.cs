﻿using System.Collections.Generic;
using System;
using MathUtils;
using static MathUtils.NumericUtils;

namespace OptimizationMethods
{
   public static class Tests
    {
        public static void VectorTest() 
        {
            DoubleVector b = new DoubleVector(5.0, 6.0, 8.0);
            Console.Write("///////////////////////////////////////////\n");
            Console.Write("////////         VectorTest        ////////\n");
            Console.Write("///////////////////////////////////////////\n");
            Console.Write($"{b} + {b} = {b + b}\n");
            Console.Write($"{b} - {b} = {b - b}\n");
            Console.Write($"{b} / {b} = {b / b}\n");
            Console.Write($"{b} * {b} = {b * b}\n");
            Console.Write($"{2} + {b} = {b + 2}\n");
            Console.Write($"{b} + {2} = {2 + b}\n");
            Console.Write($"{2} - {b} = {2 - b}\n");
            Console.Write($"{b} - {2} = {b - 2}\n");
            Console.Write($"{2} * {b} = {b * 2}\n");
            Console.Write($"{b} * {2} = {2 * b}\n");
            Console.Write($"{2} / {b} = {NumericUtils.ToRationalStr(2 / b, false)}\n");
            Console.Write($"{b} / {2} = {NumericUtils.ToRationalStr(b / 2, false)}\n");
        }
        public static void OneDimensionalMethodsTest()
        {
            Console.WriteLine("\n///////////////////////////////////////////");
            Console.WriteLine(  "//////// OneDimensionalMethodsTest ////////");
            Console.WriteLine(  "///////////////////////////////////////////\n");

            const double x_0 = 10.0;
            const double x_1 = -1.0;
            Console.WriteLine($"Search range: x_0 = {x_0}, x_1 = {x_1}");
            Console.WriteLine($"BiSect      : {OneDimensional.BiSect     (NumericUtils.TestFunc1D, x_0, x_1, 1e-4)}");
            Console.WriteLine($"GoldenRatio : {OneDimensional.GoldenRatio(NumericUtils.TestFunc1D, x_0, x_1, 1e-4)}");
            Console.WriteLine($"Fibonacchi  : {OneDimensional.Fibonacci  (NumericUtils.TestFunc1D, x_0, x_1, 1e-4)}\n");
        }
        public static void MultiDimensionalMethodsTest()
        {
            Console.WriteLine("\n/////////////////////////////////////////////");
            Console.WriteLine(  "//////// MultiDimensionalMethodsTest ////////");
            Console.WriteLine(  "/////////////////////////////////////////////\n");

            DoubleVector x_1 = new DoubleVector(0.0, 0.0);
            DoubleVector x_0 = new DoubleVector(5.0, 3.0);

            Console.WriteLine($"x_0 = {x_0},\nx_1 = {x_1}\n") ;
            Console.WriteLine($"BiSect                 : {MultiDimensional.BiSect             (NumericUtils.TestFunc2D, x_1, x_0)}");
            Console.WriteLine($"GoldenRatio            : {MultiDimensional.GoldenRatio        (NumericUtils.TestFunc2D, x_1, x_0)}");
            Console.WriteLine($"Fibonacci              : {MultiDimensional.Fibonacci          (NumericUtils.TestFunc2D, x_1, x_0)}");
            Console.WriteLine($"PerCoordDescend        : {MultiDimensional.PerCoordDescend    (NumericUtils.TestFunc2D, x_1)}");
            Console.WriteLine($"GradientDescend        : {MultiDimensional.GradientDescend    (NumericUtils.TestFunc2D, x_1)}");
            Console.WriteLine($"СonjGradientDescend    : {MultiDimensional.СonjGradientDescend(NumericUtils.TestFunc2D, x_1)}");
            Console.WriteLine($"NewtoneRaphson         : {MultiDimensional.NewtoneRaphson     (NumericUtils.TestFunc2D, x_1)}\n");
        }
        /// <summary>
        /// проверка работоспособности класса
        /// </summary>
        /// 
        public static void MatrixTest()
        {

            Console.WriteLine("\n////////////////////////////");
            Console.WriteLine(  "//////// MatrixTest ////////");
            Console.WriteLine(  "////////////////////////////\n");

            ///У меня всё работает, у вас мой класс тоже заработает
            DoubleMatrix DoubleMatrix = new DoubleMatrix(new DoubleVector(8.0, 1.0, 6.0), new DoubleVector(3.0, 5.0, 7.0), new DoubleVector(4.0, 9.0, 2.0));
            Console.WriteLine(DoubleMatrix);
            Console.WriteLine($"\nmatrix + DoubleMatrix:\n{ DoubleMatrix + DoubleMatrix }\n");
            Console.WriteLine($"\nmatrix - DoubleMatrix:\n{ DoubleMatrix - DoubleMatrix }\n");
            Console.WriteLine($"\nmatrix * DoubleMatrix:\n{ DoubleMatrix * DoubleMatrix }\n");
            DoubleMatrix l, u;
            DoubleMatrix.LU(ref DoubleMatrix, out l, out u);
            Console.WriteLine($"\nL  DoubleMatrix:\n{l}");
            Console.WriteLine($"\nU  DoubleMatrix:\n{u}");
            Console.WriteLine($"\nL * U - DoubleMatrix:\n{l * u - DoubleMatrix}");
            DoubleVector b = new DoubleVector(1.0, 2.0, 3.0);
            /// x = {0.05, 0.3, 0.05};
            DoubleVector x = DoubleMatrix.Linsolve(DoubleMatrix, b);
            Console.WriteLine($"\nx DoubleVector:\n{x}");
            Console.WriteLine($"\nAx - b:\n{DoubleMatrix * x - b}");
            Console.WriteLine($"\nA*inv(A):\n{DoubleMatrix / DoubleMatrix}");
            DoubleMatrix matrix_ = new DoubleMatrix(new DoubleVector(8.0, 1.0, 6.0), new DoubleVector(3.0, 5.0, 7.0));
            Console.WriteLine($"\nnon rect mat:\n{matrix_}");
            Console.WriteLine($"\nnon rect mat mul by transposed it self :\n{matrix_ * DoubleMatrix.Transpose(matrix_)}");
            DoubleMatrix m = DoubleMatrix.Hessian(NumericUtils.TestFuncND, new DoubleVector(0.0, 0.0, 0.0));
            Console.WriteLine($"\nHessian(MultiDimensional.TestFuncND):\n{m}");
        }
        public static void NumericTests()
        {
            Console.WriteLine("\n//////////////////////////////");
            Console.WriteLine(  "//////// NumericTests ////////");
            Console.WriteLine(  "//////////////////////////////\n");
            double[] numbers = { 1.666, 0.666, -3.0, -0.125, 3.769, -1.666, -0.666, 3.0, 0.125, -3.769 };
            foreach(double number in numbers) Console.WriteLine($"number = {number} = {new RationalNumber(number)}");
            DoubleVector b = new DoubleVector(1.412, 2.33, -3.75);
            Console.WriteLine(b);
            Console.WriteLine(NumericUtils.ToRationalStr(b, false));
        }

        public static void SimpexTest() 
        {
            Console.WriteLine("\n/////////////////////////////");
            Console.WriteLine(  "//////// SimplexTest ////////");
            Console.WriteLine(  "/////////////////////////////\n");

            DoubleVector b = new DoubleVector(40.0, 28.0, 14.0);
            DoubleVector c = new DoubleVector(2.0, 3.0);

            DoubleMatrix A = new DoubleMatrix
            (
              new DoubleVector(-2.0, 6.0),
              new DoubleVector( 3.0, 2.0),
              new DoubleVector( 2.0,-1.0)
            );

            Console.WriteLine(" f(x,c) =  2x1 + 3x2;\n arg_max = {4, 8}, f(arg_max) = 32");
            Console.WriteLine(" |-2x1 + 6x2 <= 40");
            Console.WriteLine(" | 3x1 + 2x2 <= 28");
            Console.WriteLine(" | 2x1 -  x2 <= 14\n");

            Simplex sym_0 = new Simplex(A, c, new List<Sign>() { Sign.Less, Sign.Less, Sign.Less }, b);
            sym_0.Solve(SimplexProblemType.Max);

            Console.WriteLine("\n f(x,c) = -2x1 + 3x2;\n arg_min = {7, 0}, f(arg_min) =-14\n");

            Simplex sym_1 = new Simplex(A, new DoubleVector(-2.0, 3.0), new List<Sign>() { Sign.Less, Sign.Less, Sign.Less }, b);
            sym_1.Solve(SimplexProblemType.Min);


            Console.WriteLine("/////////////////////////////");
            Console.WriteLine(" f(x,c) =  2x1 + 3x2;\n arg_min = {62/5, 54/5}, f(arg_max) = 57 1/5");
            Console.WriteLine(" |-2x1 + 6x2 >= 40");
            Console.WriteLine(" | 3x1 + 2x2 >= 28");
            Console.WriteLine(" | 2x1 -  x2 >= 14\n");
            Simplex sym_2 = new Simplex(A, new DoubleVector(2.0, 1.0), new List<Sign>() { Sign.More, Sign.More, Sign.More }, b);
            sym_2.Solve(SimplexProblemType.Min);
            Console.WriteLine(" f(x,c) =  -2x1 - x2;\n arg_min = {62/5, 54/5}, f(arg_max) = -35 3/5");

            Simplex sym_3 = new Simplex(A, new DoubleVector(-2.0, -1.0), new List<Sign>() { Sign.More, Sign.More, Sign.More }, b);
            sym_3.Solve(SimplexProblemType.Max);
            Console.WriteLine(" f(x,c) =  2x1 + 3x2;\n arg_min = {none, none}, f(arg_max) = none");
            Simplex sym_4 = new Simplex(A, c, new List<Sign>() { Sign.Equal, Sign.Equal, Sign.Equal }, b);
            sym_4.Solve(SimplexProblemType.Max);

        }

        public static void TestAll() 
        {
            // float aaa = 123.5123f;
            // Console.WriteLine($"{string.Format("{0,0:F}", aaa).Replace(',', '.')}");
            // NumericTests();
            // OneDimensionalMethodsTest();
            // MultiDimensionalMethodsTest();
            VectorTest();
            //  MatrixTest();
            // SimpexTest();
        }
    }
}

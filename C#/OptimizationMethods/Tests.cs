using System;
using System.Collections.Generic;

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
            Console.WriteLine($"BiSect      : {OneDimensional.BiSect     (OneDimensional.TestFunc, x_0, x_1, 1e-4)}");
            Console.WriteLine($"GoldenRatio : {OneDimensional.GoldenRatio(OneDimensional.TestFunc, x_0, x_1, 1e-4)}");
            Console.WriteLine($"Fibonacchi  : {OneDimensional.Fibonacci  (OneDimensional.TestFunc, x_0, x_1, 1e-4)}\n");
        }
        public static void MultiDimensionalMethodsTest()
        {
            Console.WriteLine("\n/////////////////////////////////////////////");
            Console.WriteLine(  "//////// MultiDimensionalMethodsTest ////////");
            Console.WriteLine(  "/////////////////////////////////////////////\n");

            Vector x_1 = new Vector(0, 0);
            Vector x_0 = new Vector(5, 3);

            Console.WriteLine($"x_0 = {x_0},\nx_1 = {x_1}\n") ;
            Console.WriteLine($"BiSect                 : {MultiDimensional.BiSect             (MultiDimensional.TestFunc2D, x_1, x_0)}");
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
            Matrix matrix = new Matrix(new Vector(8, 1, 6), new Vector(3, 5, 7), new Vector(4, 9, 2));
            Console.WriteLine(matrix);
            Console.WriteLine($"\nmatrix + matrix:\n{ matrix + matrix }\n");
            Console.WriteLine($"\nmatrix - matrix:\n{ matrix - matrix }\n");
            Console.WriteLine($"\nmatrix * matrix:\n{ matrix * matrix }\n");
            Matrix l, u;
            Matrix.LU(ref matrix, out l, out u);
            Console.WriteLine($"\nL  matrix:\n{l}");
            Console.WriteLine($"\nU  matrix:\n{u}");
            Console.WriteLine($"\nL * U - matrix:\n{l * u - matrix}");
            Vector b = new Vector(1, 2, 3);
            /// x = {0.05,0.3,0.05};
            Vector x = Matrix.Linsolve(matrix, b);
            Console.WriteLine($"\nx vector:\n{x}");
            Console.WriteLine($"\nAx - b:\n{matrix * x - b}");
            Console.WriteLine($"\nA*inv(A):\n{matrix * Matrix.Invert(matrix)}");
            Matrix matrix_ = new Matrix(new Vector(8, 1, 6), new Vector(3, 5, 7));
            Console.WriteLine($"\nnon rect mat:\n{matrix_}");
            Console.WriteLine($"\nnon rect mat mul by transposed it self :\n{matrix_ * Matrix.Transpose(matrix_)}");
            Matrix m = Matrix.Hessian(MultiDimensional.TestFuncND, new Vector(0, 0, 0));
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

        public static void SimpexTest() 
        {
            Console.WriteLine("\n/////////////////////////////");
            Console.WriteLine(  "//////// SimplexTest ////////");
            Console.WriteLine(  "/////////////////////////////\n");

            Vector b = new Vector(40, 28, 14);
            Vector c = new Vector(2.0, 3.0);

            Matrix A = new Matrix
            (
                          new Vector(-2.0, 6.0),
                          new Vector( 3.0, 2.0),
                          new Vector( 2.0,-1.0)
            );


            Console.WriteLine(" f(x,c) =  2x1 + 3x2;\n arg_max = {4, 8}, f(arg_max) = 32");
            Console.WriteLine(" |-2x1 + 6x2 <= 40");
            Console.WriteLine(" | 3x1 + 2x2 <= 28");
            Console.WriteLine(" | 2x1 -  x2 <= 14\n");

            Simplex sym_0 = new Simplex(A, c, new List<Sign>() { Sign.Less, Sign.Less, Sign.Less }, b);
            sym_0.Solve(SimplexProblemType.Max);

            Console.WriteLine("\n f(x,c) = -2x1 + 3x2;\n arg_min = {7, 0}, f(arg_min) =-14\n");

            Simplex sym_1 = new Simplex(A, new Vector(-2.0, 3.0), new List<Sign>() { Sign.Less, Sign.Less, Sign.Less }, b);
            sym_1.Solve(SimplexProblemType.Min);


            Console.WriteLine("/////////////////////////////");
            Console.WriteLine(" f(x,c) =  2x1 + 3x2;\n arg_min = {62/5, 54/5}, f(arg_max) = 57 1/5");
            Console.WriteLine(" |-2x1 + 6x2 >= 40");
            Console.WriteLine(" | 3x1 + 2x2 >= 28");
            Console.WriteLine(" | 2x1 -  x2 >= 14\n");
            Simplex sym_2 = new Simplex(A, new Vector(2,1), new List<Sign>() { Sign.More, Sign.More, Sign.More }, b);
            sym_2.Solve(SimplexProblemType.Min);
            Console.WriteLine(" f(x,c) =  -2x1 - x2;\n arg_min = {62/5, 54/5}, f(arg_max) = -35 3/5");

            Simplex sym_3 = new Simplex(A, new Vector(-2, -1), new List<Sign>() { Sign.More, Sign.More, Sign.More }, b);
            sym_3.Solve(SimplexProblemType.Max);
            Console.WriteLine(" f(x,c) =  2x1 + 3x2;\n arg_min = {none, none}, f(arg_max) = none");
            Simplex sym_4 = new Simplex(A, c, new List<Sign>() { Sign.Equal, Sign.Equal, Sign.Equal }, b);
            sym_4.Solve(SimplexProblemType.Max);

        }

        public static void TestAll() 
        {
            // OneDimensionalMethodsTest();
            // MultiDimensionalMethodsTest();
            // MatrixTest();
            SimpexTest();
            // NumericTests();
        }
    }
}

import mathUtils.*;

import java.util.ArrayList;
import java.util.Objects;

public class Tests {
    public static void oneDimensionalMethodsTest()
    {
        System.out.println("\n///////////////////////////////////////////");
        System.out.println(  "//////// OneDimensionalMethodsTest ////////");
        System.out.println(  "///////////////////////////////////////////\n");

        final double x_0 = 10;
        final double x_1 = -1;

        NumericCommon.SHOW_DEBUG_LOG = true;
        System.out.println("x_0 = " + x_0 + ",\nx_1 = " + x_1 + "\n");
        final double biSect   = OneDimensional.biSect      (NumericUtils.testFunc1d, x_0, x_1);
        final double gRatio   = OneDimensional.goldenRatio (NumericUtils.testFunc1d, x_0, x_1);
        final double fNumbers = OneDimensional.fibonacci   (NumericUtils.testFunc1d, x_0, x_1);

        if(NumericCommon.NUMBER_RATIONAL_FORMAT)
        {
            System.out.printf("BiSect      : %s\n", NumericUtils.toRationalStr(biSect  , false));
            System.out.printf("GoldenRatio : %s\n", NumericUtils.toRationalStr(gRatio  , false));
            System.out.printf("Fibonacci   : %s\n", NumericUtils.toRationalStr(fNumbers, false));
        }else{
            System.out.printf("BiSect      : %s\n", biSect  );
            System.out.printf("GoldenRatio : %s\n", gRatio  );
            System.out.printf("Fibonacci   : %s\n", fNumbers);
        }
        NumericCommon.SHOW_DEBUG_LOG = false;
    }

    public static void numericTests()
    {
        System.out.println("\n//////////////////////////////");
        System.out.println(  "//////// NumericTests ////////");
        System.out.println(  "//////////////////////////////\n");

        int[] numbers;

        numbers = NumericUtils.decimalToRational(1.6666);
        System.out.print(numbers[0]!=0? numbers[0] + " ":"");
        System.out.print(numbers[1]!=0? numbers[1]+"/"+numbers[2] + "\n":"\n");

        numbers = NumericUtils.decimalToRational(0.6666);
        System.out.print(numbers[0]!=0? numbers[0] + " ":"");
        System.out.print(numbers[1]!=0? numbers[1]+"/"+numbers[2] + "\n":"\n");

        numbers = NumericUtils.decimalToRational(-3);
        System.out.print(numbers[0]!=0? numbers[0] + " ":"");
        System.out.print(numbers[1]!=0? numbers[1]+"/"+numbers[2] + "\n":"\n");

        numbers = NumericUtils.decimalToRational(-0.125);
        System.out.print(numbers[0]!=0? numbers[0] + " ":"");
        System.out.print(numbers[1]!=0? numbers[1]+"/"+numbers[2] + "\n":"\n");

        numbers = NumericUtils.decimalToRational(3.769230769230769230769);
        System.out.print(numbers[0]!=0? numbers[0] + " ":"");
        System.out.print(numbers[1]!=0? numbers[1]+"/"+numbers[2] + "\n":"\n");
        DoubleVector v = new DoubleVector(1.5, 3.33, 2.75, 0.123434);
        System.out.printf("%s\n",  NumericUtils.toRationalStr(v, false));
    }

    public static void VectorTest()
    {
        DoubleVector l = new DoubleVector(5.0, 6.0, 8.0, 5.5, 6.5, 8.5, 6.6, 7.7, 8.8);
        TemplateVector<Integer> iv = new TemplateVector<>(30);
        iv.fill(v -> (v - 10) % 10);
        System.out.print("///////////////////////////////////////////\n");
        System.out.print("////////       VectorSliceTest     ////////\n");
        System.out.print("///////////////////////////////////////////\n");
        DoubleVector longVector = new DoubleVector(2.0, 3.0, 4.0, 5.0, 6.0, 8.0);
        System.out.printf("iv     = %s\n", iv);
        System.out.printf("l      = %s\n", longVector);
        Slice left  = new Slice(0, 3);
        Slice right = new Slice(3, 6);
        System.out.printf("l[%s] = %s \n", left,  longVector.get( left));
        System.out.printf("r[%s] = %s \n", right, longVector.get(right));

        DoubleVector a = longVector.get( left); // new DoubleVector(2.0, 3.0, 4.0);
        DoubleVector b = longVector.get(right); // new DoubleVector(5.0, 6.0, 8.0);
        System.out.print("///////////////////////////////////////////\n");
        System.out.print("////////         VectorTest        ////////\n");
        System.out.print("///////////////////////////////////////////\n");
        System.out.print("////////      Internal methods     ////////\n");
        System.out.printf("%s += %s = ", b, a); b.add(a); System.out.printf("%s\n", b);
        System.out.printf("%s -= %s = ", b, a); b.sub(a); System.out.printf("%s\n", b);
        System.out.printf("%s *= %s = ", b, a); b.mul(a); System.out.printf("%s\n", b);
        System.out.printf("%s /= %s = ", b, a); b.div(a); System.out.printf("%s\n", b);
        System.out.printf("%s *= %s = ", b, 2); b.mul(2); System.out.printf("%s\n", b);
        System.out.printf("%s /= %s = ", b, 2); b.div(2); System.out.printf("%s\n", b);
        System.out.printf("%s += %s = ", b, 2); b.add(2); System.out.printf("%s\n", b);
        System.out.printf("%s -= %s = ", b, 2); b.sub(2); System.out.printf("%s\n", b);
        System.out.print("////////      External methods     ////////\n");
        System.out.printf("%s + %s = %s\n", a, b, DoubleVector.add(a, b));
        System.out.printf("%s - %s = %s\n", a, b, DoubleVector.sub(a, b));
        System.out.printf("%s / %s = %s\n", a, b, DoubleVector.div(a, b));
        System.out.printf("%s * %s = %s\n", a, b, DoubleVector.mul(a, b));
        System.out.printf("%s + %s = %s\n", b, 2, DoubleVector.add(b, 2));
        System.out.printf("%s + %s = %s\n", 2, b, DoubleVector.add(2, b));
        System.out.printf("%s - %s = %s\n", 2, b, DoubleVector.sub(2, b));
        System.out.printf("%s - %s = %s\n", b, 2, DoubleVector.sub(b, 2));
        System.out.printf("%s * %s = %s\n", b, 2, DoubleVector.mul(b, 2));
        System.out.printf("%s * %s = %s\n", 2, b, DoubleVector.mul(2, b));
        System.out.printf("%s / %s = %s\n", b, 2, NumericUtils.toRationalStr(DoubleVector.div(b, 2), false));
        System.out.printf("%s / %s = %s\n", 2, b, NumericUtils.toRationalStr(DoubleVector.div(2, b), false));
        System.out.printf("| %s | = %s\n", b, b.magnitude());
        System.out.printf("%s / | %s | = %s\n", b, b, b.normalized());
        System.out.printf("(%s, %s) = %s\n", b.normalized(),  b.normalized(), b.normalized().dot( b.normalized()));
    }

    public static void matrixTest()
    {
        System.out.println("\n////////////////////////////");
        System.out.println(  "//////// MatrixTest ////////");
        System.out.println(  "////////////////////////////\n");

        DoubleMatrix matrix =  new DoubleMatrix(new DoubleVector(8.0, 1.0, 6.0),
                                                new DoubleVector(3.0, 5.0, 7.0),
                                                new DoubleVector(4.0, 9.0, 2.0));

        DoubleMatrix matrix1 =  new DoubleMatrix(new DoubleVector(2.0, 9.0, 4.0),
                                                 new DoubleVector(7.0, 5.0, 3.0),
                                                 new DoubleVector(6.0, 1.0, 8.0));

        System.out.printf("matrix1:\n%s\n", matrix);
        System.out.printf("matrix2:\n%s\n", matrix1);
        System.out.printf("matrix1 += matrix2 =\n%s\n", matrix.add(matrix1));
        System.out.printf("matrix1 -= matrix2 =\n%s\n", matrix.sub(matrix1));
        System.out.printf("matrix1 *= 2 =\n%s\n ", matrix.mul(2));
        System.out.printf("matrix1 /= 2 =\n%s\n ", matrix.div(2));
        System.out.printf("matrix1 += 2 =\n%s\n ", matrix.add(2));
        System.out.printf("matrix1 -= 2 =\n%s\n ", matrix.sub(2));
        System.out.print("////////      External methods     ////////\n");
        System.out.printf("matrix1 + matrix2 =\n%s\n", DoubleMatrix.add(matrix, matrix1));
        System.out.printf("matrix1 - matrix2 =\n%s\n", DoubleMatrix.sub(matrix, matrix1));
        System.out.printf("matrix1 / matrix2 =\n%s\n", DoubleMatrix.div(matrix, matrix1));
        System.out.printf("matrix1 * matrix2 =\n%s\n", DoubleMatrix.mul(matrix, matrix1));
        System.out.printf("matrix1 +       2 =\n%s\n", DoubleMatrix.add(matrix, 2));
        System.out.printf("2       + matrix1 =\n%s\n", DoubleMatrix.add(2,  matrix));
        System.out.printf("2       - matrix1 =\n%s\n", DoubleMatrix.sub(2,  matrix));
        System.out.printf("matrix1 -       2 =\n%s\n", DoubleMatrix.sub(matrix, 2));
        System.out.printf("matrix1 *       2 =\n%s\n", DoubleMatrix.mul(matrix, 2));
        System.out.printf("2       * matrix1 =\n%s\n", DoubleMatrix.mul(2,  matrix));

        System.out.printf("matrix + matrix:\n%s\n", DoubleMatrix.add(matrix, matrix));
        System.out.printf("matrix - matrix:\n%s\n", DoubleMatrix.sub(matrix, matrix));
        System.out.printf("matrix * matrix:\n%s\n", DoubleMatrix.mul(matrix, matrix));
        System.out.printf("matrix / matrix:\n%s\n", DoubleMatrix.div(matrix, matrix));
        DoubleMatrix[] lu = DoubleMatrix.lu(matrix);
        System.out.printf("L  matrix:\n%s\n", lu[0]);
        System.out.printf("U  matrix:\n%s\n", lu[1]);
        System.out.printf("L * U - matrix:\n%s\n", DoubleMatrix.mul(lu[0], lu[1]).sub(matrix));
        DoubleVector b = new DoubleVector(1.0, 2.0, 3.0);
        /// x = {0.05,0.3,0.05};
        DoubleVector x = DoubleMatrix.linsolve(matrix, b);
        System.out.printf("x vector  : %s\n", x);
        System.out.printf("Ax - b    : %s\n", DoubleMatrix.mul(matrix,x).sub(b));
        System.out.printf("A * inv(A):\n%s\n", DoubleMatrix.mul(matrix , Objects.requireNonNull(DoubleMatrix.invert(matrix))));
        DoubleMatrix matrix_ = new DoubleMatrix(new DoubleVector(8.0, 1.0, 6.0),
                                                new DoubleVector(3.0, 5.0, 7.0));
        System.out.printf("non rect mat:\n%s\n", matrix_);
        System.out.printf("non rect mat mul by transposed it self :\n%s\n",
                DoubleMatrix.mul(matrix_ , DoubleMatrix.transpose(matrix_)) );
        DoubleMatrix hessian= DoubleMatrix.hessian(NumericUtils.testFuncNd, new DoubleVector(0.0, 0.0, 0.0));
        System.out.printf("Hessian(MultiDimensional.TestFuncND):\n%s\n", hessian);
    }
    public static void multiDimensionalMethodsTest()
    {
        System.out.println("\n/////////////////////////////////////////////");
        System.out.println(  "//////// MultiDimensionalMethodsTest ////////");
        System.out.println(  "/////////////////////////////////////////////\n");

        DoubleVector x_0 = new DoubleVector (5.0, 3.0);
        DoubleVector x_1 = new DoubleVector (0.0, 0.0);
        DoubleVector x   = new DoubleVector (-13.0, 22.0 );
        NumericCommon.SHOW_DEBUG_LOG = true;
        System.out.println("x_0 = "+ x_0 + ",\nx_1 = " + x_1 + "\n") ;
        System.out.println("biSect                   : "+ MultiDimensional.biSect             (NumericUtils.testFunc2d, x_1, x_0));
        System.out.println("Golden ratio             : "+ MultiDimensional.goldenRatio        (NumericUtils.testFunc2d, x_1, x_0));
        System.out.println("Fibonacci                : "+ MultiDimensional.fibonacci          (NumericUtils.testFunc2d, x_1, x_0));
        System.out.println("Per coordinate descend   : "+ MultiDimensional.perCordDescend     (NumericUtils.testFunc2d, x));
        System.out.println("Gradient descend         : "+ MultiDimensional.gradientDescend    (NumericUtils.testFunc2d, x));
        System.out.println("Conj gradient descend    : "+ MultiDimensional.conjGradientDescend(NumericUtils.testFunc2d, x));
        System.out.println("Newtone - Raphson        : "+ MultiDimensional.newtoneRaphson     (NumericUtils.testFunc2d, x_1));
        NumericCommon.SHOW_DEBUG_LOG = false;
    }

    public static void simplexTest()
    {
        System.out.println("\n/////////////////////////////");
        System.out.println(  "//////// SimplexTest ////////");
        System.out.println(  "/////////////////////////////\n");

        DoubleMatrix A = new DoubleMatrix(
              new DoubleVector(-2.0, 6.0),
              new DoubleVector(3.0, 2.0),
              new DoubleVector(2.0,-1.0)
        );

        DoubleVector b  = new DoubleVector(40.0, 28.0, 14.0);
        DoubleVector c  = new DoubleVector(2.0, 3.0);
        // mathUtils.DoubleVector c1 = new mathUtils.DoubleVector(-2.0, 3.0);
        // mathUtils.DoubleVector c2 = new mathUtils.DoubleVector(2.0, 1.0);
        // mathUtils.DoubleVector c3 = new mathUtils.DoubleVector(-2.0, -1.0);

        System.out.println(" f(x,c) =  2x1 + 3x2;\n arg_max = {4, 8}, f(arg_max) = 32");
        System.out.println(" |-2x1 + 6x2 <= 40");
        System.out.println(" | 3x1 + 2x2 <= 28");
        System.out.println(" | 2x1 -  x2 <= 14\n");

        ArrayList<Sign> signs_less = new ArrayList<>(3);
        signs_less.add(Sign.Less);
        signs_less.add(Sign.Less);
        signs_less.add(Sign.Less);

        ArrayList<Sign> signs_more = new ArrayList<>(3);
        signs_more.add(Sign.More);
        signs_more.add(Sign.More);
        signs_more.add(Sign.More);

        ArrayList<Sign> signs_equal = new ArrayList<>(3);
        signs_equal.add(Sign.Equal);
        signs_equal.add(Sign.Equal);
        signs_equal.add(Sign.Equal);
        NumericCommon.SHOW_SIMPLEX_DEBUG_LOG = true;

        Simplex sym_0 = new Simplex(A,new DoubleVector(2.0,  3.0 ), signs_less, b);
        sym_0.solve(SimplexProblemType.Max);

        System.out.println("\n f(x,c) = -2x1 + 3x2;\n arg_min = {7, 0}, f(arg_min) =-14\n");
        Simplex sym_1 = new Simplex(A, new DoubleVector(-2.0,  3.0), signs_less, b);
        sym_1.solve(SimplexProblemType.Min);


        System.out.println("/////////////////////////////");
        System.out.println(" f(x,c) =  2x1 + 3x2;\n arg_min = {62/5, 54/5}, f(arg_max) = 57 1/5");
        System.out.println(" |-2x1 + 6x2 >= 40");
        System.out.println(" | 3x1 + 2x2 >= 28");
        System.out.println(" | 2x1 -  x2 >= 14\n");
        Simplex sym_2 = new Simplex(A, new DoubleVector(2.0, 1.0), signs_more, b);
        sym_2.solve(SimplexProblemType.Min);
        System.out.println(" f(x,c) =  -2x1 - x2;\n arg_min = {62/5, 54/5}, f(arg_max) = -35 3/5");
        Simplex sym_3 = new Simplex(A, new DoubleVector(-2.0, -1.0), signs_more, b);
        sym_3.solve(SimplexProblemType.Max);
        System.out.println(" f(x,c) =  2x1 + 3x2;\n arg_min = {none, none}, f(arg_max) = none");
        Simplex sym_4 = new Simplex(A, c, signs_equal, b);
        sym_4.solve(SimplexProblemType.Max);
    }
}

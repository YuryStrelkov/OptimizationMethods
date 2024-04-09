import mathUtils.*;
import mathUtils.NumericUtils.RationalNumber;
import java.util.Objects;

public class Tests {
    public static void numericTests()
    {
        System.out.print("///////////////////////////////////////////\n");
        System.out.print("////////         NumericsTest      ////////\n");
        System.out.print("///////////////////////////////////////////\n\n");
        double[] numbers = { 1.666, 0.666, -3.0, -0.125, 3.769, -1.666, -0.666, 3.0, 0.125, -3.769 };
        for(double number: numbers) System.out.printf("number = %15s = %15s\n", number, new RationalNumber(number));
    }

    public static void VectorTest()
    {
        System.out.print("///////////////////////////////////////////\n");
        System.out.print("////////       VectorSliceTest     ////////\n");
        System.out.print("///////////////////////////////////////////\n\n");
        DoubleVector longVector = new DoubleVector(2.0, 3.0, 4.0, 5.0, 6.0, 8.0);
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
        System.out.print("///////////////////////////////////////////\n\n");
        System.out.printf("%s += %s = ", b, a); b.add(a); System.out.printf("%s\n", b);
        System.out.printf("%s -= %s = ", b, a); b.sub(a); System.out.printf("%s\n", b);
        System.out.printf("%s *= %s = ", b, a); b.mul(a); System.out.printf("%s\n", b);
        System.out.printf("%s /= %s = ", b, a); b.div(a); System.out.printf("%s\n", b);
        System.out.printf("%s *= %s = ", b, 2); b.mul(2); System.out.printf("%s\n", b);
        System.out.printf("%s /= %s = ", b, 2); b.div(2); System.out.printf("%s\n", b);
        System.out.printf("%s += %s = ", b, 2); b.add(2); System.out.printf("%s\n", b);
        System.out.printf("%s -= %s = ", b, 2); b.sub(2); System.out.printf("%s\n", b);
        System.out.print("///////////////////////////////////////////\n");
        System.out.print("////////      External methods     ////////\n");
        System.out.print("///////////////////////////////////////////\n\n");
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
        // System.out.println("");
        // System.out.println("");
        // System.out.printf("a = %s \n", a);
        // System.out.printf("b = %s \n", b);
        // System.out.printf("l = %s\n", longVector);
        // a.insert(1,13.0);
        // System.out.println("");
        // System.out.printf("a = %s \n", a);
        // System.out.printf("b = %s \n", b);
        // b.removeAt(0);
        // System.out.printf("a = %s \n", a);
        // System.out.printf("b = %s \n", b);
        // System.out.printf("l = %s\n", longVector);
        // System.out.println("");
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
        DoubleMatrix bigMatrix = new DoubleMatrix(
                new DoubleVector(0.8147, 0.4218, 0.2769, 0.7094, 0.8909, 0.3517, 0.5688, 0.2290, 0.2599, 0.4018, 0.7803, 0.6491, 0.4868, 0.6225, 0.9049, 0.0855),
                new DoubleVector(0.9058, 0.9157, 0.0462, 0.7547, 0.9593, 0.8308, 0.4694, 0.9133, 0.8001, 0.0760, 0.3897, 0.7317, 0.4359, 0.5870, 0.9797, 0.2625),
                new DoubleVector(0.1270, 0.7922, 0.0971, 0.2760, 0.5472, 0.5853, 0.0119, 0.1524, 0.4314, 0.2399, 0.2417, 0.6477, 0.4468, 0.2077, 0.4389, 0.8010),
                new DoubleVector(0.9134, 0.9595, 0.8235, 0.6797, 0.1386, 0.5497, 0.3371, 0.8258, 0.9106, 0.1233, 0.4039, 0.4509, 0.3063, 0.3012, 0.1111, 0.0292),
                new DoubleVector(0.6324, 0.6557, 0.6948, 0.6551, 0.1493, 0.9172, 0.1622, 0.5383, 0.1818, 0.1839, 0.0965, 0.5470, 0.5085, 0.4709, 0.2581, 0.9289),
                new DoubleVector(0.0975, 0.0357, 0.3171, 0.1626, 0.2575, 0.2858, 0.7943, 0.9961, 0.2638, 0.2400, 0.1320, 0.2963, 0.5108, 0.2305, 0.4087, 0.7303),
                new DoubleVector(0.2785, 0.8491, 0.9502, 0.1190, 0.8407, 0.7572, 0.3112, 0.0782, 0.1455, 0.4173, 0.9421, 0.7447, 0.8176, 0.8443, 0.5949, 0.4886),
                new DoubleVector(0.5469, 0.9340, 0.0344, 0.4984, 0.2543, 0.7537, 0.5285, 0.4427, 0.1361, 0.0497, 0.9561, 0.1890, 0.7948, 0.1948, 0.2622, 0.5785),
                new DoubleVector(0.9575, 0.6787, 0.4387, 0.9597, 0.8143, 0.3804, 0.1656, 0.1067, 0.8693, 0.9027, 0.5752, 0.6868, 0.6443, 0.2259, 0.6028, 0.2373),
                new DoubleVector(0.9649, 0.7577, 0.3816, 0.3404, 0.2435, 0.5678, 0.6020, 0.9619, 0.5797, 0.9448, 0.0598, 0.1835, 0.3786, 0.1707, 0.7112, 0.4588),
                new DoubleVector(0.1576, 0.7431, 0.7655, 0.5853, 0.9293, 0.0759, 0.2630, 0.0046, 0.5499, 0.4909, 0.2348, 0.3685, 0.8116, 0.2277, 0.2217, 0.9631),
                new DoubleVector(0.9706, 0.3922, 0.7952, 0.2238, 0.3500, 0.0540, 0.6541, 0.7749, 0.1450, 0.4893, 0.3532, 0.6256, 0.5328, 0.4357, 0.1174, 0.5468),
                new DoubleVector(0.9572, 0.6555, 0.1869, 0.7513, 0.1966, 0.5308, 0.6892, 0.8173, 0.8530, 0.3377, 0.8212, 0.7802, 0.3507, 0.3111, 0.2967, 0.5211),
                new DoubleVector(0.4854, 0.1712, 0.4898, 0.2551, 0.2511, 0.7792, 0.7482, 0.8687, 0.6221, 0.9001, 0.0154, 0.0811, 0.9390, 0.9234, 0.3188, 0.2316),
                new DoubleVector(0.8003, 0.7060, 0.4456, 0.5060, 0.6160, 0.9340, 0.4505, 0.0844, 0.3510, 0.3692, 0.0430, 0.9294, 0.8759, 0.4302, 0.4242, 0.4889),
                new DoubleVector(0.1419, 0.0318, 0.6463, 0.6991, 0.4733, 0.1299, 0.0838, 0.3998, 0.5132, 0.1112, 0.1690, 0.7757, 0.5502, 0.1848, 0.5079, 0.6241)
        );
        System.out.printf("bigMatrix:\n%s\n", bigMatrix);
        System.out.printf("inv(bigMatrix):\n%s\n", DoubleMatrix.invert(bigMatrix));
        System.out.printf("bigMatrix^-1 x bigMatrix:\n%s\n",
                DoubleMatrix.mul(Objects.requireNonNull(DoubleMatrix.invert(bigMatrix)), bigMatrix));
        System.out.printf("bigMatrix x bigMatrix^-1:\n%s\n",
                DoubleMatrix.mul(bigMatrix, Objects.requireNonNull(DoubleMatrix.invert(bigMatrix))));
        System.out.println("");
    }

    public static void oneDimensionalMethodsTest()
    {
        System.out.printf("%s\n", "///////////////////////////////////////////");
        System.out.printf("%s\n", "//////// OneDimensionalMethodsTest ////////");
        System.out.printf("%s\n", "///////////////////////////////////////////");

        final double x_0 = 10;
        final double x_1 = -1;

        NumericCommon.SHOW_ZERO_ORDER_METHODS_DEBUG_LOG = true;
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
        System.out.println("");
    }

    public static void multiDimensionalMethodsTest()
    {
        System.out.printf("%s\n", "/////////////////////////////////////////////");
        System.out.printf("%s\n", "//////// MultiDimensionalMethodsTest ////////");
        System.out.printf("%s\n", "/////////////////////////////////////////////\n");

        DoubleVector x_0 = new DoubleVector (5.0, 3.0);
        DoubleVector x_1 = new DoubleVector (0.0, 0.0);
        DoubleVector x   = new DoubleVector (-13.0, 22.0 );
        NumericCommon.SHOW_DEBUG_LOG = true;
        NumericCommon.SHOW_ZERO_ORDER_METHODS_DEBUG_LOG = true;
        System.out.println("x_0 = "+ x_0 + ",\nx_1 = " + x_1 + "\n") ;
        System.out.printf("biSect                 : %s\n", MultiDimensional.biSect             (NumericUtils.testFunc2d, x_1, x_0));
        System.out.printf("Golden ratio           : %s\n", MultiDimensional.goldenRatio        (NumericUtils.testFunc2d, x_1, x_0));
        System.out.printf("Fibonacci              : %s\n", MultiDimensional.fibonacci          (NumericUtils.testFunc2d, x_1, x_0));
        System.out.printf("Per coordinate descend : %s\n", MultiDimensional.perCordDescend     (NumericUtils.testFunc2d, x));
        System.out.printf("Gradient descend       : %s\n", MultiDimensional.gradientDescend    (NumericUtils.testFunc2d, x));
        System.out.printf("Conj gradient descend  : %s\n", MultiDimensional.conjGradientDescend(NumericUtils.testFunc2d, x));
        System.out.printf("Newtone - Raphson      : %s\n", MultiDimensional.newtoneRaphson     (NumericUtils.testFunc2d, x_1));
        NumericCommon.SHOW_DEBUG_LOG = false;
        System.out.println("");
    }

    public static void simplexTest()
    {
        System.out.printf("%s\n", "/////////////////////////////");
        System.out.printf("%s\n", "//////// SimplexTest ////////");
        System.out.printf("%s\n", "/////////////////////////////\n");

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

        TemplateVector<Sign> signs_less = new TemplateVector<>(Sign.Less, Sign.Less, Sign.Less);

        TemplateVector<Sign> signs_more = new TemplateVector<>(Sign.More, Sign.More, Sign.More);

        TemplateVector<Sign> signs_equal = new TemplateVector<>(Sign.Equal, Sign.Equal, Sign.Equal);
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

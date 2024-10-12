package example.labs;

import example.labs.enums.Sign;
import example.labs.enums.SimplexProblemType;
import lombok.extern.slf4j.Slf4j;
import example.mathUtils.*;
import example.mathUtils.NumericUtils.RationalNumber;
import java.util.Objects;

@Slf4j
public class Tests {
    public static void numericTests()
    {
        log.debug("///////////////////////////////////////////\n");
        log.debug("////////         NumericsTest      ////////\n");
        log.debug("///////////////////////////////////////////\n\n");
        double[] numbers = { 1.666, 0.666, -3.0, -0.125, 3.769, -1.666, -0.666, 3.0, 0.125, -3.769 };
        for(double number: numbers) log.debug("number = %15s = %15s\n", number, new RationalNumber(number));
    }

    public static void VectorTest()
    {
        log.debug("///////////////////////////////////////////\n");
        log.debug("////////       VectorSliceTest     ////////\n");
        log.debug("///////////////////////////////////////////\n\n");
        DoubleVector longVector = new DoubleVector(2.0, 3.0, 4.0, 5.0, 6.0, 8.0);
        log.debug("l      = {}\n", longVector);
        Slice left  = new Slice(0, 3);
        Slice right = new Slice(3, 6);
        log.debug("l[{}] = {} \n", left,  longVector.get( left));
        log.debug("r[{}] = {} \n", right, longVector.get(right));
        DoubleVector a = longVector.get( left); // new DoubleVector(2.0, 3.0, 4.0);
        DoubleVector b = longVector.get(right); // new DoubleVector(5.0, 6.0, 8.0);
        log.debug("///////////////////////////////////////////\n");
        log.debug("////////         VectorTest        ////////\n");
        log.debug("///////////////////////////////////////////\n");
        log.debug("////////      Internal methods     ////////\n");
        log.debug("///////////////////////////////////////////\n\n");
        log.debug("{} += {} = ", b, a); b.add(a); log.debug("{}\n", b);
        log.debug("{} -= {} = ", b, a); b.sub(a); log.debug("{}\n", b);
        log.debug("{} *= {} = ", b, a); b.mul(a); log.debug("{}\n", b);
        log.debug("{} /= {} = ", b, a); b.div(a); log.debug("{}\n", b);
        log.debug("{} *= {} = ", b, 2); b.mul(2); log.debug("{}\n", b);
        log.debug("{} /= {} = ", b, 2); b.div(2); log.debug("{}\n", b);
        log.debug("{} += {} = ", b, 2); b.add(2); log.debug("{}\n", b);
        log.debug("{} -= {} = ", b, 2); b.sub(2); log.debug("{}\n", b);
        log.debug("///////////////////////////////////////////\n");
        log.debug("////////      External methods     ////////\n");
        log.debug("///////////////////////////////////////////\n\n");
        log.debug("{} + {} = {}\n", a, b, DoubleVectorUtils.add(a, b));
        log.debug("{} - {} = {}\n", a, b, DoubleVectorUtils.sub(a, b));
        log.debug("{} / {} = {}\n", a, b, DoubleVectorUtils.div(a, b));
        log.debug("{} * {} = {}\n", a, b, DoubleVectorUtils.mul(a, b));
        log.debug("{} + {} = {}\n", b, 2, DoubleVectorUtils.add(b, 2));
        log.debug("{} + {} = {}\n", 2, b, DoubleVectorUtils.add(2, b));
        log.debug("{} - {} = {}\n", 2, b, DoubleVectorUtils.sub(2, b));
        log.debug("{} - {} = {}\n", b, 2, DoubleVectorUtils.sub(b, 2));
        log.debug("{} * {} = {}\n", b, 2, DoubleVectorUtils.mul(b, 2));
        log.debug("{} * {} = {}\n", 2, b, DoubleVectorUtils.mul(2, b));
        log.debug("{} / {} = {}\n", b, 2, NumericUtils.toRationalStr(DoubleVectorUtils.div(b, 2), false));
        log.debug("{} / {} = {}\n", 2, b, NumericUtils.toRationalStr(DoubleVectorUtils.div(2, b), false));
        log.debug("| {} | = {}\n", b, b.magnitude());
        log.debug("{} / | {} | = {}\n", b, b, b.normalized());
        log.debug("({}, {}) = {}\n", b.normalized(),  b.normalized(), b.normalized().dot( b.normalized()));
        // System.out.println("");
        // System.out.println("");
        // log.debug("a = {} \n", a);
        // log.debug("b = {} \n", b);
        // log.debug("l = {}\n", longVector);
        // a.insert(1,13.0);
        // System.out.println("");
        // log.debug("a = {} \n", a);
        // log.debug("b = {} \n", b);
        // b.removeAt(0);
        // log.debug("a = {} \n", a);
        // log.debug("b = {} \n", b);
        // log.debug("l = {}\n", longVector);
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

        log.debug("matrix1:\n{}\n", matrix);
        log.debug("matrix2:\n{}\n", matrix1);
        log.debug("matrix1 += matrix2 =\n{}\n", matrix.add(matrix1));
        log.debug("matrix1 -= matrix2 =\n{}\n", matrix.sub(matrix1));
        log.debug("matrix1 *= 2 =\n{}\n ", matrix.mul(2));
        log.debug("matrix1 /= 2 =\n{}\n ", matrix.div(2));
        log.debug("matrix1 += 2 =\n{}\n ", matrix.add(2));
        log.debug("matrix1 -= 2 =\n{}\n ", matrix.sub(2));
        log.debug("////////      External methods     ////////\n");
        log.debug("matrix1 + matrix2 =\n{}\n", DoubleMatrixUtils.add(matrix, matrix1));
        log.debug("matrix1 - matrix2 =\n{}\n", DoubleMatrixUtils.sub(matrix, matrix1));
        log.debug("matrix1 / matrix2 =\n{}\n", DoubleMatrixUtils.div(matrix, matrix1));
        log.debug("matrix1 * matrix2 =\n{}\n", DoubleMatrixUtils.mul(matrix, matrix1));
        log.debug("matrix1 +       2 =\n{}\n", DoubleMatrixUtils.add(matrix, 2));
        log.debug("2       + matrix1 =\n{}\n", DoubleMatrixUtils.add(2,  matrix));
        log.debug("2       - matrix1 =\n{}\n", DoubleMatrixUtils.sub(2,  matrix));
        log.debug("matrix1 -       2 =\n{}\n", DoubleMatrixUtils.sub(matrix, 2));
        log.debug("matrix1 *       2 =\n{}\n", DoubleMatrixUtils.mul(matrix, 2));
        log.debug("2       * matrix1 =\n{}\n", DoubleMatrixUtils.mul(2,  matrix));

        log.debug("matrix + matrix:\n{}\n", DoubleMatrixUtils.add(matrix, matrix));
        log.debug("matrix - matrix:\n{}\n", DoubleMatrixUtils.sub(matrix, matrix));
        log.debug("matrix * matrix:\n{}\n", DoubleMatrixUtils.mul(matrix, matrix));
        log.debug("matrix / matrix:\n{}\n", DoubleMatrixUtils.div(matrix, matrix));
        DoubleMatrix[] lu = DoubleMatrixUtils.lu(matrix);
        log.debug("L  matrix:\n{}\n", lu[0]);
        log.debug("U  matrix:\n{}\n", lu[1]);
        log.debug("L * U - matrix:\n{}\n", DoubleMatrixUtils.mul(lu[0], lu[1]).sub(matrix));
        DoubleVector b = new DoubleVector(1.0, 2.0, 3.0);
        /// x = {0.05,0.3,0.05};
        DoubleVector x = DoubleMatrixUtils.linsolve(matrix, b);
        log.debug("x vector  : {}\n", x);
        log.debug("Ax - b    : {}\n", DoubleMatrixUtils.mul(matrix,x).sub(b));
        log.debug("A * inv(A):\n{}\n", DoubleMatrixUtils.mul(matrix , Objects.requireNonNull(DoubleMatrixUtils.invert(matrix))));
        DoubleMatrix matrix_ = new DoubleMatrix(new DoubleVector(8.0, 1.0, 6.0),
                new DoubleVector(3.0, 5.0, 7.0));
        log.debug("non rect mat:\n{}\n", matrix_);
        log.debug("non rect mat mul by transposed it self :\n{}\n",
                DoubleMatrixUtils.mul(matrix_ , DoubleMatrixUtils.transpose(matrix_)) );
        DoubleMatrix hessian= DoubleMatrixUtils.hessian(NumericUtils.testFuncNd, new DoubleVector(0.0, 0.0, 0.0));
        log.debug("Hessian(enjoy.labs.MultiDimensional.TestFuncND):\n{}\n", hessian);
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
        log.debug("bigMatrix:\n{}\n", bigMatrix);
        log.debug("inv(bigMatrix):\n{}\n", DoubleMatrixUtils.invert(bigMatrix));
        log.debug("bigMatrix^-1 x bigMatrix:\n{}\n",
                DoubleMatrixUtils.mul(Objects.requireNonNull(DoubleMatrixUtils.invert(bigMatrix)), bigMatrix));
        log.debug("bigMatrix x bigMatrix^-1:\n{}\n",
                DoubleMatrixUtils.mul(bigMatrix, Objects.requireNonNull(DoubleMatrixUtils.invert(bigMatrix))));
        System.out.println("");
    }

    public static void oneDimensionalMethodsTest()
    {
        log.debug("{}\n", "///////////////////////////////////////////");
        log.debug("{}\n", "//////// OneDimensionalMethodsTest ////////");
        log.debug("{}\n", "///////////////////////////////////////////");

        final double x_0 = 10;
        final double x_1 = -1;

        NumericCommon.SHOW_ZERO_ORDER_METHODS_DEBUG_LOG = true;
        System.out.println("x_0 = " + x_0 + ",\nx_1 = " + x_1 + "\n");
        final double biSect   = OneDimensional.biSect      (NumericUtils.testFunc1d, x_0, x_1);
        final double gRatio   = OneDimensional.goldenRatio (NumericUtils.testFunc1d, x_0, x_1);
        final double fNumbers = OneDimensional.fibonacci   (NumericUtils.testFunc1d, x_0, x_1);

        if(NumericCommon.NUMBER_RATIONAL_FORMAT)
        {
            log.debug("BiSect      : {}\n", NumericUtils.toRationalStr(biSect  , false));
            log.debug("GoldenRatio : {}\n", NumericUtils.toRationalStr(gRatio  , false));
            log.debug("Fibonacci   : {}\n", NumericUtils.toRationalStr(fNumbers, false));
        }else{
            log.debug("BiSect      : {}\n", biSect  );
            log.debug("GoldenRatio : {}\n", gRatio  );
            log.debug("Fibonacci   : {}\n", fNumbers);
        }
        NumericCommon.SHOW_DEBUG_LOG = false;
        System.out.println("");
    }

    public static void multiDimensionalMethodsTest()
    {
        log.debug("{}\n", "/////////////////////////////////////////////");
        log.debug("{}\n", "//////// MultiDimensionalMethodsTest ////////");
        log.debug("{}\n", "/////////////////////////////////////////////\n");

        DoubleVector x_0 = new DoubleVector (5.0, 3.0);
        DoubleVector x_1 = new DoubleVector (0.0, 0.0);
        DoubleVector x   = new DoubleVector (-13.0, 22.0 );
        NumericCommon.SHOW_DEBUG_LOG = true;
        NumericCommon.SHOW_ZERO_ORDER_METHODS_DEBUG_LOG = true;
        System.out.println("x_0 = "+ x_0 + ",\nx_1 = " + x_1 + "\n") ;
        log.debug("biSect                 : {}\n", MultiDimensional.biSect             (NumericUtils.testFunc2d, x_1, x_0));
        log.debug("Golden ratio           : {}\n", MultiDimensional.goldenRatio        (NumericUtils.testFunc2d, x_1, x_0));
        log.debug("Fibonacci              : {}\n", MultiDimensional.fibonacci          (NumericUtils.testFunc2d, x_1, x_0));
        log.debug("Per coordinate descend : {}\n", MultiDimensional.perCordDescend     (NumericUtils.testFunc2d, x));
        log.debug("Gradient descend       : {}\n", MultiDimensional.gradientDescend    (NumericUtils.testFunc2d, x));
        log.debug("Conj gradient descend  : {}\n", MultiDimensional.conjGradientDescend(NumericUtils.testFunc2d, x));
        log.debug("Newtone - Raphson      : {}\n", MultiDimensional.newtoneRaphson     (NumericUtils.testFunc2d, x_1));
        NumericCommon.SHOW_DEBUG_LOG = false;
        System.out.println("");
    }

    public static void simplexTest()
    {
        log.debug("{}\n", "/////////////////////////////");
        log.debug("{}\n", "//////// SimplexTest ////////");
        log.debug("{}\n", "/////////////////////////////\n");

        DoubleMatrix A = new DoubleMatrix(
              new DoubleVector(-2.0, 6.0),
              new DoubleVector(3.0, 2.0),
              new DoubleVector(2.0,-1.0)
        );

        DoubleVector b  = new DoubleVector(40.0, 28.0, 14.0);
        DoubleVector c  = new DoubleVector(2.0, 3.0);
        // enjoy.mathUtils.DoubleVector c1 = new enjoy.mathUtils.DoubleVector(-2.0, 3.0);
        // enjoy.mathUtils.DoubleVector c2 = new enjoy.mathUtils.DoubleVector(2.0, 1.0);
        // enjoy.mathUtils.DoubleVector c3 = new enjoy.mathUtils.DoubleVector(-2.0, -1.0);

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

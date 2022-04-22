import java.util.ArrayList;

public class Tests {
    public static void oneDimensionalMethodsTest()
    {
        System.out.println("\n///////////////////////////////////////////");
        System.out.println(  "//////// OneDimensionalMethodsTest ////////");
        System.out.println(  "///////////////////////////////////////////\n");

        double x_0 = 10;
        double x_1 = -1;

        OneDimensional.ShowDebugLog = true;
        System.out.println("x_0 = " + x_0 + ",\nx_1 = " + x_1 + "\n");
        System.out.println("Dihotomia   : " + OneDimensional.dihotomia   (OneDimensional.testFunc, x_0, x_1, 1e-4));
        System.out.println("GoldenRatio : " + OneDimensional.goldenRatio (OneDimensional.testFunc, x_0, x_1, 1e-4));
        System.out.println("Fibonacchi  : " + OneDimensional.fibonacci   (OneDimensional.testFunc, x_0, x_1, 1e-4) + " \n");
        OneDimensional.ShowDebugLog = false;
    }

    public static void numericTests()
    {
        System.out.println("\n//////////////////////////////");
        System.out.println(  "//////// NumericTests ////////");
        System.out.println(  "//////////////////////////////\n");

        int[] number;

        number = NumericUtils.decimalToRational(1.6666);
        System.out.print(number[0]!=0? number[0] + " ":"");
        System.out.print(number[1]!=0? number[1]+"/"+number[2] + "\n":"\n");

        number = NumericUtils.decimalToRational(0.6666);
        System.out.print(number[0]!=0? number[0] + " ":"");
        System.out.print(number[1]!=0? number[1]+"/"+number[2] + "\n":"\n");

        number = NumericUtils.decimalToRational(-3);
        System.out.print(number[0]!=0? number[0] + " ":"");
        System.out.print(number[1]!=0? number[1]+"/"+number[2] + "\n":"\n");

        number = NumericUtils.decimalToRational(-0.125);
        System.out.print(number[0]!=0? number[0] + " ":"");
        System.out.print(number[1]!=0? number[1]+"/"+number[2] + "\n":"\n");

        number = NumericUtils.decimalToRational(3.769230769230769230769);
        System.out.print(number[0]!=0? number[0] + " ":"");
        System.out.print(number[1]!=0? number[1]+"/"+number[2] + "\n":"\n");
    }
    public static void matrixTest()
    {
        System.out.println("\n////////////////////////////");
        System.out.println(  "//////// MatrixTest ////////");
        System.out.println(  "////////////////////////////\n");

        Matrix matrix =  new Matrix( new Vector[] {new Vector(new double[] { 8, 1, 6 }), new Vector( new double[] { 3, 5, 7 }), new Vector(new double[] { 4, 9, 2 } )}) ;
        System.out.println(matrix);
        System.out.println("\nmatrix + matrix:\n"+Matrix.add(matrix,matrix) +"\n");
        System.out.println("\nmatrix - matrix:\n"+Matrix.sub(matrix,matrix) +"\n");
        System.out.println("\nmatrix * matrix:\n"+Matrix.mul(matrix,matrix)+"\n");
        Matrix[] lu = Matrix.lu(matrix);
        System.out.println("\nL  matrix:\n" + lu[0]);
        System.out.println("\nU  matrix:\n" + lu[1]);
        System.out.println("\nL * U - matrix:\n" + Matrix.mul(lu[0] , lu[1]).sub(matrix));
        Vector b = new Vector(new double[] { 1, 2, 3 });
        /// x = {0.05,0.3,0.05};
        Vector x = Matrix.linsolve(matrix, b);
        System.out.println("\nx vector:\n" + x);
        System.out.println("\nAx - b:\n" + Matrix.mul(matrix,x).sub(b));
        System.out.println("\n A * inv(A):\n"+Matrix.mul(matrix , Matrix.invert(matrix)));
        Matrix matrix_ = new Matrix(new Vector[] { new Vector(new double[] { 8, 1, 6 }), new Vector(new double[] { 3, 5, 7 }) });
        System.out.println("\nnon rect mat:\n" + matrix_);
        System.out.println("\nnon rect mat mul by transposed it self :\n" +Matrix.mul(matrix_ , Matrix.transpose(matrix_)) );
        Matrix m = Matrix.hessian(MultiDimensional.testFuncNd, new Vector(new double[] { 0, 0, 0 }));
        System.out.println("\nHessian(MultiDimensional.TestFuncND):\n"+m);
    }
    public static void multiDimensionalMethodsTest()
    {
        System.out.println("\n/////////////////////////////////////////////");
        System.out.println(  "//////// MultiDimensionalMethodsTest ////////");
        System.out.println(  "/////////////////////////////////////////////\n");

        Vector x_0 = new Vector (new double[] { 5, 3 });
        Vector x_1 = new Vector (new double[] { 0, 0 });
        Vector x   = new Vector (new double[] { -13.0, 22 });
        MultiDimensional.ShowDebugLog = true;
        System.out.println("x_0 = "+ x_0 + ",\nx_1 = " + x_1 + "\n") ;
        System.out.println("Dihotomia                : "+ MultiDimensional.dihotomia          (MultiDimensional.testFunc2d, x_1, x_0));
        System.out.println("Golden ratio             : "+ MultiDimensional.goldenRatio        (MultiDimensional.testFunc2d, x_1, x_0));
        System.out.println("Fibonacci                : "+ MultiDimensional.fibonacci          (MultiDimensional.testFunc2d, x_1, x_0));
        System.out.println("Per coord descend        : "+ MultiDimensional.perCoordDescend    (MultiDimensional.testFunc2d, x));
        System.out.println("Gradient descend         : "+ MultiDimensional.gradientDescend    (MultiDimensional.testFunc2d, x));
        System.out.println("Conj gradient descend    : "+ MultiDimensional.conjGradientDescend(MultiDimensional.testFunc2d, x));
        System.out.println("Newtone - Raphson        : "+ MultiDimensional.newtoneRaphson     (MultiDimensional.testFunc2d, x_1));
        MultiDimensional.ShowDebugLog = false;
    }

    public static void symplexTest()
    {
        System.out.println("\n/////////////////////////////");
        System.out.println(  "//////// SymplexTest ////////");
        System.out.println(  "/////////////////////////////\n");

        Matrix A = new Matrix(
              new Vector(new double[]{-2, 6}),
              new Vector(new double[]{ 3, 2}),
              new Vector(new double[]{ 2,-1})
        );

        Vector b  = new Vector(  40.0, 28.0, 14.0);
        Vector c  = new Vector(   2.0,  3.0);
        Vector c1 = new Vector(  -2.0,  3.0);
        Vector c2 = new Vector(  2.0,  1.0);
        Vector c3 = new Vector(  -2.0, -1.0);

        System.out.println(" f(x,c) =  2x1 + 3x2;\n arg_max = {4, 8}, f(arg_max) = 32");
        System.out.println(" |-2x1 + 6x2 <= 40");
        System.out.println(" | 3x1 + 2x2 <= 28");
        System.out.println(" | 2x1 -  x2 <= 14\n");

        ArrayList<Sign> signs_less = new ArrayList<Sign>(3);
        signs_less.add(Sign.Less);
        signs_less.add(Sign.Less);
        signs_less.add(Sign.Less);
        ArrayList<Sign> signs_more = new ArrayList<Sign>(3);
        signs_more.add(Sign.More);
        signs_more.add(Sign.More);
        signs_more.add(Sign.More);
        ArrayList<Sign> signs_equal = new ArrayList<Sign>(3);
        signs_equal.add(Sign.Equal);
        signs_equal.add(Sign.Equal);
        signs_equal.add(Sign.Equal);
        Simplex.showSimplexDebugLog = true;

        Simplex sym_0 = new Simplex(A,new Vector(  2.0,  3.0 ), signs_less, b);
        sym_0.solve(SimplexProblemType.Max);

        System.out.println("\n f(x,c) = -2x1 + 3x2;\n arg_min = {7, 0}, f(arg_min) =-14\n");
        Simplex sym_1 = new Simplex(A, new Vector(-2.0,  3.0), signs_less, b);
        sym_1.solve(SimplexProblemType.Min);


        System.out.println("/////////////////////////////");
        System.out.println(" f(x,c) =  2x1 + 3x2;\n arg_min = {62/5, 54/5}, f(arg_max) = 57 1/5");
        System.out.println(" |-2x1 + 6x2 >= 40");
        System.out.println(" | 3x1 + 2x2 >= 28");
        System.out.println(" | 2x1 -  x2 >= 14\n");
        Simplex sym_2 = new Simplex(A, new Vector(new double[] { 2, 1 }), signs_more, b);
        sym_2.solve(SimplexProblemType.Min);
        System.out.println(" f(x,c) =  -2x1 - x2;\n arg_min = {62/5, 54/5}, f(arg_max) = -35 3/5");
        Simplex sym_3 = new Simplex(A, new Vector(new double[] { -2, -1 }), signs_more, b);
        sym_3.solve(SimplexProblemType.Max);
        System.out.println(" f(x,c) =  2x1 + 3x2;\n arg_min = {none, none}, f(arg_max) = none");
        Simplex sym_4 = new Simplex(A, c,signs_equal, b);
        sym_4.solve(SimplexProblemType.Max);
    }
}

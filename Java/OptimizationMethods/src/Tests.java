public class Tests {
    public static void OneDimensionalMethodsTest()
    {
        double x_0 = 10;
        double x_1 = -1;

        System.out.println("x_0 = {x_0}, x_1 = {x_1}\n");
        System.out.println("Dihotomia   : " + OneDimensional.dihotomia   (OneDimensional.testFunc, x_0, x_1, 1e-4));
        System.out.println("GoldenRatio : " + OneDimensional.goldenRatio (OneDimensional.testFunc, x_0, x_1, 1e-4));
        System.out.println("Fibonacchi  : " + OneDimensional.fibonacci   (OneDimensional.testFunc, x_0, x_1, 1e-4) + " \n");
    }

    public static void NumericTests()
    {
        int[] number;

        number = NumericUtils.DecimalToRational(1.6666);
        System.out.print(number[0]!=0? number[0] + " ":"");
        System.out.print(number[1]!=0? number[1]+"/"+number[2] + "\n":"\n");

        number = NumericUtils.DecimalToRational(0.6666);
        System.out.print(number[0]!=0? number[0] + " ":"");
        System.out.print(number[1]!=0? number[1]+"/"+number[2] + "\n":"\n");

        number = NumericUtils.DecimalToRational(-3);
        System.out.print(number[0]!=0? number[0] + " ":"");
        System.out.print(number[1]!=0? number[1]+"/"+number[2] + "\n":"\n");

        number = NumericUtils.DecimalToRational(-0.125);
        System.out.print(number[0]!=0? number[0] + " ":"");
        System.out.print(number[1]!=0? number[1]+"/"+number[2] + "\n":"\n");

        number = NumericUtils.DecimalToRational(3.769230769230769230769);
        System.out.print(number[0]!=0? number[0] + " ":"");
        System.out.print(number[1]!=0? number[1]+"/"+number[2] + "\n":"\n");
    }
    public static void MatrixTest()throws Exception
    {
        Matrix matrix =  new Matrix( new Vector[] { new Vector(new double[] { 8, 1, 6 }),new Vector( new double[] { 3, 5, 7 }), new Vector(new double[] { 4, 9, 2 } )}) ;
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
    public static void MultiDimensionalMethodsTest()throws Exception
    {
        Vector x_0 = new Vector( new double[] { 5, 3 });
        Vector x_1 = new Vector (new double[] { 0, 0 });
        Vector x   = new Vector (new double[] { -13.0, 22 });

        System.out.println("x_0 = "+ x_0 + ", x_1 = " + x_1 + "\n") ;
        System.out.println("Dihotomia                : "+ MultiDimensional.dihotomia          (MultiDimensional.testFunc2d, x_1, x_0));
        System.out.println("Golden ratio             : "+ MultiDimensional.goldenRatio        (MultiDimensional.testFunc2d, x_1, x_0));
        System.out.println("Fibonacci                : "+ MultiDimensional.fibonacci          (MultiDimensional.testFunc2d, x_1, x_0));
        System.out.println("Per coord descend        : "+ MultiDimensional.perCoordDescend    (MultiDimensional.testFunc2d, x));
        System.out.println("Gradient descend         : "+ MultiDimensional.gradientDescend    (MultiDimensional.testFunc2d, x));
        System.out.println("Conj gradient descend    : "+ MultiDimensional.conjGradientDescend(MultiDimensional.testFunc2d, x));
        System.out.println("Newtone - Raphson        : "+ MultiDimensional.newtoneRaphson     (MultiDimensional.testFunc2d, x_1));
    }
}

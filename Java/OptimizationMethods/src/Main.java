
public class Main {
    public static  void main(String[] args)
    {

        // mathUtils.DoubleVector v  = new mathUtils.DoubleVector(1.3, 2.4, 5.6);
        // mathUtils.DoubleVector v2 = new mathUtils.DoubleVector(v);
        // v.set(1, 666.0);
        // System.out.println(v);
        // System.out.println(v2);

        // Tests.numericTests();//Pass
        Tests.VectorTest();
        Tests.matrixTest(); //Pass
        // Tests.oneDimensionalMethodsTest();//Pass
        // Tests.multiDimensionalMethodsTest();//Pass
        // Tests.simplexTest();//Pass
        // Simplex.showSimplexDebugLog = true;
        // Simplex sym_0 = new Simplex(
        //            /*A*/new mathUtils.Matrix(new Vector( 1.0, -2.0),
        //                            new Vector( -1.0, 1.0),
        //                            new Vector( 1.0, -1.0)),
        //            /*c*/new Vector( 1.0,  -2.0),
        // /*inequalities*/new ArrayList<>(Arrays.asList(Sign.Less, Sign.More, Sign.Less)),
        //            /*b*/new Vector( 6.0, 2.0, 4.0));
        // sym_0.solve(SimplexProblemType.Min);
    }
}

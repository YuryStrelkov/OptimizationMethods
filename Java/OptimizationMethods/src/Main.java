import java.util.ArrayList;
import java.util.Arrays;

public class Main {
    public static  void main(String[] args)
    {
        //Tests.oneDimensionalMethodsTest();//Pass
        //Tests.multiDimensionalMethodsTest();//Pass
        //Tests.numericTests();//Pass
        //Tests.matrixTest();//Pass
        //Tests.simplexTest();//Pass
        Simplex.showSimplexDebugLog = true;
        Simplex sym_0 = new Simplex(
                   /*A*/new Matrix(new Vector( 1.0, -2.0),
                                   new Vector( -1.0, 1.0),
                                   new Vector( 1.0, -1.0)),
                   /*c*/new Vector( 1.0,  -2.0),
        /*inequalities*/new ArrayList<>(Arrays.asList(Sign.Less, Sign.More, Sign.Less)),
                   /*b*/new Vector( 6.0, 2.0, 4.0));
        sym_0.solve(SimplexProblemType.Min);
    }
}

import java.util.ArrayList;
import java.util.Arrays;

public class Main {
    public static  void main(String[] args)
    {
        Tests.oneDimensionalMethodsTest();//Pass
        Tests.multiDimensionalMethodsTest();//Pass
        Tests.numericTests();//Pass
        Tests.matrixTest();//Pass
        Tests.symplexTest();//Pass
        Simplex.showSimplexDebugLog = true;
        Simplex sym_0 = new Simplex(
                   /*A*/new Matrix(new Vector(-1.0, 3.0),
                                   new Vector( 1.0, -5.0),
                                   new Vector( 4.0,-3.0),
                                   new Vector(-4.0, 1.0)),
                  /*c*/new Vector( 5.0,  1.0),
              /*ineqs*/new ArrayList<>(Arrays.asList(Sign.Less, Sign.Less, Sign.Less, Sign.More)),
                  /*b*/new Vector( 44.0,  22.0, 16.0, 8.0));
        sym_0.solve(SimplexProblemType.Max);
    }
}

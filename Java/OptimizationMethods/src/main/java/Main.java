import mathUtils.*;

public class Main {
    public static  void main(String[] args)
    {
        TemplateVector<Float> vector = new TemplateVector<>(1.0f, 2.0f, 3.0f, 4.0f, 5.0f);
        System.out.println(vector);
        System.out.println(vector.insert(0, 111.0f));
        System.out.println(vector.insert(3, 333.0f));
        System.out.println(vector.insert(vector.size(), 999.0f));
        System.out.println(vector.pushBack(1.333f));
        System.out.println(vector.pushBack(1.333f));
        System.out.println(vector.pushBack(1.333f));
        System.out.println(vector.pushBack(1.333f));
        System.out.println(vector.pushBack(1.333f));
        System.out.println(vector.pushBack(1.333f));
        System.out.println(vector.pushBack(1.333f));
        System.out.println(vector.pushBack(1.333f));
        System.out.println(vector.pushBack(1.333f));
        System.out.println(vector.pushBack(9.999f));
        System.out.println(vector.removeAt(0));
        System.out.println(vector.removeAt(2));
        System.out.println(vector.removeAt(vector.size() - 1));
        System.out.println(new TemplateVector<>(vector));
        System.out.println(vector.get(new Slice(0, -1, 2)));

        // Tests.numericTests(); //Pass
        Tests.VectorTest(); //Pass
        Tests.matrixTest(); //Pass
        // Tests.oneDimensionalMethodsTest(); //Pass
        // Tests.multiDimensionalMethodsTest(); //Pass
        // Tests.simplexTest(); //Pass
        // TemplateVector<Sign> signs_equal = new TemplateVector<>(Sign.More, Sign.Less, Sign.Less);
        // Simplex sym_0 = new Simplex(
        //            /*A*/new DoubleMatrix(new DoubleVector( 1.0, 1.0),
        //                                  new DoubleVector( 2.0, -1.0),
        //                                  new DoubleVector( 1.0, 4.0)),
        //            /*c*/new DoubleVector( -1.0,  1.0),
        // /*inequalities*/signs_equal,
        //            /*b*/new DoubleVector( 5.0, 7.0, 17.0));
        // NumericCommon.SHOW_SIMPLEX_DEBUG_LOG = true;
        // sym_0.solve(SimplexProblemType.Max);
    }
}

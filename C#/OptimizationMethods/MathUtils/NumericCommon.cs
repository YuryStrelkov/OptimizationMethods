namespace MathUtils
{
    public class NumericCommon
    {
        public static readonly double PHI = 1.61803398874989484820;
        public static readonly double PSI = 1.0 / PHI;
        public static readonly double NUMERIC_ACCURACY_MIDDLE = 1e-6;
        public static readonly double NUMERIC_ACCURACY_LOW = 1e-3;
        public static readonly double NUMERIC_ACCURACY_HIGH = 1e-9;
        public static readonly int    ZERO_ORDER_METHODS_MAX_ITERATIONS_COUNT = 1000;
        public static readonly int    ITERATIONS_COUNT_HIGH = 1000;
        public static readonly int    ITERATIONS_COUNT_MIDDLE = 500;
        public static readonly int    ITERATIONS_COUNT_LOW = 100;
        public static bool            SHOW_ZERO_ORDER_METHODS_DEBUG_LOG = false;
        public static bool            SHOW_DEBUG_LOG = false;
        public static bool            SHOW_SIMPLEX_DEBUG_LOG = false;
        public static bool            SHOW_MATRIX_DEBUG_LOG = false;
    }

}

package mathUtils;

import functionalInterfaces.IFunction1D;
import functionalInterfaces.IFunctionND;

public class NumericUtils
{
    public static final IFunction1D testFunc1d = NumericUtils::testFunc1D;

    public static final IFunctionND testFunc2d = NumericUtils::_testFunc2D;

    public static final IFunctionND testFuncNd = NumericUtils::_testFuncND;

    public static int clamp(int value, int min, int max)
    {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    public static float clamp(float value, float min, float max)
    {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    public static double clamp(double value, double min, double max)
    {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    public static double testFunc1D(double x) {
        return (x - 5) * (x - 2); // min at point x = 3.5
    }

    private static double _testFunc2D(DoubleVector x) {
        return (x.get(0) - 5) * x.get(0) + (x.get(1) - 3) * x.get(1); // min at point x = 2.5, y = 1.5
    }

    private static double _testFuncND(DoubleVector x) {
        double val = 0.0;

        for (int i = 0; i < x.size(); i++) {
            val += (x.get(i) - i) * x.get(i);
        }
        return val; // min at point x_i = i/2, i from 0 to x.Size-1
    }

    public static int[] fibonacciNumbers(int index) {
        if (index < 1) return new int[]{0};
        if (index < 2) return new int[]{0, 1};
        int[] res = new int[index];
        res[0] = 0;
        res[1] = 1;
        for (int i = 2; i < index; i++) res[i] = res[i - 1] + res[i - 2];
        return res;
    }

    public static int[] closestFibonacciPair(double value) {
        int f_n = 0;
        int f_n_1 = 0;
        if (value < 1) return new int[]{0};
        f_n_1 = 1;
        if (value < 2) return new int[]{0, 1};
        int f_tmp;
        while (f_n < value) {
            f_tmp = f_n;
            f_n = f_n_1;
            f_n_1 += f_tmp;
        }
        return new int[]{f_n, f_n_1};
    }
    /**
     * Конвертирует десятичную запись числа в рациональную,
     * например, 1.666 -> [1, 2, 3]
     *
     * Converts decimal representation of number into rational,
     * ex. 1.666 -> [1, 2, 3]
     *
     * @param value исходное число | original number
     * @param max_den максимально допустимый знаменатель | max denominator of number
     * @return int[]{integer_part, numerator, denominator}
     */
    public static int[] decimalToRational(double value, int max_den)
    {
         long m00 = 1;
         long m01 = 0;
         long m10 = 0;
         long m11 = 1;

        int[] number = new int[3];

        long ai;

        double x;

        int sign = value >= 0 ? 1 : -1;

        x = Math.abs(value);

        long t;

        while (m10 * (ai = (long)x) + m11 <= max_den)
        {
            t =m00 * ai + m01;

            m01 = m00;
            m00 = t;

            t = m10 * ai + m11;

            m11 = m10;
            m10 = t;
            if (x == (double)ai)
            {
                break;
            }   // AF: division by zero
            x = 1 / (x - (double)ai);
            if (x > (double)0x7FFFFFFF)
            {
                break;
            }  // AF: representation failure
        }

        if ((number[0] = (int)(m00 / m10)) != 0)
        {
            number[1] = (int)(m00 - number[0] * m10);

            number[0] *= sign;

            number[2] = (int)m10;

            return number;
        }
        number[1]  = (int)(sign * m00);

        number[2]  = (int)m10;

        return number;
    }

    public static int[] decimalToRational(double value)
    {
        return decimalToRational(value, NumericCommon.ITERATIONS_COUNT_HIGH);
    }

    public static String toRationalStr(double value, boolean fullRational)
    {
        int[] number =  NumericUtils.decimalToRational(value);
        if (number[1] == 0)
        {
            return String.valueOf(number[0]);
        }
        if (number[0] == 0)
        {
            return String.format("%s/%s", number[1], number[2]);
        }

        if (fullRational)
        {
            return String.format("%s/%s",
                    (number[1] + Math.abs(number[0]) * number[2]) * (number[0] >= 0 ? 1 : -1), number[2]);
        }
        return String.format("%s %s/%s", number[0], number[1], number[2]);
    }

    public static String toRationalStr(double value)
    {
        return toRationalStr(value, true);
    }

    public static String toRationalStr(DoubleVector value, boolean fullRational)
    {
        return String.format("{ %s }",
                String.join("; ", DoubleVector.map(value, v->toRationalStr(v, fullRational))));
    }

    public static String toRationalStr(DoubleVector value)
    {
        return toRationalStr( value,  false);
    }
}

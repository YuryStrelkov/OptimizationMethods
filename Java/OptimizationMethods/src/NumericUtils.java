public class NumericUtils
{
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
        number[0]  = 0;

        number[1]  = (int)(sign * m00);

        number[2]  = (int)m10;

        return number;
    }

    public static int[] decimalToRational(double value)
    {
        return decimalToRational(value,1000);
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
            return String.valueOf(number[1]) + "/" + String.valueOf(number[2]);
        }

        if (fullRational)
        {
            return String.valueOf((number[1] + Math.abs(number[0]) * number[2]) * (number[0] >= 0 ? 1 : -1)) + "/" + String.valueOf(number[2]);
        }
        return String.valueOf(number[0]) + " " + String.valueOf(number[1]) + "/" + String.valueOf(number[2]);
    }

    public static String toRationalStr(double value)
    {
        return toRationalStr(value, true);
    }

    public static String toRationalStr(Vector value, boolean fullRational)
    {
        StringBuilder str = new StringBuilder("{ ");
        for (int i = 0; i < value.size() - 1; i++)
        {
            str.append(toRationalStr(value.get(i), fullRational));
            str.append(", ");
        }
        str.append(toRationalStr(value.get(value.size() - 1), fullRational));

        str.append(" }");
        return str.toString();
    }
    public static String toRationalStr(Vector value)
    {
        return toRationalStr( value,  true);
    }

}

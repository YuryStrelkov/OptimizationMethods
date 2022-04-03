public class NumericUtils
{
    /// <summary>
    /// Конвертирует десятичную запись числа в рациональную, например, для числа 1.666 получим 1 2/3
    /// </summary>
    /// <param name="value">исходное число</param>
    /// <param name="rational_part">целые части</param>
    /// <param name="nominator">числитель</param>
    /// <param name="denominator">знаменатель</param>
    /// <param name="max_den">максимально допустимый знаменатель</param>
    public static int[] DecimalToRational(double value, int max_den)
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

    public static int[] DecimalToRational(double value)
    {
        return DecimalToRational(value,1000);
    }
}

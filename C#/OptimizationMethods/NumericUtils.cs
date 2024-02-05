using System;

namespace OptimizationMethods
{
    public static class NumericUtils
    {
        private struct Mat2
        {
            public long m00;
            public long m01;
            public long m10;
            public long m11;
            public static Mat2 Identity() => new Mat2() { m00 = 1, m01 = 0, m10 = 0, m11 = 1 };
        }

        /// <summary>
        /// Конвертирует десятичную запись числа в рациональную, например, для числа 1.666 получим 1 2/3
        /// </summary>
        /// <param name="value">исходное число</param>
        /// <param name="rational_part">целые части</param>
        /// <param name="nominator">числитель</param>
        /// <param name="denominator">знаменатель</param>
        /// <param name="max_den">максимально допустимый знаменатель</param>
        public static void DecimalToRational(double value, out int rational_part, out int numerator, out int denominator, int max_den = 1000)
        {
            Mat2 m = Mat2.Identity();

            long ai;

            double x;

            int sign = value >= 0 ? 1 : -1;

            x = Math.Abs(value);
           
            long t;

            while (m.m10 * (ai = (long)x) + m.m11 <= max_den)
            {
                t = m.m00 * ai + m.m01;
            
                m.m01 = m.m00;
                m.m00 = t;
                
                t = m.m10 * ai + m.m11;
                
                m.m11 = m.m10;
                m.m10 = t;

                if (x == (double)ai) break; // AF: division by zero
                
                x = 1 / (x - (double)ai);
                
                if (x > (double)0x7FFFFFFF) break; // AF: representation failure
            }

            if ((rational_part = (int)(m.m00 / m.m10)) != 0)
            {
                numerator = (int)(m.m00 - rational_part * m.m10);

                rational_part *= sign;

                denominator = (int)m.m10;

                return;
            }
            rational_part = 0;

            numerator = (int)(sign * m.m00);

            denominator = (int)m.m10;
        }

        public static string ToRationalStr(double value, bool fullRational = true)
        {
            int r_part, num, denom;
            DecimalToRational(value, out r_part, out num, out denom);
            if (num    == 0)  return $"{r_part}";
            if (r_part == 0)  return $"{num}/{denom}";
            if (fullRational) return $"{(num + Math.Abs(r_part) * denom) * (r_part >= 0 ? 1 : -1)}/{denom}";
            return $"{r_part} {num}/{denom}";
        }

        public static string ToRationalStr(Vector vector, bool fullRational = true) => $"{{{string.Join(", ", vector.Map((v) => ToRationalStr(v, fullRational)))}}}";
    }
}

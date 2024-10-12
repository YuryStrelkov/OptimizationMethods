using System.Collections.Generic;
using System;

namespace MathUtils
{
    public static class NumericUtils
    {

        public static int Clamp(int value, int min, int max)
        {
            if (value < min) return min;
            if (value > max) return max;
            return value;
        }

        public static float Clamp(float value, float min, float max)
        {
            if (value < min) return min;
            if (value > max) return max;
            return value;
        }

        public static double Clamp(double value, double min, double max)
        {
            if (value < min) return min;
            if (value > max) return max;
            return value;
        }

        public static void Swap<T>(ref T lhs, ref T rhs)
        {
            T temp;
            temp = lhs;
            lhs = rhs;
            rhs = temp;
        }

        public static double TestFunc1D(double x)
        {
            return (x - 5) * (x - 2); // min at point x = 3.5
        }

        public static double TestFunc2D(DoubleVector x)
        {
            return (x[0] - 5) * x[0] + (x[1] - 3) * x[1]; // min at point x = 2.5, y = 1.5
        }

        public static double TestFuncND(DoubleVector x)
        {
            double val = 0.0;

            for (int i = 0; i < x.Count; i++) val += (x[i] - i) * x[i];

            return val; // min at point x_i = i/2, i from 0 to x.Size-1
        }

        public static int[] FibonacchiNumbers(int index)
        {
            if (index < 1) return new int[] { 0 };

            if (index < 2) return new int[] { 0, 1 };

            int[] res = new int[index];

            res[0] = 0;

            res[1] = 1;

            for (int i = 2; i < index; i++) res[i] = res[i - 1] + res[i - 2];

            return res;
        }

        public static void ClosestFibonacciPair(double value, ref int f_n, ref int f_n_1)
        {
            f_n = 0;
            f_n_1 = 0;

            if (value < 1) return;

            f_n_1 = 1;

            if (value < 2) return;
            int f_tmp;

            while (f_n < value)
            {
                f_tmp = f_n;
                f_n = f_n_1;
                f_n_1 += f_tmp;
            }
        }

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
            return ToRationalStr(r_part, num, denom);
        }
        private static string ToRationalStr(int r_part, int num, int denom, bool fullRational = true) 
        {
            if (num == 0) return $"{r_part}";
            if (r_part == 0) return $"{num}/{denom}";
            if (fullRational) return $"{(num + Math.Abs(r_part) * denom) * (r_part >= 0 ? 1 : -1)}/{denom}";
            return $"{r_part} {num}/{denom}";
        }


        public static string ToRationalStr(IEnumerable<double> DoubleVector, bool fullRational = true) => $"{{{string.Join(", ", DoubleVector.Map(v => ToRationalStr(v, fullRational)))}}}";

        public struct RationalNumber: IEquatable<RationalNumber>
        {
            private readonly int _numerator;
            private readonly int _denominator;
            private readonly int _intPart;
            public bool Equals(RationalNumber number)
            {
                if (number.Numerator   != Numerator) return false;
                if (number.Denominator != Denominator) return false;
                if (number.IntPart     != IntPart) return false;
                return true;
            }
            
            public override string ToString() => ToRationalStr(IntPart, Numerator, Denominator, false);

            public int Numerator => _numerator;
            public int Denominator => _denominator;
            public int IntPart => _intPart;

            public RationalNumber(int numerator, int denominator, int intPart)
            {
                _numerator = numerator;
                _denominator = denominator;
                _intPart = intPart;
            }

            public RationalNumber(RationalNumber other): this(other.IntPart, other.Numerator, other.Denominator){}

            public RationalNumber(double value) => DecimalToRational(value, out _intPart, out _numerator, out _denominator);
        }

    }
}

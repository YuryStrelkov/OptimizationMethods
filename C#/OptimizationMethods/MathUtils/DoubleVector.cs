using System.Diagnostics.CodeAnalysis;
using System.Collections.Generic;
using System.Globalization;
using System;

namespace MathUtils
{
    public delegate double FunctionND(DoubleVector x);
    public sealed class DoubleVector: TemplateVector<double>, IEquatable<DoubleVector>
    {
        /// <summary>
        /// Длина вектра
        /// </summary>
        public double MagnitudeSqr => this.Reduce((accum, value) => accum += value * value);

        /// <summary>
        /// Длина вектра
        /// </summary>
        public double Magnitude => Math.Sqrt(MagnitudeSqr);

        /// <summary>
        /// Нормированый вариант вектора
        /// </summary>
        public DoubleVector Normalized
        {
            get
            {
                DoubleVector normalized = new DoubleVector(this);
                double inv_mag = 1.0 / Magnitude;
                normalized.Apply(v => v * inv_mag);
                return normalized; 
            }
        }

        /// <summary>
        /// Нормализация вектора
        /// </summary>
        /// <returns></returns>
        public DoubleVector Normalize()
        {
            double inv_mag = 1.0 / Magnitude;
            Apply(v => v * inv_mag);
            return this;
        }

        /// <summary>
        /// Скалярное произведение (this;other)
        /// </summary>
        /// <param name="other"></param>
        /// <returns>(this;other)</returns>
        public double Dot(DoubleVector other)
        {
            if (Count != other.Count) throw new Exception("Unable DoubleVector dot multiply");
            return this.Combine(other, (l, r) => l * r).Reduce((acc, v) => acc + v);
        }

        /// <summary>
        /// Скалярное произведение (a;b)
        /// </summary>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <returns>(a;b)</returns>
        public static double Dot(DoubleVector left, DoubleVector right) => left.Dot(right);

        /// <summary>
        /// Строковое представление вектора
        /// </summary>
        /// <returns></returns>
        public override string ToString() => $"{{{string.Join(", ", this.Map(v => v.ToString("0.000", CultureInfo.InvariantCulture)))}}}";

        public bool Equals([AllowNull] DoubleVector other) => base.Equals(other);

        /// <summary>
        /// Конструктор вектора из массива
        /// </summary>
        /// <param name="args"></param>
        public DoubleVector(): base() {}
        
        /// <summary>
        /// Конструктор вектора из массива
        /// </summary>
        /// <param name="args"></param>
        public DoubleVector(params double[] args): base(args) {}
       
        /// <summary>
        /// Конструктор вектора по размеру и элементу по умолчанию
        /// </summary>
        /// <param name="cap"></param>
        public DoubleVector(int cap): base(cap) {}

        /// <summary>
        /// Конструктор копирования
        /// </summary>
        /// <param name="vect"></param>
        public DoubleVector(DoubleVector other): base(other) {}
        public DoubleVector(IEnumerable<double> other) : base(other) { }

        /// <summary>
        /// Элементарные математические операции над векторами
        /// </summary>

        ///////////////////////////
        /////    Operator +   /////
        ///////////////////////////
        public static DoubleVector operator + (DoubleVector left, DoubleVector right)
        {
            if (left.Count != right.Count) throw new Exception("error:: operator + :: DoubleVectors of different dimensions");
            return new DoubleVector(left.Combine(right, (l, r) => l + r));
        }
        public static DoubleVector operator + (DoubleVector left, double right) => new DoubleVector(left.Combine(right, (l, r) => l + r));
        public static DoubleVector operator + (double left, DoubleVector right) => new DoubleVector(right.Combine(left, (l, r) => r + l));

        ///////////////////////////
        /////    Operator -   /////
        ///////////////////////////
        public static DoubleVector operator - (DoubleVector left, DoubleVector right)
        {
            if (left.Count != right.Count) throw new Exception("error:: operator - :: DoubleVectors of different dimensions");
            return new DoubleVector(left.Combine(right, (l, r) => l - r));
        }
        public static DoubleVector operator - (DoubleVector left, double right) => new DoubleVector(left.Combine(right, (l, r) => l - r));
        public static DoubleVector operator - (double left, DoubleVector right) => new DoubleVector(right.Combine(left, (l, r) => r - l));
    
        ///////////////////////////
        /////    Operator *   /////
        ///////////////////////////
        public static DoubleVector operator * (DoubleVector left, DoubleVector right)
        {
            if (left.Count != right.Count) throw new Exception("error :: operator * :: DoubleVectors of different dimensions");
            return new DoubleVector(left.Combine(right, (l, r) => l * r));
        }
        public static DoubleVector operator * (DoubleVector left, double right) => new DoubleVector(left.Combine(right, (l, r) => l * r));
        public static DoubleVector operator * (double left, DoubleVector right) => new DoubleVector(right.Combine(left, (l, r) => r * l));

        ///////////////////////////
        /////    Operator /   /////
        ///////////////////////////
        public static DoubleVector operator / (DoubleVector left, DoubleVector right)
        {
            if (left.Count != right.Count) throw new Exception("error :: operator / :: DoubleVectors of different dimensions");
            return new DoubleVector(left.Combine(right, (l, r) => l / r));
        }
        public static DoubleVector operator / (DoubleVector left, double right) => new DoubleVector(left.Combine(right, (l, r) => l / r));
        public static DoubleVector operator / (double left, DoubleVector right) => new DoubleVector(right.Combine(left, (l, r) => r / l));

        /// <summary>
        /// Рассчитывет единичный вектор в направлении от a до b
        /// </summary>
        /// <param name="left"></param>
        /// <param name="right"></param>
        /// <returns></returns>
        public static DoubleVector Direction(DoubleVector left, DoubleVector right)
        {
            if (left.Count != right.Count) throw new Exception("error :: dirction :: DoubleVectors of different dimensions");
            return (right - left).Normalize();
        }

        public static double Distance(DoubleVector left, DoubleVector right)
        {
            if (left.Count != right.Count) throw new Exception("error :: dirction :: DoubleVectors of different dimensions");
            return right.Combine(left, (l, r) => r - l).Reduce((accum, itеm) => accum + itеm * itеm);
        }

        /// <summary>
        /// Градиент скалярной функции векторного аргумента 
        /// </summary>
        /// <param name="func">функция для которой рассчитываем градиент</param>
        /// <param name="x">   точка, где рассчитываем градиент</param>
        /// <param name="eps"> шаг центрального разностного аналога</param>
        /// <returns></returns>
        public static DoubleVector Gradient(FunctionND func, DoubleVector x, double eps)
        {
            DoubleVector df = new DoubleVector(x.Count);
            for (int i = 0; i < x.Count; i++) df.PushBack(Partial(func, x, i, eps));
            return df;
        }
        public static DoubleVector Gradient(FunctionND func, DoubleVector x) => Gradient(func, x, NumericCommon.NUMERIC_ACCURACY_MIDDLE);
        
        /// <summary>
        /// Частная производная в точке x вдоль координаты coord_index
        /// </summary>
        /// <param name="func"></param>
        /// <param name="x"></param>
        /// <param name="coord"></param>
        /// <param name="eps"></param>
        /// <returns></returns>
        public static double Partial (FunctionND func, DoubleVector x, int coord_index, double eps)
        {
            if (x.NotInRange(coord_index)) throw new Exception("Partial derivative index out of bounds!");
            x[coord_index] += eps;
            double f_r = func(x);
            x[coord_index] -= (2.0 * eps);
            double f_l = func(x);
            x[coord_index] += eps;
            return (f_r - f_l) / eps * 0.5;
        }

        public static double Partial(FunctionND func, DoubleVector x, int coord_index) => Partial(func, x, coord_index, NumericCommon.NUMERIC_ACCURACY_MIDDLE);

        public static double Partial2(FunctionND func, DoubleVector x, int coord_index_1, int coord_index_2, double eps)
        {
            if (x.NotInRange(coord_index_2)) throw new Exception("Partial derivative index out of bounds!");
            x[coord_index_2] -= eps;
            double f_l = Partial(func, x, coord_index_1, eps);
            x[coord_index_2] += (2 * eps);
            double f_r = Partial(func, x, coord_index_1, eps);
            x[coord_index_2] -= eps;
            return (f_r - f_l) / eps * 0.5;
        }

        public static double Partial2(FunctionND func, DoubleVector x, int coord_index_1, int coord_index_2) => Partial2(func, x, coord_index_1, coord_index_2, NumericCommon.NUMERIC_ACCURACY_MIDDLE);

    }
}

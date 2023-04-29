using System.Diagnostics.CodeAnalysis;
using System.Collections;
using System.Text;
using System;

namespace OptimizationMethods
{
    public delegate double FunctionND(Vector x);

    public class Vector: TemplateVector<double>
    { 
        /// <summary>
        /// Длина вектра
        /// </summary>
        public double Magnitude
        {
            get
            {
                double mag = 0.0;
                foreach (double element in this) mag += (element * element);
                return Math.Sqrt(mag);
            }
        }
        /// <summary>
        /// Нормированый вариант вектора
        /// </summary>
        public Vector Normalized
        {
            get
            {
                Vector v = new Vector(this);
                double inv_mag = 1.0 / v.Magnitude;
                for (int i = 0; i < v.Count; i++) v[i] *= inv_mag;
                return v;
            }
        }
        /// <summary>
        /// Нормализация вектора
        /// </summary>
        /// <returns></returns>
        public Vector Normalize()
        {
            double inv_mag = 1.0 / Magnitude;
            for (int i = 0; i < Count; i++) this[i] *= inv_mag;
            return this;
        }
        /// <summary>
        /// Скалярное произведение (this;other)
        /// </summary>
        /// <param name="other"></param>
        /// <returns>(this;other)</returns>
        public double Dot(Vector other)
        {
            if (Count != other.Count) throw new Exception("Unable vector dot multiply");
            double dot = 0.0;
            foreach (var p in Zip(this, other)) dot += p.First * p.Second;
            return dot;
        }
        /// <summary>
        /// Строковое представление вектора
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append("{ ");
            foreach (var v in this)
            {
                sb.Append(string.Format("{0,0}; ", String.Format("{0:0.000}", v)));
            }
            sb.Remove(sb.Length - 2, 2);
            sb.Append(" }");
            return sb.ToString();
        }

        /// <summary>
        /// Конструктор вектора из массива
        /// </summary>
        /// <param name="args"></param>
        public Vector() : base() { }
        /// <summary>
        /// Конструктор вектора из массива
        /// </summary>
        /// <param name="args"></param>
        public Vector(params double[] args):base(args){}
        /// <summary>
        /// Конструктор вектора по размеру и элементу по умолчанию
        /// </summary>
        /// <param name="cap"></param>
        /// <param name="defaultValue"></param>
        public Vector(int cap):base(cap) {}

        /// <summary>
        /// Конструктор копирования
        /// </summary>
        /// <param name="vect"></param>
        public Vector(Vector other) : base(other) { }

        /// <summary>
        /// Элементарные математические операции над векторами
        /// </summary>
        public static Vector operator +(Vector a, Vector b)
        {
            if (a.Count != b.Count) throw new Exception("error:: operator+:: vectors of different dimensions");
            Vector res = new Vector(a.Count);
            foreach (var p in Zip(a, b)) res.PushBack(p.First + p.Second);
            return res;
        }
        public static Vector operator +(Vector a, double b)
        {
            Vector res = new Vector(a.Count);
            foreach (double v in a) res.PushBack(v + b);
            return res;
        }
        public static Vector operator +(double b, Vector a)
        {
            return a + b;
        }
        public static Vector operator -(Vector a, Vector b)
        {
            if (a.Count != b.Count) throw new Exception("error:: operator-:: vectors of different dimensions");
            Vector res = new Vector(a.Count);
            foreach (var p in Zip(a, b)) res.PushBack(p.First - p.Second);
            return res;
        }
        public static Vector operator -(Vector a, double b)
        {
            Vector res = new Vector(a.Count);
            foreach (double v in a) res.PushBack(v - b);
            return res;
        }
        public static Vector operator -(double b, Vector a)
        {
            Vector res = new Vector(a.Count);
            foreach (double v in a) res.PushBack(b - v);
            return res;
        }
        public static Vector operator *(Vector a, double val)
        {
            Vector res = new Vector(a.Count);
            foreach (double v in a) res.PushBack(v * val);
            return res;
        }
        public static Vector operator *(double val, Vector a)
        {
            return a * val;
        }
        /// <summary>
        /// Рассчитывет единичный вектор в направлении от a до b
        /// </summary>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <returns></returns>
        public static Vector Direction(Vector a, Vector b)
        {
            if (a.Count != b.Count) return a;
            return (b - a).Normalized;
        }
        /// <summary>
        /// Градиент скалярной функции векторного аргумента 
        /// </summary>
        /// <param name="func">функция для которой рассчитываем градиент</param>
        /// <param name="x">   точка, где рассчитываем градиент</param>
        /// <param name="eps"> шаг центрального разностного аналога</param>
        /// <returns></returns>
        public static Vector Gradient(FunctionND func, Vector x, double eps = 1e-6)
        {
            Vector df = new Vector(x.Count);

            for (int i = 0; i < x.Count; i++) df[i] = Partial(func, x, i, eps);
            
            return df;
        }
        /// <summary>
        /// Частная производная в точке x вдоль координаты coord_index
        /// </summary>
        /// <param name="func"></param>
        /// <param name="x"></param>
        /// <param name="coord"></param>
        /// <param name="eps"></param>
        /// <returns></returns>
        public static double  Partial (FunctionND func, Vector x, int coord_index, double eps = 1e-6)
        {
            if (x.NotInRange(coord_index)) throw new Exception("Partial derivative index out of bounds!");
            x[coord_index] += eps;
            double f_r = func(x);
            x[coord_index] -= (2.0 * eps);
            double f_l = func(x);
            x[coord_index] += eps;
            return (f_r - f_l) / eps * 0.5;
        }

        public static double Partial2(FunctionND func, Vector x, int coord_index_1, int coord_index_2, double eps = 1e-6)
        {
            if (x.NotInRange(coord_index_2)) throw new Exception("Partial derivative index out of bounds!");
            x[coord_index_2] -= eps;
            double f_l = Partial(func, x, coord_index_1, eps);
            x[coord_index_2] += (2 * eps);
            double f_r = Partial(func, x, coord_index_1, eps);
            x[coord_index_2] -= eps;
            return (f_r - f_l) / eps * 0.5;
        }
        /// <summary>
        /// Скалярное произведение (a;b)
        /// </summary>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <returns>(a;b)</returns>
        public static double Dot(Vector a, Vector b) => a.Dot(b);
    }
}

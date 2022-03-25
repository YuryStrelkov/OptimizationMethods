using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;


namespace OptimizationMethods
{
    public delegate double func_n(Vector x);
    public class Vector : IEquatable<Vector>
    {
        /// <summary>
        /// Массив элементов
        /// </summary>
        private List<double> data;
        /// <summary>
        /// Размерность вектора
        /// </summary>
        public int Size
        {
            get { return data.Count; }
        }
        /// <summary>
        /// Длина вектра
        /// </summary>
        public double Magnitude
        {
            get
            {
                double mag = 0.0f;
                foreach (double element in data)
                {
                    mag += (element * element);
                }
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
                double inv_mag = 1.0f / v.Magnitude;
                for (int i = 0; i < v.Size; i++)
                {
                    v[i] *= inv_mag;
                }
                return v;
            }
        }
        /// <summary>
        /// Нормализация вектора
        /// </summary>
        /// <returns></returns>
        public Vector Normalize()
        {
            double inv_mag = 1.0f / Magnitude;
            for (int i = 0; i < Size; i++)
            {
                this[i] *= inv_mag;
            }
            return this;
        }
        /// <summary>
        /// Скалярное произведение (this;other)
        /// </summary>
        /// <param name="other"></param>
        /// <returns>(this;other)</returns>
        public double Dot(Vector other)
        {
            if (Size != other.Size)
            {
                throw new Exception("Unable vector dot multiply");
            }

            double dot = 0.0f;

            for (int i = 0; i < other.Size; i++)
            {
                dot += this[i] * other[i];
            }
            return dot;
        }

        /// <summary>
        /// Изменение размера вектора
        /// </summary>
        /// <param name="size"></param>
        public void Resize(int size)
        {
            if (size == Size)
            {
                return;
            }

            if (size > Size)
            {
                for (int i = 0; i < size - Size; i++)
                {
                    data.Add(0.0);
                }
                return;
            }

            data.RemoveRange(size, Size);
        }

        public void PushBack(double val)
        {
            data.Add(val);
        }
        /// <summary>
        /// Строковое представление вектора
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            string s = "{ ";
            for (int i = 0; i < data.Count - 1; i++)
            {
                s += string.Format("{0,0}, ", String.Format("{0:0.000}", data[i]));// .ToString();
            }
            s += string.Format("{0,0}", String.Format("{0:0.000}", data[data.Count - 1]));// data[data.Length - 1].ToString();
            s += " }";
            return s;
        }
        /// <summary>
        /// Базовое сравнение двух векторов
        /// </summary>
        /// <param name="obj"></param>
        /// <returns></returns>
        public override bool Equals(object obj)
        {
            if (!(obj is Vector))
            {
                return false;
            }
            return Equals(obj as Vector);
        }
        /// <summary>
        /// Сравнение двух векторов
        /// </summary>
        /// <param name="other"></param>
        /// <returns></returns>
        public bool Equals([AllowNull] Vector other)
        {
            if (other.Size != Size)
            {
                return false;
            }
            for (int i = 0; i < other.Size; i++)
            {
                if (other[i] != this[i])
                {
                    return false;
                }
            }
            return true;
        }
        /// <summary>
        /// Хешкод вектора
        /// </summary>
        /// <returns></returns>
        public override int GetHashCode()
        {
            return HashCode.Combine(data);
        }
        /// <summary>
        /// Получение элемента вктора
        /// </summary>
        /// <param name="id"></param>
        /// <returns></returns>
        public double this[int id]
        {
            get
            {
                return data[id];
            }
            set
            {
                data[id] = value;
            }
        }
        /// <summary>
        /// Конструктор вектора из массива
        /// </summary>
        /// <param name="_data"></param>
        public Vector(double[] _data)
        {
            data = new List<double>(_data);
        }
        /// <summary>
        /// Конструктор вектора по размеру и элементу по умолчанию
        /// </summary>
        /// <param name="size"></param>
        /// <param name="defaultValue"></param>
        public Vector(int size, double defaultValue = 0.0f)
        {
            data = new List<double>(size);
            for (int i = 0; i < size; i++)
            {
                data.Add(defaultValue);
            }
        }
        /// <summary>
        /// Конструктор копирования
        /// </summary>
        /// <param name="vect"></param>
        public Vector(Vector vect)
        {
            data = new List<double>(vect.data);
        }

        /// <summary>
        /// Элементарные математические операции над векторами
        /// </summary>
        public static Vector operator +(Vector a, Vector b)
        {
            if (a.Size != b.Size)
            {
                throw new Exception("error:: operator+:: vectors of different dimensions");
            }

            Vector res = new Vector(a);
            for (int i = 0; i < a.Size; i++)
            {
                res[i] = a[i] + b[i];
            }
            return res;
        }
        public static Vector operator +(Vector a, double b)
        {
            Vector res = new Vector(a);
            for (int i = 0; i < a.Size; i++)
            {
                res[i] = a[i] + b;
            }
            return res;
        }
        public static Vector operator +(double b, Vector a)
        {
            return a + b;
        }
        public static Vector operator -(Vector a, Vector b)
        {
            if (a.Size != b.Size)
            {
                throw new Exception("error:: operator-:: vectors of different dimensions");
            }

            Vector res = new Vector(a);
            for (int i = 0; i < a.Size; i++)
            {
                res[i] = a[i] - b[i];
            }
            return res;
        }
        public static Vector operator -(Vector a, double b)
        {
            Vector res = new Vector(a);
            for (int i = 0; i < a.Size; i++)
            {
                res[i] = a[i] - b;
            }
            return res;
        }
        public static Vector operator -(double b, Vector a)
        {
            Vector res = new Vector(a);
            for (int i = 0; i < a.Size; i++)
            {
                res[i] = b - a[i];
            }
            return res;
        }
        public static Vector operator *(Vector a, double val)
        {
            Vector res = new Vector(a);
            for (int i = 0; i < a.Size; i++)
            {
                res[i] = a[i] * val;
            }
            return res;
        }
        public static Vector operator *(double val, Vector a)
        {
            return a * val;
        }
        /// <summary>
        /// Позволяет при иницилизации экземпляра класса вместо:
        /// double [] vals = new double[] {1,2,3};
        /// Vector v = new Vector(rows);
        /// делать так:
        /// Vector v = vals;
        /// </summary>
        /// <param name="value"></param>
        public static implicit operator Vector(double[] value)
        {
            return new Vector(value);
        }
        /// <summary>
        /// Рассчитывет единичный вектор в направлении от a до b
        /// </summary>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <returns></returns>
        public static Vector Direction(Vector a, Vector b)
        {
            if (a.Size != b.Size)
            {
                return a;
            }
            return (b - a).Normalized;
        }
        /// <summary>
        /// Градиент скалярной функции векторного аргумента 
        /// </summary>
        /// <param name="func">функция для которой рассчитываем градиент</param>
        /// <param name="x">   точка, где рассчитываем градиент</param>
        /// <param name="eps"> шаг центрального разностного аналога</param>
        /// <returns></returns>
        public static Vector Gradient(func_n func, Vector x, double eps = 1e-6)
        {
            Vector x_l = new Vector(x);
            Vector x_r = new Vector(x);
            Vector df = new Vector(x.Size);
            for (int i = 0; i < x.Size; i++)
            {
                x_l[i] -= eps;
                x_r[i] += eps;

                df[i] = (func(x_r) - func(x_l)) * (0.5f / eps);

                x_l[i] += eps;
                x_r[i] -= eps;
            }
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
        public static double  Partial (func_n func, Vector x, int coord_index, double eps = 1e-6)
        {
            if (x.Size <= coord_index)
            {
                throw new Exception("Partial derivative index out of bounds!");
            }
            x[coord_index] += eps;
            double f_r = func(x);
            x[coord_index] -= (2.0f * eps);
            double f_l = func(x);
            x[coord_index] += eps;
            return (f_r - f_l) / eps * 0.5f;
        }

        public static double Partial2(func_n func, Vector x, int coord_index_1, int coord_index_2, double eps = 1e-6)
        {
            if (x.Size <= coord_index_2)
            {
                throw new Exception("Partial derivative index out of bounds!");
            }
            x[coord_index_2] -= eps;
            double f_l = Partial(func, x, coord_index_1, eps);
            x[coord_index_2] += (2 * eps);
            double f_r = Partial(func, x, coord_index_1, eps);
            x[coord_index_2] -= eps;
            return (f_r - f_l) / eps * 0.5f;
        }
        /// <summary>
        /// Скалярное произведение (a;b)
        /// </summary>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <returns>(a;b)</returns>
        public static double Dot(Vector a, Vector b)
        {
            return a.Dot(b);
        }
    }
}

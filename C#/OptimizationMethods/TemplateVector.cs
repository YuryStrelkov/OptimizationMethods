using System.Diagnostics.CodeAnalysis;
using System.Text;
using System;
using System.Collections.Generic;
using System.Collections;

namespace OptimizationMethods
{
    public readonly struct TemplatePair<T>
    {
        public readonly T First;
        public readonly T Second;
        public TemplatePair(T First, T Second)
        {
            this.First = First;
            this.Second = Second;
        }
    }

    public class TemplateVector<T>: IEquatable<TemplateVector<T>>, IEnumerable<T>, ICloneable where T : new()
    {
        public struct VectorEnumerator : IEnumerator<T>
        {
            int index;

            private readonly TemplateVector<T> _enumerableVector;
            public T Current
            {
                get
                {
                    try
                    {
                        return _enumerableVector._data[index];
                    }
                    catch (IndexOutOfRangeException)
                    {
                        throw new InvalidOperationException();
                    }
                }
            }
            object IEnumerator.Current => Current;
            public bool MoveNext()
            {
                index++;
                return (index < _enumerableVector._filling);
            }
            public void Reset() => index = -1;
            public void Dispose(){}
            public VectorEnumerator(TemplateVector<T> v)
            {
                _enumerableVector = v;
                index = -1;
            }
        }

        public struct VectorsZipEnumerator : IEnumerator<TemplatePair<T>>, IEnumerable<TemplatePair<T>>
        {
            int index;

            private readonly TemplateVector<T> _enumerableVectorFirst;

            private readonly TemplateVector<T> _enumerableVectorSecond;
            // IEnumerator
            object IEnumerator.Current => Current;
            public TemplatePair<T> Current
            {
                get
                {
                    try
                    {
                        return new TemplatePair<T>(_enumerableVectorFirst._data[index], _enumerableVectorSecond._data[index]);
                    }
                    catch (IndexOutOfRangeException)
                    {
                        throw new InvalidOperationException();
                    }
                }
            }
            public bool MoveNext()
            {
                index++;
                return (index < _enumerableVectorFirst._filling) && (index < _enumerableVectorSecond._filling);
            }
            public void Reset() => index = -1;
            // IEnumerable
            public void Dispose() { }
            public IEnumerator<TemplatePair<T>> GetEnumerator() => this;
            IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
            public VectorsZipEnumerator(TemplateVector<T> v1, TemplateVector<T> v2)
            {
                _enumerableVectorFirst = v1;
                _enumerableVectorSecond = v2;
                index = -1;
            }
        }
        public static IEnumerable<TemplatePair<T>> Zip(TemplateVector<T> first, TemplateVector<T> second) => new VectorsZipEnumerator(first, second);
        public VectorEnumerator GetEnumerator() => new VectorEnumerator(this);
        IEnumerator<T> IEnumerable<T>.GetEnumerator() => new VectorEnumerator(this);
        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();

        /// <summary>
        /// заполнение массива
        /// </summary>
        private int _filling = 0;
        /// <summary>
        /// заполнение массива
        /// </summary>
        // private int _capacity = 0;
        /// <summary>
        /// Массив элементов
        /// </summary>
        private T[] _data;
        /// <summary>
        /// Размерность вектора
        /// </summary>
        public int Count => _filling;

        /// <summary>
        /// Скалярное произведение (this;other)
        /// </summary>
        /// <param name="other"></param>
        /// <returns>(this;other)</returns>

        protected bool NotInRange(int index) => index < 0 || index >= _data.Length;

        public TemplateVector<T> PushBack(T value)
        {
            if (_filling == _data.Length)
            {
                T[] new_data = new T[(int)(Count * 1.5)];
                if (Count >= 0) Array.Copy(_data, 0, new_data, 0, Count);
                _data = new_data;
            }
            _data[_filling] = value;
            _filling++;
            return this;
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
                sb.Append(v.ToString());
                sb.Append("; ");
            }
            sb.Remove(sb.Length - 2, sb.Length - 1);
            sb.Append(" }");
            return sb.ToString();
        }
        /// <summary>
        /// Базовое сравнение двух векторов
        /// </summary>
        /// <param name="obj"></param>
        /// <returns></returns>
        public override bool Equals(object obj)
        {
            if (!(obj is Vector)) return false;
            return Equals(obj as Vector);
        }
        /// <summary>
        /// Сравнение двух векторов
        /// </summary>
        /// <param name="other"></param>
        /// <returns></returns>
        public bool Equals([AllowNull] TemplateVector<T> other)
        {
            if (other.Count != Count) return false;
            foreach (TemplatePair<T> p in Zip(this, other)) if (!p.First.Equals(p.Second)) return false;
            return true;
        }
        /// <summary>
        /// Хешкод вектора
        /// </summary>
        /// <returns></returns>
        public override int GetHashCode() => HashCode.Combine(_data);
        /// <summary>
        /// Получение элемента вктора
        /// </summary>
        /// <param name="id"></param>
        /// <returns></returns>
        public T this[int index]
        {
            get
            {
                if (NotInRange(index)) throw new Exception("get :: index " + index + " out of range");
                _filling = Math.Max(_filling, index + 1);
                return _data[index];
            }
            set
            {
                if (NotInRange(index)) throw new Exception("get :: index " + index + " out of range");
                _filling = Math.Max(_filling, index + 1);
                _data[index] = value;
            }
        }

        /// <summary>
        /// Конструктор вектора из массива
        /// </summary>
        /// <param name="args"></param>
        public TemplateVector()
        {
            _data = new T[9];
            _filling = 0;
        }

        /// <summary>
        /// Конструктор вектора из массива
        /// </summary>
        /// <param name="args"></param>
        public TemplateVector(params T[] args)
        {
            _data = new T[(int)(args.Length * 1.5)];
            _filling = 0;
            foreach (T v in args) PushBack(v);
        }
        /// <summary>
        /// Конструктор вектора по размеру и элементу по умолчанию
        /// </summary>
        /// <param name="cap"></param>
        /// <param name="defaultValue"></param>
        public TemplateVector(int cap)// T defaultValue = 0.0)
        {
            _data = new T[(int)(cap * 1.5f)];
            _filling = 0;
        }
        /// <summary>
        /// Конструктор копирования
        /// </summary>
        /// <param name="vect"></param>
        public TemplateVector(TemplateVector<T> other)
        {
            _data = new T[(int)(other.Count * 1.5)];
            _filling = other.Count;
            if (Count >= 0) Array.Copy(other._data, 0, _data, 0, Count);
        }

   
        /// <summary>
        /// Позволяет при иницилизации экземпляра класса вместо:
        /// T [] vals = new T[] {1,2,3};
        /// Vector v = new Vector(rows);
        /// делать так:
        /// Vector v = vals;
        /// </summary>
        /// <param name="value"></param>
        public static implicit operator TemplateVector<T>(T[] value) => new TemplateVector<T>(value);
    
        public object Clone()
        {
            return new TemplateVector<T>(this);
        }
    }
}

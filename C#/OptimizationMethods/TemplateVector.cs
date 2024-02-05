using System.Collections.Generic;
using System.Collections;
using System;

namespace OptimizationMethods
{
    public readonly struct TemplatePair<T, K>
    {
        public readonly T First;
        public readonly K Second;
        public TemplatePair(T First, K Second)
        {
            this.First = First;
            this.Second = Second;
        }
    }

    public delegate T    ReduceFunction <T>            (T accum, T value) ;
    public delegate T2   CombineFunction<in T1, out T2>(T1 left, T1 right);
    public delegate T2   MapFunction    <in T1, out T2>(T1 item);
    public delegate T    FillingFunction<T>            (uint index);
    public delegate T    ApplyFunction  <T>            (T value);
    public delegate bool Predicate      <T>            (T item);

    public static class VectorIteratorUtils 
    {
        public static T Reduce<T>(this IEnumerable<T> vector, ReduceFunction<T> function) where T : new()
        {
            T accumulator = new T();
            foreach (var v in vector) accumulator = function(accumulator, v);
            return accumulator;
        }
        public static IEnumerable<T2> Map<T1, T2>(this IEnumerable<T1> vector, MapFunction<T1, T2> function) 
        {
            foreach (T1 item in vector) yield return function(item);
        }
        public static IEnumerable<TemplatePair<T1, T2>> Zip<T1, T2>(this IEnumerable<T1> first, IEnumerable<T2> second)
        {
            IEnumerator<T1> iterator1 = first.GetEnumerator();
            IEnumerator<T2> iterator2 = second.GetEnumerator();
            while (iterator1.MoveNext() && iterator2.MoveNext()) yield return new TemplatePair<T1, T2>(iterator1.Current, iterator2.Current);
        }
        public static IEnumerable<T2> Combine<T1, T2>(this IEnumerable<T1> left, IEnumerable<T1> right, CombineFunction<T1, T2> function) 
        {
           foreach (var pair in left.Zip(right)) yield return function(pair.First, pair.Second);
        }
        public static IEnumerable<T2> Combine<T1, T2>(this IEnumerable<T1> left, T1 right, CombineFunction<T1, T2> function)
        {
            foreach (var val in left) yield return function(val, right);
        }
        public static IEnumerable<T> Filter<T>(this IEnumerable<T> left, Predicate<T> predicate)
        {
            foreach (T element in left) if (predicate(element)) yield return element;
        }
    }

    public class TemplateVector<T>: ICollection<T>, IEquatable<TemplateVector<T>>, ICloneable where T : IEquatable<T>, new()
    {
        public static readonly int    MINIMAL_VECTOR_SIZE = 5;
        public static readonly double VECTOR_SIZE_UPSCALE = 1.5;

        #region Private fields
        /// <summary>
        /// заполнение массива
        /// </summary>
        private int _filling = 0;
        /// <summary>
        /// заполнение массива
        /// </summary>
        private T[] _data;
        #endregion
        //////////////////////////////////////////////
        ///     Simple iterator implementation     ///
        //////////////////////////////////////////////
        #region IEnumerable<T>, IEnumerable
        public struct VectorEnumerator : IEnumerator<T>
        {
            int index;
            private readonly TemplateVector<T> _enumerableVector;
            public T Current => _enumerableVector._data[index];
            object IEnumerator.Current => Current;
            public bool MoveNext()
            {
                index++;
                return index < _enumerableVector._filling;
            }
            public void Reset() => index = -1;
            public void Dispose() { }
            public VectorEnumerator(TemplateVector<T> v)
            {
                _enumerableVector = v;
                index = -1;
            }
        }
        public VectorEnumerator GetEnumerator() => new VectorEnumerator(this);
        IEnumerator<T> IEnumerable<T>.GetEnumerator() => new VectorEnumerator(this);
        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
        #endregion

        //////////////////////////////////////////////
        ///             Static methods             ///
        /// Map, Zip, Combine, Reduce, Apply, Fill ///
        //////////////////////////////////////////////
        #region Static Map, Zip, Combine, Reduce, Apply, Fill
        public static void Apply(TemplateVector<T> vector, ApplyFunction<T> function) => vector.Apply(function);
        public static void Fill (TemplateVector<T> vector, FillingFunction<T> function) => vector.Fill(function);
        public static bool All  (TemplateVector<T> vector, Predicate<T> predicate) => vector.All(predicate);
        public static bool Any  (TemplateVector<T> vector, Predicate<T> predicate) => vector.Any(predicate);
        public static TemplateVector<T> Filter(TemplateVector<T> vector, Predicate<T> predicate) => new TemplateVector<T>(vector.Filter(predicate));
        #endregion
        
        //////////////////////////////////////////////
        ///            Instance methods            ///
        /// Map, Zip, Combine, Reduce, Apply, Fill ///
        //////////////////////////////////////////////
        #region Instance Map, Zip, Combine, Reduce, Apply, Fill
        protected IEnumerable<T> FilterEnum(Predicate<T> predicate)
        {
            foreach (T element in this) if (predicate(element)) yield return element;
        }
        public void Apply(ApplyFunction<T> function) 
        {
            for (uint index = 0; index < Count; index++) _data[index] = function(_data[index]);
        }
        public void Fill (FillingFunction<T> function)
        {
            for (uint index = 0; index < Count; index++) _data[index] = function(index);
        }
        public bool All  (Predicate<T> predicate) 
        {
            foreach (T element in this) if (!predicate(element)) return false;
            return true;
        }
        public bool Any  (Predicate<T> predicate)
        {
            foreach (T element in this) if (predicate(element)) return true;
            return false;
        }
        #endregion

        #region get/set
        protected int Filling
        {
            get => _filling;
            set
            {
                _filling = Math.Max(0, Math.Min(Capacity, value));
            }
        }
        /// <summary>
        /// Размерность вектора
        /// </summary>
        public int Count => _filling;

        /// <summary>
        /// Размерность буфера вектора
        /// </summary>
        public int Capacity => _data.Length;

        /// <summary>
        /// Получение элемента вктора
        /// </summary>
        /// <param name="id"></param>
        /// <returns></returns>
        public T this[int index]
        {
            get
            {
                if (NotInRange(index)) throw new Exception($"get :: index {index} out of range");
                return _data[index];
            }
            set
            {
                if (NotInRange(index)) throw new Exception($"get :: index {index} out of range");
                _filling = Math.Max(_filling, index + 1);
                _data[index] = value;
            }
        }
        #endregion

        #region Object class methods, IEquatable<TemplateVector<T>>
        /// <summary>
        /// Строковое представление вектора
        /// </summary>
        /// <returns></returns>
        public override string ToString() => $"{{\"vector\":  [{string.Join(", ", this.Map((v) => v.ToString()))}]}}";

        /// <summary>
        /// Базовое сравнение двух векторов
        /// </summary>
        /// <param name="obj"></param>
        /// <returns></returns>
        public override bool Equals(object obj)
        {
            if (!(obj is TemplateVector<T>)) return false;
            return Equals(obj as TemplateVector<T>);
        }

        /// <summary>
        /// Сравнение двух векторов
        /// </summary>
        /// <param name="other"></param>
        /// <returns></returns>
        public bool Equals(TemplateVector<T> other)
        {
            // var b = other.Filter((a) => a != null);
            if (other.Count != Count) return false;
            foreach (TemplatePair<T, T> p in this.Zip(other)) if (!p.First.Equals(p.Second)) return false;
            return true;
        }

        /// <summary>
        /// Хешкод вектора
        /// </summary>
        /// <returns></returns>
        public override int GetHashCode() => HashCode.Combine(_data);
        #endregion

        #region ICollection<T>
        protected bool InRange(int index) => index >= 0 && index < _filling;
        protected bool NotInRange(int index) => !InRange(index);
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
        public object Clone() => new TemplateVector<T>(this);
        public void Clear() => Filling = 0;
        public void Add(T item) => PushBack(item);
        public int IndexOf(T item)
        {
            for (int index = 0; index < this.Filling; index++) if (_data[index].Equals(item)) return index;
            return -1;
        }
        public bool IsReadOnly => false;
        public bool Contains(T item) => IndexOf(item) != -1;

        public void CopyTo(T[] array, int arrayIndex)
        {
            if (array != null)
                throw new ArgumentException("Target array is null...");

            if (array.Rank != 1)
                throw new ArgumentException("Only single dimensional arrays are supported for the requested action...");

            if (array.Length != Count)
                throw new ArgumentException("Target array is length is less then length of source collection...");

            Array.Copy(_data, array, Count);
        }

        public bool Remove(T item)
        {
            return RemoveAt(IndexOf(item));
        }
        public bool RemoveAt(int index)
        {
            if (!InRange(index)) return false;
            for (int i = index; i < Count - 1; i++)
                _data[i] = _data[i + 1];
            _filling--;
            return true;
        }
        #endregion

        #region Constructors
        /// <summary>
        /// Конструктор вектора из массива
        /// </summary>
        /// <param name="args"></param>
        public TemplateVector()
        {
            _data = new T[MINIMAL_VECTOR_SIZE];
            _filling = 0;
        }

        /// <summary>
        /// Конструктор вектора из массива
        /// </summary>
        /// <param name="args"></param>
        public TemplateVector(params T[] args)
        {
            _data = new T[(int)(args.Length * VECTOR_SIZE_UPSCALE)];
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
            _data = new T[(int)(cap * VECTOR_SIZE_UPSCALE)];
            _filling = cap;
        }

        /// <summary>
        /// Конструктор копирования
        /// </summary>
        /// <param name="vect"></param>
        public TemplateVector(TemplateVector<T> other)
        {
            _data = new T[(int)(other.Count * VECTOR_SIZE_UPSCALE)];
            _filling = other.Count;
            if (Count >= 0) Array.Copy(other._data, 0, _data, 0, Count);
        }

        public TemplateVector(IEnumerable<T> other)
        {
            _data = new T[MINIMAL_VECTOR_SIZE];
            foreach (var val in other) PushBack(val);
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
        #endregion
    }
}

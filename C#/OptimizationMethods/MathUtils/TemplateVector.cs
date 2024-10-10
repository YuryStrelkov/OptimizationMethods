using System.Diagnostics.CodeAnalysis;
using System.Collections.Generic;
using System.Collections;
using System;
using OptimizationMethods.MathUtils;

namespace MathUtils
{
    public delegate T    ReduceFunction         <T>            (T accum, T value);
    public delegate T2   CombineFunction        <in T1, out T2>(T1 left, T1 right);
    public delegate T2   MapFunction            <in T1, out T2>(T1 item);
    public delegate T    FillingFunction        <T>            (int index);
    public delegate T    ApplyEnumerateFunction <T>            (int index, T value);
    public delegate T    ApplyFunction          <T>            (T value);
    public delegate bool Predicate              <T>            (T item);
    public readonly struct TemplatePair<T, K> : IEquatable<TemplatePair<T, K>> where T: IEquatable<T> where K: IEquatable<K>
    {
        public readonly T First;
        public readonly K Second;
        public TemplatePair(T First, K Second)
        {
            this.First = First;
            this.Second = Second;
        }
        public bool Equals([AllowNull] TemplatePair<T, K> other)
        {
            if (!First.Equals(other.First)) return false;
            if (!Second.Equals(other.Second)) return false;
            return true;
        }
    }

    public static class VectorIteratorUtils 
    {
        public static T Reduce<T>(this IEnumerable<T> vector, ReduceFunction<T> function) where T: IEquatable<T>, new()
        {
            T accumulator = new T();
            foreach (var v in vector) accumulator = function(accumulator, v);
            return accumulator;
        }
        public static IEnumerable<T2> Map<T1, T2>(this IEnumerable<T1> vector, MapFunction<T1, T2> function) where T1: IEquatable<T1> where T2 : IEquatable<T2>
        {
            foreach (T1 item in vector) yield return function(item);
        }
        public static IEnumerable<TemplatePair<T1, T2>> Zip<T1, T2>(this IEnumerable<T1> left, IEnumerable<T2> right) where T1 : IEquatable<T1> where T2 : IEquatable<T2>
        {
            IEnumerator<T1> iterator1 = left.GetEnumerator();
            IEnumerator<T2> iterator2 = right.GetEnumerator();
            while (iterator1.MoveNext() && iterator2.MoveNext()) yield return new TemplatePair<T1, T2>(iterator1.Current, iterator2.Current);
        }
        public static IEnumerable<T2> Combine<T1, T2>(this IEnumerable<T1> left, IEnumerable<T1> right, CombineFunction<T1, T2> function) where T1 : IEquatable<T1> where T2 : IEquatable<T2>
        {
           foreach (var pair in left.Zip(right)) yield return function(pair.First, pair.Second);
        }
        public static IEnumerable<T2> Combine<T1, T2>(this IEnumerable<T1> left, T1 right, CombineFunction<T1, T2> function) where T1 : IEquatable<T1> where T2 : IEquatable<T2>
        {
            foreach (var val in left) yield return function(val, right);
        }
        public static IEnumerable<T>  Filter<T>(this IEnumerable<T> vector, Predicate<T> predicate) where T : IEquatable<T>
        {
            foreach (T element in vector) if (predicate(element)) yield return element;
        }
        public static bool All<T>(this IEnumerable<T> vector, Predicate<T> predicate) 
        {
            foreach (T element in vector) if (!predicate(element)) return false;
            return true;
        }
        public static bool Any<T>(this IEnumerable<T> vector, Predicate<T> predicate)
        {
            foreach (T element in vector) if (predicate(element)) return true;
            return false;
        }
    }

    public class TemplateVector<T>: ICollection<T>, IEquatable<TemplateVector<T>>, ICloneable where T : IEquatable<T>, new()
    {
        public static readonly uint    MINIMAL_VECTOR_SIZE = 5;
        public static readonly double VECTOR_SIZE_UPSCALE = 1.5;
        private sealed class SliceObject
        {
            private Slice _slice;
            private TemplateVector<T> _source;
            public Slice Slice => _slice;
            public TemplateVector<T> Source => _source;
            public SliceObject(SliceObject other)
            {
                _slice = other.Slice;
                _source = other.Source;
            }
            public SliceObject(Slice slice, TemplateVector<T> source)
            {
                _slice = slice;
                _source = source;
            }
        }

        #region Private fields
        /// <summary>
        /// заполнение массива
        /// </summary>
        private int _filling = 0;
        /// <summary>
        /// заполнение массива
        /// </summary>
        private T[] _data;

        private SliceObject _slice = null;
        #endregion

        //////////////////////////////////////////////
        ///     Simple iterator implementation     ///
        //////////////////////////////////////////////
        #region IEnumerable<T>, IEnumerable
        public struct ValuesEnumerator : IEnumerator<T>
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
            public ValuesEnumerator(TemplateVector<T> v)
            {
                _enumerableVector = v;
                index = -1;
            }
        }
        public struct IndicesEnumerator : IEnumerator<int>, IEnumerable<int>
        {
            private readonly int _begin;
            private readonly int _end;
            private readonly int _step;
            private int _index;

            public IndicesEnumerator(TemplateVector<T> vector)
            {
                _index = -1;
                if (vector.IsSlice)
                {
                    _begin = vector._slice.Slice.Begin;
                    _end   = vector._slice.Slice.End  ;
                    _step  = vector._slice.Slice.Step ;
                    return;
                }
                _begin = 0;
                _step = 1;
                _end = vector.Count - 1;
            }
            
            public int Current => _begin + _index * _step;
            object IEnumerator.Current => Current;
            public bool MoveNext()
            {
                _index++;
                return _index != _end;
            }
            public void Reset() => _index = -1;
            public void Dispose() { }
            public IEnumerator<int> GetEnumerator() => GetEnumerator();
            IEnumerator IEnumerable.GetEnumerator() => this;
        }
        public ValuesEnumerator GetEnumerator() => new ValuesEnumerator(this);
        IEnumerator<T> IEnumerable<T>.GetEnumerator() => new ValuesEnumerator(this);
        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
        #endregion
        
        //////////////////////////////////////////////
        ///             Static methods             ///
        //////////////////////////////////////////////
        #region Static Map, Zip, Combine, Reduce, Apply, Fill
      
        public static IEnumerable<TemplatePair<T1, T2>> Zip<T1, T2>(IEnumerable<T1> left, IEnumerable<T2> right) where T1: IEquatable<T1> where T2 : IEquatable<T2> => left.Zip(right);
        
        public static void Apply<T1>(TemplateVector<T1> vector, ApplyFunction<T1> function) where T1 : IEquatable<T1>, new() {
            for(int index = 0; index < vector.Count; index++)vector._data[index] = function(vector._data[index]);
        }
        
        public static void Fill <T1>(TemplateVector<T1> vector, FillingFunction<T1> function) where T1 : IEquatable<T1>, new() => vector.Fill(function);
        
        public static bool All  <T1>(IEnumerable<T1> vector, Predicate<T1> predicate) where T1 : IEquatable<T1>, new() => vector.All(predicate);
        
        public static bool Any  <T1>(IEnumerable<T1> vector, Predicate<T1> predicate) where T1 : IEquatable<T1>, new() => vector.Any(predicate);
        
        public static TemplateVector<T1> Filter<T1>(TemplateVector<T1> vector, Predicate<T1> predicate) where T1 : IEquatable<T1>, new() => new TemplateVector<T1>(vector.Filter(predicate));
        #endregion

        //////////////////////////////////////////////
        ///            Instance methods            ///
        //////////////////////////////////////////////
        #region Instance Map, Zip, Combine, Reduce, Apply, Fill

        public IEnumerable<T> Values => this;
        public IEnumerable<int> Indices => new IndicesEnumerator(this);
        protected IEnumerable<T> FilterEnum(Predicate<T> predicate)
        {
            foreach (T element in this) if (predicate(element)) yield return element;
        }
        public void Apply(IEnumerable<T> vector, ApplyFunction<T> function) 
        {
            foreach (var pair in Zip(Indices, vector)) _data[pair.First] = function(pair.Second);
        }
        public void Apply(ApplyFunction<T> function) => Apply(this, function);
        public void ApplyEnumerate(IEnumerable<T> vector, ApplyEnumerateFunction<T> function)
        {
            int index = -1;
            foreach (var pair in Zip(Indices, vector))
            {
                 index++;
                 _data[pair.First] = function(index, pair.Second);
            }
        }
        public void ApplyEnumerate(ApplyEnumerateFunction<T> function) => ApplyEnumerate(this, function);
        public void Fill (FillingFunction<T> function)
        {
            foreach (int index in Indices) _data[index] = function(index);
        }
        #endregion

        #region get/set
        public bool IsSlice => _slice != null;

        protected int Filling
        {
            get => _filling;
            set => _filling = Math.Max(0, Math.Min(Capacity, value));
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
        public override string ToString() => $"{{\"vector\":  [{string.Join(", ", this.Map(v => v.ToString()))}]}}";

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
            if (other.Count != Count) return false;
            return All(Zip(this, other), p => p.First.Equals(p.Second));
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
            foreach(int index in Indices) if (_data[index].Equals(item)) return index;
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

        public bool Remove(T item) => RemoveAt(IndexOf(item));

        public bool RemoveAt(int index)
        {
            if (!InRange(index)) return false;
            Array.Copy(_data, index + 1, _data, index, Count - index - 1);
            _filling--;
            if (IsSlice) _slice.Source.Filling--;
            return true;
        }
        #endregion

        #region Constructors
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
            if (args.Length == 0)
            {
                _data = new T[MINIMAL_VECTOR_SIZE];
                _filling = 0;
                _slice = null;
            }
            _data = new T[(int)(args.Length * VECTOR_SIZE_UPSCALE)];
            _filling = args.Length;
            Array.Copy(args, 0, _data, 0, Count);
        }


        /// <summary>
        /// Конструктор копирования
        /// </summary>
        /// <param name="vect"></param>
        public TemplateVector(TemplateVector<T> other): this(other.Count)
        {
            if (other.IsSlice) 
            {
                Apply(other, v => v);
                return;
            }
            if (Count >= 0) Array.Copy(other._data, 0, _data, 0, Count);
        }

        public TemplateVector(IEnumerable<T> other)
        {
            _data = new T[MINIMAL_VECTOR_SIZE];
            foreach (var val in other) PushBack(val);
        }

        protected TemplateVector(Slice rawSlice, TemplateVector<T> source)
        {
            Slice slice = rawSlice.Rebuild(source.Count);
            _filling = slice.Length;
            _slice = new SliceObject(slice, source);
            _data  = source._data;
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

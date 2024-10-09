using System;
using System.Diagnostics.CodeAnalysis;

namespace OptimizationMethods.MathUtils
{
    public struct Slice: IEquatable<Slice>
    {
        private readonly int _end;
        private readonly int _begin;
        private readonly int _step;
        //exclusive index
        public int End => _end;
        //inclusive index
        public int Begin => _begin;
        public int Step => _step;
        public int Index(int index) => Begin + index * Step;

        public int Length
        {
            get
            {
                int total = Math.Abs(End - Begin);
                return Math.Abs(total / Step + total % Step);
            }
        }

        private static int CalcIndex(int index, int stride) => ((index % stride) + stride) % stride;
        public Slice Rebuild(int targetLength)
        {
            int begin = Begin == 0 ? 0 : CalcIndex(Begin, targetLength);
            int end = End == 0 ? targetLength : CalcIndex(End, targetLength);
            return Step > 0 ? new Slice(begin, end == 0 ? targetLength : end, Step): new Slice(end == 0 ? targetLength : end, begin, Step);
        }

        public bool Equals([AllowNull] Slice other)
        {
            if (other.Begin!= Begin) return false;
            if (other.End  != End  ) return false;
            if (other.Step != Step ) return false;
            return true;
        }

        public override string ToString() => Step == 1 ? $"{Begin}:{End}" : $"{Begin}:{End}:{Step}";

        public Slice(int begin, int end) : this(begin, end, 1){}

        public Slice(int begin, int end, int step)
        {
            _step = step == 0 ? 1 : step;
            _begin = begin;
            _end = end;
        }

    }
}

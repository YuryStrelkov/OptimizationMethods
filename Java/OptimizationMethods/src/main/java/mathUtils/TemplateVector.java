package mathUtils;
import mathUtils.functionalInterfaces.*;

import java.util.Iterator;
import java.util.Arrays;


@SuppressWarnings("all")
public class TemplateVector<T> implements Iterable<T>, Cloneable {
    // later :: parallel ops...
    // https://raygun.com/blog/java-performance-optimization-tips/
    public static final int MINIMAL_VECTOR_SIZE = 8;
    public static final double VECTOR_SIZE_UPSCALE = 1.5;
    private T[] _data; // данные (размер 1.5N, где N исходный размер вектора)
    private int _filling; // Заполнение данными от левого края
    private int _begin;
    private int _end;
    private int _step;
    private boolean _is_slice;

    // private SliceObject<T> _slice = null;

    private static <T> T[] alloc(int capacity) {
        return (T[]) (new Object[capacity]);
    }

    private int elementIndex(final int index)
    {
        return _begin + index * _step;
        // return isSlice() ? _slice.sourceIndex(index) : index;
    }

    public final static class Pair<T1, T2> {
        public final T1 First;
        public final T2 Second;

        public Pair(T1 f, T2 s) {
            First = f;
            Second = s;
        }
    }

    private final static class ConstValueIterator<Type> implements Iterator<Type>, Iterable<Type> {
        private final Type _value;

        public ConstValueIterator(Type _value) {
            this._value = _value;
        }

        @Override
        public boolean hasNext() {
            return true;
        }

        @Override
        public Type next() {
            return _value;
        }

        @Override
        public Iterator<Type> iterator() {
            return this;
        }
    }

    public final static class IndicesIterator<Type> implements Iterator<Integer>, Iterable<Integer> {
        private final int _end;
        private final int _step;
        private int _begin;

        public IndicesIterator(final TemplateVector<Type> vector) {
            this(vector, vector._begin, vector._end);
        }

        public IndicesIterator(final TemplateVector<Type> vector, final int rangeBegin, final int rangeEnd) {
            this._begin = vector._begin - vector._step; // + beginShift;
            this._end   = vector.elementIndex(vector._filling)  ;
            this._step  = vector._step ;
            // if(!vector.inRange(rangeBegin))throw
            //         new RuntimeException(
            //                 String.format("IndicesIterator::rangeBegin {%s} is out of range vector indices...",
            //                         rangeBegin));
            // if(!vector.inRange(rangeEnd))throw
            //         new RuntimeException(
            //                 String.format("IndicesIterator::rangeEnd {%s} is out of range vector indices...",
            //                         rangeEnd));
            // if (vector.isSlice()) {
            //     // final int beginShift = (rangeBegin - 1) * vector._slice.step();
            //     // final int endShift   = (rangeEnd + 1 - rangeBegin) * vector._slice.step();
            //     this._begin = vector._begin; // + beginShift;
            //     this._end   = vector._end  ;
            //     this._step  = vector._step ;
            //     return;
            // }
            // this._begin = rangeBegin - 1;
            // this._step  = 1;
            // this._end   = rangeEnd + 1;
        }

        @Override
        public boolean hasNext() {
            _begin += _step;
            return _begin != _end;
        }

        @Override
        public Integer next() {
            return _begin;
        }

        @Override
        public Iterator<Integer> iterator() {
            return this;
        }
    }

    public final static class ValuesIterator<Type> implements Iterator<Type>, Iterable<Type> {
        private final IndicesIterator<Type> _indices;
        private final Type[] _data;

        public ValuesIterator(TemplateVector<Type> vector) {
            _data = vector._data;
            _indices = new IndicesIterator<>(vector);
        }

        @Override
        public boolean hasNext() {
            return _indices.hasNext();
        }

        @Override
        public Type next() {
            return _data[_indices.next()];
        }

        @Override
        public Iterator<Type> iterator() {
            return this;
        }
    }

    public final static class ValuesZipIterator<T1, T2> implements Iterable<Pair<T1, T2>>, Iterator<Pair<T1, T2>> {
        private final Iterator<T1> _iterator1;
        private final Iterator<T2> _iterator2;

        public ValuesZipIterator(Iterable<T1> vector1, Iterable<T2> vector2) {
            _iterator1 = vector1.iterator();
            _iterator2 = vector2.iterator();
        }

        @Override
        public boolean hasNext() {
            return _iterator1.hasNext() && _iterator2.hasNext();
        }

        @Override
        public Pair<T1, T2> next() {
            return new Pair<>(_iterator1.next(), _iterator2.next());
        }

        @Override
        public Iterator<Pair<T1, T2>> iterator() {
            return this;
        }
    }

    public final static class ValuesMapIterator<T1, T2> implements Iterator<T1>, Iterable<T1> {
        private final Iterator<T2> _values;
        private final IMapFunction<T1, T2> _mapFunction;

        @Override
        public boolean hasNext() {
            return _values.hasNext();
        }

        @Override
        public T1 next() {
            return _mapFunction.call(_values.next());
        }

        @Override
        public Iterator<T1> iterator() {
            return this;
        }

        public ValuesMapIterator(Iterable<T2> iterable, IMapFunction<T1, T2> mapFunction) {
            this._values = iterable.iterator();
            this._mapFunction = mapFunction;
        }
    }

    public final static class ValuesCombineIterator<T1, T2> implements Iterator<T2>, Iterable<T2> {
        private final ValuesZipIterator<T1, T1> _zipValues;
        private final ICombineFunction<T1, T2> _combineFunction;

        @Override
        public boolean hasNext() {
            return _zipValues.hasNext();
        }

        @Override
        public T2 next() {
            Pair<T1, T1> pair = _zipValues.next();
            return _combineFunction.call(pair.First, pair.Second);
        }

        @Override
        public Iterator<T2> iterator() {
            return this;
        }

        public ValuesCombineIterator(Iterable<T1> iterable1, Iterable<T1> iterable2,
                                     ICombineFunction<T1, T2> combineFunction) {
            _zipValues = new ValuesZipIterator<>(iterable1, iterable2);
            this._combineFunction = combineFunction;
        }
    }

    public static <T> T reduce(Iterable<T> vector, IReduceFunction<T> function, T initValue) {
        for (final T item : vector) initValue = function.call(initValue, item);
        return initValue;
    }

    public static <T> TemplateVector<T> filter(Iterable<T> vector, IConditionFunction<T> condition) {
        TemplateVector<T> filteredVector = new TemplateVector<>();
        for (final T item : vector) if (condition.call(item)) filteredVector.pushBack(item);
        return filteredVector;
    }

    public static <T> boolean any(Iterable<T> vector, IConditionFunction<T> condition) {
        for (final T item : vector) if (condition.call(item)) return true;
        return false;
    }

    public static <T> boolean all(Iterable<T> vector, IConditionFunction<T> condition) {
        for (final T item : vector) if (!condition.call(item)) return false;
        return true;
    }

    public static <T1, T2> Iterable<Pair<T1, T2>> zip(TemplateVector<T1> first, TemplateVector<T2> second) {
        return new ValuesZipIterator<>(first, second);
    }

    public static <T1, T2> Iterable<T1> map(TemplateVector<T2> vector, IMapFunction<T1, T2> function) {
        return new ValuesMapIterator<>(vector, function);
    }

    public static <T1, T2> Iterable<T2> combine(TemplateVector<T1> left, TemplateVector<T1> right,
                                                ICombineFunction<T1, T2> combineFunction) {
        return new ValuesCombineIterator<>(left, right, combineFunction);
    }

    public static <T1, T2> Iterable<T2> combine(T1 left, TemplateVector<T1> right,
                                                ICombineFunction<T1, T2> combineFunction) {
        return new ValuesCombineIterator<>(new ConstValueIterator<>(left), right, combineFunction);
    }

    public static <T1, T2> Iterable<T2> combine(TemplateVector<T1> left, T1 right,
                                                ICombineFunction<T1, T2> combineFunction) {
        return new ValuesCombineIterator<>(left, new ConstValueIterator<>(right), combineFunction);
    }

    public void fill(IFillFunction<T> fillFunction) {
        for (final int index : new IndicesIterator<>(this)) this._data[index] = fillFunction.call(index);
    }

    public void apply(Iterable<T> sequence, IForEachApplyFunction<T> applyFunction) {
        for (final Pair<Integer, T> indexValue : new ValuesZipIterator<>(new IndicesIterator<>(this), sequence)) {
            _data[indexValue.First] = applyFunction.call(indexValue.Second);
        }
    }

    public void apply(IForEachApplyFunction<T> applyFunction) {
        apply(this, applyFunction);
    }

    public void applyEnumerate(Iterable<T> sequence, IForEnumerateApplyFunction<T> applyFunction) {
        int index = 0;
        for (final Pair<Integer, T> indexValue : new ValuesZipIterator<>(new IndicesIterator<>(this), sequence)) {
            _data[indexValue.First] = applyFunction.call(index++, indexValue.Second);
        }
    }

    public void applyEnumerate(IForEnumerateApplyFunction<T> applyFunction) {
        applyEnumerate(this, applyFunction);
    }

    public void combine(Iterable<T> sequence, ICombineFunction<T, T> combineFunction) {
        for (final Pair<Integer, T> indexValue : new ValuesZipIterator<>(new IndicesIterator<>(this), sequence)) {
            _data[indexValue.First] = combineFunction.call(_data[indexValue.First], indexValue.Second);
        }
    }

    public boolean any(IConditionFunction<T> condition) {
        return any(this, condition);
    }

    public boolean all(IConditionFunction<T> condition) {
        return all(this, condition);
    }

    public TemplateVector<T> filter(IConditionFunction<T> condition) {
        return filter(this, condition);
    }

    @Override
    public Iterator<T> iterator() {
        return new ValuesIterator<>(this);
    }

    protected boolean notInRange(final int index) {
        return !inRange(index);
    }

    protected boolean inRange(final int index) {
        return index >= 0 && index < size();
    }

    public int size() {
        return _filling;
    }

    public boolean isSlice() {
        return _is_slice;
    }

    public int capacity() {
        return _data.length;
    }

    public void clear(){
        _filling = 0;
        _end = 0;
        _begin = 0;
        _step = 1;
        _data = null;
    }

    public T get(final int index) {
        if (notInRange(index))
            throw new RuntimeException(String.format("get :: index {%s} out of range [%i, %i]", index, 0, size()));
        return uncheckedGet(index);
    }

    public TemplateVector<T> get(final Slice slice) {
        return new TemplateVector<>(slice, this);
    }

    public TemplateVector<T> set(final int index, final T value) {
        if (notInRange(index))
            throw new RuntimeException(String.format("get :: index {%s} out of range [%i, %i]", index, 0, size()));
        uncheckedSet(index, value);
        return this;
    }

    public TemplateVector<T> set(final Slice slice, TemplateVector<T> values) {
        final int length = Math.abs(slice.end() - slice.begin()) / slice.step();
        if (length != values.size())
            throw new RuntimeException(
                    String.format("get sliced access error :: slice values count {%s} while values vector size is {%s}",
                            length,  values.size()));
        if (length != size())
            throw new RuntimeException(
                    String.format("get sliced access error :: slice values count {%s} while target vector size is {%s}",
                            length, size()));
        TemplateVector<T> view = new TemplateVector<>(slice, this);
        view.apply(values, v -> v);
        return this;
    }

    protected T uncheckedGet(final int index) {
        return _data[elementIndex(index)];
    }

    protected void uncheckedSet(final int index, final T value) {
        _data[elementIndex(index)] = value;
    }

    private void checkAndIncreaseSize() {
        if (size() != capacity()) return;
        T[] newData = alloc((int) (_data.length * VECTOR_SIZE_UPSCALE));
        System.arraycopy(_data, 0, newData, 0, size());
        _data = newData;
    }

    public TemplateVector<T> pushBack(final T value) {
        return insert(size(), value);
    }

    public int indexOf(final T item) {
        for (final int index : new IndicesIterator<>(this))
            if (_data[index].equals(item)) return elementIndex(index);
        return -1;
    }

    public boolean contains(final T item) {
        return indexOf(item) != -1;
    }

    public TemplateVector<T> remove(final T item) {
        final int index = indexOf(item);
        return index != -1 ? removeAt(index) : this;
    }

    public TemplateVector<T> removeAt(final int index) {
        if(isSlice())
            throw new RuntimeException(
                    String.format("removeAt::attempt to remove element at index \"%i\" from silce...", index));
        if(!inRange(index))
            throw new IndexOutOfBoundsException(
                    String.format("removeAt::" +
                            "attempt to remove element at index \"%i\" while indices range is \"0\" to %i...",
                            index, size() - 1));
        final int deletionIndex = index;
        final int lastElemIndex = size() - 1;
        if (lastElemIndex != deletionIndex)
            System.arraycopy(_data, index + 1, _data, index, lastElemIndex - index);
        _filling--;
        _end--;
        return this;
    }

    public TemplateVector<T> insert(final int index, final T value) {
        if(isSlice())
            throw new RuntimeException(
                    String.format("insert::attempt to insert/pushBack element" +
                                  " at index \"%i\" into the silce...", index));
        if(index < 0 || index > size())
            throw new IndexOutOfBoundsException(
                    String.format("insert::insert to insert/pushBack element at index \"%i\"" +
                                  " while indices range is \"0\" to %i...",
                            index, size()));
        checkAndIncreaseSize();
        // вставка эдемента не в конец
        if(size() != index)
            System.arraycopy(_data, index, _data, index + 1, capacity() - index - 1);
        // вставка элемента в конец
        _data[index] = value;
        _filling++;
        _end++;
        return this;
    }

    @Override
    public String toString() {
        return String.format("{%s}", String.join(", ", map(this, Object::toString)));
    }

    @Override
    public boolean equals(final Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        TemplateVector<T> vector = (TemplateVector<T>) o;
        //return any(zip(this, o, (l, r) -> l.equals(r)));
        return Arrays.equals(vector._data, this._data);
    }

    @Override
    public int hashCode() {
        return Arrays.hashCode(_data);
    }

    @Override
    public TemplateVector<T> clone() {
        return new TemplateVector<>(this);
    }

    public TemplateVector(final int cap) {
        _data    = alloc((int) (cap * VECTOR_SIZE_UPSCALE));
        _filling = cap;
        _begin   = 0;
        _step    = 1;
        _end     = _filling;
    }

    public TemplateVector(final TemplateVector<T> other) {
        this(other.size());
        apply(other, v -> v);
    }

    @SafeVarargs
    public TemplateVector(final T... other) {
        _filling = other.length;
        _begin   = 0;
        _step    = 1;
        _end     = Math.max((int) (other.length * VECTOR_SIZE_UPSCALE), MINIMAL_VECTOR_SIZE);
        _data    = alloc(_end - _begin);
        if (other.length != 0)
            System.arraycopy(other, 0, _data, 0, size());
    }

    protected TemplateVector(final Iterable<T> other) {
        _data    = alloc(MINIMAL_VECTOR_SIZE);
        _filling = 0;
        _begin   = 0;
        _end     = 0;
        _step    = 1;
        for (final T v : other) pushBack(v);
    }

    protected TemplateVector(final Slice slice, final TemplateVector<T> source) {
        _end      = slice.end() <= 0 ? source.size() + slice.end() : slice.end(); // Math.max(Math.min(slice.end() <= 0 ? size() + slice.end() : slice.end(), size() - 1), slice.begin());
        _end      = Math.max(_end, 0);
        _end      = Math.min(_end, source.size());
        _begin    = Math.min(Math.max(0, slice.begin()), _end);
        _step     = slice.step();
        _filling  = (_end - _begin) / _step;
        if(_filling == 0)
            throw new RuntimeException(String.format("Silce with begin: %d, end: %d and step: %d is empty",
                    slice.begin(), slice.end(), slice.step()));
        _data     = source._data;
        _is_slice = true;
    }
}

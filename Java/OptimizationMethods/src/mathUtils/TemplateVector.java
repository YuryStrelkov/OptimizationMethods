package mathUtils;

import functionalInterfaces.*;

import java.util.Iterator;
import java.util.Arrays;


public class TemplateVector<T> implements Iterable<T>, Cloneable {

    private static class SliceObject<T> {
        private final Slice _slice;
        private final TemplateVector<T> _source;

        public final Slice slice() {
            return _slice;
        }

        public final TemplateVector<T> source() {
            return _source;
        }

        public SliceObject(SliceObject<T> other) {
            _slice = other.slice();
            _source = other.source();
        }

        public SliceObject(Slice slice, TemplateVector<T> source) {
            _slice = slice;
            _source = source;
        }
    }
    public static final int MINIMAL_VECTOR_SIZE = 9;
    public static final double VECTOR_SIZE_UPSCALE = 1.5;
    private T[] _data; // данные (размер 1.5N, где N исходный размер вектора)
    private int _filling; // Заполнение данными от левого края
    private SliceObject<T> _slice = null;

    @SuppressWarnings("unchecked")
    private static <T> T[] alloc(int capacity) {
        return (T[]) (new Object[capacity]);
    }

    public static class Pair<T1, T2> {
        public final T1 First;
        public final T2 Second;

        public Pair(T1 f, T2 s) {
            First = f;
            Second = s;
        }
    }

    private static class ConstValueIterator<Type> implements Iterator<Type>, Iterable<Type> {
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

    public static class IndicesIterator<Type> implements Iterator<Integer>, Iterable<Integer> {
        private final int _begin;
        private final int _end;
        private final int _step;
        private int _index;

        public IndicesIterator(TemplateVector<Type> vector) {
            this._index = -1;
            if (vector.isSlice()) {
                this._begin = vector._slice.slice().begin();
                this._end   = vector._slice.slice().end();
                this._step  = vector._slice.slice().step();
                return;
            }
            this._begin = 0;
            this._step  = 1;
            this._end   = vector.size() - 1;
        }

        @Override
        public boolean hasNext() {
            return _index != _end;
        }

        @Override
        public Integer next() {
            _index++;
            return _begin + _index * _step;
        }

        @Override
        public Iterator<Integer> iterator() {
            return this;
        }
    }

    public static class ValuesIterator<Type> implements Iterator<Type>, Iterable<Type> {
        private final TemplateVector<Type> _iterableVector;

        int _index;

        public ValuesIterator(TemplateVector<Type> vector) {
            _index = -1;
            _iterableVector = vector;
        }

        @Override
        public boolean hasNext() {
            return (_index != _iterableVector.size() - 1);
        }

        @Override
        public Type next() {
            _index++;
            return _iterableVector.unchecked_get(_index);
        }

        @Override
        public Iterator<Type> iterator() {
            return this;
        }
    }

    public static class ValuesZipIterator<T1, T2> implements Iterable<Pair<T1, T2>>, Iterator<Pair<T1, T2>> {
        private final Iterator<T1> _iterable1;
        private final Iterator<T2> _iterable2;

        public ValuesZipIterator(Iterable<T1> vector1, Iterable<T2> vector2) {
            _iterable1 = vector1.iterator();
            _iterable2 = vector2.iterator();
        }

        @Override
        public boolean hasNext() {
            return _iterable1.hasNext() && _iterable2.hasNext();
        }

        @Override
        public Pair<T1, T2> next() {
            return new Pair<>(_iterable1.next(), _iterable2.next());
        }

        @Override
        public Iterator<Pair<T1, T2>> iterator() {
            return this;
        }
    }

    public static class ValuesMapIterator<T1, T2> implements Iterator<T1>, Iterable<T1> {
        private final Iterator<T2> iterable;
        private final IMapFunction<T1, T2> mapFunction;

        @Override
        public boolean hasNext() {
            return iterable.hasNext();
        }

        @Override
        public T1 next() {
            return mapFunction.call(iterable.next());
        }

        @Override
        public Iterator<T1> iterator() {
            return this;
        }

        public ValuesMapIterator(Iterable<T2> iterable, IMapFunction<T1, T2> mapFunction) {
            this.iterable = iterable.iterator();
            this.mapFunction = mapFunction;
        }
    }

    public static class ValuesCombineIterator<T1, T2> implements Iterator<T2>, Iterable<T2> {
        private final ValuesZipIterator<T1, T1> iterator;
        private final ICombineFunction<T1, T2> combineFunction;

        @Override
        public boolean hasNext() {
            return iterator.hasNext();
        }

        @Override
        public T2 next() {
            Pair<T1, T1> pair = iterator.next();
            return combineFunction.call(pair.First, pair.Second);
        }

        @Override
        public Iterator<T2> iterator() {
            return this;
        }

        public ValuesCombineIterator(Iterable<T1> iterable1,
                                     Iterable<T1> iterable2,
                                     ICombineFunction<T1, T2> combineFunction) {
            iterator = new ValuesZipIterator<>(iterable1, iterable2);
            this.combineFunction = combineFunction;
        }
    }

    public static <T> T reduce(Iterable<T> vector, IReduceFunction<T> function, T initValue) {
        for (T value : vector) initValue = function.call(initValue, value);
        return initValue;
    }

    public static <T> TemplateVector<T> filter(Iterable<T> vector, IConditionFunction<T> condition) {
        TemplateVector<T> filteredVector = new TemplateVector<>();
        for (T item : vector) if (condition.call(item)) filteredVector.pushBack(item);
        return filteredVector;
    }

    public static <T> boolean any(Iterable<T> vector, IConditionFunction<T> condition) {
        for (T item : vector) if (condition.call(item)) return true;
        return false;
    }

    public static <T> boolean all(Iterable<T> vector, IConditionFunction<T> condition) {
        for (T item : vector) if (!condition.call(item)) return false;
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
        for (int index : new IndicesIterator<>(this)) {
            this._data[index] = fillFunction.call(index);
        }
    }

    public void apply(Iterable<T> sequence, IForEachApplyFunction<T> applyFunction) {
        for (Pair<Integer, T> indexValue: new ValuesZipIterator<>(new IndicesIterator<>(this), sequence)) {
            _data[indexValue.First] = applyFunction.call(indexValue.Second);
        }
    }

    public void apply(IForEachApplyFunction<T> applyFunction) {
        apply(this, applyFunction);
    }

    public void applyEnumerate(Iterable<T> sequence, IForEnumerateApplyFunction<T> applyFunction) {
        int index = -1;
        for (Pair<Integer, T> indexValue : new ValuesZipIterator<>(new IndicesIterator<>(this), sequence)) {
            index++;
            _data[indexValue.First] = applyFunction.call(index, indexValue.Second);
        }
    }

    public void applyEnumerate(IForEnumerateApplyFunction<T> applyFunction) {
        applyEnumerate(this, applyFunction);
    }

    public void combine(Iterable<T> sequence, ICombineFunction<T, T> combineFunction) {
        for (Pair<Integer, T> indexValue : new ValuesZipIterator<>(new IndicesIterator<>(this), sequence)) {
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
        return _slice != null;
    }

    public int capacity() {
        return _data.length;
    }

    public T get(final int index) {
        if (notInRange(index)) throw new RuntimeException(String.format("get :: index {%s} out of range", index));
        return unchecked_get(index);
    }

    public TemplateVector<T> get(final Slice slice) {
        return new TemplateVector<>(slice, this);
    }

    public void set(final int index, final T value) {
        if (notInRange(index)) throw new RuntimeException(String.format("get :: index {%s} out of range", index));
        unchecked_set(index, value);
    }

    protected T unchecked_get(final int index) {
        return isSlice() ? _data[getSliceIndex(index)] : _data[index];
    }

    protected void unchecked_set(final int index, final  T value) {
        if (isSlice())
            _data[getSliceIndex(index)] = value;
        else
            _data[index] = value;
    }

    private void checkAnIncreaseSize() {
        if (size() != _data.length) return;
        T[] new_data = alloc((int) (size() * VECTOR_SIZE_UPSCALE));
        if (size() >= 0) System.arraycopy(_data, 0, new_data, 0, size());
        _data = new_data;
    }

    @SuppressWarnings("all")
    public TemplateVector<T> pushBack(final T value) {
        checkAnIncreaseSize();
        _data[_filling] = value;
        _filling++;
        return this;
    }

    public int indexOf(final T item) {
        for (int index : new IndicesIterator<>(this)) if (_data[index].equals(item)) return index;
        return -1;
    }

    public boolean contains(final T item) {
        return indexOf(item) != -1;
    }

    public TemplateVector<T> remove(final T item) {
        return removeAt(indexOf(item));
    }

    public TemplateVector<T> removeAt(final int index) {
        if (!inRange(index)) return this;
        System.arraycopy(_data, index + 1, _data, index, size() - index - 1);
        _filling--;
        if (isSlice()) _slice.source()._filling--;
        return this;
    }

    public TemplateVector<T> insert(final int index, final T value) {
        if (index > size()) return pushBack(value);
        checkAnIncreaseSize();
        int _index = Math.max(0, index);
        System.arraycopy(_data, index, _data, _index + 1, size() - _index);
        if (isSlice()) {
            _data[getSliceIndex(index)] = value;
            _filling++;
            _slice.source()._filling++;
        } else {
            _filling++;
            _data[index] = value;
        }
        return this;
    }

    @Override
    public String toString() {
        return String.format("{%s}", String.join(", ", map(this, Object::toString)));
    }

    @Override
    @SuppressWarnings("unchecked")
    public boolean equals(final Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        TemplateVector<T> vector = (TemplateVector<T>) o;
        return Arrays.equals(vector._data, this._data);
    }

    @Override
    public int hashCode() {
        return Arrays.hashCode(_data);
    }

    @Override
    @SuppressWarnings("unchecked")
    public TemplateVector<T> clone() {
        if (isSlice()) {
            return new TemplateVector<>(this);
        }
        try {
            TemplateVector<T> cloneObj = (TemplateVector<T>) super.clone();
            cloneObj._data = Arrays.copyOf(_data, _data.length);
            cloneObj._filling = _filling;
            return cloneObj;
        } catch (CloneNotSupportedException e) {
            throw new InternalError(e);
        }
    }
    public TemplateVector(final int cap) {
        _data = alloc((int) (cap * VECTOR_SIZE_UPSCALE));
        _filling = cap;
    }

    @SuppressWarnings("all")
    public TemplateVector(final TemplateVector<T> other) {
        this(other.size());
        _slice = null;
        if (other.isSlice()) {
            apply(other, v->v);
            return;
        }
        if (size() > 0) System.arraycopy(other._data, 0, _data, 0, size());
    }

    @SafeVarargs
    public TemplateVector(final T... other) {
        if (other.length == 0) {
            _data = alloc(MINIMAL_VECTOR_SIZE);
            _filling = 0;
            _slice = null;
            return;
        }
        _data = alloc((int) (other.length * VECTOR_SIZE_UPSCALE));
        _filling = other.length;
        _slice = null;
        System.arraycopy(other, 0, _data, 0, size());
    }

    protected TemplateVector(final Iterable<T> other) {
        _data = alloc(MINIMAL_VECTOR_SIZE);
        _filling = 0;
        _slice = null;
        for (T v : other) pushBack(v);
    }

   private int getSliceIndex(final int index) {
        return _slice.slice().index(index);
    }

   protected TemplateVector(final Slice rawSlice, final TemplateVector<T> source) {
       Slice slice = rawSlice.rebuild(source.size());
       _filling = slice.length();
       _slice   = new SliceObject<>(slice, source);
       _data    = source._data;
   }
}

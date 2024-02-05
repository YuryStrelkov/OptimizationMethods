package mathUtils;

import functionalInterfaces.*;

import java.util.Iterator;
import java.util.Arrays;


public class TemplateVector<T> implements Iterable<T>, Cloneable {
    public static final int MINIMAL_VECTOR_SIZE = 9;
    public static final double VECTOR_SIZE_UPSCALE = 1.5;
    private Object[] _data; // данные (размер 1.5N, где N исходный размер вектора)
    private int _filling; // Заполнение данными от левого края

    private static Object[] alloc(int capacity) {
        return new Object[capacity];
    }

    public static class Pair<T1, T2> {
        public final T1 First;
        public final T2 Second;

        public Pair(T1 f, T2 s) {
            First = f;
            Second = s;
        }
    }

    @SuppressWarnings("unchecked")
    public static class ValuesIterator<Type> implements Iterator<Type>, Iterable<Type> {
        private final TemplateVector<Type> _iterableVector;

        int _index;

        public ValuesIterator(TemplateVector<Type> vector) {
            _index = -1;
            _iterableVector = vector;
        }

        @Override
        public boolean hasNext() {
            return (_index + 1 < _iterableVector._filling);
        }

        @Override
        public Type next() {
            _index++;
            return (Type) _iterableVector._data[_index];
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
        return new ValuesCombineIterator<>(() -> new Iterator<T1>() {
            @Override
            public boolean hasNext() {
                return true;
            }

            @Override
            public T1 next() {
                return left;
            }
        }, right, combineFunction);
    }

    public static <T1, T2> Iterable<T2> combine(TemplateVector<T1> left, T1 right,
                                                ICombineFunction<T1, T2> combineFunction) {
        return new ValuesCombineIterator<>(left, () -> new Iterator<T1>() {
            @Override
            public boolean hasNext() {
                return true;
            }

            @Override
            public T1 next() {
                return right;
            }
        }, combineFunction);
    }

    public void fill(IFillFunction<T> fillFunction) {
        for (int index = 0; index < this._data.length; index++)
            _data[index] = fillFunction.call(index);
    }

    public void apply(Iterable<T> sequence, IApplyFunction<T> applyFunction) {
        Iterator<T> iterator = sequence.iterator();
        int index = 0;
        while (iterator.hasNext() && index < capacity()) _data[index] = applyFunction.call(iterator.next());
    }

    public void apply(IApplyFunction<T> applyFunction) {
        for (int index = 0; index < this.size(); index++)
            _data[index] = applyFunction.call((T) _data[index]);
    }

    @SuppressWarnings("unchecked")
    public void combine(Iterable<T> sequence, ICombineFunction<T, T> combineFunction) {
        Iterator<T> iterator = sequence.iterator();
        int index = 0;
        while (iterator.hasNext()) {
            _data[index] = combineFunction.call((T) _data[index], iterator.next());
            index++;
        }
    }


    @Override
    public Iterator<T> iterator() {
        return new ValuesIterator<>(this);
    }

    protected boolean notInRange(int index) {
        return !InRange(index);
    }

    protected boolean InRange(int index) {
        return index >= 0 && index < capacity();
    }

    public int size() {
        return _filling;
    }

    public int capacity() {
        return _data.length;
    }

    @SuppressWarnings("unchecked")
    public T get(int index) {
        if (notInRange(index)) throw new RuntimeException(String.format("get :: index {%s} out of range", index));
        return (T) _data[index];
    }

    public void set(int index, T value) {
        if (notInRange(index)) throw new RuntimeException(String.format("get :: index {%s} out of range", index));
        _filling = Math.max(_filling, index + 1);
        _data[index] = value;
    }

    @SuppressWarnings("unchecked")
    protected T unchecked_get(int index) {
        return (T) _data[index];
    }

    protected void unchecked_set(int index, T value) {
        _filling = Math.max(_filling, index + 1);
        _data[index] = value;
    }

    @SuppressWarnings("all")
    public TemplateVector<T> pushBack(T value) {
        if (_filling == _data.length) {
            Object[] new_data = alloc((int) (size() * VECTOR_SIZE_UPSCALE));
            if (size() >= 0) System.arraycopy(_data, 0, new_data, 0, size());
            _data = new_data;
        }
        _data[_filling] = value;
        _filling++;
        return this;
    }

    @Override
    public String toString() {
        return String.format("{%s}", String.join(", ", TemplateVector.map(this, Object::toString)));
    }

    @Override
    @SuppressWarnings("unchecked")
    public boolean equals(Object o) {
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
        try {
            TemplateVector<T> v = (TemplateVector<T>) super.clone();
            v._data = Arrays.copyOf(_data, _data.length);
            v._filling = _filling;
            return v;
        } catch (CloneNotSupportedException e) {
            throw new InternalError(e);
        }
    }

    public TemplateVector(int cap) {
        _data = alloc((int) (cap * VECTOR_SIZE_UPSCALE));
        _filling = cap;
    }

    public TemplateVector(TemplateVector<T> other) {
        _data = alloc((int) (other.size() * VECTOR_SIZE_UPSCALE));
        _filling = other.size();
        if (size() >= 0) System.arraycopy(other._data, 0, _data, 0, size());
    }

    @SuppressWarnings("unchecked")
    public TemplateVector(T... other) {
        if (other.length == 0) {
            _data = alloc(MINIMAL_VECTOR_SIZE);
            _filling = 0;
            return;
        }
        _data = alloc((int) (other.length * VECTOR_SIZE_UPSCALE));
        _filling = other.length;
        if (size() >= 0) System.arraycopy(other, 0, _data, 0, size());
    }

    protected TemplateVector(Iterable<T> other) {
        _data = alloc(MINIMAL_VECTOR_SIZE);
        _filling = 0;
        for (T v : other) pushBack(v);
    }
}

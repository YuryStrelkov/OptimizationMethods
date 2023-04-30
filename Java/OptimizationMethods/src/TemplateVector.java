import java.util.Iterator;
import java.util.Arrays;

public class TemplateVector<TypeName> implements Iterable<TypeName>, Cloneable
{
    private Object[] _data; // данные (размер 1.5N, где N исходный размер вектора)
    private int _filling;   // Заполнение данными от левого края

    @Override
    public Iterator<TypeName> iterator() {
        return new TemplateVectorIterator<>(this);
    }

    @SuppressWarnings("unchecked")
    public static class TemplateVectorIterator<Type> implements Iterator<Type>
    {
        int _index;

        TemplateVector<Type> _iterableVector;

        public TemplateVectorIterator(TemplateVector<Type> vector)
        {
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
            return (Type)_iterableVector._data[_index];
        }
    }

    public static class Pair<TypeName>
    {
        public TypeName First;
        public TypeName Second;
        public Pair(TypeName f, TypeName s)
        {
            First = f;
            Second = s;
        }
    }

    @SuppressWarnings("unchecked")
    public static class TemplateVectorZipIterator<Type> implements Iterable<Pair<Type>>, Iterator<Pair<Type>>
    {
        int _index;

        TemplateVector<Type> _iterableVector1;
        TemplateVector<Type> _iterableVector2;

        public TemplateVectorZipIterator(TemplateVector<Type> vector1, TemplateVector<Type> vector2)
        {
            _index = -1;
            _iterableVector1 = vector1;
            _iterableVector2 = vector2;
        }

        @Override
        public boolean hasNext()
        {
            return (_index + 1 < _iterableVector2._filling) && (_index + 1 < _iterableVector1._filling);
        }

        @Override
        public Pair<Type> next()
        {
            _index++;
            return new Pair<>((Type)_iterableVector1._data[_index], (Type)_iterableVector2._data[_index]);
        }

        @Override
        public Iterator<Pair<Type>> iterator() {
            return this;
        }
    }

    public static <Type> Iterable<Pair<Type>> zip(TemplateVector<Type> first, TemplateVector<Type> second)
    {
        return new TemplateVectorZipIterator<>(first, second);
    }

    private Object[] alloc(int capacity)
    {
        return new Object[capacity];
    }

    protected boolean notInRange(int index)
    {
        return index < 0 || index >= _data.length;
    }

    @Override
    @SuppressWarnings("unchecked")
    public TemplateVector<TypeName> clone()
    {
        try
        {
            TemplateVector<TypeName> v = (TemplateVector<TypeName>)super.clone();
            v._data = Arrays.copyOf(_data, _data.length);
            v._filling  = _filling;
            return v;
        }
        catch (CloneNotSupportedException e)
        {
            throw new InternalError(e);
        }
    }

    public TemplateVector()
    {
        _data = alloc((int)(6 * 1.5));
        _filling = 0;
    }

    public TemplateVector(int cap)
    {
        _data = alloc((int)(cap * 1.5));
        _filling = 0;
    }

    public TemplateVector(TemplateVector<TypeName> other)
    {
        _data = alloc((int)(other.size()* 1.5));
        _filling = other.size();
        if (size() >= 0) System.arraycopy(other._data, 0, _data, 0, size());
    }

    @SuppressWarnings("unchecked")
    public TemplateVector(TypeName... other)
    {
        _data = alloc((int)(other.length * 1.5));
        _filling = other.length;
        if (size() >= 0) System.arraycopy(other, 0, _data, 0, size());
    }

    @SuppressWarnings("all")
    public TemplateVector(Iterable<TypeName> other)
    {
        _data = alloc(9);
        _filling = 0;
        for(TypeName v : other) pushBack(v);
    }

    @SuppressWarnings("unchecked")
    public TypeName get(int index)
    {
        if(notInRange(index)) throw new RuntimeException("get :: index "+index+" out of range");
        _filling = Math.max(_filling, index + 1);
        return (TypeName)_data[index];
    }

    public void set(int index, TypeName value)
    {
        if(notInRange(index)) throw new RuntimeException("get :: index "+index+" out of range");
        _filling = Math.max(_filling, index + 1);
        _data[index] = value;
    }

    @SuppressWarnings("all")
    public TemplateVector<TypeName> pushBack(TypeName value)
    {
        if (_filling == _data.length)
        {
            Object[] new_data = alloc((int)(size() * 1.5));
            if (size() >= 0) System.arraycopy(_data, 0, new_data, 0, size());
            _data = new_data;
        }
        _data[_filling] = value;
        _filling++;
        return this;
    }

    public int size()
    {
        return  _filling;
    }

    @Override
    public String toString()
    {
        StringBuilder sb = new StringBuilder();
        sb.append("{ ");
        for(TypeName value: this)
        {
            sb.append(value.toString());
            sb.append(", ");
        }
        sb.delete(sb.length() - 2, sb.length());
        sb.append(" }");
        return sb.toString();
    }

    @Override
    @SuppressWarnings("unchecked")
    public boolean equals(Object o)
    {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        TemplateVector<TypeName> vector = (TemplateVector<TypeName>) o;
        return Arrays.equals(vector._data, this._data);
    }

    @Override
    public int hashCode()
    {
        return Arrays.hashCode(_data);
    }
}

import java.util.Arrays;
import java.util.Iterator;

public class VectorBase<TypeName> implements Iterable<TypeName>, Cloneable
{
    private Object[] _data; // данные (размер 1.5N, где N исходный размер вектора)
    private int _capacity;    // текущий размер вектора
    private int _filling;     // Заполнение данными от левого края

    private Object[] alloc(int capacity)
    {
        return new Object[capacity];
    }

    protected boolean notInRange(int index)
    {
        return index < 0 || index >= _capacity;
    }

    @Override
    public Iterator<TypeName> iterator() {
        return new VectorBaseIterator(this);
    }

    @SuppressWarnings("unchecked")
    public class VectorBaseIterator implements Iterator<TypeName>
    {
        int _index;

        VectorBase<TypeName> _iterableVector;

        public VectorBaseIterator(VectorBase<TypeName> vector)
        {
            _index = -1;
            _iterableVector = vector;
        }

        @Override
        public boolean hasNext() {
            return (_index + 1 < _iterableVector._capacity);
        }

        @Override
        public TypeName next() {
            _index++;
            return (TypeName)_iterableVector._data[_index];
        }
    }

    @Override
    @SuppressWarnings("unchecked")
    public VectorBase<TypeName> clone()
    {
        try
        {
            VectorBase<TypeName> v = (VectorBase<TypeName>)super.clone();
            v._data = Arrays.copyOf(_data, _data.length);
            v._capacity = _capacity;
            v._filling  = _filling;
            return v;
        }
        catch (CloneNotSupportedException e)
        {
            throw new InternalError(e);
        }
    }

    public VectorBase()
    {
        _data = alloc((int)(6 * 1.5));
        _capacity = 0;
        _filling = 0;
    }

    public VectorBase(int cap)
    {
        _data = alloc((int)(cap * 1.5));
        _capacity = cap;
        _filling = 0;
    }

    public VectorBase(VectorBase<TypeName> other)
    {
        _data = alloc((int)(other.size()* 1.5));
        _capacity = other.size();
        _filling = other.size();
        if (size() >= 0) System.arraycopy(other._data, 0, _data, 0, size());
    }

    @SuppressWarnings("unchecked")
    public VectorBase(TypeName... other)
    {
        _data = alloc((int)(other.length * 1.5));
        _capacity = other.length;
        _filling = other.length;
        if (size() >= 0) System.arraycopy(other, 0, _data, 0, size());
    }

    @SuppressWarnings("all")
    public VectorBase(Iterable<TypeName> other)
    {
        _data = alloc(9);
        _capacity = 0;
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
    public VectorBase<TypeName> pushBack(TypeName value)
    {
        if (_filling == _data.length)
        {
            Object[] new_data = alloc((int)(size() * 1.5));
            if (size() >= 0) System.arraycopy(_data, 0, new_data, 0, size());
            _data = new_data;
        }
        _data[_filling] = value;
        _filling++;
        _capacity = Math.max(_filling, _capacity);
        return this;
    }

    public int size()
    {
        return  _capacity;
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
        VectorBase<TypeName> vector = (VectorBase<TypeName>) o;
        return Arrays.equals(vector._data, this._data);
    }

    @Override
    public int hashCode()
    {
        return Arrays.hashCode(_data);
    }
}

package mathUtils;

public class Slice {
    private final int _end;
    private final int _begin;
    private final int _step;

    private static int calcIndex(final int index, final int stride)
    {
        return ((index % stride) + stride) % stride;
    }

    public Slice rebuild(final int targetLength) {
        final int begin = begin() == 0 ? 0 : calcIndex(begin(), targetLength);
        final int end   = end()   == 0 ? targetLength: calcIndex(end(),   targetLength);
        return step() > 0 ? new  Slice(begin, end == 0 ? targetLength: end, step()) :
                new  Slice( end == 0 ? targetLength: end, begin, step());
    }

    public final int index(final int index)
    {
        return begin() + index * step();
    }

    public final int length() {
        int total = Math.abs(end() - begin());
        return Math.abs(total / step() + total % step());
    }

    //exclusive index
    public final int end() {
        return _end;
    }

    //inclusive index
    public final int begin() {
        return _begin;
    }

    public final int step() {
        return _step;
    }

    public Slice() {
        this(0, 0, 1);
    }

    public Slice(final int begin, final int end) {
        this(begin, end, 1);
    }

    public Slice(final int begin, final int end, final int step) {
        this._step  = step == 0 ? 1 : step;
        this._begin = begin;
        this._end   = end;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        return equals((Slice) o);
    }

    public boolean equals(Slice slice) {
        if(slice.begin() != begin())return false;
        if(slice.end()   != end())return false;
        if(slice.step()  != step())return false;
        return true;
    }

    @Override
    public String toString(){
        return step() == 1 ? String.format("%s:%s", begin(), end()) : String.format("%s:%s:%s", begin(), end(), step());
    }
}

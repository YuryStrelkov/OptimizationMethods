package mathUtils;

@SuppressWarnings("all")
public final class Slice {
    private final int _end;
    private final int _begin;
    private final int _step;
    //exclusive index
    public int end() {
        return _end;
    }
    //inclusive index
    public int begin() {
        return _begin;
    }

    public int step() {
        return _step;
    }

    public Slice() {
        this(0, 0, 1);
    }

    public Slice(final int begin, final int end) {
        this(begin, end, 1);
    }

    public Slice(final int begin, final int end, final int step) {
        this._step  = step; // == 0 ? 1 : step;
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
        if(slice.end()   != end() )return false;
        if(slice.step()  != step())return false;
        return true;
    }

    @Override
    public String toString(){
        return step() == 1 ? String.format("%s:%s", begin(), end()) : String.format("%s:%s:%s", begin(), end(), step());
    }
}

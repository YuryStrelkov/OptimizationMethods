package mathUtils;

import mathUtils.functionalInterfaces.IFunctionND;
import java.util.HashSet;
import java.util.Set;


public final class PenaltyFunction implements IFunctionND {
    private int _penaltyFunctionMixMode;

    private final Set<IFunctionND> _boundaries;

    private IFunctionND _target;

    public int penaltyFunctionMixMode(){
        return _penaltyFunctionMixMode;
    }

    public void penaltyFunctionMixMode(int value){
         _penaltyFunctionMixMode = value % 4;
    }

    public IFunctionND target(){
        return _target;
    }

    public IFunctionND target(IFunctionND target){
        return _target = target;
    }

    public boolean appendBound(IFunctionND bound) {
        if(_boundaries.contains(bound))
            return false;
        _boundaries.add(bound);
        return true;
    }

    public boolean removeBound(IFunctionND bound) {
        if(!_boundaries.contains(bound))
            return false;
        _boundaries.remove(bound);
        return true;
    }

    public boolean appendBoundaries(IFunctionND... boundaries) {
        boolean result = false;
        for (IFunctionND bound: boundaries)
            result |= appendBound(bound);
        return result;
    }

    public boolean removeBoundaries(IFunctionND... boundaries) {
        boolean result = false;
        for (IFunctionND bound: boundaries)
            result |= removeBound(bound);
        return result;
    }

    public PenaltyFunction(){
        _boundaries = new HashSet<>();
        _target = v -> (v.dot(v));
    }

    @Override
    public double call(final DoubleVector arg) {
        double result = target() == null ? 0.0 : target().call(arg);
        switch (penaltyFunctionMixMode()){
            case 1 /*MUL*/ : for (IFunctionND bound: _boundaries) result *= bound.call(arg); break;
            case 2 /*MAX*/ : for (IFunctionND bound: _boundaries) result = Math.max(result, bound.call(arg)); break;
            case 3 /*MIN*/ : for (IFunctionND bound: _boundaries) result = Math.min(result, bound.call(arg)); break;
            default: /*SUM*/ for (IFunctionND bound: _boundaries) result += bound.call(arg); break;
        }
        return result;
    }
}

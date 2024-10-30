package mathUtils.functionalInterfaces;
import mathUtils.DoubleVector;

@FunctionalInterface
public interface IFunctionND{
    double call(final DoubleVector arg);
}

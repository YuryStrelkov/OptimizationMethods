package functionalInterfaces;
import mathUtils.DoubleVector;

@FunctionalInterface
public interface IFunctionND{
    double call(DoubleVector arg);
}

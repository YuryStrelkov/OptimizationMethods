package example.functionalInterfaces;
import example.mathUtils.DoubleVector;

@FunctionalInterface
public interface IFunctionND{
    double call(DoubleVector arg);
}

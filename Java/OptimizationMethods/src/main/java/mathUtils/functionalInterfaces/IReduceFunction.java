package mathUtils.functionalInterfaces;

@FunctionalInterface
public interface IReduceFunction <T>{
    T call(T accumulator, T value);
}

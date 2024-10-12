package example.functionalInterfaces;

@FunctionalInterface
public interface IConditionFunction<T> {
    boolean call(T element);
}

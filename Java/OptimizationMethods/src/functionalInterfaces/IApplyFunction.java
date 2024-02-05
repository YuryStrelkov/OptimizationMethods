package functionalInterfaces;

@FunctionalInterface
public interface IApplyFunction<T>{
    T call(T element);
}

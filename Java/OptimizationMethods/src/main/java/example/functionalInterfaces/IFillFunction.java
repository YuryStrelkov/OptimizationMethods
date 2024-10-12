package example.functionalInterfaces;

@FunctionalInterface
public interface IFillFunction<T>{
    T call(int index);
}
package mathUtils.functionalInterfaces;

@FunctionalInterface
public interface IForEnumerateApplyFunction<T>{
    T call(int elementIndex, T element);
}

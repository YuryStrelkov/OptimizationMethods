package mathUtils.functionalInterfaces;

@FunctionalInterface
public interface IMapFunction<T1, T2>{
    T1 call(T2 arg);
}

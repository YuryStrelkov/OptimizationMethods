package mathUtils.functionalInterfaces;

@FunctionalInterface
public interface ICombineFunction <T1, T2>{
    T2 call(T1 left, T1 right);
}

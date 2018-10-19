
public class Main{
    public static void main(String[] args) {
        
        double probability = args[1];
        ValueIterator vi = new ValueIterator(probability);

        vi.iterate();

        vi.print_policy();

    }
}
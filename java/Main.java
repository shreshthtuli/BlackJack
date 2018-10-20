
public class Main{
    public static void main(String[] args) {
        
        double probability = Double.parseDouble(args[0]);

        ValueIterator vi = new ValueIterator(probability);

        vi.iterate();

        // vi.print_policy();

    }
}
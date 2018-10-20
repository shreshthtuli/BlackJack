
public class Main{
    public static void main(String[] args) {
        
        double probability = Double.parseDouble(args[0]);

        Model m = new Model();
        for(int i = 2; i <= 11; i++){
            System.out.println(m.cardToState(i));
        }

        ValueIterator vi = new ValueIterator(probability);

        vi.iterate();

        vi.print_policy();

    }
}
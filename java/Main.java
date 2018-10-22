import javafx.util.Pair;
import java.util.ArrayList;

public class Main{
    public static void main(String[] args) {
        
        double probability = Double.parseDouble(args[0]);

        ValueIterator vi = new ValueIterator(probability);

        vi.iterate();

        vi.print_policy();

        State s = new State("6,3,false,false");
        System.out.println(vi.qStar(s, 0));
        System.out.println(vi.qStar(s, 3));
        s = new State("18,6,false,false");
        System.out.println(vi.qStar(s, 0));
        System.out.println(vi.qStar(s, 3));
        s = new State("27,2,false,false");
        System.out.println(vi.qStar(s, 0));
        System.out.println(vi.qStar(s, 2));
    }
}
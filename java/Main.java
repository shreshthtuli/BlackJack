import javafx.util.Pair;
import java.util.ArrayList;

public class Main{
    public static void main(String[] args) {
        
        double probability = Double.parseDouble(args[0]);

        ValueIterator vi = new ValueIterator(probability);

        vi.iterate();

        vi.print_policy();

        // for(String str : vi.value.keySet()){
        //     System.out.println(str + " : " + vi.value.get(str));
        // }

        // State s = new State("16,3,false,false");
        // System.out.println(vi.qStar(s, 0));
        // System.out.println(vi.qStar(s, 1));

        // System.out.println(vi.m.legalAction(s));
        // ArrayList<Pair<State,Double>> ns = vi.m.nextStates(s, 0);
        // System.out.println(vi.value.get(s.to_string()));
        // for(Pair<State,Double> nxs : ns){
        //     System.out.println("NS : " + nxs.getKey().to_string());
        //     System.out.println("Prob : " + nxs.getValue());
        //     System.out.println("Reward : " + vi.m.getReward(nxs.getKey(), 0));
        //     System.out.println("Value : " + vi.value.get(nxs.getKey().to_string()));
        // }

        // s = new State("36,3,false,false");
        // for(int i = 17; i <= 23; i++){
        //     System.out.println("Rewardhelper : " + i + " : " + vi.m.dealer_prob.get(new Pair(s.dealer_hand, i)) + " : " + vi.m.rewardHelper(s, i));
        // }

        // System.out.println(vi.qStar(new State("1,2,false,false"), 0));
        // System.out.println(vi.m.getReward(new State("36,3,false,false"), 1));


    }
}
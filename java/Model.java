import java.util.ArrayList;

public class Model{

    static int HIT = 0, STAND = 1, SPLIT = 2, DD = 3;
    double probability;
    HashMap<Pair<Integer, Integer>, Pair<Double, Double>> dealer_prob;

    Model(double p){
        this.probability = p;
    }

    ArrayList<Integer> legalAction(State s){

        ArrayList<Integer> actions = new ArrayList<>();

        if(s.hand >= 36 || s.hand == 0){
            return actions;
        }

        if(s.hand >= 26 && s.hand <= 35 && this.ace_split == false){
            actions.insert(SPLIT);
        }   

        if(s.doubled == false){
            actions.insert(HIT);
            actions.insert(DD);
        }
        
        actions.insert(STAND);

        return actions;
    }

    ArrayList<Pair<State, Integer>> nextStates(State s, int a){

    }

    double getReward(State s){
        int mysum = s.stand();
        int dealer = s.dealer_hand;
        Pair<Double, Double> probability = dealer_prob.get(Pair(mysum, dealer));
        if(s.hand == 37){
            return probability.first * 1.5;
        }
        else{
            return probability.first - probability.second;
        }
        return 0;
    }
}
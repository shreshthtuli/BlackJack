import java.util.ArrayList;

import com.sun.swing.internal.plaf.metal.resources.metal;

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

    ArrayList<Pair<State, Double>> nextStates(State s, int a){

        ArrayList<Pair<State, Double>> ans = new ArrayList<>();

        if(a = STAND)
            return ans;

        int weight;
        
        for(int i = 2; i <= 11; i++){
            State news = new State(s);
            weight = 1;
            switch(a){
                case HIT: news.hit(i); break;
                case DD: news.double_down(i); break;
                case SPLIT: news.split(i); weight = 2; break;                            
            }
            double prob;
            if(i == 10)
                prob = weight * this.probability;
            else
                prob = weight * (1 - this.probability) / 9;
            ans.insert(Pair(news, prob));
        }
        return ans;
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
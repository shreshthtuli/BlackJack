import java.util.ArrayList;

import com.sun.swing.internal.plaf.metal.resources.metal;

public class Model{

    static int HIT = 0, STAND = 1, SPLIT = 2, DD = 3;
    double probability;
    HashMap<Pair<Integer, Integer>, Double> dealer_prob;
    static double bet = 1;

    Model(double p){
        this.probability = p;
        form_dealer_prob();
    }

    void form_dealer_prob(){

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
        int weight;

        if(a == STAND)
            return ans;    

        for(int i = 2; i <= 11; i++){
            State news = new State(s);
            weight = 1;
            switch(a){
                case HIT: news.hit(i); break;
                case DD: news.double_down(i); break;
                case SPLIT: news.split(i); weight = 2; break;                            
            }
            double prob = (i == 10) ? weight * this.probability : weight * (1 - this.probability) / 9;
            ans.insert(Pair(news, prob));
        }
        return ans;
    }

    double rewardHelper(State s, int dealer_sum){
        int my_sum = s.stand();
        bool blackjack = (s.hand == 37);

        // Check blackjack
        if(blackjack && dealer_sum == 22)
            return 0;
        else if(blackjack && dealer_sum != 21)
            return 1.5 * bet;

        // Check busted
        if(s.hand == 0)
            return -bet;
        if(dealer_sum == 23)
            return +bet;

        // Check values
        if(my_sum < dealer_sum)
            return -bet;
        if(my_sum > dealer_sum)
            return +bet;

        if(dealer_sum = 21 && !blackjack)
            return -bet;
        
        // Equal values
        return 0;
    }

    double getReward(State s){
        double reward = 0;
        for(int i = 17; i <= 23; i++)
            reward += dealer_prob(Pair(s.dealer_hand, i)) * rewardHelper(s, i);

        return reward;
    }
}
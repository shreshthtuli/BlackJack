import java.util.ArrayList;
import java.util.*;
import javafx.util.*; 
import javafx.util.Pair;

public class Model{

    static int HIT = 0, STAND = 1, SPLIT = 2, DD = 3;
    double probability;
    HashMap<Pair<Integer, Integer>, Double> dealer_prob;
    static double bet = 1;

    Model(double p){
        this.probability = p;
        this.dealer_prob = new HashMap<>();
        form_dealer_prob();
    }

    double get_dealer_prob(int init_hand, int target){
        //Base cases
        State s = new State();
        s.hand = init_hand;
        Pair<Integer,Integer> p = new Pair<>(init_hand,target);

        if(dealer_prob.get(p) != null){
            return dealer_prob.get(p);
        }

        else if(s.stand() > target){
            dealer_prob.put(p,0.0);
        }

        else if(s.stand() == target){
            dealer_prob.put(p,1.0);
        }

        else{
            ArrayList<Integer> actions = legalAction(s);
            double prob = 0;
            for(int i = 0; i < actions.size(); i++){
                int action = actions.get(i);
                if(action != SPLIT && action != DD){
                    ArrayList<Pair<State, Double>> next_states = nextStates(s,action);
                    for(int j = 0; j < next_states.size(); j++){
                        prob += next_states.get(j).getValue()*get_dealer_prob(s.hand,p.getValue());
                    }
                }
            }
            dealer_prob.put(p,prob);
        }
        return dealer_prob.get(p);
    }

    void form_dealer_prob(){
        //iterate over all targets
        for(int i = 17; i <= 23; i++){
            //iterate over all initial hands
            for(int j = 2; j <= 11; j++){
                Pair<Integer,Integer> p = new Pair<Integer,Integer>(j,i);
                double prob = get_dealer_prob(j,i);
                dealer_prob.put(p,prob);
            }
        }
    }

    ArrayList<Integer> legalAction(State s){

        ArrayList<Integer> actions = new ArrayList<>();

        if(s.hand >= 36 || s.hand == 0){
            return actions;
        }

        if(s.hand >= 26 && s.hand <= 35 && s.ace_split == false){
            actions.add(SPLIT);
        }   

        if(s.doubled == false){
            actions.add(HIT);
            actions.add(DD);
        }
        
        actions.add(STAND);

        return actions;
    }

    ArrayList<Pair<State, Double>> nextStates(State s, int a){

        ArrayList<Pair<State, Double>> ans = new ArrayList<>();
        int weight;
        State news;

        if(a == STAND)
            return ans;    

        for(int i = 2; i <= 11; i++){
            news = new State(s);
            weight = 1;
            switch(a){
                case 0: news.hit(i); break;
                case 3: news.double_down(i); break;
                case 2: news.split(i); weight = 2; break;                            
            }
            double prob = (i == 10) ? weight * this.probability : weight * (1 - this.probability) / 9;
            ans.add(new Pair(news, prob));
        }
        return ans;
    }

    ArrayList<Pair<Integer, Double>> cardToState(int card){
        ArrayList<Pair<State, Double>> ans = new ArrayList<>();
        
        if(card == 11){
            // hit card is 2 to 9 so A2 to A9
            for(int i = 2; i <= 9; i++){
                ans.add(new Pair(16 + i, ((1 - this.probability) / 9)));
            }
            // hit card is face so BJ
            ans.add(new Pair(37, this.probability));
            // hit card is ace so AA
            ans.add(new Pair(35, ((1 - this.probability) / 9)));
        }
        else{
            // hit card is Ace
            if(card == 10){
                // BJ
                ans.add(new Pair(37, ((1 - this.probability) / 9)));
            }
            else{
                // A2 to A9
                ans.add(new Pair(16 + card, ((1 - this.probability) / 9)));
            }
            // hit card is 2 to 9
            for(int i = 2; i <= 9; i++){
                if(card == i){
                    // pair
                    and.add(new Pair(24 + i, ((1 - this.probability) / 9)));
                }
                else{
                    ans.add(new Pair(card + i - 3, ((1 - this.probability) / 9)));
                }
            }
            // hit card = 10
            if(card == 10){
                and.add(new Pair(34, this.probability));
            }
            else{
                and.add(new Pair(card + 7, this.probability));
            }
            
        }
        return ans;
    }

    double rewardHelper(State s, int dealer_sum){
        int my_sum = s.stand();
        boolean blackjack = (s.hand == 37);

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

        if(dealer_sum == 21 && !blackjack)
            return -bet;
        
        // Equal values
        return 0;
    }

    double getReward(State s){
        double reward = 0;
        for(int i = 17; i <= 23; i++)
            reward += dealer_prob.get(new Pair(s.dealer_hand, i)) * rewardHelper(s, i);

        return reward;
    }
}


import java.util.ArrayList;
import java.util.HashMap;
import java.util.function.DoubleToLongFunction;

import javax.lang.model.type.NullType;

import javafx.util.Pair;

public class ValueIterator{

    static double epsilon = 0.00000000002;
    HashMap <String, Double> value;
    HashMap <String, Integer> policy;
    ArrayList<State> allStates;
    Model m;

    ValueIterator(double p){
        this.value = new HashMap<>();
        this.policy = new HashMap<>();
        this.allStates = new ArrayList<>();
        this.m = new Model(p);
        // this.find_all_states();
        this.initialise_value();
    }

    void print_policy(){
        for(int i = 2; i <= 35; i++){
            if(i == 20)
                continue;
            System.out.print("Hand : " + i + "\t");
            for(int j = 2; j <= 11; j++){
                State newState = new State();
                newState.hand = i;
                newState.dealer_hand = j;
                System.out.print(policy.get(newState.to_string()) + " ");
            }
            System.out.println();
        }
    }

    // void find_all_states_helper(State s){
    //     if(allStates.contains(s))
    //         return;

    //     allStates.add(s);
    //     ArrayList<Integer> actions = this.m.legalAction(s);
    //     ArrayList< Pair<State,Double> > nextState;

    //     for(int i = 0; i < actions.size(); i++){
    //         nextState = m.nextStates(s, actions.get(i));
    //         for(int j = 0; j < nextState.size(); j++){
    //             find_all_states_helper(nextState.get(j).getKey());
    //         }
    //     } 
    // }

    // void find_all_states(){
    //     for(int i = 1; i <= 37; i++){
    //         for(int j = 2; j <= 11; j++){
    //             State newState = new State();
    //             newState.hand = i;
    //             newState.dealer_hand = j;
    //             find_all_states_helper(newState);
    //         }
    //     }
    // }

    void initialise_value(){
        for(int i = 1; i <= 37; i++){
            for(int j = 2; j <= 11; j++){
                // System.out.println(allStates.size());
                State newState = new State();
                newState.hand = i;
                newState.dealer_hand = j;
                value.put(newState.to_string(), 0.0);

                newState.doubled = true;
                value.put(newState.to_string(), 0.0);

                newState.doubled = false;
                newState.ace_split = true;
                value.put(newState.to_string(), 0.0);

                newState.doubled = true;
                value.put(newState.to_string(), 0.0);
            }
        }
    }

    double qStar(State s, int a){
        double prob, prob1;
        State sPrime, sPrime1;
        ArrayList< Pair<State,Double> > next_States = m.nextStates(s, a);
        double val = 0;
        if(a == 1){
            return m.getReward(s, 1);
        }
        if(a == 2){
            for(int i = 0; i < next_States.size(); i++){
                for(int j = 0; j < next_States.size(); j++){
                    sPrime = next_States.get(i).getKey();
                    sPrime1 = next_States.get(j).getKey();
                    prob = next_States.get(i).getValue();
                    prob1 = next_States.get(j).getValue();
                    val += (m.getReward(sPrime, a) + value.get(sPrime.to_string())
                         + m.getReward(sPrime1, a) + value.get(sPrime1.to_string())) * prob * prob1;
                }
            }
            return val;
        }
        
        // Iterate on all next states
        for(int j = 0; j < next_States.size(); j++){
            sPrime = next_States.get(j).getKey(); // s'
            prob = next_States.get(j).getValue(); // T(s, pi(s), s')        
            // System.out.println(sPrime.to_string() + "  " + value.get(sPrime.to_string()));
            if(sPrime.hand != 0)    
                val += prob * (m.getReward(sPrime, a) + value.get(sPrime.to_string()));
            else
                val += prob * m.getReward(sPrime, a);
        }
        return val;
    }

    double vStar(State s)
    {
        ArrayList<Integer> actions = m.legalAction(s);

        if(actions.size() == 0)
            return m.getReward(s, 1);

        int a, aMax;
        double val, valMax;

        aMax = actions.get(0);
        valMax = qStar(s, aMax);
        // Iterate over all action
        for(int i = 0; i < actions.size(); i++){
            a = actions.get(i);
            val = this.qStar(s, a);
            if(val > valMax){
                valMax = val;
                aMax = a;
            }
        }
        return valMax;
    }

    void updatePolicy()
    {
        ArrayList<Integer> actions;
        int a, aMax;
        double val, valMax;
    
        // Iterate over all states
        for(String str : value.keySet()){
            State s = new State(str);
            // Get action vector for this state
            actions = m.legalAction(s);
            
            if(actions.isEmpty()){
                policy.put(str, 0); continue;
            }

            aMax = actions.get(0);
            valMax = qStar(s, aMax);
            // Check best action
            for(int i = 0; i < actions.size(); i++){
                a = actions.get(i);
                val = qStar(s, a);
                if(val >= valMax){
                    valMax = val;
                    aMax = a;
                }
            }
            // Update policy
            policy.put(str, aMax);
        }
    }

    void iterate()
    {
        double residual = epsilon;
        HashMap <String, Double> nextValue = new HashMap<>(value);
    
        // Compute values till residual < epsilon
        while(residual >= epsilon){
            // Iterate over all states
            residual = 0;
            for(String str : value.keySet()){
                State s = new State(str);
                // Update value based on bellman equation
                nextValue.replace(str, vStar(s));
                // Calculate residual
                residual = Math.max(residual, Math.abs(nextValue.get(str) - value.get(str)));
            }
            System.out.println(residual);
            // Update value to next value
            value = (HashMap)nextValue.clone();
        }

        updatePolicy();
    }

    public static void main(String[] args){
        ValueIterator v = new ValueIterator(0.7);
        State s= new State();
        s.hand = 0;
        s.dealer_hand = 3;
        v.find_all_states_helper(s);
    }
}
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
    String[] convert = {"bust", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13",
        "14", "15", "16", "17", "18", "19", "20", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9",
        "22", "33", "44", "55", "66", "77", "88", "99", "1010", "AA", "21", "BJ"};

    ValueIterator(double p){
        this.value = new HashMap<>();
        this.policy = new HashMap<>();
        this.allStates = new ArrayList<>();
        this.m = new Model(p);
        this.initialise_value();
    }

    void print_policy(){
        for(int i = 2; i <= 35; i++){
            if(i == 20)
                continue;
            System.out.print(convert[i] + "\t");
            for(int j = 2; j <= 11; j++){
                State newState = new State();
                newState.hand = i;
                newState.dealer_hand = j;
                int a = policy.get(newState.to_string());
                switch (a){
                    case 0 : System.out.print("H "); break;
                    case 1 : System.out.print("S "); break;
                    case 2 : System.out.print("P "); break;
                    case 3 : System.out.print("D "); break;
                }
                
            }
            System.out.println();
        }
    }

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
                    val += (value.get(sPrime.to_string())
                         + value.get(sPrime1.to_string())) * prob * prob1;
                }
            }
            return val;
        }
        // Iterate on all next states
        for(int j = 0; j < next_States.size(); j++){
            sPrime = next_States.get(j).getKey(); // s'
            prob = next_States.get(j).getValue(); // T(s, pi(s), s')        
            if(sPrime.hand != 0)    
                val += prob * (value.get(sPrime.to_string()));
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
        int iteration_no = 0;
    
        // Compute values till residual < epsilon
        while(residual >= epsilon && iteration_no < 500){
            // Iterate over all states
            residual = 0;
            for(String str : value.keySet()){
                State s = new State(str);
                // Update value based on bellman equation
                nextValue.replace(str, vStar(s));
                // Calculate residual
                residual = Math.max(residual, Math.abs(nextValue.get(str) - value.get(str)));
            }
            // Update value to next value
            value = (HashMap)nextValue.clone();
            iteration_no++;
        }

        updatePolicy();
    }
}
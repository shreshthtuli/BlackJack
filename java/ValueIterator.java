import java.util.ArrayList;
import java.util.HashMap;
import java.util.function.DoubleToLongFunction;

import javax.lang.model.type.NullType;

public class ValueIterator{

    static double epsilon = 0.002;
    HashMap <State, Double> value;
    HashMap <State, Integer> policy;
    ArrayList<State> allStates;
    Model m;

    ValueIterator(double p){
        this.value = new HashMap<>();
        this.policy = new HashMap<>();
        this.allStates = new ArrayList<>();
        this.m = new Model(p);
        this.find_all_states();
        this.initialise_value();
    }

    void print_policy(){
        for(int i = 1; i <= 37; i++){
            for(int j = 2; j <= 11; j++){
                State newState = new State();
                newState.hand = i;
                newState.dealer_hand = j;
                System.out.print(policy.get(newState) + " ");
            }
            System.out.println();
        }
    }

    void find_all_states_helper(State s){
        allStates.insert(s);
        ArrayList<Integer> actions = m.legalActions(s);
        ArrayList< Pair<State,Double> > nextState;

        for(int i = 0; i < actions.size(); i++){
            nextState = m.next_States(s, actions.at(i));
            for(int j = 0; j < nextState.size(); j++){
                find_all_states_helper(nextState.at(j).first);
            }
        } 
    }

    void find_all_states(){
        for(int i = 1; i <= 37; i++){
            for(int j = 2; j <= 11; j++){
                State newState = new State();
                newState.hand = i;
                newState.dealer_hand = j;
                find_all_states_helper(newState);
            }
        }
    }

    void initialise_value(){
        for(int i = 0; i < allStates.size(); i++){
            value.put(allStates.at(i), 0);
        }
    }

    double qStar(State s, int a){
        State sPrime;
        double prob;
        ArrayList< Pair<State,Double> > next_States = m.next_States(s, a);
        double val = 0;
        // Iterate on all next states
        for(int j = 0; j < next_States.size(); j++){
            sPrime = next_States.at(j).first; // s'
            prob = next_States.at(j).second; // T(s, pi(s), s')
            val += prob * (m.get_reward(sPrime) + value.get(sPrime));
        }
        return val;
    }

    double vStar(State s)
    {
        ArrayList<Integer> actions = m.legalActions(s);

        if(actions.size() == 0)
            return m.get_reward(s);

        int a, aMax;
        double val, valMax;

        aMax = actions.at(0);
        valMax = qStar(s, aMax);
        // Iterate over all action
        for(int i = 0; i < actions.size(); i++){
            a = actions.at(i);
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
        for(State s : value.keySet()){
            // Get action vector for this state
            actions = m.legalActions(s);
    
            aMax = actions.at(0);
            valMax = qStar(itr.first, aMax);
            // Check best action
            for(int i = 0; i < actions.size(); i++){
                a = actions.at(i);
                val = qStar(itr.first, a);
                if(val > valMax){
                    valMax = val;
                    aMax = a;
                }
            }
            // Update policy
            policy.put(s, aMax);
        }
    }

    void iterate()
    {
        double residual = epsilon;
        HashMap <State, Double> nextValue = new HashMap<>(value);
    
        // Compute values till residual < epsilon
        while(residual >= epsilon){
            // Iterate over all states
            for(State s : value.keySet()){
                // Update value based on bellman equation
                nextValue.replace(s, vStar(s));
                // Calculate residual
                residual = max(residual, abs(nextValue.get(curState) - value.get(curState)));
            }
            // Update value to next value
            value = (HashMap)nextValue.clone();
        }

        updatePolicy();
    }
}
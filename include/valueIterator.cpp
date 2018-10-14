#include "valueIterator.h"
#define epsilon 0.01

using namespace std;

void ValueIterator::initialiser(vector< pair<State,double> > initital_value)
{
    // Initialise policy and values
    for(int i = 0; i < initial_value.size(); i++){
        value[initial_value.at(i).first] = initital_value.at(i).second;
    }
}

double ValueIterator::qStar(State s, int a)
{
    State sPrime;
    double prob;
    vector< pair<State,float> > next_States = m.next_States(s, a);
    // determine value as sum of expected rewards
    double val = 0;
    // Iterate on all next states
    for(int j = 0; j < next_States.size(); j++){
        sPrime = next_States.at(j).first; // s'
        prob = next_States.at(j).second; // T(s, pi(s), s')
        val += prob * (m.get_reward(sPrime) + value.at(sPrime));
    }
    return val;
}

void ValueIterator::updateValue(State s)
{
    vector<int> actions = m.actions(s);
    int a, aMax;
    double val, valMax;

    aMax = actions.at(0);
    valMax = qStar(s, aMax);
    // Iterate over all action
    for(int i = 0; i < actions.size(); i++){
        a = actions.at(i);
        val = qStar(s, a);
        if(val > valMax){
            valMax = val;
            aMax = a;
        }
        // Update value
        value[s] = aMax;
    }
}

void ValueIterator::updatePolicy()
{
    map <State, int> :: iterator itr;
    vector<int> actions;
    int a, aMax;
    double val, valMax;

    // Iterate over all states
    for(itr = policy.begin(); itr != policy.end(); ++itr){
        // Get action vector for this state
        actions = m.actions(itr->first);

        aMax = actions.at(0);
        valMax = qStar(itr->first, aMax);
        // Check best action
        for(int i = 0; i < actions.size(); i++){
            a = actions.at(i);
            val = qStar(itr->first, a);
            if(val > valMax){
                valMax = val;
                aMax = a;
            }
        }
        // Update policy
        policy[itr->first] = aMax;
    }
}

void ValueIterator::iterate()
{
    double residual = epsilon;
    State sPrime;
    double prob;
    map <State, double> :: iterator itr;
    map<State, double> nextValue;

    // Compute values till residual < epsilon
    while(residual >= epsilon){
        // Iterate over all states
        for(itr = value.begin(); itr != value.end(); ++itr){
            // Update value based on bellman equation
            nextValue[itr->first] = qStar(itr->first, itr->second);
            // Calculate residual
            residual = max(residual, abs(nextValue.at(itr->first) - value.at(itr->first)));
        }
        // Update value to next value
        value = nextValue;
    }

    updatePolicy();
}
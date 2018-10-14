#include "policyIterator.h";
#define epsilon 0.01

using namespace std;

void PolicyIterator::initialiser(vector< pair<State,int> > initial_policy)
{
    // Initialise policy and values
    for(int i = 0; i < initial_policy.size(); i++){
        policy[initial_policy.at(i).first] = initial_policy.at(i).second;
        value[initial_policy.at(i).first] = 0;
    }
}

double PolicyIterator::qStar(State s, int a)
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

void PolicyIterator::evaluate_policy()
{
    double residual = 0.01;
    State sPrime;
    double prob;
    map <State, int> :: iterator itr;
    map<State, double> nextValue;

    while(residual >= epsilon){
        // Iterate over all states
        for(itr = policy.begin(); itr != policy.end(); ++itr){
            // Update next value
            nextValue[itr->first] = qStar(itr->first, itr->second);
            // Calculate residual
            residual = max(residual, abs(nextValue.at(itr->first) - value.at(itr->first)));
        }
        // Update value to next value
        value = nextValue;
    }
}

bool PolicyIterator::updatePolicy()
{
    map <State, int> :: iterator itr;
    vector<int> actions;
    int a, aMax;
    double val, valMax;
    bool same = true;

    // Iterate over all states
    for(itr = policy.begin(); itr != policy.end(); ++itr){
        // Get action vector for this state
        actions = m.legalActions(itr->first);

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

        // Check if policy is same
        same = same && (aMax == itr->second);
        // Update policy
        policy[itr->first] = aMax;
    }

    return same;
}

void PolicyIterator::iterate()
{
    bool same = true;
    map <State, int> :: iterator itr;

    while(!same){
        // Iterate over all states
        for(itr = policy.begin(); itr != policy.end(); ++itr){
            evaluate_policy();
            same = updatePolicy();
        }

    }
}

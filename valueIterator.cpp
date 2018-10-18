#include "./include/valueIterator.h"
#define epsilon 0.01

using namespace std;

void ValueIterator::initialiser(vector<State*> initial_value)
{
    // Initialise policy and values
    for(int i = 0; i < initial_value.size(); i++){
        value.insert(make_pair(initial_value[i], 0));
    }
    cerr << value.size() << endl;
}

double ValueIterator::qStar(State* s, int a)
{
    // cerr << "entered qstar, action : " << a << "\n";
    State* sPrime = new State();
    double prob;
    vector< pair<State*,float> > next_States = m.next_States(s, a);
    // determine value as sum of expected rewards
    double val = 0;
    double vPrime = 0;
    // Iterate on all next states
    for(int j = 0; j < next_States.size(); j++){
        // cerr << "j : " << j << endl;
        sPrime = next_States.at(j).first; // s'
        prob = next_States.at(j).second; // T(s, pi(s), s')
        if(value.find(sPrime) == value.end()){
            // cerr << "New state found!!\n";
            value.insert(make_pair(sPrime, 0));
        } 
        val += prob * (m.get_reward(sPrime) + value.at(sPrime));
    }
    // cerr << "exiting qstar\n";
    return val;
}

double ValueIterator::vStar(State* s)
{
    // cerr << "entered vstar\n";
    vector<int> actions = m.legalActions(s);

    if(actions.size() == 0)
        return m.get_reward(s);

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
    }
    // cerr << "exiting vstar\n";
    return valMax;
}

void ValueIterator::updatePolicy()
{
    map <State, double> :: iterator itr;
    vector<int> actions;
    int a, aMax;
    double val, valMax;
    State *s;

    // Iterate over all states
    for(auto itr : value){
        s = itr.first;
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
        policy[s] = aMax;
    }
}

void ValueIterator::iterate()
{
    double residual = epsilon;
    State *curState;
    map <State, double> :: iterator itr;
    map <State*, double> nextValue;

    // Compute values till residual < epsilon
    while(residual >= epsilon){
        std::cerr << "Residual value : " << residual << std::endl;
        cerr << "Size of value : " << value.size() << endl;
        // Iterate over all states
        for(auto itr : value){
            curState = itr.first;
            // m.showState(curState);
            // Update value based on bellman equation
            nextValue[curState] = vStar(curState);
            // Calculate residual
            residual = max(residual, abs(nextValue.at(curState) - value.at(curState)));
        }
        // Update value to next value
        value = nextValue;
    }

    updatePolicy();
}
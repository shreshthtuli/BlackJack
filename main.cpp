#include "./include/policyIterator.h"
#include "./include/valueIterator.h"
#include "./include/model.h"
#include <sstream>
#include <iostream>

using namespace std;

vector< pair<State,double> > initial_value;
ValueIterator vi;
vector <State*> allStates;
Model m;

void state_gen_helper(State* s)
{
    // m.showState(s);
    allStates.push_back(s);
    vector <int> actions = m.legalActions(s);
    // m.showActions(actions);
    vector< pair<State,float> > nextState;
    if(actions.size() == 0)
        cerr << "-";
    // for(int i = 0; i < actions.size(); i++){
    //     nextState = m.next_States(s, actions[i]);
    //     // cout << actions[i] << endl;
    //     for(int j = 0; j < nextState.size(); j++){
    //         state_gen_helper(nextState[j].first);
    //     }
    // } 
}

void state_generator()
{
    vector<int> v;
    for(int i = 2; i <= 11; i++){
        for(int j = 2; j <= 11; j++){
            for(int k = 2; k <= 11; k++){
                State* s = new State();
                s->curr_hand = 0;
                s->dealer_final = false;
                s->initial.push_back(true);
                v = {i, j};
                s->hands.push_back(v);
                s->dealer_hand = {k};
                state_gen_helper(s);
            }
        }
    }
}

void initialize_value()
{
    // Initial value for value iteration
    for(int i = 0; i < allStates.size(); i++){
        initial_value.push_back(make_pair(allStates[i], 0));
    }
}

void print_policy(map<State*,int> policy)
{
    stringstream ss;
    // Print the policy for required states
    for(int i = 0; i <= 11; i++){
        for(int j = 0; j <= 11; j++){
            for(int k = 0; k <= 11; k++){
                State *s;
                s->curr_hand = 0;
                s->dealer_final = false;
                s->initial.push_back(true);
                s->hands.push_back({i, j});
                s->dealer_hand.push_back(k);
                ss << policy.at(s) << std::endl;  
                std::cout << ss.str();              
            }
        }
    }
}

int main(int argc, char *argv[])
{
    state_generator();
    std::cerr << "Generated all states\n";
    // Form intial value
    // initialize_value(allStates);
    vi.initialiser(allStates);
    std::cerr << "Initialized value\n";
    vi.m.probability = int(argv[1]);

    // Start iteration
    vi.iterate();
    std::cerr << "Iterations complete\n";

    // Print policy
    print_policy(vi.policy);
}
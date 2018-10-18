#include "./include/policyIterator.h"
#include "./include/valueIterator.h"
#include "./include/model.h"
#include <sstream>
#include <iostream>

using namespace std;

vector< pair<State,double> > initial_value;
ValueIterator vi;
vector <State> allStates;
vector< pair<State,float> > nextState;
vector <int> actions;
Model m;

void state_gen_helper(State s)
{
    allStates.push_back(s);
    actions = m.legalActions(s);
    for(int i = 0; i < actions.size(); i++){
        nextState = m.next_States(s, actions.at(i));
        for(int j = 0; j < nextState.size(); j++){
            state_gen_helper(nextState.at(j).first);
        }
    } 
}

void state_generator()
{
    for(int i = 0; i <= 11; i++){
        for(int j = 0; j <= 11; j++){
            for(int k = 0; k <= 11; k++){
                State s;
                s.curr_hand = 0;
                s.dealer_final = false;
                s.initial.push_back(true);
                s.hands.push_back({i, j});
                s.dealer_hand.push_back(k);
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

void print_policy(map<State,int> policy)
{
    stringstream ss;
    // Print the policy for required states
    for(int i = 0; i <= 11; i++){
        for(int j = 0; j <= 11; j++){
            for(int k = 0; k <= 11; k++){
                State s;
                s.curr_hand = 0;
                s.dealer_final = false;
                s.initial.push_back(true);
                s.hands.push_back({i, j});
                s.dealer_hand.push_back(k);
                ss << policy.at(s) << std::endl;  
                std::cout << ss.str();              
            }
        }
    }
}

int main(int argc, char *argv[])
{
    // Form intial value
    initialize_value();
    vi.initialiser(initial_value);
    vi.m.probability = int(argv[1]);

    // Start iteration
    vi.iterate();

    // Print policy
    print_policy(vi.policy);
}
#include "./include/policyIterator.h"
#include "./include/valueIterator.h"

using namespace std;

vector< pair<State,double> > initial_value;
ValueIterator vi;
vector <State> allStates;

void state_generator()
{
    
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
    // Print the policy for required states
    for(int i = 0; i < allStates.size(); i++){
        // Print all required states
    }
}

int main(int argc, char *argv[])
{
    // Form intial value
    initialize_value();
    vi.initialiser(initial_value);
    vi.m.probability = int(argv[0]);

    // Start iteration
    vi.iterate();

    // Print policy
    print_policy(vi.policy);
}
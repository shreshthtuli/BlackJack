#include "./include/policyIterator.h"
#include "./include/valueIterator.h"

using namespace std;

vector< pair<State,double> > initial_value;
ValueIterator vi;

void initialize_value()
{
    // Initial value for value iteration
}

void print_policy(map<State,int> policy)
{
    // Print the policy for required states
}

int main()
{
    // Form intial value
    initialize_value();
    vi.initialiser(initial_value);

    // Start iteration
    vi.iterate();

    // Print policy
    print_policy(vi.policy);
}
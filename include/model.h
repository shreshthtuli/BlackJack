#ifndef MODEL_H
#define MODEL_H

#include "state.h"

class Model{
public:
	//prob given as input
	float probability;
	//my bet
	float bet;
	//function to get reward given state 
	float get_reward(State& s);
	//function returns a vector of states alongwith probabilities given current state and action
	vector< pair<State,float> > next_States(State currState, int action);
	//function returns a vector of actions for a possible state
	vector<int> actions(State currState);
};

#endif






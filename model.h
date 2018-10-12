class State{
public:
	//boolean to tell whether it is initial stage of game
	bool initial;
	//vector of all my hands (could be more than 1 due to split)
	vector< vector<int> > hands;
	//dealer card we know
	int dealer_card;
	//reward of this state
	float reward;
	//whether state is final or not
	bool final;
};

class Model{
public:
	//prob given as input
	float probability;
	//my bet
	float bet;
	//function to get reward given state 
	float get_reward(State& s);
	//function returns a vector of states alongwith probabilities given current state and action
	vector< pair<State,float> > next_States(State& currState, int action);
};

class PolicyIterator{
public:
	Model m;
	map<State,int> policy;
	void initialiser(vector< pair<State,int> > initial_policy);
	void evaluate_policy();
	void updatePolicy();
}





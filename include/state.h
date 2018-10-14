#ifndef STATE_H
#define STATE_H

#include <iostream>
#include <vector>

using namespace std;

class State{ 
public:
	//keeps track of current hand
	int curr_hand = 0;
	//boolean to tell whether each hand is initial or not
	vector<bool> initial;
	//vector of all my hands (could be more than 1 due to split)
	vector< vector<int> > hands;
	//dealer card we know
	vector<int> dealer_hand;
	//reward of this state
	float reward;
	//whether each hand is final or not
	vector<bool> hands_final;
	//if dealer is final
	bool dealer_final;
	//if all hands and dealer are final, then state is final

};

#endif
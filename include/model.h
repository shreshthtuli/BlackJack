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
	float get_reward(State* s);
	//function returns a vector of states alongwith probabilities given current state and action
	vector< pair<State*,float> > next_States(State* currState, int action);
	//legal actions for a state
	vector<int> legalActions(State* currState);

void showActions(vector<int> actions){
	int act;
	for(int i = 0; i < actions.size(); i++){
		act = actions[i];
		if(act == 0){
			cout << "HIT" << endl;
		}
		else if(act == 1){
			cout << "STAND" << endl;
		}
		else if(act == 2){
			cout << "SPLIT" << endl;
		}
		else if(act == 3){
			cout << "DOUBLE DOWN" << endl;
		}
		else if(act == 4){
			cout << "DEALER HIT" << endl;
		}
		else if(act == 5){
			cout << "DEALER STAND" << endl;
		}
	}
}

void showHand(vector<int> hand){
	for(int i = 0; i < hand.size(); i++){
		cout << hand[i] << " ";
	}
}

void showHands(vector< vector<int> > hands){
	for(int i = 0; i < hands.size(); i++){
		showHand(hands[i]);
		cout << ",";
	}
	cout << endl;
}

void showState(State* s){
	cout << "My hands " << endl;
	showHands(s->hands);
	cout << "Dealer hand" << endl;
	showHand(s->dealer_hand);
	cout << endl;
}

void showNextStates(vector< pair<State*,float> > nextStates){
	for(int i = 0; i < nextStates.size(); i++){
		cout << "Probability : " << nextStates[i].second << endl;
		cout << "State :" << endl;
		showState(nextStates[i].first);
		cout << "\n\n";
	}
}
};

#endif






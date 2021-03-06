#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <random>
#include "./include/model.h"

using namespace std;

//global variables representing actions
int HIT = 0;
int STAND = 1;
int SPLIT = 2;
//double down
int DD = 3;
//Dealer hit
int D_HIT = 4;
//Dealer stand
int D_STAND = 5;
//Maximum number of hands
int MAX_NUM_HANDS = 2;


int sum_hand(vector<int> hand){
	int num_aces = 0, value = 0;
	int soften = 0; // number of aces to give value = 1

	for(int i = 0; i < hand.size(); i++){
		if(hand[i] == 11)
			num_aces++;
		value += hand[i];
	}

	// Now value is sum with all aces counted as 11 i.e. hard value
	while(value > 21 && soften <= num_aces){
		value -= 10 * soften;
		soften++;
	}
	return value;
}


vector<int> Model::legalActions(State* currState){
	vector<int> actions;
	// cerr << "entered legal actions\n";
	// cerr << currState->curr_hand << endl;
	// cerr << currState->hands.size() << endl;
	//atleast one hand is not final
	if(currState->curr_hand != -1){
		
		//if single hand
		if(currState->hands.size() == 1){

			//if hand is initial
			if(currState->initial[0]){

				//if BlackJack
				if(currState->hands[0][0] == 10 && currState->hands[0][1] == 11){
					currState->curr_hand = -1;
					//currState->hands_final[0] = true;
					currState->dealer_final = true;
					actions = legalActions(currState);
					return actions;
				}

				//else if both Aces
				else if(currState->hands[0][0] == 11 && currState->hands[0][1] == 11){
					actions.push_back(HIT);
					actions.push_back(STAND);
					actions.push_back(SPLIT);
					actions.push_back(DD);
				}

				//else if both cards are same
				else if(currState->hands[0][0] == currState->hands[0][1]){
					actions.push_back(HIT);
					actions.push_back(STAND);
					actions.push_back(SPLIT);
					actions.push_back(DD);
				}

				//else if both cards are different
				else{
					actions.push_back(HIT);
					actions.push_back(STAND);
					actions.push_back(DD);
				}
			}

			//if hand is not initial
			else{
				int value = sum_hand(currState->hands[0]);
				//we can either hit or stand
				//can't hit if total is 21
				if(value < 21){
					actions.push_back(STAND);
					actions.push_back(HIT);
				}

				else if(value == 21){
					actions.push_back(STAND);
				}
			}
		}

		//if multiple hands
		else if(currState->hands.size() > 1){

			//find first non-final hand,
			int first_NF = currState->curr_hand;
			//if first card of current hand is Ace, no further actions possible for this hand
			//move to next hand
			if(currState->hands[currState->curr_hand][0] == 11){
				State *nextState = new State(currState);
				//if more hands available, move to next hand
				if(currState->curr_hand < currState->hands.size() - 1){
					nextState->curr_hand = currState->curr_hand + 1;
				}
				//else declare final state
				else{
					nextState->curr_hand = -1;
				}
				actions = legalActions(nextState);
				return actions;
			}

			bool new_init = currState->initial[first_NF]; 
			vector<int> newHand = currState->hands[first_NF];

			//create a new state corresponding to that hand only and find all nextStates for this state
			State *newState = new State();
			newState->curr_hand = 0;
			if(currState->hands.size() < MAX_NUM_HANDS){
				newState->initial.push_back(currState->initial[first_NF]);
			}
			else{
				newState->initial.push_back(false);
			}
			newState->hands.push_back(currState->hands[currState->curr_hand]);
			newState->dealer_hand = currState->dealer_hand;
			newState->reward = currState->reward;
			//newState->hands_final.push_back(false);
			newState->dealer_final = currState->dealer_final;

			actions = legalActions(newState);

		}


	}

	//all hands are final and dealer is final too
	else if(currState->dealer_final){
		//no further states possible, this is a reward state
		return actions;
	}

	//all hands are final but dealer is not final
	else{

		//follow deterministic policy of dealer
		if(sum_hand(currState->dealer_hand) < 17){
			actions.push_back(D_HIT);
		}
		else{
			actions.push_back(D_STAND);
		}
	}
	// cerr <<"exit legal actions\n";
	return actions;
}

//assumes the given action is legal in this state
vector< pair<State*,float> > Model::next_States(State* currState, int action){
	int prob = this->probability;
	vector< pair<State*,float> > res;

	if(action == HIT){
		pair<State*,float> p;
		State *newState = new State(currState);
		//check here that changing newState doesn't change currState

		//face card
		newState->hands[newState->curr_hand].push_back(10);
		//check if this hand busts
		if(sum_hand(newState->hands[newState->curr_hand]) >= 21){
			//if more hands available, move to next hand
			if(currState->curr_hand < currState->hands.size() - 1){
				newState->curr_hand = currState->curr_hand + 1;
			}
			//else declare final state
			else{
				newState->curr_hand = -1;
			}
		}
		p.first = newState;
		p.second = prob;
		res.push_back(p);

		//2-9 cards
		for(int i = 2; i < 10; i++){
			State *newState = new State(currState);
			newState->hands[newState->curr_hand].push_back(i);
			//check if this hand busts
			if(sum_hand(newState->hands[newState->curr_hand]) >= 21){
				//if more hands available, move to next hand
				if(currState->curr_hand < currState->hands.size() - 1){
					newState->curr_hand = currState->curr_hand + 1;
				}
				//else declare final state
				else{
					newState->curr_hand = -1;
				}
			}
			p.first = newState;
			p.second = (1-prob)/9.0;
			res.push_back(p);
		}

		//Ace
		*newState = new State(currState);
		newState->hands[newState->curr_hand].push_back(11);
		//check if this hand busts
		//sum_hand takes into account hard and soft, return best value possible
		if(sum_hand(newState->hands[newState->curr_hand]) >= 21){
			//if more hands available, move to next hand
			if(currState->curr_hand < currState->hands.size() - 1){
				newState->curr_hand = currState->curr_hand + 1;
			}
			//else declare final state
			else{
				newState->curr_hand = -1;
			}
		}
		p.first = newState;
		p.second = (1-prob)/9.0;
		res.push_back(p);
	}

	if(action == D_HIT){
		pair<State*,float> p;
		State *newState = new State(currState);

		newState->dealer_hand.push_back(10);
		p.first = newState;
		p.second = prob;
		res.push_back(p);

		for(int i = 2; i < 12; i++){
			State *newState = new State(currState);
			newState->dealer_hand.push_back(i);
			if(sum_hand(newState->dealer_hand) >= 17){
				newState->dealer_final = true;
			}
			p.first = newState;
			p.second = (1-prob)/9.0;
			res.push_back(p);
		}
	}

	if(action == D_STAND){
		State *newState = new State(currState);
		newState->dealer_final = true;
		pair<State*,float> p;
		p.first = newState;
		p.second = 1.0;
		res.push_back(p);
	}

	if(action == STAND){
		State *newState = new State(currState);
		//if more hands available, move to next hand
		if(currState->curr_hand < currState->hands.size() - 1){
			newState->curr_hand = currState->curr_hand + 1;
		}
		//else declare final state
		else{
			newState->curr_hand = -1;
		}
		pair<State*,float> p;
		p.first = newState;
		p.second = 1.0;
		res.push_back(p);
	}

	//equivalent to hit, then stand, double the bet
	if(action == DD){
		//double the bet
		this->bet = 2*this->bet;

		//stores result of hitting
		vector< pair<State*,float> > hit_res;
		pair<State,float> p;
		State s;
		hit_res = next_States(currState,HIT);
		// cout << "fdff" << hit_res.size() << endl;
		for(int i = 0; i < hit_res.size(); i++){
			//if more hands available, move to next hand
			if(hit_res[i].first->curr_hand < hit_res[i].first->hands.size() - 1){
				hit_res[i].first->curr_hand = hit_res[i].first->curr_hand + 1;
			}
			//else declare final state
			else{
				hit_res[i].first->curr_hand = -1;
			}
		}
		res.insert(res.end(),hit_res.begin(),hit_res.end());
	}


	if(action == SPLIT){

		//two same cards
		//only split two same cards if they are initial, else do not split them
		if(currState->initial[currState->curr_hand] == true){
			State *newState = new State(currState);
			newState->curr_hand = 0;
			newState->initial.push_back(true);
			vector<int> hand;
			//push first card of spli pair into new State
			hand.push_back(currState->hands[newState->curr_hand][0]);
			newState->hands.push_back(hand);
			vector< pair<State*,float> > hit_res;
			pair<State*,float> p1;
			pair<State*,float> p2;
			pair<State*,float> p;
			//stores result of hitting the split card
			hit_res = next_States(newState, HIT);
			// cout << hit_res.size() << "ghuuhgu" << endl;
			for(int i = 0; i < hit_res.size(); i++){
				for(int j = 0; j < hit_res.size(); j++){
					p1 = hit_res[i];
					p2 = hit_res[j];
					State *newState = new State(currState);
					// cout << i << " " << j << endl;
					//remove the split hand from current position and move two split hands to the end
					newState->initial.erase(newState->initial.begin() + newState->curr_hand);
					// cout << "here" << endl;
					// cout << (currState->hands[currState->curr_hand][0]) << endl;
					if(currState->hands[currState->curr_hand][0] == 11){
						// cout << "here" << endl;
						newState->initial.insert(newState->initial.begin() + newState->curr_hand, false);
						newState->initial.insert(newState->initial.begin() + newState->curr_hand + 1, false);
					}
					else{
						// cout << "here" << endl;
						newState->initial.insert(newState->initial.begin() + newState->curr_hand, true);
						newState->initial.insert(newState->initial.begin() + newState->curr_hand + 1, true);
					}
					newState->hands.erase(newState->hands.begin() + newState->curr_hand);
					newState->hands.insert(newState->hands.begin() + newState->curr_hand, p1.first->hands[0]);
					newState->hands.insert(newState->hands.begin() + newState->curr_hand + 1, p2.first->hands[0]);
					if(sum_hand(p1.first->hands[0]) >= 21){
						newState->curr_hand = newState->curr_hand + 1;
					}
					if(sum_hand(p2.first->hands[0]) >= 21){
						//if more hands available, move to next hand
						if(newState->curr_hand < newState->hands.size() - 1){
							newState->curr_hand = newState->curr_hand + 1;
						}
						//else declare final state
						else{
							newState->curr_hand = -1;
						}
					}
					p.first = newState;
					p.second = p1.second*p2.second;
					res.push_back(p);
				}
			}
		}
	}
	return res;
}

float Model::get_reward(State* s)
{	
	// Check final
	if(s->dealer_final && s->curr_hand == -1){
		int maxVal = 0;
		bool blackjack = false;
		bool player_busted =  true;
		bool dealer_busted = (sum_hand(s->dealer_hand) > 21);
		int sum;
		for(int i = 0; i < s->hands.size(); i++){
			sum = sum_hand(s->hands[i]);
			if(s->hands[i].size() == 2 && ((s->hands[i].at(0) == 11 && s->hands[i].at(1) == 10) || (s->hands[i].at(0) == 10 && s->hands[i].at(1) == 11)) )
				blackjack = true;
			if(sum < 21)
				player_busted = false;
			maxVal = max(maxVal, sum);
		}

		// Check blackjack
		if(blackjack && sum_hand(s->dealer_hand) == 21)
			return 0;
		else if(blackjack && sum_hand(s->dealer_hand) != 21)
			return 2.5 * bet;

		// Check busted
		if(player_busted)
			return -bet;
		if(dealer_busted)
			return +bet;

		// Check values
		if(maxVal < sum_hand(s->dealer_hand))
			return -bet;
		if(maxVal > sum_hand(s->dealer_hand))
			return +bet;

		if(sum_hand(s->dealer_hand) == 21 && !blackjack)
			return -bet;
		
		// Equal values
		return 0;
	}
	// Otherwise 0
	else{
		return 0;
	}
}

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
	}
	cout << endl;
}

void showState(State s){
	cout << "My hands" << endl;
	showHands(s.hands);
	cout << "Dealer hand" << endl;
	showHand(s.dealer_hand);
}

void showNextStates(vector< pair<State,float> > nextStates){
	for(int i = 0; i < nextStates.size(); i++){
		cout << "Probability : " << nextStates[i].second << endl;
		cout << "State :" << endl;
		showState(nextStates[i].first);
		cout << "\n\n";
	}
}

vector<State> initialStatesGenerator(){
	vector<State> res;
	for(int i = 2; i < 12; i++){
		for(int j = 2; j < 12; j++){
			for(int k = 2; k < 12; k++){
				State s;
				s.curr_hand = 0;
				s.initial.push_back(true);
				vector<int> hand;
				hand.push_back(i);
				hand.push_back(j);
				s.hands.push_back(hand);
				s.dealer_hand.push_back(k);
				s.reward = 0;
				s.dealer_final = false;
				res.push_back(s);
			}
		}
	}
	return res;
}

// vector<State> allStatesDFS(State init_state){
// 	Model m;
// 	vector<State> res;
// 	//res.push_back(init_state);
// 	vector<int> actions = m.legalActions(init_state);
// 	showState(init_state);
// 	cout << endl;
// 	showActions(actions);
// 	cout << endl;
// 	cout << actions.size() << endl;
// 	for (int i = 0; i < actions.size(); ++i)
// 	{
// 		vector< pair<State,float> > nextStatesProb = m.next_States(init_state,actions[i]);
// 		for (int j = 0; j < nextStatesProb.size(); j++)
// 			{
// 				State neighbour = nextStatesProb[i].first;
// 				vector<State> subtree = allStatesDFS(neighbour);
// 				res.insert(res.end(),subtree.begin(),subtree.end());
// 			}	
// 	}
// 	cout << "vvrrw" << endl;
// 	return res;
// }


// vector<State> allStatesGenerator(){
// 	vector<State> initStates = initialStatesGenerator();
// 	vector<State> res;
// 	vector<State> next;
// 	Model m;
// 	for(int i = 0; i < initStates.size(); i++){
// 		res.push_back(initStates[i]);
// 		next = allStatesDFS(initStates[i]);
// 		res.insert(res.end(),next.begin(),next.end());
// 	}
// 	return res;
// }

// int main(){
// 	State s;
// 	s.curr_hand = 0;
// 	s.initial.push_back(true);
// 	vector<int> hand;
// 	hand.push_back(10);
// 	hand.push_back(10);
// 	s.hands.push_back(hand);
// 	s.dealer_hand.push_back(11);
// 	s.reward = 0;
// 	s.dealer_final = false;
// 	Model m;
// 	vector<int> actions = m.legalActions(s);
// 	// showActions(actions);
// 	vector< pair<State,float> > next = m.next_States(s,SPLIT);
// 	// showNextStates(next);
// 	// cout << next.size() << endl;
// 	State aces = next[next.size()-10].first;
// 	cout << aces.curr_hand << " fssfsf" << endl;
// 	actions = m.legalActions(aces);
// 	showActions(actions);
// 	next = m.next_States(aces,SPLIT);
// 	showNextStates(next);
// 	cout << next.size() << endl;


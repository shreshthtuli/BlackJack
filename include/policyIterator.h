#ifndef POLICYITERATOR_H
#define POLICYITERATOR_H

#include<map>

#include "model.h"

class PolicyIterator{
public:
	Model m;
	map<State,int> policy;
	map<State, double> value;
	void initialiser(vector< pair<State,int> > initial_policy);
	void evaluate_policy();
	// returns true is same otherwise false
	bool updatePolicy();
	void iterate();
	double qStar(State s, int a);
};

#endif
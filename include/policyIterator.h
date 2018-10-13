#ifndef POLICYITERATOR_H
#define POLICYITERATOR_H

#include<map>

#include "model.h"

class PolicyIterator{
public:
	Model m;
	map<State,int> policy;
	void initialiser(vector< pair<State,int> > initial_policy);
	void evaluate_policy();
	void updatePolicy();
};

#endif
#ifndef VALUEITERATOR_H
#define VALUEITERATOR_H

#include <map>

#include "model.h"

class ValueIterator{
public:
	Model m;
	map<State*,int> policy;
	map<State*, double> value;
	void initialiser(vector<State*> initial_value);
	double vStar(State* s);
	// returns true is same otherwise false
	void updatePolicy();
	void iterate();
	double qStar(State* s, int a);
};

#endif
#ifndef FEATURE_H
#define FEATURE_H

#include <vector>
#include <algorithm>

using namespace std;

// Feature object in the Decision Tree

class Feature {

public:

	Feature() {
		
	}

	// Constructor
	Feature(string name) {
		this->name = name;
	}

	// Gets name of feature
	string getName() {
		return name;
	}

	// Adds new value if values list does not already contain it
	void addValue(string val) {
		if(find(values.begin(), values.end(), val) == values.end()) {
			values.push_back(val);
		} 
	}

	vector<string> getFeatureValues() {
		return values;
	}


private:

	string name; 
	vector<string> values; // Distinct values of a feature

};

#endif
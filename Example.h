#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <map>
#include <string>

using namespace std;

// Example object in the Decision Tree

class Example {

public:

	// Default Constructor
	Example() {
	
	}

	// Sets value of the class of the example
	void setClass(string class_value) {
		this->class_value = class_value;
	}

	// Gets class value of example
	string getClass() {
		return class_value;
	}

	// Adds feature and value to example's feature list
	void addFeature(string feature, string value) {
		features.insert(pair<string, string>(feature, value));
	}

	string getFeatureVal(string feature) {
		return features[feature];
	}


private:

	string class_value; 
	map<string, string> features;

};

#endif
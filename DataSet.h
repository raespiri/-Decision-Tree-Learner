#ifndef DATA_SET_H
#define DATA_SET_H

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "Example.h"
#include "Feature.h"

using namespace std;

// Parses input file and generates randomized sets for testing, training, and validation

class DataSet {

public: 

	// Constructor
	DataSet(string file) {
		srand(time(0)); // random seed

		parseFile(file);
		generateSets();
	}

	// Destructor
	~DataSet() {
		for (vector<Example*>::iterator it = examples.begin(); it != examples.end(); ++it) {
    		delete (*it);
   		} 
	}

	// Parse file to generate full set of examples
	void parseFile(string file) {
		ifstream inFile(file);

		string line;
		int line_count = 0;

		// Loop to get each line in file
		while(getline(inFile, line)) {
   	 		stringstream ss(line);
    		
    		string value;
    		
    		Example* ex = NULL;

    		int word_count = 0;

    		// Loop to get each word in line
   			while(getline(ss, value, ',')) {	
   				// Getting class name and feature labels
   				if(line_count == 0) {
   					// Set class name if first word
   					if(word_count == 0) {
   						class_name = value;
   						word_count++;
   					}
   					// Otherwise add to list of features
   					else {
   						Feature feature(value);
   						features.push_back(feature);
   						word_count++;
   					}
   				}
   				// Getting examples
   				else {
   					// Set class value is first word
   					if(word_count == 0) {
   						ex = new Example();
   						ex->setClass(value);
   						word_count++;
   					}
   					// Otherwise update example's features and feature list
   					else {
   						ex->addFeature(features[word_count-1].getName(), value);
   						features[word_count-1].addValue(value);
   						word_count++;
   					}
   				}
    		}

    		// If current example isn't NULL add to list of examples
    		if(ex != NULL) {
    			examples.push_back(ex);
    		}

    		line_count++;
		}

	}

	// Randomly generates Training, Validation, and Testing sets
	void generateSets() {
		int training_size = examples.size() * 0.6;
		int validation_size = examples.size() * 0.2;
		int testing_size = examples.size() * 0.2;

		vector<int> sets_toFill = {1, 2, 3}; // List of sets to be filled

		for(int i = 0; i < examples.size(); i++) {
			int random = getRandomSet(sets_toFill); // Get random set

			if(random == 1) {
				training.push_back(examples[i]);
				non_testing.push_back(examples[i]); // add element to non_testing set too
				// If set is full remove it from list to be filled
				if(training.size() == training_size) {
					sets_toFill.erase(
						remove(sets_toFill.begin(), sets_toFill.end(), 1), 
						sets_toFill.end()
					);				
				}
			}
			else if(random == 2) {
				validation.push_back(examples[i]);
				non_testing.push_back(examples[i]); // add element to non_testing set too
				// If set is full remove it from list to be filled
				if(validation.size() == validation_size) {
					sets_toFill.erase(
						remove(sets_toFill.begin(), sets_toFill.end(), 2), 
						sets_toFill.end()
					);
				}
			}
			else if(random == 3) {
				testing.push_back(examples[i]);
				// If set is full remove it from list to be filled
				if(testing.size() == testing_size) {
					sets_toFill.erase(
						remove(sets_toFill.begin(), sets_toFill.end(), 3), 
						sets_toFill.end()
					);
				}
			}

			// Break out of for loop if all sets have been filled
			if(sets_toFill.empty()) {
				break;
			}
		}
 	}

	// Gets random set from the sets that still need to be filled
	int getRandomSet(vector<int> sets_toFill) {
		return sets_toFill[rand() % sets_toFill.size()];
	}

	vector<Example*>& getExamples() {
		return examples;
	}

	vector<Example*>& getTraining() {
		return training;
	}

	vector<Example*>& getValidation() {
		return validation;
	}

	vector<Example*>& getTesting() {
		return testing;
	}

	vector<Example*>& getNonTesting() {
		return non_testing;
	}

	vector<Feature>& getFeatures() {
		return features;
	}

private:

	string class_name; // Class name of dataset
	vector<Feature> features; // List of features
	vector<Example*> examples; // Complete list of examples
	vector<Example*> training; // Training set
	vector<Example*> validation; // Validation set
	vector<Example*> testing; // Testing set
	vector<Example*> non_testing; // Non-testing set

};

#endif
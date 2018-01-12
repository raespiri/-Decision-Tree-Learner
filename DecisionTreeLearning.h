#ifndef DECISION_TREE_LEARING_H
#define DECISION_TREE_LEARING_H

#include <cmath>

#include "DataSet.h"
#include "TreeNode.h"

using namespace std;

// Object which creates the decision tree through learning

class DecisionTreeLearning {

public:
	
	// Default Constructor
	DecisionTreeLearning() {

	}

	// Decision tree learning algorithm
	TreeNode* learn(vector<Example*> &examples, vector<Feature> &features, vector<Example*> &parent_examples) {
		// Get pluralityValue of parents if examples is empty
		if(examples.empty()) {
			TreeNode* tree = new TreeNode(pluralityValue(parent_examples));
			return tree;
		}
		// If all examples have the same class return that class
		else if(sameClassification(examples)) {
			TreeNode* tree = new TreeNode(examples[0]->getClass());
			return tree;
		}
		// If there are no features return the pluralityValue of examples
		else if(features.empty()) {
			TreeNode* tree = new TreeNode(pluralityValue(examples));
			return tree;
		}
		else {
			Feature important_feature = importance(features, examples); // Get feature of highest importance
			TreeNode* tree = new TreeNode(important_feature.getName()); // Create new Tree with root at this feature

			// Loop through each feature value 
			vector<string> feature_values = important_feature.getFeatureValues();
			for(int i = 0; i < feature_values.size(); i++) {
				vector<Example*> examples_subset;
				generateExampleSubset(examples_subset, important_feature, feature_values[i], examples); // Get example subset matching the feature
				
				// Recursively create new subtree
				vector<Feature> feature_subset = features;
				removeFeature(feature_subset, important_feature);
				TreeNode* subtree = new TreeNode(*learn(examples_subset, feature_subset, examples));

				// Add subtree to tree
				tree->addBranch(subtree, feature_values[i]);
			}

			return tree;
		}
	}

	// Decision tree learning algorithm depth bounded
	TreeNode* learnWithDepthBound(vector<Example*> &examples, vector<Feature> &features, vector<Example*> &parent_examples, int bound, int depth) {
		// If it is equal to the depth bound, this node has to be a leaf node predicting the majority class
		if(depth == bound) {
			TreeNode* tree = new TreeNode(pluralityValue(examples), depth);
			return tree;
		}
		// Get pluralityValue of parents if examples is empty
		else if(examples.empty()) {
			TreeNode* tree = new TreeNode(pluralityValue(parent_examples), depth);
			return tree;
		}
		// If all examples have the same class return that class
		else if(sameClassification(examples)) {
			TreeNode* tree = new TreeNode(examples[0]->getClass(), depth);
			return tree;
		}
		// If there are no features return the pluralityValue of examples
		else if(features.empty()) {
			TreeNode* tree = new TreeNode(pluralityValue(examples), depth);
			return tree;
		}
		else {
			Feature important_feature = importance(features, examples); // Get feature of highest importance
			TreeNode* tree = new TreeNode(important_feature.getName(), depth); // Create new Tree with root at this feature

			// Loop through each feature value 
			vector<string> feature_values = important_feature.getFeatureValues();
			for(int i = 0; i < feature_values.size(); i++) {
				vector<Example*> examples_subset;
				generateExampleSubset(examples_subset, important_feature, feature_values[i], examples); // Get example subset matching the feature
				
				// Recursively create new subtree
				vector<Feature> feature_subset = features;
				removeFeature(feature_subset, important_feature);
				TreeNode* subtree = learnWithDepthBound(examples_subset, feature_subset, examples, bound, tree->getDepth()+1);

				// Add subtree to tree
				tree->addBranch(subtree, feature_values[i]);
				//cout << tree->getLabel() << "--" << feature_values[i] << "->" << subtree->getLabel() << " " << subtree->getParent() << endl;
			}

			return tree;
		}
	}

	// Removes feature from vector
	void removeFeature(vector<Feature> &features, Feature feature) {
		for(int i = 0; i < features.size(); i++) {
			if(features[i].getName() == feature.getName()) {
				features.erase(features.begin() + i);
				break;
			}
		}
	}

	// Generate set of examples which match the value of the given feature
	void generateExampleSubset(vector<Example*> &examples_subset, Feature feature, string feature_val, vector<Example*> &examples) {
		for(int i = 0; i < examples.size(); i++) {
			// If example's feature value matches
			if(examples[i]->getFeatureVal(feature.getName()) == feature_val) {
				examples_subset.push_back(examples[i]);
			}
		}
	}

	// Selects the most common output value among a set of examples
	string pluralityValue(vector<Example*> &examples) {
		int poison_count = 0; // Count for poison output
		int notPoison_count = 0; // Count for not poison output

		for(int i = 0; i < examples.size(); i++) {
			if(examples[i]->getClass() == "1") {
				poison_count++;
			}
			else if(examples[i]->getClass() == "0") {
				notPoison_count++;
			}
		}

		// Return whichever output was more frequent
		if(poison_count >= notPoison_count) {
			return "1";
		}
		else {
			return "0";
		}
	}

	// Checks if all the examples have the same classification
	bool sameClassification(vector<Example*> &examples) {
		for(int i = 0; i < examples.size()-1; i++) {
			if(examples[i]->getClass() != examples[i+1]->getClass()) {
				return false;
			}
		}
		return true;
	}

	// Gets max importance attribute
	Feature importance(vector<Feature> &features, vector<Example*> &examples) {
		int max_importance = -1;
		Feature important_feature;

		// Loop to find max importance feature
		for(int i = 0; i < features.size(); i++) {
			int curr_importance = Gain(features[i], examples);
			if( curr_importance > max_importance) {
				max_importance = curr_importance;
				important_feature = features[i];
			}
		}
		return important_feature;
	}

	// Calculates information gain for a specific feature
	double Gain(Feature feature, vector<Example*> &examples) {
		map<string, ClassValues> feature_values; // List of feature values and their class values
		map<string, ClassValues>::iterator it;

		// Loop through examples and calibrate their values for this feature
		for(int i = 0; i < examples.size(); i++) {
			string feature_val = examples[i]->getFeatureVal(feature.getName());
			it = feature_values.find(feature_val);
			// If feature value found
			if(it != feature_values.end()) {
				// Increment corresponding class value
				if(examples[i]->getClass() == "1") {
					it->second.p++;
				}
				else {
					it->second.n++;		
				}
			}
			// Else add feature value to map
			else {
				ClassValues cv(examples[i]->getClass());
				feature_values.insert(pair<string, ClassValues>(
					feature_val, cv
					)
				);
			}
		}

		// Gain calculation
		double result = 0.0;
		for (it = feature_values.begin(); it != feature_values.end(); it++) {
			result += ((double)((it->second.p + it->second.n) / examples.size())) * B((double)(it->second.p / (it->second.p + it->second.n)));
		}
		if(result < 0.0) {
    		return 1.0 + abs(result);
  		}
  		else {
  			return 1.0 - result;
  		}
	}

	// B(q) is the entropy of a Boolean random variable that is true with probability q
	double B(double q) {
		if(q == 0.0) {
        	return -1.0 * ((q * log2(1)) + ((1 - q) * log2(1 - q)));
    	}
	    else if(q == 1.0) {
	        return -1.0 * ((q * log2(q)) + ((1 - q) * log2(1)));
	    }
	    else {       
		    return -1.0 * ((q * log2(q)) + ((1 - q) * log2(1 - q)));
	    }
	}

private:

	// Structure to hold the values of a class
	struct ClassValues {

		ClassValues(string classVal) {
			if(classVal == "1") {
				p = 1;
				n = 0;
			}
			else {
				p = 0;
				n = 1;
			}
		}

		int p;
		int n;
	};

};

#endif
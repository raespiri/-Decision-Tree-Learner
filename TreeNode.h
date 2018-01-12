#ifndef TREE_NODE_H
#define TREE_NODE_H

using namespace std;

class TreeNode {

public:
	
	// Constructor
	TreeNode(string label) {
		this->label = label;
		parent = NULL;
	}

	// Copy Constructor
	TreeNode(const TreeNode &tree_node) {
		label = tree_node.label;
		branch_from_parent = tree_node.branch_from_parent;
		parent = tree_node.parent;
		// Copy child nodes
		for(int i = 0; i < tree_node.children.size(); i++) {
			children.push_back(tree_node.children[i]);
		}

	}

	// Constructor
	TreeNode(string label, int depth) {
		this->label = label;
		parent = NULL;
		this->depth = depth;
	}

	// Copy Constructor
	TreeNode(const TreeNode &tree_node, int depth) {
		label = tree_node.label;
		branch_from_parent = tree_node.branch_from_parent;
		parent = tree_node.parent;
		// Copy child nodes
		for(int i = 0; i < tree_node.children.size(); i++) {
			children.push_back(tree_node.children[i]);
		}
		this->depth = depth;
	}

	// Adds branch between this tree node and the one passed as a parameter
	void addBranch(TreeNode* tree_node, string branch_from_parent) {
		children.push_back(tree_node);
		tree_node->parent = this;
		tree_node->branch_from_parent = branch_from_parent;
	}

	int getDepth() {
		return depth;
	}

	string getLabel() {
		return label;
	}


	void printChildren() {
		for(int i = 0; i < children.size(); i++) {
			cout << children[i]->parent->label << " -> " << children[i]->label << endl;
			children[i]->printChildren();
		}
	}

	int getAccuracy(vector<Example*> &examples) {
		// Count for number of correctly classified examples
		int correct = 0;

		for(int i = 0; i < examples.size(); i++) {
			string dt_classification = getExampleClass(examples[i]); // get Dt's classification
			// Increment count if classification is correct
			if(dt_classification == examples[i]->getClass()) {
				correct++;
			}
		}
		return correct * 100 / examples.size();
	}

	// Get's classification of example from DT
	string getExampleClass(Example* example) {
		// Base case return if class value is found
		if(label == "1" || label == "0") {
			return label;
		}
		string feature_val = example->getFeatureVal(label); // Get example's value for current feature
		for(int i = 0; i < children.size(); i++) {
			// Follow branch to child node linked to this value
			if(children[i]->branch_from_parent == feature_val) {
				return children[i]->getExampleClass(example); // Recurse down to next level of tree
			}
		}
	}

	string getParent() {
		return parent->label;
	}

private: 

	string label;
	string branch_from_parent; 
	vector<TreeNode*> children;
	TreeNode* parent;
	int depth;

};

#endif
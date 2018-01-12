#include <iomanip>

#include "DecisionTreeLearning.h"

using namespace std;

int main(int argc, char *argv[]) {

	// Extract data
	DataSet data_set(argv[1]);

	DecisionTreeLearning dtl;
	vector<Example*> parent_examples;

	// Train decision train from non_testing set
	TreeNode* root = dtl.learn(data_set.getNonTesting(), 
							   data_set.getFeatures(), 
							   parent_examples
							  );

	// Get accuracy on test set
	cout << "Test set accuracy with no depth bound: " << root->getAccuracy(data_set.getTesting()) << endl << endl;

	// Train depth bounded decision by training set
	cout << setw(7) << "depth " << setw(7) << "train% " << setw(7) << "valid%" << endl;

	int max_accuracy = 0;
	int optimal_depth = 1;
	// Output accuracies for each depth for training and valid sets
	for(int i = 1; i < 16; i++) {
		vector<Example*> parent_examples2;
		TreeNode* tree_root = dtl.learnWithDepthBound(data_set.getTraining(), 
													  data_set.getFeatures(), 
													  parent_examples2, 
													  i, 
													  0
													 );
		
		cout << setw(4) << i << " " << setw(6) 
		<< tree_root->getAccuracy(data_set.getTraining()) << " " << setw(7) 
		<< tree_root->getAccuracy(data_set.getValidation()) << endl;

		if(tree_root->getAccuracy(data_set.getValidation()) > max_accuracy) {
			max_accuracy = tree_root->getAccuracy(data_set.getValidation());
			optimal_depth = i;
		}
	}
	
	cout << endl << "Optimal depth: " << optimal_depth << endl << endl;

	vector<Example*> parent_examples3;
	DecisionTreeLearning dtl1;
	TreeNode* tree_root = dtl1.learnWithDepthBound(data_set.getNonTesting(), 
												   data_set.getFeatures(), 
												   parent_examples3, 
												   optimal_depth, 
												   0
												 );
	cout << "Test set accuracy with depth bound of " << optimal_depth << ": " << tree_root->getAccuracy(data_set.getTesting()) << endl;
	
	return 0;
}
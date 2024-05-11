
#include "main.hpp"
#include "Dataset.hpp"
/* TODO: Please design your data structure carefully so that you can work with the given dataset
 *       in this assignment. The below structures are just some suggestions.
 */
struct kDTreeNode
{
    vector<int> data;
    int label;
    kDTreeNode *left;
    kDTreeNode *right;
    kDTreeNode(vector<int> data, int lable, kDTreeNode *left = nullptr, kDTreeNode *right = nullptr)
    {
        this->data = data;
        this->label = label;
        this->left = left;
        this->right = right;
    }
    kDTreeNode(vector<int> data, kDTreeNode *left = nullptr, kDTreeNode *right = nullptr)
    {
        this->data = data;
        this->left = left;
        this->right = right;
    }
    friend ostream &operator<<(ostream &os, const kDTreeNode &node)
    {
        os << "(";
        for (int i = 0; i < node.data.size(); i++)
        {
            os << node.data[i];
            if (i != node.data.size() - 1)
            {
                os << ", ";
            }
        }
        os << ")";
        return os;
    }
};

class kDTree
{
private:
    int k;
    int count;
    kDTreeNode *root;
private:
    kDTreeNode* buildTreeRecursive(const vector<vector<int>> &pointList, int depth);
    kDTreeNode* buildTreeLabelRec(const vector<vector<int>> &pointList, const vector<int> &labelList, int depth);
    void insertRecursive(kDTreeNode *&node, const vector<int> &point, int depth);
    bool searchRecursive(kDTreeNode *node, const vector<int> &point, int depth);
    void inorderTraversalRecursive(kDTreeNode *node) const;
    void preorderTraversalRecursive(kDTreeNode *node) const;
    void postorderTraversalRecursive(kDTreeNode *node) const;
    int heightRecursive(kDTreeNode *node) const;
    int nodeCountRecursive(kDTreeNode *node) const;
    int leafCountRecursive(kDTreeNode *node) const;
    void removeRecursive(kDTreeNode *&node, const vector<int> &point, int depth);
    kDTreeNode* findMinNode(kDTreeNode *node, int axis, int depth);
    void nearestNeighbourRecursive(kDTreeNode *node, const vector<int> &target, kDTreeNode *&best, int depth);
    void kNearestNeighbourRecursive(kDTreeNode* node, const std::vector<int>& target, int k, std::list<std::pair<double, kDTreeNode*>>& nearestNeighbors, int depth);
    void insertSorted(std::list<std::pair<double, kDTreeNode*>>& nearestNeighbors, const std::pair<double, kDTreeNode*>& nodePair);
    void delTree(kDTreeNode* node);
    
public:
    kDTree(int k = 2){
        this-> k = k;
        this-> count = 0;
        this->root = nullptr;
    };
    ~kDTree(){
        delTree(root);
    };
    kDTreeNode* minNode(kDTreeNode* x,kDTreeNode* y,kDTreeNode* z,int d) {  // hàm này để tìm node nhỏ nhất trong 3 node theo chiều d
	kDTreeNode* res=x;
	if (y!=NULL && y->data[d]<res->data[d]) {
		res=y;
	}
	if (z!=NULL && z->data[d]<res->data[d]) {
		res=z;
	}
	return res;
}
    kDTreeNode* deepCopy(const kDTreeNode* node);
    const kDTree &operator=(const kDTree &other);
    kDTree(const kDTree &other);

    vector<vector<int>> merge(const vector<vector<int>> &left, const vector<vector<int>>&right, int axis);
    vector<vector<int>> mergeSort(const vector<vector<int>> &arr, int axis);
kDTreeNode* deleteNode(kDTreeNode* tmp,const std::vector<int>& point,int depth) {
	if (tmp==nullptr) {
		return nullptr;
	}
	int alpha=depth%k;
	if (tmp->data==point) {
		if(tmp->right!=nullptr) {
			kDTreeNode* min=findMinNode(tmp->right,alpha,depth+1);
			tmp->data=min->data;
			tmp->right=deleteNode(tmp->right,min->data,depth+1);
			
			
			
		}		
		
		else if (tmp->left!=nullptr) {
			kDTreeNode* min=findMinNode(tmp->left,alpha,depth+1);
			tmp->data=min->data;
			tmp->right=tmp->left;
			tmp->left=nullptr;
			tmp->right=deleteNode(tmp->right,min->data,depth+1);

		}
		else if(!tmp->left && !tmp->right) {
     delete tmp;
     
     return nullptr;	
		}
		return tmp;

	} 
	if (point[alpha]<tmp->data[alpha]) {
		tmp->left=deleteNode(tmp->left,point,depth+1);
	}
	else {
		tmp->right=deleteNode(tmp->right,point,depth+1);
	}
	
	return tmp;
}
    void inorderTraversal() const;
    void preorderTraversal() const;
    void postorderTraversal() const;
    int height() const;
    int nodeCount() const;
    int leafCount() const;

    void insert(const vector<int> &point);
    void remove(const vector<int> &point);
    bool search(const vector<int> &point);
    void buildTree(const vector<vector<int>> &pointList);
    void buildTreeLabel(const vector<vector<int>> &pointList, const vector<int> &lableList);
    void nearestNeighbour(const vector<int> &target, kDTreeNode *&best);
    void kNearestNeighbour(const vector<int> &target, int k, vector<kDTreeNode *> &bestList);
};

class kNN
{
private:
    int k;
    Dataset X_train;
    Dataset Y_train;
    kDTree kdtree;
public:
    kNN(int k = 5){
        this-> k = k;
    }
    void fit(Dataset &X_train, Dataset &Y_train);
    Dataset predict(const Dataset& X_test) {
    Dataset y_pred;
    y_pred.columnName.push_back("label");
    for (const auto& data_point : X_test.data) {
        vector<int> tmp (data_point.begin(), data_point.end());
        vector<kDTreeNode*> neighbors;
        kdtree.kNearestNeighbour(tmp, k, neighbors);

        vector<int> label_counts(10, 0); 
        for (auto neighbor : neighbors) {
            label_counts[neighbor->label]++;
        }

        int max_count = 0;
        int predicted_label = -1;
        for (int i = 0; i < label_counts.size(); ++i) {
            if (label_counts[i] > max_count) {
                max_count = label_counts[i];
                predicted_label = i;
            }
        }

        // Add the predicted label to the y_pred dataset
        y_pred.data.push_back({predicted_label});
    }

    return y_pred;
}


double score(const Dataset& y_test, const Dataset& y_pred) {
    int total_images = y_test.data.size();
    int correct_predictions = 0;

    auto test_it = y_test.data.begin();
    auto pred_it = y_pred.data.begin();

    while (test_it != y_test.data.end() && pred_it != y_pred.data.end()) {
        int actual_label = (*test_it).front();   
        int predicted_label = (*pred_it).front();

        if (actual_label == predicted_label) {
            correct_predictions++;
        }

        ++test_it;
        ++pred_it;
    }

    double accuracy = static_cast<double>(correct_predictions) / total_images;
    return accuracy;
}

};

// Please add more or modify as needed

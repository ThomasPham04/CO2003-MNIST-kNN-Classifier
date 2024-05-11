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

    kDTreeNode* minNode(kDTreeNode* x, kDTreeNode* y, kDTreeNode* z, int axis);
public:
    kDTree(int k = 2){
        this-> k = k;
        this-> count = 0;
        this->root = nullptr;
    };
    ~kDTree(){
        delTree(root);
    };
    kDTreeNode* deepCopy(const kDTreeNode* node);
    const kDTree &operator=(const kDTree &other);
    kDTree(const kDTree &other);

    vector<vector<int>> merge(const vector<vector<int>> &left, const vector<vector<int>>&right, int axis);
    vector<vector<int>> mergeSort(const vector<vector<int>> &arr, int axis);
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
    void fit(Dataset &X_train, Dataset &Y_train) {
    this->X_train = X_train;
    this->Y_train = Y_train;
    
    vector<vector<int>> pointList;
    vector<int> labelList;

    for (const auto& it:X_train.data){
        vector<int> tmp (it.begin(), it.end());
        pointList.push_back(tmp);
    }
    for (const auto& it:Y_train.data){
        labelList.push_back(it.front());
    }
    kdtree.buildTreeLabel(pointList, labelList);
}
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
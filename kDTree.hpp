
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
public:
    kDTree(int k = 2){
        this-> k = k;
        this-> count = 0;
        this->root = nullptr;
    };
    ~kDTree();
 
    kDTreeNode* deepCopy(const kDTreeNode* node);
    const kDTree &operator=(const kDTree &other);
    kDTree(const kDTree &other);

    vector<vector<int>> merge(const vector<vector<int>> &left, const vector<vector<int>>&right, int axis);
    vector<vector<int>> mergeSort(const vector<vector<int>> &arr, int axis);

    vector<pair<vector<int>, int>> kDTree::mergeSortLabel(const vector<pair<vector<int>, int>>& arr, int axis);
    vector<pair<vector<int>, int>> kDTree::mergeLabel(const vector<pair<vector<int>, int>>& left, const vector<pair<vector<int>, int>>& right, int axis);

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
    Dataset predict(Dataset &X_test);
    double score(const Dataset &y_test, const Dataset &y_pred);
};

// Please add more or modify as needed

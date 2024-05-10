#include "kDTree.hpp"

/* TODO: You can implement methods, functions that support your data structures here.
 * */
/****************************************BEGIN KDTREE****************************************/
kDTreeNode* kDTree::deepCopy(const kDTreeNode* node){
    if (node == nullptr){
        return nullptr;
    } else{
        kDTreeNode* newNode = new kDTreeNode(node->data, node->label);
        newNode->left = deepCopy(node->left);
        newNode->right = deepCopy(node->right);
        return newNode;    
    }
}
const kDTree& kDTree::operator=(const kDTree &other){
    this-> k = other.k;
    this-> root = deepCopy(other.root);
    return *this;
}
kDTree::kDTree(const kDTree &other)
{
    this->k = other.k;
    this->root = deepCopy(other.root);
}
vector<vector<int>> kDTree::mergeSort(const vector<vector<int>> &arr, int axis) {
    if (arr.size() <= 1) {
        return arr;
    }

    size_t mid = arr.size() / 2;

    vector<vector<int>> left(arr.begin(), arr.begin() + mid);
    vector<vector<int>> right(arr.begin() + mid, arr.end());

    left = mergeSort(left, axis);
    right = mergeSort(right, axis);

    return merge(left, right, axis);
}
void kDTree::delTree(kDTreeNode* node){
    if (node==nullptr){
        return;
    }
    delTree(node->left);
    delTree(node->right);
    delete node;
}
vector<vector<int>> kDTree::merge(const vector<vector<int>> &left, const vector<vector<int>> &right, int axis) {
    vector<vector<int>> result;
    size_t leftIdx = 0;
    size_t rightIdx = 0;

    while (leftIdx < left.size() && rightIdx < right.size()) {
        if (left[leftIdx][axis] < right[rightIdx][axis]) {
            result.push_back(left[leftIdx]);
            leftIdx++;
        } else {
            result.push_back(right[rightIdx]);
            rightIdx++;
        }
    }

    while (leftIdx < left.size()) {
        result.push_back(left[leftIdx]);
        leftIdx++;
    }

    while (rightIdx < right.size()) {
        result.push_back(right[rightIdx]);
        rightIdx++;
    }

    return result;
}

kDTreeNode* kDTree::buildTreeRecursive(const vector<vector<int>> &pointList, int depth) {
    if (pointList.empty()) {
        return nullptr;
    }

    int axis = depth % k;

    vector<vector<int>> sortedPointList = mergeSort(pointList, axis);

    size_t medianIndex = sortedPointList.size() / 2;

    kDTreeNode *medianNode = new kDTreeNode(sortedPointList[medianIndex]);

    vector<vector<int>> leftPointList(sortedPointList.begin(), sortedPointList.begin() + medianIndex);
    medianNode->left = buildTreeRecursive(leftPointList, depth + 1);

    vector<vector<int>> rightPointList(sortedPointList.begin() + medianIndex + 1, sortedPointList.end());
    medianNode->right = buildTreeRecursive(rightPointList, depth + 1);

    return medianNode;
}

void kDTree::buildTree(const vector<vector<int>> &pointList) {
    root = buildTreeRecursive(pointList, 0);
}
vector<pair<vector<int>, int>> kDTree::mergeSortLabel(const vector<pair<vector<int>, int>>& arr, int axis) {
    if (arr.size() <= 1) {
        return arr;
    }

    size_t mid = arr.size() / 2;

    vector<pair<vector<int>, int>> left(arr.begin(), arr.begin() + mid);
    vector<pair<vector<int>, int>> right(arr.begin() + mid, arr.end());

    left = mergeSortLabel(left, axis);
    right = mergeSortLabel(right, axis);

    return mergeLabel(left, right, axis);
}

vector<pair<vector<int>, int>> kDTree::mergeLabel(const vector<pair<vector<int>, int>>& left, const vector<pair<vector<int>, int>>& right, int axis) {
    vector<pair<vector<int>, int>> result;
    size_t leftIdx = 0;
    size_t rightIdx = 0;

    while (leftIdx < left.size() && rightIdx < right.size()) {
        if (left[leftIdx].first[axis] < right[rightIdx].first[axis]) {
            result.push_back(left[leftIdx]);
            leftIdx++;
        } else {
            result.push_back(right[rightIdx]);
            rightIdx++;
        }
    }

    while (leftIdx < left.size()) {
        result.push_back(left[leftIdx]);
        leftIdx++;
    }

    while (rightIdx < right.size()) {
        result.push_back(right[rightIdx]);
        rightIdx++;
    }

    return result;
}

kDTreeNode* kDTree::buildTreeLabelRec(const vector<vector<int>>& pointList, const vector<int>& labelList, int depth) {
    if (pointList.empty() || labelList.empty() || pointList.size() != labelList.size()) {
        return nullptr;
    }

    int axis = depth % k;

    // Merge sort the combined list based on the current axis
    vector<pair<vector<int>, int>> combinedList;
    for (size_t i = 0; i < pointList.size(); ++i) {
        combinedList.push_back({pointList[i], labelList[i]});
    }
    combinedList = mergeSortLabel(combinedList, axis);

    // Find the median index
    size_t medianIndex = combinedList.size() / 2;

    // Create a new node with the median point and label
    kDTreeNode* medianNode = new kDTreeNode(combinedList[medianIndex].first);
    medianNode->label = combinedList[medianIndex].second;

    // Recursively build the left subtree with points before the median
    vector<pair<vector<int>, int>> leftCombinedList(combinedList.begin(), combinedList.begin() + medianIndex);
    vector<vector<int>> leftPointList;
    vector<int> leftLabelList;
    for (const auto& pair : leftCombinedList) {
        leftPointList.push_back(pair.first);
        leftLabelList.push_back(pair.second);
    }
    medianNode->left = buildTreeLabelRec(leftPointList, leftLabelList, depth + 1);

    // Recursively build the right subtree with points after the median
    vector<pair<vector<int>, int>> rightCombinedList(combinedList.begin() + medianIndex + 1, combinedList.end());
    vector<vector<int>> rightPointList;
    vector<int> rightLabelList;
    for (const auto& pair : rightCombinedList) {
        rightPointList.push_back(pair.first);
        rightLabelList.push_back(pair.second);
    }
    medianNode->right = buildTreeLabelRec(rightPointList, rightLabelList, depth + 1);

    return medianNode;
}

void kDTree::buildTreeLabel(const vector<vector<int>>& pointList, const vector<int>& labelList) {
    this->count = pointList.size();
    this->root = buildTreeLabelRec(pointList, labelList, 0);
}

void kDTree::insert(const vector<int> &point) {
    if (point.size() != k){
        return;
    }
    insertRecursive(root, point, 0);
    count++;
}

void kDTree::insertRecursive(kDTreeNode *&node, const vector<int> &point, int depth){
    if (node == nullptr){
        node = new kDTreeNode(point);
        return;
    }
    int axis = depth % k;
    if (point[axis] < node->data[axis])
        insertRecursive(node->left, point, depth + 1);
    else 
        insertRecursive(node->right, point, depth + 1);
}
bool kDTree::search(const vector<int> &point) {
    return searchRecursive(root, point, 0);
}

bool kDTree::searchRecursive(kDTreeNode *node, const vector<int> &point, int depth) {
    if (node == nullptr) return false;
    if (node->data == point) return true;

    int axis = depth % k;
    if (point[axis] < node->data[axis])
        return searchRecursive(node->left, point, depth + 1);
    else
        return searchRecursive(node->right, point, depth + 1);
}

void kDTree::remove(const vector<int> &point) {
    if (point.size() != k){
        return;
    }
    removeRecursive(root, point, 0);
    count--;
}

void kDTree::removeRecursive(kDTreeNode *&node, const vector<int> &point, int depth) {
    if (node == nullptr) {
        return;
    }

    int axis = depth % k;

    if (node->data == point) {
        if (node->right != nullptr) {
            kDTreeNode *minNode = findMinNode(node->right, axis, depth + 1);
            node->data = minNode->data;
            removeRecursive(node->right, minNode->data, depth + 1);
        } else if (node->left != nullptr) {
            kDTreeNode *minNode = findMinNode(node->left, axis, depth + 1);
            node->data = minNode->data;
            removeRecursive(node->left, minNode->data, depth + 1);
        } else {
            delete node;
            node = nullptr;
        }
    } else if (point[axis] < node->data[axis]) {
        removeRecursive(node->left, point, depth + 1);
    } else {
        removeRecursive(node->right, point, depth + 1);
    }
}


kDTreeNode* kDTree::findMinNode(kDTreeNode *node, int axis, int depth) {
    if (node == nullptr) return nullptr;

    int nextAxis = (depth + 1) % k;
    if (node->left == nullptr && node->right == nullptr) return node;

    kDTreeNode *leftMin = findMinNode(node->left, axis, depth + 1);
    kDTreeNode *rightMin = findMinNode(node->right, axis, depth + 1);

    if (leftMin != nullptr && rightMin != nullptr) {
        if (leftMin->data[axis] < rightMin->data[axis])
            return leftMin;
        else
            return rightMin;
    } else if (leftMin != nullptr)
        return leftMin;
    else
        return rightMin;
}

void kDTree::inorderTraversal() const {
    inorderTraversalRecursive(root);
}

void kDTree::inorderTraversalRecursive(kDTreeNode *node) const {
        cout << "(";
    for (int i = 0; i < k - 1; ++i)
        cout << node->data[i] << ", ";
    cout << node->data[k - 1] << ") ";
    inorderTraversalRecursive(node->right);
}

void kDTree::preorderTraversal() const {
    preorderTraversalRecursive(root);
}

void kDTree::preorderTraversalRecursive(kDTreeNode *node) const {
    if (node == nullptr) return;
    cout << "(";
    for (int i = 0; i < k - 1; ++i)
        cout << node->data[i] << ", ";
    cout << node->data[k - 1] << ") ";
    preorderTraversalRecursive(node->left);
    preorderTraversalRecursive(node->right);
}

void kDTree::postorderTraversal() const {
    postorderTraversalRecursive(root);
}

void kDTree::postorderTraversalRecursive(kDTreeNode *node) const {
    if (node == nullptr) return;
    postorderTraversalRecursive(node->left);
    postorderTraversalRecursive(node->right);
    cout << "(";
    for (int i = 0; i < k - 1; ++i)
        cout << node->data[i] << ", ";
    cout << node->data[k - 1] << ") ";
}

int kDTree::height() const {
    return heightRecursive(root);
}

int kDTree::heightRecursive(kDTreeNode *node) const {
    if (node == nullptr) return -1;
    int leftHeight = heightRecursive(node->left);
    int rightHeight = heightRecursive(node->right);
    return 1 + max(leftHeight, rightHeight);
}

int kDTree::nodeCount() const {
    return count;
}
int kDTree::leafCount() const {
    return leafCountRecursive(root);
}

int kDTree::leafCountRecursive(kDTreeNode *node) const {
    if (node == nullptr) return 0;
    if (node->left == nullptr && node->right == nullptr) return 1;
    return leafCountRecursive(node->left) + leafCountRecursive(node->right);
}
double distance(const vector<int> &point1, const vector<int> &point2) {
    if (point1.size() != point2.size()) {
        return -1;
    }

    double sum = 0;

    for (size_t i = 0; i < point1.size(); ++i) {
        sum += pow(point1[i] - point2[i], 2);
    }

    return sqrt(sum);
}
void kDTree::nearestNeighbour(const vector<int>& target, kDTreeNode*& best) {
    nearestNeighbourRecursive(root, target, best, 0);
}

void kDTree::nearestNeighbourRecursive(kDTreeNode* node, const vector<int>& target, kDTreeNode*& best, int depth) {
    if (node == nullptr) {
        return;
    }

    double dist = distance(node->data, target);

    if (best == nullptr || dist < distance(best->data, target)) {
        best = node;
    }

    int axis = depth % k;

    if (target[axis] < node->data[axis]) {
        nearestNeighbourRecursive(node->left, target, best, depth + 1);
    } else {
        nearestNeighbourRecursive(node->right, target, best, depth + 1);
    }

    double diff = target[axis] - node->data[axis];
    if (diff * diff < distance(best->data, target) || best == nullptr) {
        if (target[axis] < node->data[axis]) {
            nearestNeighbourRecursive(node->right, target, best, depth + 1);
        } else {
            nearestNeighbourRecursive(node->left, target, best, depth + 1);
        }
    }
}
void kDTree::kNearestNeighbour(const std::vector<int>& target, int k, std::vector<kDTreeNode*>& bestList) {
    std::list<std::pair<double, kDTreeNode*>> nearestNeighbors;

    kNearestNeighbourRecursive(root, target, k, nearestNeighbors, 0);

    for (const auto& pair : nearestNeighbors) {
        bestList.push_back(pair.second);
    }
}

void kDTree::kNearestNeighbourRecursive(kDTreeNode* node, const std::vector<int>& target, int k, std::list<std::pair<double, kDTreeNode*>>& nearestNeighbors, int depth) {
    if (node == nullptr) {
        return;
    }

    double dist = distance(node->data, target);

    insertSorted(nearestNeighbors, {dist, node});

    if (nearestNeighbors.size() > k) {
        nearestNeighbors.pop_back();
    }

    int axis = depth % k;

    if (target[axis] < node->data[axis]) {
        kNearestNeighbourRecursive(node->left, target, k, nearestNeighbors, depth + 1);
    } else {
        kNearestNeighbourRecursive(node->right, target, k, nearestNeighbors, depth + 1);
    }

    double diff = target[axis] - node->data[axis];
    if (nearestNeighbors.size() < k || diff * diff < nearestNeighbors.back().first) {
        if (target[axis] < node->data[axis]) {
            kNearestNeighbourRecursive(node->right, target, k, nearestNeighbors, depth + 1);
        } else {
            kNearestNeighbourRecursive(node->left, target, k, nearestNeighbors, depth + 1);
        }
    }
}

void kDTree::insertSorted(std::list<std::pair<double, kDTreeNode*>>& nearestNeighbors, const std::pair<double, kDTreeNode*>& nodePair) {
    auto it = nearestNeighbors.begin();
    while (it != nearestNeighbors.end() && it->first < nodePair.first) {
        ++it;
    }
    nearestNeighbors.insert(it, nodePair);
}



/****************************************END KDTREE****************************************/

/****************************************BEGIN KNN****************************************/
void kNN::fit(Dataset &X_train, Dataset &Y_train) {
    this->X_train = X_train;
    this->Y_train = Y_train;
    
    int dim = X_train.data.front().size();
    vector<vector<int>> pointList;
    vector<int> label;

    for (const auto& it:X_train.data){
        vector<int> tmp (next(it.begin()), it.end());
        pointList.push_back(tmp);
    }
    for (const auto& it:X_train.data){
        label.push_back(it.front());
    }
    // Build the k-D Tree using the converted data
    kdtree.buildTreeLabel(pointList, label);
}

/****************************************END KNN****************************************/
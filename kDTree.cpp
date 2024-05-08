#include "kDTree.hpp"

/* TODO: You can implement methods, functions that support your data structures here.
 * */
/****************************************BEGIN KDTREE****************************************/
kDTreeNode* kDTree::deepCopy(const kDTreeNode* node){
    if (node == nullptr){
        return nullptr;
    } else{
        kDTreeNode* newNode = new kDTreeNode(node->data);
        newNode->left = deepCopy(node->left);
        newNode->right = deepCopy(node->right);

        return newNode;    
    }
}
const kDTree& kDTree::operator=(const kDTree &other){
    this-> k = other.k;
    this-> root = deepCopy(other.root);   
}
kDTree::kDTree(const kDTree &other)
{
    this->k = other.k;
    this->root = deepCopy(other.root);
}


// void kDTree::buildTree(const vector<vector<int>> &pointList) {
//     root = buildTreeRecursive(pointList, 0);
// }

// kDTreeNode* kDTree::buildTreeRecursive(const vector<vector<int>> &pointList, int depth) {
//     if (pointList.empty()) return nullptr;

//     int axis = depth % k;
//     int median = pointList.size() / 2;

//     // Sort pointList based on the axis
//     // For simplicity, you can use a sorting algorithm like quicksort

//     kDTreeNode *node = new kDTreeNode(pointList[median]);
//     node->left = buildTreeRecursive(/* left half of pointList */, depth + 1);
//     node->right = buildTreeRecursive(/* right half of pointList */, depth + 1);

//     return node;
// }

int kDTree::chooseSplitAxis(const vector<vector<int>> &pointList, int depth) {
    // Implement your split axis selection logic here
    return depth % k;
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
    removeRecursive(root, point, 0);
    count--;
}

void kDTree::removeRecursive(kDTreeNode *&node, const vector<int> &point, int depth) {
    if (node == nullptr) return;

    int axis = depth % k;
    if (point[axis] < node->data[axis])
        removeRecursive(node->left, point, depth + 1);
    else if (point[axis] > node->data[axis])
        removeRecursive(node->right, point, depth + 1);
    else {
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
        } else if (node->right != nullptr) {
            kDTreeNode *minNode = findMinNode(node->right, axis, depth + 1);
            node->data = minNode->data;
            removeRecursive(node->right, minNode->data, depth + 1);
        } else {
            kDTreeNode *minNode = findMinNode(node->left, axis, depth + 1);
            node->data = minNode->data;
            removeRecursive(node->left, minNode->data, depth + 1);
        }
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

// void kDTree::nearestNeighbour(const vector<int> &target) {
//     kDTreeNode *best = nullptr;
//     nearestNeighbourRecursive(root, target, best, 0);
//     if (best != nullptr) {
//         cout << "Nearest neighbor to target (";
//         for (int i = 0; i < k - 1; ++i)
//             cout << target[i] << ", ";
//         cout << target[k - 1] << ") is (";
//         for (int i = 0; i < k - 1; ++i)
//             cout << best->data[i] << ", ";
//         cout << best->data[k - 1] << ")" << endl;
//     } else {
//         cout << "No nearest neighbor found!" << endl;
//     }
// }

// void kDTree::nearestNeighbourRecursive(kDTreeNode *node, const vector<int> &target, kDTreeNode *&best, int depth) {
//     if (node == nullptr) return;
//     if (best == nullptr || distance(node->data, target) < distance(best->data, target))
//         best = node;

//     int axis = depth % k;
//     if (target[axis] < node->data[axis]) {
//         nearestNeighbourRecursive(node->left, target, best, depth + 1);
//         if (node->right != nullptr && distance(node->right->data, target) < distance(best->data, target))
//             nearestNeighbourRecursive(node->right, target, best, depth + 1);
//     } else {
//         nearestNeighbourRecursive(node->right, target, best, depth + 1);
//         if (node->left != nullptr && distance(node->left->data, target) < distance(best->data, target))
//             nearestNeighbourRecursive(node->left, target, best, depth + 1);
//     }
// }

// void kDTree::kNearestNeighbour(const vector<int> &target, int k, vector<kDTreeNode *> &bestList) {
//     kNearestNeighbourRecursive(root, target, k, bestList, 0);
// }

// void kDTree::kNearestNeighbourRecursive(kDTreeNode *node, const vector<int> &target, int k, vector<kDTreeNode *> &bestList, int depth) {
//     if (node == nullptr) return;

//     if (bestList.size() < k) {
//         bestList.push_back(node);
//     } else {
//         double maxDistance = 0;
//         int indexToReplace = -1;
//         for (int i = 0; i < k; ++i) {
//             double dist = distance(bestList[i]->data, target);
//             if (dist > maxDistance) {
//                 maxDistance = dist;
//                 indexToReplace = i;
//             }
//         }
//         if (distance(node->data, target) < maxDistance) {
//             bestList[indexToReplace] = node;
//         } else {
//             return; // No need to explore further
//         }
//     }

//     int axis = depth % k;
//     if (target[axis] < node->data[axis]) {
//         kNearestNeighbourRecursive(node->left, target, k, bestList, depth + 1);
//         if (node->right != nullptr && distance(node->right->data, target) < distance(bestList.back()->data, target))
//             kNearestNeighbourRecursive(node->right, target, k, bestList, depth + 1);
//     } else {
//         kNearestNeighbourRecursive(node->right, target, k, bestList, depth + 1);
//         if (node->left != nullptr && distance(node->left->data, target) < distance(bestList.back()->data, target))
//             kNearestNeighbourRecursive(node->left, target, k, bestList, depth + 1);
//     }
// }
/****************************************END KDTREE****************************************/
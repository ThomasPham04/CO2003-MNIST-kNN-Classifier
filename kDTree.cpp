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


/****************************************END KDTREE****************************************/
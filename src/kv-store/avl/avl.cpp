#include "avl.h"

// Start of AVLNode class
AVLNode::AVLNode(KVPair kv) : kv(kv), left(nullptr), right(nullptr), height(1) {}

// Get the KVPair object of the node
KVPair& AVLNode::getKV() {
    return kv;
}

// Get the left child of the node
AVLNode *AVLNode::getLeft() {
    return left;
}

// Get the right child of the node
AVLNode *AVLNode::getRight() {
    return right;
}

// Get the height of the node
int AVLNode::getHeight() {
    return height;
}

// Set the KVPair object of the node
void AVLNode::setKV(KVPair kv) {
    this->kv = kv;
}

// Set the left child of the node
void AVLNode::setLeft(AVLNode *left) {
    this->left = left;
}

// Set the right child of the node
void AVLNode::setRight(AVLNode *right) {
    this->right = right;
}

// Set the height of the node
void AVLNode::setHeight(int height) {
    this->height = height;
}
// End of AVLNode class


// Start of AVLTree class
AVLTree::AVLTree() : root(nullptr), treeSize(0) {}

AVLTree::~AVLTree() {
    clearTree();
}

// Get the root of the tree
AVLNode *AVLTree::getRoot() {
    return root;
}

// Get the size of the tree
int AVLTree::getTreeSize() {
    return treeSize;
}

// Get the height of a node
int AVLTree::getNodeHeight(AVLNode *node) {
    if (node == nullptr) {
        return 0;
    }
    return node->getHeight();
}

// Get the balance factor of a node
int AVLTree::getBalanceFactor(AVLNode *node) {
    return getNodeHeight(node->getLeft()) - getNodeHeight(node->getRight());
}

// Helper method to rotate a node to the left
AVLNode *AVLTree::rotateLeft(AVLNode *node) {
    AVLNode *right = node->getRight();
    AVLNode *left = right->getLeft();

    right->setLeft(node);
    node->setRight(left);

    node->setHeight(1 + std::max(getNodeHeight(node->getLeft()), getNodeHeight(node->getRight())));
    right->setHeight(1 + std::max(getNodeHeight(right->getLeft()), getNodeHeight(right->getRight())));

    return right;
}

// Helper method to rotate a node to the right
AVLNode *AVLTree::rotateRight(AVLNode *node) {
    AVLNode *left = node->getLeft();
    AVLNode *right = left->getRight();

    left->setRight(node);
    node->setLeft(right);

    node->setHeight(1 + std::max(getNodeHeight(node->getLeft()), getNodeHeight(node->getRight())));
    left->setHeight(1 + std::max(getNodeHeight(left->getLeft()), getNodeHeight(left->getRight())));

    return left;
}

// Helper method to insert a KVPair object into the tree
AVLNode *AVLTree::insertHelper(AVLNode *node, KVPair kv) {
    if (node == nullptr) { 
        node = new AVLNode(kv);
        return node;
    }

    const int key = kv.getKey();
    if (key < node->getKV().getKey()) {
        node->setLeft(insertHelper(node->getLeft(), kv));
    }
    else if (key > node->getKV().getKey()) {
        node->setRight(insertHelper(node->getRight(), kv));
    }
    else {
        node->setKV(kv);
        treeSize -= 1;
        return node;
    }

    node->setHeight(1 + std::max(getNodeHeight(node->getLeft()), getNodeHeight(node->getRight())));
    int balanceFactor = getBalanceFactor(node);

    // Left Left Case
    if (balanceFactor > 1 && key < node->getLeft()->getKV().getKey()) {
        return rotateRight(node);
    }
    // Left Right Case
    if (balanceFactor > 1 && key > node->getLeft()->getKV().getKey()) {
        node->setLeft(rotateLeft(node->getLeft()));
        return rotateRight(node);
    }
    // Right Right Case
    if (balanceFactor < -1 && key > node->getRight()->getKV().getKey()) {
        return rotateLeft(node);
    }
    // Right Left Case
    if (balanceFactor < -1 && key < node->getRight()->getKV().getKey()) {
        node->setRight(rotateRight(node->getRight()));
        return rotateLeft(node);
    }

    return node;
}

// Helper method to the scan method
vector<KVPair> AVLTree::scanHelper(AVLNode *node, uint64_t start, uint64_t end, vector<KVPair> &result) {
    if (node == nullptr) {
        return result;
    }

    if (node->getKV().getKey() > start) {
        scanHelper(node->getLeft(), start, end, result);
    }
    if (node->getKV().getKey() >= start && node->getKV().getKey() <= end) {
        result.push_back(node->getKV());
    }
    if (node->getKV().getKey() < end) {
        scanHelper(node->getRight(), start, end, result);
    }

    return result;
}

// Helper method to return an inorder traversal of the tree nodes
vector<KVPair> AVLTree::inorderHelper(AVLNode *node, vector<KVPair> &result) {
    if (node == nullptr) {
        return result;
    }
    inorderHelper(node->getLeft(), result);
    result.push_back(node->getKV());
    inorderHelper(node->getRight(), result);
    return result;
}

// Helper method to return a preorder traversal of the tree nodes
vector<KVPair> AVLTree::preorderHelper(AVLNode *node, vector<KVPair> &result) {
    if (node == nullptr) {
        return result;
    }
    result.push_back(node->getKV());
    preorderHelper(node->getLeft(), result);
    preorderHelper(node->getRight(), result);
    return result;
}

// Helper method to return a postorder traversal of the tree nodes
vector<KVPair> AVLTree::postorderHelper(AVLNode *node, vector<KVPair> &result) {
    if (node == nullptr) {
        return result;
    }
    postorderHelper(node->getLeft(), result);
    postorderHelper(node->getRight(), result);
    result.push_back(node->getKV());
    return result;
}

// Recursively deletes all nodes in the tree
void AVLTree::clearTreeHelper(AVLNode *node) {
    if (node == nullptr) {
        return;
    }
    clearTreeHelper(node->getLeft());
    clearTreeHelper(node->getRight());
    delete node;
}

// Inserts a KVPair object into the tree
void AVLTree::insert(KVPair kv) {
    treeSize++;
    root = insertHelper(root, kv);
}

// Returns a pointer to the KVPair object with the given key
KVPair *AVLTree::get(uint64_t key) {
    AVLNode *node = root;
    while (node != nullptr) {
        if (key < node->getKV().getKey()) {
            node = node->getLeft();
        }
        else if (key > node->getKV().getKey()) {
            node = node->getRight();
        }
        else {
            return &(node->getKV());
        }
    }
    // Key not found
    return nullptr; 
}

// Returns a vector of KVPair objects with keys in the range [start, end]
vector<KVPair> AVLTree::scan(uint64_t start, uint64_t end) {
    vector<KVPair> result;
    return scanHelper(root, start, end, result);
}

// Clears the nodes in the tree without deleting the tree object
void AVLTree::clearTree() {
    clearTreeHelper(root);
    root = nullptr;
    treeSize = 0;
}

// Returns an inorder traversal of the tree nodes
vector<KVPair> AVLTree::inorderTraversal() {
    vector<KVPair> result;
    return inorderHelper(root, result);
}

// Returns a preorder traversal of the tree nodes
vector<KVPair> AVLTree::preorderTraversal() {
    vector<KVPair> result;
    return preorderHelper(root, result);
}

// Returns a postorder traversal of the tree nodes
vector<KVPair> AVLTree::postorderTraversal() {
    vector<KVPair> result;
    return postorderHelper(root, result);
}
// End of AVLTree class
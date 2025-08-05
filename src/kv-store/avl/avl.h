#ifndef AVLTREE_H
#define AVLTREE_H

#include "../util/common.h"
#include "../util/namespace-std.h"
#include <vector>
#include <iostream>
#include <algorithm>

/**
 * @class AVLNode
 * @brief Represents a node in an AVL tree
 */
class AVLNode {
    KVPair kv;
    AVLNode *left;
    AVLNode *right;
    int height;
public:
    /**
     * @brief Construct a new AVLNode object
     * 
     * @param kv The KV pair to store in the node
     */
    AVLNode(KVPair kv);
    
    /**
     * @brief Get the KV pair stored in the node
     * 
     * @return KVPair& The KV pair
     */
    KVPair& getKV();

    /**
     * @brief Get the left child of the node
     * 
     * @return AVLNode* The left child
     */
    AVLNode *getLeft();

    /**
     * @brief Get the right child of the node
     * 
     * @return AVLNode* The right child
     */
    AVLNode *getRight();

    /**
     * @brief Get the height of the node
     * 
     * @return int The height
     */
    int getHeight();
    
    /**
     * @brief Set the KV pair stored in the node
     * 
     * @param kv The KV pair
     */
    void setKV(KVPair kv);

    /**
     * @brief Set the left child of the node
     * 
     * @param left The left child
     */
    void setLeft(AVLNode *left);

    /**
     * @brief Set the right child of the node
     * 
     * @param right The right child
     */
    void setRight(AVLNode *right);

    /**
     * @brief Set the height of the node
     * 
     * @param height The height
     */
    void setHeight(int height);
};

/**
 * @class AVLTree
 * @brief Represents an AVL tree to be used as a memtable
 */
class AVLTree {
    AVLNode *root;
    int treeSize;

    /**
     * @brief Get the height of a node
     * 
     * @param node The node
     * 
     * @return int The height of the node
     */
    int getNodeHeight(AVLNode *node);

    /**
     * @brief Get the balance factor of a node
     * 
     * @param node The node
     * 
     * @return int The balance factor of the node
     */
    int getBalanceFactor(AVLNode *node);

    /**
     * @brief Perform a left rotation on a node
     * 
     * @param node The node to rotate
     * 
     * @return AVLNode* The new root of the subtree
     */
    AVLNode *rotateLeft(AVLNode *node);

    /**
     * @brief Perform a right rotation on a node
     * 
     * @param node The node to rotate
     * 
     * @return AVLNode* The new root of the subtree
     */
    AVLNode *rotateRight(AVLNode *node);
    
    /**
     * @brief Helper function to insert a KV pair into the tree
     * 
     * @param node The root of the subtree
     * @param kv The KV pair to insert
     * 
     * @return AVLNode* The new root of the subtree
     */
    AVLNode *insertHelper(AVLNode *node, KVPair kv);
    
    /**
     * @brief Helper function to scan for KV pairs from the tree
     * 
     * @param node The root of the subtree
     * @param start The start key of the scan
     * @param end The end key of the scan
     * @param result The vector to store the KV pairs
     * 
     * @return KVPair* The KV pair
     */
    vector<KVPair> scanHelper(AVLNode *node, uint64_t start, uint64_t end, vector<KVPair> &result);

    /**
     * @brief Helper function to do an inorder traversal of the tree
     * 
     * @param node The root of the subtree
     * @param result The vector to store the KV pairs
     * 
     * @return vector<KVPair> The vector of KV pairs
     */
    vector<KVPair> inorderHelper(AVLNode *node, vector<KVPair> &result);

    /**
     * @brief Helper function to do a preorder traversal of the tree
     * 
     * @param node The root of the subtree
     * @param result The vector to store the KV pairs
     * 
     * @return vector<KVPair> The vector of KV pairs
     */
    vector<KVPair> preorderHelper(AVLNode *node, vector<KVPair> &result);

    /**
     * @brief Helper function to do a postorder traversal of the tree
     * 
     * @param node The root of the subtree
     * @param result The vector to store the KV pairs
     * 
     * @return vector<KVPair> The vector of KV pairs
     */
    vector<KVPair> postorderHelper(AVLNode *node, vector<KVPair> &result);
    

    /**
     * @brief Helper function to clear the tree
     * 
     * @param node The root of the subtree
     */
    void clearTreeHelper(AVLNode *node);
    
public:
    /**
     * @brief Construct a new AVLTree object
     */
    AVLTree();

    /**
     * @brief Destroy the AVLTree object
     */
    ~AVLTree();

    /**
     * @brief Get the root of the tree
     * 
     * @return AVLNode* The root of the tree
     */
    AVLNode *getRoot();

    /**
     * @brief Get the size of the tree
     * 
     * @return int The size of the tree
     */
    int getTreeSize();

    /**
     * @brief Insert a KV pair into the tree
     * 
     * @param kv The KV pair to insert
     */
    void insert(KVPair kv);

    /**
     * @brief Get a KV pair from the tree
     * 
     * @param key The key of the KV pair
     * 
     * @return KVPair* The KV pair
     */
    KVPair *get(uint64_t key);

    /**
     * @brief Scan for KV pairs from the tree
     * 
     * @param start The start key of the scan
     * @param end The end key of the scan
     * 
     * @return vector<KVPair> The vector of KV pairs
     */
    vector<KVPair> scan(uint64_t start, uint64_t end);  
    
    /**
     * @brief Clear the tree
     */
    void clearTree();

    /**
     * @brief Perform an inorder traversal of the tree
     * 
     * @return vector<KVPair> The vector of KV pairs
     */
    vector<KVPair> inorderTraversal();

    /**
     * @brief Perform a preorder traversal of the tree
     * 
     * @return vector<KVPair> The vector of KV pairs
     */
    vector<KVPair> preorderTraversal();

    /**
     * @brief Perform a postorder traversal of the tree
     * 
     * @return vector<KVPair> The vector of KV pairs
     */
    vector<KVPair> postorderTraversal();
};

#endif // AVLTREE_H
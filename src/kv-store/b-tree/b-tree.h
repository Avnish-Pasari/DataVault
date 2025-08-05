#ifndef BTREE_H
#define BTREE_H

#include "../util/common.h"
#include "../util/namespace-std.h"
#include <cmath>
#include <deque>

/**
 * @class BTreeNode
 * 
 * @brief Represents a node in the B+ tree
 */
class BTreeNode {
    public:
        vector<uint64_t> keys;
};

/**
 * @class BTreeUtil
 * 
 * @brief Utility class for creating and printing B+ trees
 */
class BTreeUtil {
    private:
        /**
         * @brief Split the arrays 'arr1' and 'arr2' into 'left' and 'right' arrays
         * 
         * Ensures that the length of the left and right arrays are within 1 of each other.
         * 
         * @param arr1 The first array to split
         * @param arr2 The second array to split
         */
        static void splitArrays(vector<uint64_t>& arr1, vector<uint64_t>& arr2, vector<uint64_t>& left, vector<uint64_t>& right);

        /**
         * @brief Helper function to create the internal node levels of the B+ tree
         * 
         * @param internalNodeLevels The internal node levels of the B+ tree
         */
        static void createBPlusTreeInternalNodesHelper(deque<vector<BTreeNode*> >& internalNodeLevels);
    public:
        /**
         * @brief Create a B+ tree from the sorted array of keys 'sortedMaxKeys' and the number of pages 'numPages'
         * 
         * @param sortedMaxKeys The sorted array of the maximum keys in each page of the SST
         * @param numPages The number of pages
         */
        static deque<vector<BTreeNode*> > createBPlusTree(uint64_t *sortedMaxKeys, int numPages);

        
        /**
         * This function takes in a sorted array of keys 'sortedMaxKeys' (the highest key of each page) and 
         * the number of keys 'numPages' since each key is from a different page, thus the number of keys gives
         * us the number of pages.
         * This array does NOT need to be 4KB page aligned.
         * 
         * This function returns an array of KV pairs where the value is actually the pointer (i.e. page_number)
         * so we get an array of <key, page_number> representing the internal nodes of the B+ tree.
         * 
         * Each internal node would be padded to make it 4KB page aligned.
         * 
         * A node of <0, page_number> represents the last pointer of the internal node.
         * A node of <0, 0> represents a redundant entry (for padding) to make the internal node 4KB page aligned.
         * 
         * The value of 'numInternalNodes' is set to the number of internal nodes in the B+ tree.
         */
        static KVPair *flushMemtableToBPlusTree(uint64_t *sortedMaxKeys, int numPages, int &numInternalNodes);
        
        /**
         * @brief Print the B+ tree
         * 
         * @param bPlusTree The B+ tree to print
         */
        static void printBPlusTree(deque<vector<BTreeNode*> > bPlusTree);

        /**
         * @brief Print the B+ tree level
         * 
         * @param level The level of the B+ tree to print
         */
        static void printBPlusTreeLevel(vector<BTreeNode*> level);
};  

#endif // BTREE_H
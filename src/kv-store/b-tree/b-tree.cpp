#include "b-tree.h"

/**
 * Split the input arrays into two arrays, left and right, of equal size.
 * If the input arrays have an odd number of elements, the left array will have one more element.
 */
void BTreeUtil::splitArrays(vector<uint64_t>& arr1, vector<uint64_t>& arr2, vector<uint64_t>& left, vector<uint64_t>& right) {
    std::vector<int> combined(arr1.begin(), arr1.end());
    combined.insert(combined.end(), arr2.begin(), arr2.end());

    int totalSize = combined.size();
    int mid = (totalSize + 1) / 2; 

    left.assign(combined.begin(), combined.begin() + mid);
    right.assign(combined.begin() + mid, combined.end());
}

/**
 * Helper function to create internal nodes of the B+ tree, beyond the internal nodes at the base level.
 */
void BTreeUtil::createBPlusTreeInternalNodesHelper(deque<vector<BTreeNode*> > &internalNodeLevels) {
    vector<BTreeNode*> &currLevel = internalNodeLevels[0];
    
    vector<vector<uint64_t> > nextLevelGroupings;
    vector<uint64_t> currNodeArr;
    for (int i = 0; i < currLevel.size() - 1; i++) {
        BTreeNode* &currNode = currLevel[i];
        if (currNodeArr.size() == B_TREE_FANOUT) {
            nextLevelGroupings.push_back(currNodeArr);
            currNodeArr.clear();
        }
        currNodeArr.push_back(currNode->keys.back());
        currNode->keys.pop_back();
    }

    if (currNodeArr.size() == B_TREE_FANOUT) {
        int mid = (currNodeArr.size() + 1) / 2;
        vector<uint64_t> left(currNodeArr.begin(), currNodeArr.begin() + mid);
        vector<uint64_t> right(currNodeArr.begin() + mid, currNodeArr.end());
        nextLevelGroupings.push_back(left);
        nextLevelGroupings.push_back(right);
    }
    else if (currNodeArr.size() < ceil(static_cast<double>(B_TREE_FANOUT) / 2) && nextLevelGroupings.size() > 0) {
        vector<uint64_t> currLastArr = nextLevelGroupings.back();
        nextLevelGroupings.pop_back();
        vector<uint64_t> left;
        vector<uint64_t> right;
        splitArrays(currLastArr, currNodeArr, left, right);
        nextLevelGroupings.push_back(left);
        nextLevelGroupings.push_back(right);
    }
    else {
        nextLevelGroupings.push_back(currNodeArr);
    }

    vector<BTreeNode*> nextLevel;
    for (int i = 0; i < nextLevelGroupings.size(); i++) {
        BTreeNode* node = new BTreeNode();
        node->keys = nextLevelGroupings[i];
        nextLevel.push_back(node);
    }
    internalNodeLevels.push_front(nextLevel);
}

/**
 * Create a B+ tree from the input sorted keys.
 */
deque<vector<BTreeNode*> > BTreeUtil::createBPlusTree(uint64_t *sortedMaxKeys, int numPages) {
    // Extract max from all pages, grouped by fanout
    vector<vector<uint64_t> > internalNodesBaseLevelGroupings;
    vector<uint64_t> currNodeArr;
    for (int i = 0; i < numPages; i++) {
        if (currNodeArr.size() == B_TREE_FANOUT) {
            internalNodesBaseLevelGroupings.push_back(currNodeArr);
            currNodeArr.clear();
        }
        currNodeArr.push_back(sortedMaxKeys[i]);
    }
    
    /**
     * Check:
     *  - If currNodeArr has B_TREE_FANOUT elements, split and add to internalNodesBaseLevelGroupings
     *  - If currNodeArr has less than ceil(B_TREE_FANOUT/2) elements, merge with previous node
     */
    if (currNodeArr.size() == B_TREE_FANOUT) {
        int mid = (currNodeArr.size() + 1) / 2;
        vector<uint64_t> left(currNodeArr.begin(), currNodeArr.begin() + mid);
        vector<uint64_t> right(currNodeArr.begin() + mid, currNodeArr.end());
        internalNodesBaseLevelGroupings.push_back(left);
        internalNodesBaseLevelGroupings.push_back(right);
    }
    else if (currNodeArr.size() <= ceil(static_cast<double>(B_TREE_FANOUT) / 2) && internalNodesBaseLevelGroupings.size() > 0) {
        vector<uint64_t> currLastArr = internalNodesBaseLevelGroupings.back();
        internalNodesBaseLevelGroupings.pop_back();
        vector<uint64_t> left;
        vector<uint64_t> right;
        splitArrays(currLastArr, currNodeArr, left, right);
        internalNodesBaseLevelGroupings.push_back(left);
        internalNodesBaseLevelGroupings.push_back(right);
    }
    else {
        internalNodesBaseLevelGroupings.push_back(currNodeArr);
    }
    
    // Create the BTreeNodes for the base level internal nodes
    vector<BTreeNode*> internalNodesBaseLevel;
    for (int i = 0; i < internalNodesBaseLevelGroupings.size(); i++) {
        BTreeNode* node = new BTreeNode();
        node->keys = internalNodesBaseLevelGroupings[i];
        if (i == internalNodesBaseLevelGroupings.size() - 1) {
            node->keys.pop_back();
        }
        if (node->keys.size() > 0) {
            internalNodesBaseLevel.push_back(node);
        }
    }

    // We now have our base level of internal nodes
    // Continue building the tree from bottom up until we reach a root node
    deque<vector<BTreeNode*> > internalNodeLevels;
    internalNodeLevels.push_back(internalNodesBaseLevel);
    while (internalNodeLevels[0].size() > 1) {
        createBPlusTreeInternalNodesHelper(internalNodeLevels);
    }

    return internalNodeLevels;
}

/**
 * Flush the input sorted keys to a B+ tree.
 * Each internal node will be represented by a series of KV pairs.
 * Pages are 1-Indexed. The root node is at page 1. 
 */
KVPair* BTreeUtil::flushMemtableToBPlusTree(uint64_t *sortedMaxKeys, int numPages, int &numInternalNodes) {
    deque<vector<BTreeNode*> > bPlusTree = createBPlusTree(sortedMaxKeys, numPages);
    int countInternalNodes = 0;

    // count the number of internal nodes
    for (int i = 0; i < bPlusTree.size(); i++) {
        countInternalNodes += bPlusTree[i].size();
    }
    numInternalNodes = countInternalNodes;

    // create the KV pairs for the internal nodes
    KVPair* internalNodes = new KVPair[countInternalNodes * KV_PAIRS_PER_PAGE];
    int internalNodeIndex = 0;
    int currPageNumber = 1;
    for (int level = 0; level < bPlusTree.size(); level++) {
        vector<BTreeNode*> &levelNodes = bPlusTree[level];
        const int numNodesInLevel = levelNodes.size();
        int numKeysInLevelBeforeCurrentNode = 0;
        
        for (int levelNodeIndex = 0; levelNodeIndex < numNodesInLevel; levelNodeIndex++) {
            BTreeNode* &node = levelNodes[levelNodeIndex];
            vector<uint64_t> &keys = node->keys;
            const int numKeysInNode = keys.size();

            for (int keyIndex = 0; keyIndex <= numKeysInNode; keyIndex++) {
                const int correspondingPageIndex = currPageNumber + numNodesInLevel - (levelNodeIndex + 1) + numKeysInLevelBeforeCurrentNode + keyIndex + 1;
                if (keyIndex < numKeysInNode) {
                    internalNodes[internalNodeIndex] = KVPair(keys[keyIndex], correspondingPageIndex);
                }
                else {
                    internalNodes[internalNodeIndex] = KVPair(0, correspondingPageIndex);
                }
                internalNodeIndex++;
            }
            
            // zero pad the rest of the page, if not 4kb page aligned
            while (internalNodeIndex % KV_PAIRS_PER_PAGE != 0) {
                internalNodes[internalNodeIndex] = KVPair(0, 0);
                internalNodeIndex++;
            }

            numKeysInLevelBeforeCurrentNode += numKeysInNode + 1;
            currPageNumber += 1;
        }
    }

    return internalNodes;
}

/**
 * Print the B+ tree's internal nodes to the console.
 */
void BTreeUtil::printBPlusTree(deque<vector<BTreeNode*> > bPlusTree) {
    for (int i = 0; i < bPlusTree.size(); i++) {
        cout << "Level " << i << ": " << endl;
        vector<BTreeNode*> &level = bPlusTree[i];
        for (int j = 0; j < level.size(); j++) {
            BTreeNode* &node = level[j];
            cout << "\t[";
            for (int k = 0; k < node->keys.size(); k++) {
                cout << node->keys[k];
                if (k != node->keys.size() - 1) {
                    cout << ", ";
                }
            }
            cout << "]" << endl;
        }
    }
}

/**
 * Print a single level of the B+ tree's internal nodes to the console.
 */
void BTreeUtil::printBPlusTreeLevel(vector<BTreeNode*> level) {
    for (int i = 0; i < level.size(); i++) {
        BTreeNode* node = level[i];
        cout << "\t[";
        for (int j = 0; j < node->keys.size(); j++) {
            cout << node->keys[j];
            if (j != node->keys.size() - 1) {
                cout << ", ";
            }
        }
        cout << "]" << endl;
    }
}
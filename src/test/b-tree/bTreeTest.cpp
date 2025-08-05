#include "bTreeTest.h"

/**
 * Test the manual example created by Aakash and Avnish.
 * B_TREE_FANOUT must be set to 3. 
 * This function is for visualization purposes only.
 */
void testFanout3() {
    int numPages = 28;
    uint64_t sortedMaxKeys[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                                11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                                21, 22, 23, 24, 25, 26, 27, 28};
    deque<vector<BTreeNode*> > bPlusTree = BTreeUtil::createBPlusTree(sortedMaxKeys, numPages);
    BTreeUtil::printBPlusTree(bPlusTree);
}

/**
 * Test an example with 120 keys and a fanout of 4.
 * This example assumes 30 keys at the base internal node level. 
 */
void testFanout4() {
    int numPages = 30;
    uint64_t sortedMaxKeys[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                              11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                              21, 22, 23, 24, 25, 26, 27, 28, 29, 30};
    deque<vector<BTreeNode*> > bPlusTree = BTreeUtil::createBPlusTree(sortedMaxKeys, numPages);

    // assert number of levels
    assert(bPlusTree.size() == 3);

    // check root level
    vector<BTreeNode*> rootLevel = bPlusTree[0];
    assert(rootLevel.size() == 1);
    assert(rootLevel[0]->keys.size() == 1);
    assert(rootLevel[0]->keys[0] == 16);

    // check next level
    vector<BTreeNode*> nextLevel = bPlusTree[1];
    assert(nextLevel.size() == 2);
    assert(nextLevel[0]->keys.size() == 3);
    for (int i = 0; i < 3; i++) {
        assert(nextLevel[0]->keys[i] == 4 * (i + 1));
    }
    assert(nextLevel[1]->keys.size() == 3);
    assert(nextLevel[1]->keys[0] == 20);
    assert(nextLevel[1]->keys[1] == 24);
    assert(nextLevel[1]->keys[2] == 27);

    // check base level
    vector<BTreeNode*> baseLevel = bPlusTree[2];
    assert(baseLevel.size() == 8);
    assert(baseLevel[0]->keys.size() == 3);
    for (int i = 0; i < 3; i++) {
        assert(baseLevel[0]->keys[i] == i + 1);
    }
    assert(baseLevel[1]->keys.size() == 3);
    for (int i = 0; i < 3; i++) {
        assert(baseLevel[1]->keys[i] == 4 + (i + 1));
    }
    assert(baseLevel[2]->keys.size() == 3);
    for (int i = 0; i < 3; i++) {
        assert(baseLevel[2]->keys[i] == 8 + (i + 1));
    }
    assert(baseLevel[3]->keys.size() == 3);
    for (int i = 0; i < 3; i++) {
        assert(baseLevel[3]->keys[i] == 12 + (i + 1));
    }
    assert(baseLevel[4]->keys.size() == 3);
    for (int i = 0; i < 3; i++) {
        assert(baseLevel[4]->keys[i] == 16 + (i + 1));
    }
    assert(baseLevel[5]->keys.size() == 3);
    for (int i = 0; i < 3; i++) {
        assert(baseLevel[5]->keys[i] == 20 + (i + 1));
    }
    assert(baseLevel[6]->keys.size() == 2);
    for (int i = 0; i < 2; i++) {
        assert(baseLevel[6]->keys[i] == 24 + (i + 1));
    }
    assert(baseLevel[7]->keys.size() == 2);
    for (int i = 0; i < 2; i++) {
        assert(baseLevel[7]->keys[i] == 27 + (i + 1));
    }
}

void testNumKeysLessThanFanout() {
    int numPages = 1;
    uint64_t sortedMaxKeys[] = {1};
    deque<vector<BTreeNode*> > bPlusTree = BTreeUtil::createBPlusTree(sortedMaxKeys, numPages);

    // assert number of levels
    assert(bPlusTree.size() == 1);
    assert(bPlusTree[0].size() == 0);
}

void testFlushMemtableToBPlusTreeFanout4() {
    int numPages = 30;
    uint64_t sortedMaxKeys[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                              11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                              21, 22, 23, 24, 25, 26, 27, 28, 29, 30};

    int numInternalNodes;
    KVPair* internalNodes = BTreeUtil::flushMemtableToBPlusTree(sortedMaxKeys, numPages, numInternalNodes);
    for (int i = 0; i < numInternalNodes * KV_PAIRS_PER_PAGE; i++) {
        if (i % KV_PAIRS_PER_PAGE == 0) {
            cout << endl;
        }
        cout << "<" << internalNodes[i].getKey() << "," << internalNodes[i].getValue() << "> ";
    }
}

// Note: To run these tests, please set the appropriate B-Tree fanout.
void bTreeTestAll() {
    cout << "Running B-Tree tests..." << endl;
    // testFanout3(); // This testing function is for visualization purposes only
    // testFanout4();
    // cout << "testFanout4 passed" << endl;

    cout << "Running testNumKeysLessThanFanout..." << endl;
    testNumKeysLessThanFanout();
    cout << "testNumKeysLessThanFanout passed" << endl;

    // NOTE: To run this test, set B_TREE_FANOUT = 4 and KV_PAIRS_PER_PAGE = 4
    // testFlushMemtableToBPlusTreeFanout4();
    // cout << "testFlushMemtableToBPlusTreeFanout4 passed" << endl;

    cout << "B-Tree tests passed!" << endl;
}
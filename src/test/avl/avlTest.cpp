#include "avlTest.h"

void testInsertBalanced() {
    AVLTree tree;
    tree.insert(KVPair(2, 2));
    assert(tree.getTreeSize() == 1);
    tree.insert(KVPair(1, 1));
    assert(tree.getTreeSize() == 2);
    tree.insert(KVPair(3, 3));
    assert(tree.getTreeSize() == 3);
    
    AVLNode *root = tree.getRoot();
    assert(root->getKV().getKey() == 2);
    assert(root->getLeft()->getKV().getKey() == 1);
    assert(root->getRight()->getKV().getKey() == 3);
}

void testInsertLeftLeft() {
    AVLTree tree;
    tree.insert(KVPair(3, 3));
    assert(tree.getTreeSize() == 1);
    tree.insert(KVPair(2, 2));
    assert(tree.getTreeSize() == 2);
    tree.insert(KVPair(1, 1));
    assert(tree.getTreeSize() == 3);
    
    AVLNode *root = tree.getRoot();
    assert(root->getKV().getKey() == 2);
    assert(root->getLeft()->getKV().getKey() == 1);
    assert(root->getRight()->getKV().getKey() == 3);
}

void testInsertRightRight() {
    AVLTree tree;
    tree.insert(KVPair(1, 1));
    assert(tree.getTreeSize() == 1);
    tree.insert(KVPair(2, 2));
    assert(tree.getTreeSize() == 2);
    tree.insert(KVPair(3, 3));
    assert(tree.getTreeSize() == 3);
    
    AVLNode *root = tree.getRoot();
    assert(root->getKV().getKey() == 2);
    assert(root->getLeft()->getKV().getKey() == 1);
    assert(root->getRight()->getKV().getKey() == 3);
}

void testInsertLeftRight() {
    AVLTree tree;
    tree.insert(KVPair(3, 3));
    assert(tree.getTreeSize() == 1);
    tree.insert(KVPair(1, 1));
    assert(tree.getTreeSize() == 2);
    tree.insert(KVPair(2, 2));
    assert(tree.getTreeSize() == 3);
    
    AVLNode *root = tree.getRoot();
    assert(root->getKV().getKey() == 2);
    assert(root->getLeft()->getKV().getKey() == 1);
    assert(root->getRight()->getKV().getKey() == 3);
}

void testInsertRightLeft() {
    AVLTree tree;
    tree.insert(KVPair(1, 1));
    assert(tree.getTreeSize() == 1);
    tree.insert(KVPair(3, 3));
    assert(tree.getTreeSize() == 2);
    tree.insert(KVPair(2, 2));
    assert(tree.getTreeSize() == 3);
    
    AVLNode *root = tree.getRoot();
    assert(root->getKV().getKey() == 2);
    assert(root->getLeft()->getKV().getKey() == 1);
    assert(root->getRight()->getKV().getKey() == 3);
}

void testComplexInsert() {
    AVLTree tree;
    tree.insert(KVPair(10, 10));
    assert(tree.getTreeSize() == 1);
    tree.insert(KVPair(20, 20));
    assert(tree.getTreeSize() == 2);
    tree.insert(KVPair(30, 30)); 
    assert(tree.getTreeSize() == 3);
    tree.insert(KVPair(25, 25));
    assert(tree.getTreeSize() == 4);
    tree.insert(KVPair(22, 22));
    assert(tree.getTreeSize() == 5);
    
    AVLNode *root = tree.getRoot();
    assert(root->getKV().getKey() == 20);
    assert(root->getLeft()->getKV().getKey() == 10);
    assert(root->getRight()->getKV().getKey() == 25);
    assert(root->getRight()->getLeft()->getKV().getKey() == 22);
    assert(root->getRight()->getRight()->getKV().getKey() == 30);
}

void testGetKeyExists() {
    AVLTree tree;
    tree.insert(KVPair(1, 1));
    tree.insert(KVPair(2, 2));
    tree.insert(KVPair(3, 3));
    KVPair *result = tree.get(2);
    assert(result->getKey() == 2);
    assert(result->getValue() == 2);
}

void testGetKeyDoesNotExist() {
    AVLTree tree;
    tree.insert(KVPair(1, 1));
    tree.insert(KVPair(2, 2));
    tree.insert(KVPair(3, 3));
    KVPair *result = tree.get(4);
    assert(result == nullptr);
}

void testScanEmpty() {
    AVLTree tree;
    std::vector<KVPair> result = tree.scan(1, 3);
    assert(result.size() == 0);
}

void testScanBothKeysExist() {
    AVLTree tree;
    tree.insert(KVPair(1, 1));
    tree.insert(KVPair(2, 2));
    tree.insert(KVPair(3, 3));
    std::vector<KVPair> result = tree.scan(1, 3);
    assert(result.size() == 3);
    assert(result[0].getKey() == 1);
    assert(result[1].getKey() == 2);
    assert(result[2].getKey() == 3);
}

void testScanStartKeyExists() {
    AVLTree tree;
    tree.insert(KVPair(1, 1));
    tree.insert(KVPair(2, 2));
    tree.insert(KVPair(4, 4));
    std::vector<KVPair> result = tree.scan(2, 3);
    assert(result.size() == 1);
    assert(result[0].getKey() == 2);
}

void testScanEndKeyExists() {
    AVLTree tree;
    tree.insert(KVPair(1, 1));
    tree.insert(KVPair(3, 3));
    tree.insert(KVPair(4, 4));
    std::vector<KVPair> result = tree.scan(2, 4);
    assert(result.size() == 2);
    assert(result[0].getKey() == 3);
    assert(result[1].getKey() == 4);
}

void testScanNeitherKeyExists() {
    AVLTree tree;
    tree.insert(KVPair(1, 1));
    tree.insert(KVPair(3, 3));
    tree.insert(KVPair(4, 4));
    std::vector<KVPair> result = tree.scan(2, 5);
    assert(result.size() == 2);
    assert(result[0].getKey() == 3);
    assert(result[1].getKey() == 4);
}

void testClearTree() {
    AVLTree tree;
    tree.insert(KVPair(1, 1));
    tree.insert(KVPair(2, 2));
    tree.insert(KVPair(3, 3));
    assert(tree.getTreeSize() == 3);
    assert(tree.getRoot() != nullptr);
    tree.clearTree();
    assert(tree.getTreeSize() == 0);
    assert(tree.getRoot() == nullptr);
}

void avlTestAll() {
    cout << "RUNNING AVL TREE TESTS.." << endl << endl;
    cout << "Running AVL Tree insertion tests..." << endl;

    cout << "Running testInsertBalanced..." << endl;
    testInsertBalanced();
    cout << "testInsertBalanced passed" << endl;
    cout << "Running testInsertLeftLeft..." << endl;
    testInsertLeftLeft();
    cout << "testInsertLeftLeft passed" << endl;
    cout << "Running testInsertRightRight..." << endl;
    testInsertRightRight();
    cout << "testInsertRightRight passed" << endl;
    cout << "Running testInsertLeftRight..." << endl;
    testInsertLeftRight();
    cout << "testInsertLeftRight passed" << endl;
    cout << "Running testInsertRightLeft..." << endl;
    testInsertRightLeft();
    cout << "testInsertRightLeft passed" << endl;
    cout << "Running testComplexInsert..." << endl;
    testComplexInsert();
    cout << "testComplexInsert passed" << endl;

    cout << endl << "Running AVL Tree get tests..." << endl;
    cout << "Running testGetKeyExists..." << endl;
    testGetKeyExists();
    cout << "testGetKeyExists passed" << endl;
    cout << "Running testGetKeyDoesNotExist..." << endl;
    testGetKeyDoesNotExist();
    cout << "testGetKeyDoesNotExist passed" << endl;

    cout << endl << "Running AVL Tree scan tests..." << endl;
    cout << "Running testScanEmpty..." << endl;
    testScanEmpty();
    cout << "testScanEmpty passed" << endl;
    cout << "Running testScanBothKeysExist..." << endl;
    testScanBothKeysExist();
    cout << "testScanBothKeysExist passed" << endl;
    cout << "Running testScanStartKeyExists..." << endl;
    testScanStartKeyExists();
    cout << "testScanStartKeyExists passed" << endl;
    cout << "Running testScanEndKeyExists..." << endl;
    testScanEndKeyExists();
    cout << "testScanEndKeyExists passed" << endl;
    cout << "Running testScanNeitherKeyExists..." << endl;
    testScanNeitherKeyExists();
    cout << "testScanNeitherKeyExists passed" << endl;
    
    cout << endl << "Running AVL Tree clear tests..." << endl;
    testClearTree();
    cout << "testClearTree passed" << endl;

    cout << endl << "All AVL Tree tests passed!" << endl;
}
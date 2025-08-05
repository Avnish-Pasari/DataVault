#include "dequeTest.h"

void testDequeInitialization() {
    DoubleEndedQueue deq;
    assert(deq.getSize() == 0);
}

void testDequePushBackAndFront() {
    DoubleEndedQueue deq;
    deq.push_back("page1");

    assert(deq.front() == "page1");
    assert(deq.back() == "page1");
    assert(deq.getSize() == 1);

    deq.push_back("page2");
    deq.push_back("page3");

    assert(deq.getSize() == 3);
    assert(deq.front() == "page1");
    assert(deq.back() == "page3");
}

void testDequePopFront() {
    DoubleEndedQueue deq;
    deq.push_back("page1");
    deq.push_back("page2");
    deq.push_back("page3");

    deq.pop_front();
    assert(deq.getSize() == 2);
    assert(deq.front() == "page2");
    deq.pop_front();
    assert(deq.getSize() == 1);
    assert(deq.front() == "page3");
    deq.pop_front();
    assert(deq.getSize() == 0);
}

void testDequeErase() {
    DoubleEndedQueue deq;
    deq.push_back("page1");
    deq.push_back("page2");
    deq.push_back("page3");

    deq.erase("page2");
    assert(deq.getSize() == 2);
    assert(deq.front() == "page1");
    assert(deq.back() == "page3");

    deq.erase("page1");
    assert(deq.getSize() == 1);
    assert(deq.front() == "page3");

    deq.erase("page3");
    assert(deq.getSize() == 0);
}

void testDequeMoveToBack() {
    DoubleEndedQueue deq;
    deq.push_back("page1");
    deq.push_back("page2");
    deq.push_back("page3");

    deq.move_to_back("page2");
    assert(deq.getSize() == 3);
    assert(deq.front() == "page1");
    assert(deq.back() == "page2");
}

void dequeTestAll() {
    cout << "Running Deque tests..." << endl;
    cout << "Running testDequeInitialization..." << endl;
    testDequeInitialization();
    cout << "testDequeInitialization passed" << endl;
    cout << "Running testDequePushBackAndFront..." << endl;
    testDequePushBackAndFront();
    cout << "testDequePushBackAndFront passed" << endl;
    cout << "Running testDequePopFront..." << endl;
    testDequePopFront();
    cout << "testDequePopFront passed" << endl;
    cout << "Running testDequeErase..." << endl;
    testDequeErase();
    cout << "testDequeErase passed" << endl;
    cout << "Running testDequeMoveToBack..." << endl;
    testDequeMoveToBack();
    cout << "testDequeMoveToBack passed" << endl;
    cout << "All Deque tests passed!" << endl;
}
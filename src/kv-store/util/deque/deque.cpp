#include "deque.h"

DoubleEndedQueue::DoubleEndedQueue() : head(nullptr), tail(nullptr), size(0) {}

DoubleEndedQueue::~DoubleEndedQueue() {
    while (head != nullptr) {
        DoubleEndedQueueNode *temp = head;
        head = head->getNext();
        delete temp;
    }
}

string DoubleEndedQueue::front() {
    return head->getPageId();
}

string DoubleEndedQueue::back() {
    return tail->getPageId();
}

void DoubleEndedQueue::pop_front() {
    if (head == nullptr) {
        return;
    }

    DoubleEndedQueueNode *temp = head;
    head = head->getNext();
    if (head == nullptr) {
        tail = nullptr;
    }
    else {
        head->setPrev(nullptr);
    }
    delete temp;
    size -= 1;
}

void DoubleEndedQueue::push_back(string pageId) {
    DoubleEndedQueueNode *newNode = new DoubleEndedQueueNode(pageId);
    if (size == 0) {
        head = newNode;
    }
    else {
        tail->setNext(newNode);
        newNode->setPrev(tail);
    }
    tail = newNode;
    size += 1;
}

void DoubleEndedQueue::erase(string pageId) {
    DoubleEndedQueueNode *curr = head;
    while (curr != nullptr) {
        if (curr->getPageId() == pageId) {
            // curr is the node we want to erase
            if (curr == head) {
                pop_front();
            }
            else if (curr == tail) {
                tail = tail->getPrev();
                tail->setNext(nullptr);
                delete curr;
                size -= 1;
            }
            else {
                curr->getPrev()->setNext(curr->getNext());
                curr->getNext()->setPrev(curr->getPrev());
                delete curr;
                size -= 1;
            }
            return;
        }
        curr = curr->getNext();
    }
}

void DoubleEndedQueue::move_to_back(string pageId) {
    erase(pageId);
    push_back(pageId);
}
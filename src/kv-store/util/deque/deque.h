#ifndef DOUBLE_ENDED_QUEUE_H
#define DOUBLE_ENDED_QUEUE_H

#include "../../util/common.h"
#include "../../util/namespace-std.h"

class DoubleEndedQueueNode {
    private:
        string pageId;
        DoubleEndedQueueNode *prev;
        DoubleEndedQueueNode *next;
    public:
        DoubleEndedQueueNode(string pageId) : pageId(pageId), prev(nullptr), next(nullptr) {}

        string getPageId() { return pageId; }
        DoubleEndedQueueNode *getPrev() { return prev; }
        DoubleEndedQueueNode *getNext() { return next; }
        void setPageId(string newPageId) { pageId = newPageId; }
        void setPrev(DoubleEndedQueueNode *newPrev) { prev = newPrev; }
        void setNext(DoubleEndedQueueNode *newNext) { next = newNext; }
};

class DoubleEndedQueue {
    private:
        DoubleEndedQueueNode *head;
        DoubleEndedQueueNode *tail;
        int size;
    public:
        DoubleEndedQueue();
        ~DoubleEndedQueue();

        int getSize() { return size; }
        DoubleEndedQueueNode *getHead() { return head; }

        string front();
        string back();

        void pop_front();
        void push_back(string pageId);

        void erase(string pageId);
        void move_to_back(string pageId);
};

#endif // DOUBLE_ENDED_QUEUE_H
#include <Arduino.h>

uint8_t intensityMin = 55;
uint8_t intensityMax = 255;
uint8_t intensityStart = 155;

struct ScarfLinkedlist {
    struct Node {
        uint8_t r;
        uint8_t b;
        Node *next;

        Node() : Node(intensityStart, intensityStart) {}
        Node(uint16_t _r, uint16_t _b) : next(NULL) {
            int8_t intensityChange = random(-50, 51);
            int8_t intensityChangeRelativeRB = random(-5, 6);

            _r += intensityChange + intensityChangeRelativeRB;
            _b += intensityChange - intensityChangeRelativeRB;

            if (_r < intensityMin) {
                _r = intensityMin;
            } else if (_r > intensityMax) {
                _r = intensityMax;
            }
            if (_b < intensityMin) {
                _b = intensityMin;
            } else if (_b > intensityMax) {
                _b = intensityMax;
            }

            r = _r;
            b = _b;
        }
    };
    Node *head;
    Node *tail;
    Node *current;

    ScarfLinkedlist() : ScarfLinkedlist(0) {}
    ScarfLinkedlist(uint8_t size) : head(NULL), tail(NULL), current(NULL) {
        for (uint8_t i = 0; i < size; i++) {
            addNodeTail();
        }
    }

    void addNodeHead() {
        Node *newNode = (head == NULL) ? new Node() : new Node(head->r, head->b);
        if (head != NULL) {
            newNode->next = head;
        }
        head = newNode;
        if (tail == NULL) {
            tail = head;
        }
    }

    void addNodeTail() {
        Node *newNode = (tail == NULL) ? new Node() : new Node(tail->r, tail->b);
        if (tail != NULL) {
            tail->next = newNode;
        }
        tail = newNode;
        if (head == NULL) {
            head = tail;
        }
    }

    void resetCurrent() {
        current = head;
    }

    void nextCurrent() {
        if ((current == NULL) || (current->next == NULL)) {
            current = head;
        } else {
            current = current->next;
        }
    }

    void shiftAddNodeHead() {
        removeTail();
        addNodeHead();
    }

    void shiftAddNodeTail() {
        removeHead();
        addNodeTail();
    }

    void removeHead() {
        if (head == NULL) {
            return;
        }
        Node *temp = head;
        head = head->next;
        delete temp;
    }

    void removeTail() {
        if (tail == NULL) {
            return;
        }
        // Find tail from head
        Node *temp = head;
        while (temp->next != tail) {
            temp = temp->next;
        }
        delete tail;
        tail = temp;
        tail->next = NULL;
    }
};

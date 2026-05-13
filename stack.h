#ifndef STACK_H
#define STACK_H

#include "vehicle.h"
#include <string>
using namespace std;

// ─────────────────────────────────────────────
//  UndoEntry: one record on the undo stack
//  Remembers what action was done so it can be reversed
// ─────────────────────────────────────────────
struct UndoEntry {
    string  action;     // "add" or "process"
    string  lane;
    Vehicle vehicle;
    int     toll_paid;
};

// ─────────────────────────────────────────────
//  Stack: array-based stack for undo operations
// ─────────────────────────────────────────────
const int MAX_STACK = 50;

class Stack {
private:
    UndoEntry items[MAX_STACK];
    int top_idx;

public:
    Stack();

    bool push(const UndoEntry& e);
    bool pop(UndoEntry& out);
    bool peek(UndoEntry& out) const;

    bool isEmpty() const;
    int  size()    const;
};

#endif

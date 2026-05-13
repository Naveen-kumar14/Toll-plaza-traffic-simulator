#include "stack.h"
using namespace std;

Stack::Stack() : top_idx(-1) {}

// Push a new undo entry onto the stack
bool Stack::push(const UndoEntry& e)
{
    if (top_idx >= MAX_STACK - 1) return false;
    items[++top_idx] = e;
    return true;
}

// Pop the most recent undo entry
bool Stack::pop(UndoEntry& out) {
    if (isEmpty()) return false;
    out = items[top_idx--];
    return true;
}

// Look at top without removing
bool Stack::peek(UndoEntry& out) const {
    if (isEmpty()) return false;
    out = items[top_idx];
    return true;
}

bool Stack::isEmpty() const { return top_idx == -1; }
int  Stack::size()    const { return top_idx + 1; }

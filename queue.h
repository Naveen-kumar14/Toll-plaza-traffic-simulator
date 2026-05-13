#ifndef QUEUE_H
#define QUEUE_H

#include "vehicle.h"
#include <vector>
using namespace std;

// ─────────────────────────────────────────────
//  Queue: array-based circular queue
//  Holds up to MAX_QUEUE vehicles in one lane
// ─────────────────────────────────────────────
const int MAX_QUEUE = 10;

class Queue {
private:
    Vehicle items[MAX_QUEUE];
    int front_idx;
    int rear_idx;
    int count;

public:
    Queue();

    bool enqueue(const Vehicle& v);          // add to rear
    bool dequeue(Vehicle& out);              // remove from front
    bool peek(Vehicle& out) const;           // look at front without removing

    bool isEmpty() const;
    bool isFull()  const;
    int  size()    const;

    // Used by undo: remove a specific vehicle by plate
    bool removeByPlate(const string& plate);

    // Used by undo: put a vehicle back at the front
    bool pushFront(const Vehicle& v);

    // Returns all vehicles front-to-back as a vector (for display)
    vector<Vehicle> toVector() const;
};

#endif

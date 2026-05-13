#include "queue.h"
using namespace std;

Queue::Queue() : front_idx(0), rear_idx(-1), count(0) {}

// Add vehicle to the rear of the queue
bool Queue::enqueue(const Vehicle& v) {
    if (isFull()) return false;
    rear_idx = (rear_idx + 1) % MAX_QUEUE;
    items[rear_idx] = v;
    count++;
    return true;
}

// Remove vehicle from the front of the queue
bool Queue::dequeue(Vehicle& out) {
    if (isEmpty()) return false;
    out = items[front_idx];
    front_idx = (front_idx + 1) % MAX_QUEUE;
    count--;
    return true;
}

// Look at the front vehicle without removing it
bool Queue::peek(Vehicle& out) const {
    if (isEmpty()) return false;
    out = items[front_idx];
    return true;
}

bool Queue::isEmpty() const { return count == 0; }
bool Queue::isFull()  const { return count == MAX_QUEUE; }
int  Queue::size()    const { return count; }

// Remove a specific vehicle by plate number (used by undo-add)
// Rebuilds the queue without that vehicle
bool Queue::removeByPlate(const string& plate) {
    Vehicle temp[MAX_QUEUE];
    int new_count = 0;

    for (int i = 0; i < count; i++) {
        int idx = (front_idx + i) % MAX_QUEUE;
        if (items[idx].plate != plate) {
            temp[new_count++] = items[idx];
        }
    }

    if (new_count == count) return false;  // plate not found

    // Reload the queue from temp array
    front_idx = 0;
    rear_idx  = new_count - 1;
    count     = new_count;
    for (int i = 0; i < new_count; i++) items[i] = temp[i];
    return true;
}

// Put a vehicle back at the front (used by undo-process)
bool Queue::pushFront(const Vehicle& v) {
    if (isFull()) return false;
    front_idx = (front_idx - 1 + MAX_QUEUE) % MAX_QUEUE;
    items[front_idx] = v;
    count++;
    return true;
}

// Convert queue to a plain vector for easy iteration in display
vector<Vehicle> Queue::toVector() const {
    vector<Vehicle> result;
    for (int i = 0; i < count; i++) {
        result.push_back(items[(front_idx + i) % MAX_QUEUE]);
    }
    return result;
}

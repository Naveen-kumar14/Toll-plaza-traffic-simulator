#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
using namespace std;

// ─────────────────────────────────────────────
//  Vehicle: stores info about one vehicle
// ─────────────────────────────────────────────
struct Vehicle {
    string plate;
    string vtype;       // car | bus | truck | bike
    string lane;        // fastag | cash | emergency
    string entry_time;
    string owner_name;  // NEW: optional owner name

    Vehicle();
    Vehicle(string p, string v, string l, string owner = "");

    int    toll()     const;   // calculate toll based on type and lane
    string label()    const;   // short display label
    string typeIcon() const;   // NEW: icon for vehicle type
};

// ─────────────────────────────────────────────
//  ProcessedRecord: vehicle that has been cleared
// ─────────────────────────────────────────────
struct ProcessedRecord {
    Vehicle vehicle;
    int     toll_paid;
    string  process_time;
};

#endif

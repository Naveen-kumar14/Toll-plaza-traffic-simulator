#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "vehicle.h"
#include "queue.h"
#include "stack.h"
#include <vector>
using namespace std;

class TollPlazaSimulator {
private:
    Queue fastagLane;
    Queue cashLane;
    Queue emergencyLane;

    Stack undoStack;

    vector<ProcessedRecord> history;

    int processedFastag,    processedCash,    processedEmergency;
    int enteredFastag,      enteredCash,      enteredEmergency;
    int totalToll;
    int totalProcessed;
    int totalEntered;

    // Session tracking
    int sessionStartToll;
    int sessionStartProcessed;

    Queue& getLane(const string& name);
    int&   getProcessedCount(const string& name);
    int&   getEnteredCount(const string& name);

    void printHeader();
    void printLane(const string& name, const string& label, const char* color);
    void printAllLanes();
    void printStats();
    void printHistory(int lastN = 10);
    void printMenu();

    void processLane(const string& laneName);

    // NEW helpers
    void printTollRateCard();
    void printLaneSummaryBar();
    bool isDuplicatePlate(const string& plate);
    void exportHistoryToFile();

public:
    TollPlazaSimulator();

    void seedDemoData();

    void addVehicle();
    void processLaneInteractive();
    void processAll();
    void viewLanes();
    void viewStats();
    void viewHistory();
    void searchVehicle();
    void undoLast();
    void clearAll();

    // NEW menu actions
    void viewTollRates();
    void filterHistory();
    void exportHistory();
    void processEmergencyFirst();

    void run();
};

#endif

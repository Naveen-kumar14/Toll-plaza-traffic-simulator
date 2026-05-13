#include "simulator.h"
#include "utils.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
using namespace std;

TollPlazaSimulator::TollPlazaSimulator()
    : processedFastag(0),   processedCash(0),   processedEmergency(0),
      enteredFastag(0),     enteredCash(0),     enteredEmergency(0),
      totalToll(0), totalProcessed(0), totalEntered(0),
      sessionStartToll(0),  sessionStartProcessed(0)
{}

// ── Lane helpers ──────────────────────────────────────────────────────────────
Queue& TollPlazaSimulator::getLane(const string& name) {
    if (name == "fastag")    return fastagLane;
    if (name == "emergency") return emergencyLane;
    return cashLane;
}

int& TollPlazaSimulator::getProcessedCount(const string& name) {
    if (name == "fastag")    return processedFastag;
    if (name == "emergency") return processedEmergency;
    return processedCash;
}

int& TollPlazaSimulator::getEnteredCount(const string& name) {
    if (name == "fastag")    return enteredFastag;
    if (name == "emergency") return enteredEmergency;
    return enteredCash;
}

// ── Duplicate plate check across all queues ───────────────────────────────────
bool TollPlazaSimulator::isDuplicatePlate(const string& plate) {
    for (const string l : {"fastag", "cash", "emergency"}) {
        vector<Vehicle> vehicles = getLane(l).toVector();
        for (int i = 0; i < (int)vehicles.size(); i++)
            if (vehicles[i].plate == plate) return true;
    }
    return false;
}

// ── Header ────────────────────────────────────────────────────────────────────
void TollPlazaSimulator::printHeader() {
    clearScreen();
    cout << CYAN << BOLD;
    cout << "╔══════════════════════════════════════════════════════════════╗\n";
    cout << "║     TOLL PLAZA TRAFFIC SIMULATOR  v2.0  —  C++ DSA          ║\n";
    cout << "╚══════════════════════════════════════════════════════════════╝\n";
    cout << RESET;
    cout << DIM   << "  Entered: "    << RESET << WHITE  << BOLD << totalEntered   << RESET;
    cout << DIM   << "  Processed: "  << RESET << GREEN  << BOLD << totalProcessed << RESET;
    cout << DIM   << "  Toll: "       << RESET << YELLOW << BOLD << "Rs." << totalToll << RESET;
    cout << DIM   << "  In Queue: "   << RESET << CYAN   << BOLD
         << (fastagLane.size() + cashLane.size() + emergencyLane.size()) << RESET;
    cout << DIM   << "  Undo Stack: " << RESET << ORANGE << BOLD << undoStack.size() << RESET;
    cout << "\n";
    hr();
}

// ── Lane summary bar ──────────────────────────────────────────────────────────
void TollPlazaSimulator::printLaneSummaryBar() {
    cout << DIM << "  Lanes: " << RESET;
    cout << CYAN   << "FASTag["   << fastagLane.size()    << "] " << RESET;
    cout << ORANGE << "Cash["     << cashLane.size()      << "] " << RESET;
    cout << RED    << "Emergency["<< emergencyLane.size() << "]"  << RESET;
    cout << "\n";
    hr('-', 64);
}

// ── Single lane display ───────────────────────────────────────────────────────
void TollPlazaSimulator::printLane(const string& name, const string& label, const char* color) {
    Queue& q = getLane(name);

    // Capacity bar
    int sz  = q.size();
    int cap = MAX_QUEUE;
    cout << color << BOLD << "  " << label << RESET;
    cout << DIM   << "  [" << sz << "/" << cap << "]  ";

    // Visual fill bar
    cout << "[";
    for (int i = 0; i < cap; i++) {
        if (i < sz) cout << color << "█" << RESET;
        else        cout << DIM   << "░" << RESET;
    }
    cout << "]\n";

    if (q.isEmpty()) {
        cout << DIM << "    (empty — no vehicles)\n" << RESET;
        return;
    }

    vector<Vehicle> vehicles = q.toVector();
    for (int i = 0; i < (int)vehicles.size(); i++) {
        const Vehicle& v = vehicles[i];
        if (i == 0) cout << GREEN << "    [NEXT] " << RESET;
        else        cout << DIM   << "    [" << setw(4) << (i + 1) << "] " << RESET;

        cout << color  << v.typeIcon() << RESET << "  ";
        cout << WHITE  << BOLD << setw(12) << left << v.plate << RESET;
        cout << DIM    << "  Rs." << setw(4) << right << v.toll();
        cout << "  @" << v.entry_time;
        if (!v.owner_name.empty()) cout << "  " << CYAN << v.owner_name << RESET;
        cout << RESET  << "\n";
    }
}

// ── All three lanes ───────────────────────────────────────────────────────────
void TollPlazaSimulator::printAllLanes() {
    cout << "\n";
    printLane("fastag",    "FASTAG    LANE  (electronic)", CYAN);
    cout << "\n";
    printLane("cash",      "CASH      LANE  (manual toll)", ORANGE);
    cout << "\n";
    printLane("emergency", "EMERGENCY LANE  (free / VIP)", RED);
    cout << "\n";
}

// ── Statistics panel ──────────────────────────────────────────────────────────
void TollPlazaSimulator::printStats() {
    hr();
    cout << BOLD << "  STATISTICS\n" << RESET;
    cout << DIM  << "  ──────────────────────────────────────────────\n" << RESET;

    // Per-lane stats with efficiency %
    auto pct = [](int processed, int entered) -> string {
        if (entered == 0) return "  -  ";
        int p = (processed * 100) / entered;
        return to_string(p) + "%";
    };

    cout << "  " << CYAN   << "FASTag   " << RESET
         << " processed: " << BOLD << setw(3) << processedFastag    << RESET
         << "  entered: "  << DIM  << setw(3) << enteredFastag
         << "  efficiency: " << GREEN << pct(processedFastag, enteredFastag) << RESET << "\n";

    cout << "  " << ORANGE << "Cash     " << RESET
         << " processed: " << BOLD << setw(3) << processedCash      << RESET
         << "  entered: "  << DIM  << setw(3) << enteredCash
         << "  efficiency: " << GREEN << pct(processedCash, enteredCash) << RESET << "\n";

    cout << "  " << RED    << "Emergency" << RESET
         << " processed: " << BOLD << setw(3) << processedEmergency << RESET
         << "  entered: "  << DIM  << setw(3) << enteredEmergency
         << "  efficiency: " << GREEN << pct(processedEmergency, enteredEmergency) << RESET << "\n";

    // Busiest lane
    string busiest      = "fastag";
    int    busiestCount = enteredFastag;
    if (enteredCash      > busiestCount) { busiest = "cash";      busiestCount = enteredCash; }
    if (enteredEmergency > busiestCount) { busiest = "emergency"; busiestCount = enteredEmergency; }

    // Average toll
    double avgToll = (totalProcessed > 0) ? (double)totalToll / totalProcessed : 0;

    // Vehicle type breakdown from history
    int cars=0, buses=0, trucks=0, bikes=0;
    for (int i=0;i<(int)history.size();i++) {
        string t = history[i].vehicle.vtype;
        if (t=="car")   cars++;
        else if(t=="bus")   buses++;
        else if(t=="truck") trucks++;
        else if(t=="bike")  bikes++;
    }

    cout << DIM << "  ──────────────────────────────────────────────\n" << RESET;
    cout << "  Busiest lane     : " << YELLOW << BOLD << toUpper(busiest)
         << " (" << busiestCount << " vehicles)\n" << RESET;
    cout << "  Total toll       : " << YELLOW << BOLD << "Rs." << totalToll     << RESET << "\n";
    cout << "  Average toll     : " << YELLOW << BOLD << "Rs." << fixed << setprecision(0) << avgToll << RESET << "\n";
    cout << "  Currently waiting: " << WHITE
         << (fastagLane.size() + cashLane.size() + emergencyLane.size()) << RESET << "\n";
    cout << DIM << "  ──────────────────────────────────────────────\n" << RESET;
    cout << "  Vehicle mix      :";
    if (cars)   cout << CYAN   << "  Car:"   << cars   << RESET;
    if (buses)  cout << ORANGE << "  Bus:"   << buses  << RESET;
    if (trucks) cout << RED    << "  Truck:" << trucks << RESET;
    if (bikes)  cout << GREEN  << "  Bike:"  << bikes  << RESET;
    if (!cars && !buses && !trucks && !bikes) cout << DIM << "  (no data)" << RESET;
    cout << "\n";
    hr();
}

// ── History table ─────────────────────────────────────────────────────────────
void TollPlazaSimulator::printHistory(int lastN) {
    hr();
    cout << BOLD << "  PROCESSED VEHICLE HISTORY (last " << lastN << ")\n" << RESET;
    hr('-', 64);

    if (history.empty()) {
        cout << DIM << "  No vehicles processed yet.\n" << RESET;
        hr();
        return;
    }

    cout << DIM << left
         << setw(14) << "  PLATE"
         << setw(10) << "TYPE"
         << setw(12) << "LANE"
         << setw(10) << "TOLL"
         << setw(10) << "TIME"
         << RESET << "\n";
    hr('-', 64);

    int start = max(0, (int)history.size() - lastN);
    for (int i = (int)history.size() - 1; i >= start; i--) {
        const ProcessedRecord& r = history[i];
        string laneColor = (r.vehicle.lane=="fastag") ? CYAN :
                           (r.vehicle.lane=="cash")   ? ORANGE : RED;
        cout << "  " << WHITE << BOLD << left << setw(12) << r.vehicle.plate << RESET;
        cout << laneColor << setw(10) << r.vehicle.vtype << RESET;
        cout << DIM  << setw(12) << r.vehicle.lane << RESET;
        if (r.toll_paid > 0)
            cout << GREEN << "Rs." << setw(7) << r.toll_paid << RESET;
        else
            cout << CYAN  << setw(10) << "FREE" << RESET;
        cout << DIM << r.process_time << RESET << "\n";
    }
    hr();
}

// ── Toll rate card ────────────────────────────────────────────────────────────
void TollPlazaSimulator::printTollRateCard() {
    hr();
    cout << BOLD << "  TOLL RATE CARD\n" << RESET;
    hr('-', 64);
    cout << DIM << left
         << setw(14) << "  VEHICLE"
         << setw(16) << "FASTAG (Rs.)"
         << setw(16) << "CASH (Rs.)"
         << "EMERGENCY\n" << RESET;
    hr('-', 64);

    struct Rate { string type; int fastag; int cash; };
    Rate rates[] = {
        {"Car",   65,  90},
        {"Bus",   130, 180},
        {"Truck", 160, 215},
        {"Bike",  35,  50},
    };

    for (auto& r : rates) {
        cout << "  " << WHITE << BOLD << left << setw(12) << r.type << RESET;
        cout << CYAN   << setw(16) << r.fastag << RESET;
        cout << ORANGE << setw(16) << r.cash   << RESET;
        cout << GREEN  << "FREE\n" << RESET;
    }
    hr();
    cout << DIM << "  Tip: FASTag saves up to 26% vs cash!\n" << RESET;
    hr();
}

// ── Main menu ─────────────────────────────────────────────────────────────────
void TollPlazaSimulator::printMenu() {
    cout << "\n" << BOLD << "  MENU\n" << RESET;
    hr('-', 64);
    cout << "  " << CYAN   << "[1]"  << RESET << " Add vehicle\n";
    cout << "  " << GREEN  << "[2]"  << RESET << " Process next vehicle from a lane\n";
    cout << "  " << GREEN  << "[3]"  << RESET << " Process ALL lanes (emergency first)\n";
    cout << "  " << RED    << "[4]"  << RESET << " Process emergency lane priority\n";
    cout << "  " << YELLOW << "[5]"  << RESET << " View all lanes\n";
    cout << "  " << YELLOW << "[6]"  << RESET << " View statistics\n";
    cout << "  " << YELLOW << "[7]"  << RESET << " View processed history\n";
    cout << "  " << YELLOW << "[8]"  << RESET << " Filter history by type/lane\n";
    cout << "  " << WHITE  << "[9]"  << RESET << " Search vehicle by plate\n";
    cout << "  " << WHITE  << "[10]" << RESET << " View toll rate card\n";
    cout << "  " << ORANGE << "[11]" << RESET << " Undo last action\n";
    cout << "  " << BLUE   << "[12]" << RESET << " Export history to file\n";
    cout << "  " << RED    << "[13]" << RESET << " Clear all lanes\n";
    cout << "  " << RED    << "[0]"  << RESET << " Exit\n";
    hr('-', 64);
    cout << "  Enter choice: ";
}

// ── Seed demo data ────────────────────────────────────────────────────────────
void TollPlazaSimulator::seedDemoData() {
    struct Seed { string plate, vtype, lane, owner; };
    Seed seeds[] = {
        {"TN09AB1234", "car",   "fastag",    "Naveen"},
        {"MH12CD5678", "bus",   "cash",      "Akash"},
        {"DL01EF9012", "truck", "cash",      "Ravi"},
        {"KA05GH3456", "bike",  "fastag",    "Priya"},
        {"UP32IJ7890", "car",   "emergency", "Ambulance"},
        {"GJ03KL2211", "car",   "fastag",    "Suresh"},
        {"RJ14MN4455", "truck", "cash",      "Logistics Co"},
    };

    for (auto& s : seeds) {
        Vehicle v(s.plate, s.vtype, s.lane, s.owner);
        getLane(s.lane).enqueue(v);
        getEnteredCount(s.lane)++;
        totalEntered++;

        UndoEntry ue;
        ue.action    = "add";
        ue.lane      = s.lane;
        ue.vehicle   = v;
        ue.toll_paid = 0;
        undoStack.push(ue);
    }
}

// ── ACTION 1: Add vehicle ─────────────────────────────────────────────────────
void TollPlazaSimulator::addVehicle() {
    printHeader();
    cout << BOLD << "\n  ADD VEHICLE\n" << RESET;
    printLaneSummaryBar();

    string plate, vtype, lane, owner;

    cout << "  Number plate (Enter = random): ";
    getline(cin, plate);

    if (plate.empty() || plate == " ") plate = genPlate();
    plate.erase(remove(plate.begin(), plate.end(), ' '), plate.end());
    plate = toUpper(plate);

    // Duplicate check
    if (isDuplicatePlate(plate)) {
        cout << RED << BOLD << "\n  ERROR: " << plate << " is already waiting in a lane!\n" << RESET;
        cout << "  Press Enter to continue...";
        string dummy; getline(cin, dummy);
        return;
    }

    cout << "  Vehicle type  [ car / bus / truck / bike ]: ";
    getline(cin, vtype);
    vtype = toLower(vtype);
    if (!isValidVtype(vtype)) {
        cout << RED << "  Invalid type. Defaulting to 'car'.\n" << RESET;
        vtype = "car";
    }

    cout << "  Lane          [ fastag / cash / emergency ]: ";
    getline(cin, lane);
    lane = toLower(lane);
    if (!isValidLane(lane)) {
        cout << RED << "  Invalid lane. Defaulting to 'fastag'.\n" << RESET;
        lane = "fastag";
    }

    cout << "  Owner name    (optional, Enter to skip): ";
    getline(cin, owner);

    Queue& q = getLane(lane);
    if (q.isFull()) {
        cout << RED << BOLD << "\n  ERROR: " << toUpper(lane)
             << " lane is FULL (max " << MAX_QUEUE << "). Try another lane.\n" << RESET;
        cout << "  Press Enter to continue...";
        string dummy; getline(cin, dummy);
        return;
    }

    Vehicle v(plate, vtype, lane, owner);
    q.enqueue(v);
    getEnteredCount(lane)++;
    totalEntered++;

    UndoEntry ue;
    ue.action    = "add";
    ue.lane      = lane;
    ue.vehicle   = v;
    ue.toll_paid = 0;
    undoStack.push(ue);

    cout << GREEN << "\n  ✔ Vehicle " << WHITE << BOLD << plate << RESET
         << GREEN << " added to " << toUpper(lane) << " lane.\n" << RESET;
    cout << DIM   << "  Toll when processed: " << RESET
         << YELLOW << BOLD << "Rs." << v.toll() << RESET << "\n";
    if (!owner.empty())
        cout << DIM << "  Owner: " << RESET << CYAN << owner << RESET << "\n";

    cout << "\n  Press Enter to continue...";
    string dummy; getline(cin, dummy);
}

// ── Internal: process one vehicle ────────────────────────────────────────────
void TollPlazaSimulator::processLane(const string& laneName) {
    Queue& q = getLane(laneName);
    if (q.isEmpty()) {
        cout << RED << "  " << toUpper(laneName) << " lane is empty.\n" << RESET;
        return;
    }

    Vehicle v;
    q.dequeue(v);
    int toll = v.toll();

    totalToll      += toll;
    totalProcessed++;
    getProcessedCount(laneName)++;

    ProcessedRecord rec;
    rec.vehicle      = v;
    rec.toll_paid    = toll;
    rec.process_time = currentTimeStr();
    history.push_back(rec);

    UndoEntry ue;
    ue.action    = "process";
    ue.lane      = laneName;
    ue.vehicle   = v;
    ue.toll_paid = toll;
    undoStack.push(ue);

    string laneColor = (laneName=="fastag") ? CYAN : (laneName=="cash") ? ORANGE : RED;
    cout << GREEN << "  ✔ Processed: " << RESET
         << laneColor << BOLD << v.plate << RESET
         << DIM   << "  (" << v.vtype << ")"
         << "  Lane: " << toUpper(laneName) << RESET;
    if (toll > 0) cout << YELLOW << "  Toll: Rs." << toll << RESET;
    else          cout << CYAN   << "  Toll: FREE" << RESET;
    if (!v.owner_name.empty()) cout << DIM << "  [" << v.owner_name << "]" << RESET;
    cout << "\n";
}

// ── ACTION 2: Process lane interactive ───────────────────────────────────────
void TollPlazaSimulator::processLaneInteractive() {
    printHeader();
    cout << BOLD << "\n  PROCESS VEHICLE\n" << RESET;
    printLaneSummaryBar();
    cout << "  Which lane? [ fastag / cash / emergency ]: ";
    string lane;
    getline(cin, lane);
    lane = toLower(lane);

    if (!isValidLane(lane))
        cout << RED << "  Invalid lane name.\n" << RESET;
    else
        processLane(lane);

    cout << "\n  Press Enter to continue...";
    string dummy; getline(cin, dummy);
}

// ── ACTION 3: Process all lanes ───────────────────────────────────────────────
void TollPlazaSimulator::processAll() {
    printHeader();
    cout << BOLD << "\n  PROCESS ALL LANES  (Emergency → FASTag → Cash)\n" << RESET;
    hr('-', 64);

    int processed = 0;
    for (const string l : {"emergency", "fastag", "cash"}) {
        if (!getLane(l).isEmpty()) { processLane(l); processed++; }
        else cout << DIM << "  " << toUpper(l) << " lane: empty\n" << RESET;
    }

    if (processed == 0) cout << YELLOW << "\n  All lanes are empty!\n" << RESET;

    cout << "\n  Press Enter to continue...";
    string dummy; getline(cin, dummy);
}

// ── ACTION 4: Process emergency first ────────────────────────────────────────
void TollPlazaSimulator::processEmergencyFirst() {
    printHeader();
    cout << BOLD << RED << "\n  EMERGENCY PRIORITY PROCESSING\n" << RESET;
    hr('-', 64);

    if (emergencyLane.isEmpty()) {
        cout << YELLOW << "  Emergency lane is empty. Processing FASTag next.\n" << RESET;
        if (!fastagLane.isEmpty()) processLane("fastag");
        else if (!cashLane.isEmpty()) processLane("cash");
        else cout << RED << "  All lanes empty!\n" << RESET;
    } else {
        int count = emergencyLane.size();
        cout << RED << BOLD << "  Processing ALL " << count << " emergency vehicle(s) first!\n" << RESET;
        while (!emergencyLane.isEmpty()) processLane("emergency");
        cout << GREEN << "\n  Emergency lane cleared. Processing one from each remaining lane.\n" << RESET;
        if (!fastagLane.isEmpty()) processLane("fastag");
        if (!cashLane.isEmpty())   processLane("cash");
    }

    cout << "\n  Press Enter to continue...";
    string dummy; getline(cin, dummy);
}

// ── ACTION 5: View lanes ──────────────────────────────────────────────────────
void TollPlazaSimulator::viewLanes() {
    printHeader();
    printAllLanes();
    cout << "  Press Enter to continue...";
    string dummy; getline(cin, dummy);
}

// ── ACTION 6: View stats ──────────────────────────────────────────────────────
void TollPlazaSimulator::viewStats() {
    printHeader();
    printAllLanes();
    printStats();
    cout << "  Press Enter to continue...";
    string dummy; getline(cin, dummy);
}

// ── ACTION 7: View history ────────────────────────────────────────────────────
void TollPlazaSimulator::viewHistory() {
    printHeader();
    cout << "  How many recent records? (default 10): ";
    string s;
    getline(cin, s);
    int n = 10;
    if (!s.empty()) { try { n = stoi(s); } catch (...) { n = 10; } }
    printHistory(n);
    cout << "  Press Enter to continue...";
    string dummy; getline(cin, dummy);
}

// ── ACTION 8: Filter history ──────────────────────────────────────────────────
void TollPlazaSimulator::filterHistory() {
    printHeader();
    cout << BOLD << "\n  FILTER HISTORY\n" << RESET;
    hr('-', 64);
    cout << "  Filter by [ type / lane ]: ";
    string mode;
    getline(cin, mode);
    mode = toLower(mode);

    string filterVal;
    if (mode == "type") {
        cout << "  Enter type [ car / bus / truck / bike ]: ";
        getline(cin, filterVal);
        filterVal = toLower(filterVal);
    } else if (mode == "lane") {
        cout << "  Enter lane [ fastag / cash / emergency ]: ";
        getline(cin, filterVal);
        filterVal = toLower(filterVal);
    } else {
        cout << RED << "  Invalid filter mode.\n" << RESET;
        cout << "\n  Press Enter to continue...";
        string dummy; getline(cin, dummy);
        return;
    }

    hr('-', 64);
    cout << DIM << left
         << setw(14) << "  PLATE"
         << setw(10) << "TYPE"
         << setw(12) << "LANE"
         << setw(10) << "TOLL"
         << "TIME\n" << RESET;
    hr('-', 64);

    int found = 0;
    for (int i = (int)history.size() - 1; i >= 0; i--) {
        const ProcessedRecord& r = history[i];
        bool match = (mode == "type" && r.vehicle.vtype == filterVal) ||
                     (mode == "lane" && r.vehicle.lane  == filterVal);
        if (match) {
            found++;
            cout << "  " << WHITE << BOLD << left << setw(12) << r.vehicle.plate << RESET;
            cout << DIM  << setw(10) << r.vehicle.vtype << setw(12) << r.vehicle.lane << RESET;
            if (r.toll_paid > 0) cout << GREEN  << "Rs." << setw(7) << r.toll_paid << RESET;
            else                  cout << CYAN   << setw(10) << "FREE" << RESET;
            cout << DIM << r.process_time << RESET << "\n";
        }
    }

    if (found == 0) cout << YELLOW << "  No records match the filter.\n" << RESET;
    else            cout << GREEN  << "\n  " << found << " record(s) found.\n" << RESET;

    hr();
    cout << "  Press Enter to continue...";
    string dummy; getline(cin, dummy);
}

// ── ACTION 9: Search vehicle ──────────────────────────────────────────────────
void TollPlazaSimulator::searchVehicle() {
    printHeader();
    cout << BOLD << "\n  SEARCH VEHICLE BY PLATE\n" << RESET;
    hr('-', 64);
    cout << "  Enter plate number: ";
    string plate;
    getline(cin, plate);
    plate = toUpper(plate);
    plate.erase(remove(plate.begin(), plate.end(), ' '), plate.end());

    bool found = false;

    for (const string l : {"fastag", "cash", "emergency"}) {
        vector<Vehicle> vehicles = getLane(l).toVector();
        for (int i = 0; i < (int)vehicles.size(); i++) {
            if (vehicles[i].plate == plate) {
                cout << GREEN << "\n  FOUND in " << toUpper(l)
                     << " lane (position " << (i + 1) << "):\n" << RESET;
                cout << "    Plate  : " << WHITE << BOLD << vehicles[i].plate << RESET << "\n";
                cout << "    Type   : " << vehicles[i].typeIcon() << "\n";
                cout << "    Toll   : " << YELLOW << "Rs." << vehicles[i].toll() << RESET << "\n";
                cout << "    Entered: " << DIM << vehicles[i].entry_time << RESET << "\n";
                if (!vehicles[i].owner_name.empty())
                    cout << "    Owner  : " << CYAN << vehicles[i].owner_name << RESET << "\n";
                found = true;
            }
        }
    }

    for (int i = (int)history.size() - 1; i >= 0; i--) {
        if (history[i].vehicle.plate == plate) {
            cout << YELLOW << "\n  FOUND in PROCESSED HISTORY:\n" << RESET;
            cout << "    Plate     : " << WHITE << BOLD << history[i].vehicle.plate  << RESET << "\n";
            cout << "    Type      : " << history[i].vehicle.typeIcon() << "\n";
            cout << "    Lane      : " << history[i].vehicle.lane    << "\n";
            cout << "    Toll Paid : " << YELLOW << "Rs." << history[i].toll_paid << RESET << "\n";
            cout << "    Processed : " << DIM << history[i].process_time << RESET << "\n";
            if (!history[i].vehicle.owner_name.empty())
                cout << "    Owner     : " << CYAN << history[i].vehicle.owner_name << RESET << "\n";
            found = true;
        }
    }

    if (!found) cout << RED << "\n  Vehicle " << plate << " not found anywhere.\n" << RESET;

    cout << "\n  Press Enter to continue...";
    string dummy; getline(cin, dummy);
}

// ── ACTION 10: Toll rate card ─────────────────────────────────────────────────
void TollPlazaSimulator::viewTollRates() {
    printHeader();
    printTollRateCard();
    cout << "  Press Enter to continue...";
    string dummy; getline(cin, dummy);
}

// ── ACTION 11: Undo ───────────────────────────────────────────────────────────
void TollPlazaSimulator::undoLast() {
    printHeader();
    cout << BOLD << "\n  UNDO LAST ACTION\n" << RESET;
    hr('-', 64);

    if (undoStack.isEmpty()) {
        cout << RED << "  Nothing to undo.\n" << RESET;
        cout << "\n  Press Enter to continue...";
        string dummy; getline(cin, dummy);
        return;
    }

    UndoEntry ue;
    undoStack.pop(ue);

    if (ue.action == "add") {
        bool removed = getLane(ue.lane).removeByPlate(ue.vehicle.plate);
        if (removed) {
            getEnteredCount(ue.lane)--;
            totalEntered--;
            cout << YELLOW << "  UNDO: Removed " << WHITE << BOLD << ue.vehicle.plate
                 << RESET << YELLOW << " from " << toUpper(ue.lane) << " lane.\n" << RESET;
        } else {
            cout << RED << "  Undo failed — vehicle may have already been processed.\n" << RESET;
        }
    } else if (ue.action == "process") {
        bool ok = getLane(ue.lane).pushFront(ue.vehicle);
        if (ok) {
            totalToll      -= ue.toll_paid;
            totalProcessed--;
            getProcessedCount(ue.lane)--;
            if (!history.empty()) history.pop_back();
            cout << YELLOW << "  UNDO: Restored " << WHITE << BOLD << ue.vehicle.plate
                 << RESET << YELLOW << " to front of " << toUpper(ue.lane) << " lane.\n" << RESET;
        } else {
            cout << RED << "  Undo failed — lane is full.\n" << RESET;
        }
    }

    cout << "\n  Press Enter to continue...";
    string dummy; getline(cin, dummy);
}

// ── ACTION 12: Export history ─────────────────────────────────────────────────
void TollPlazaSimulator::exportHistory() {
    printHeader();
    cout << BOLD << "\n  EXPORT HISTORY TO FILE\n" << RESET;
    hr('-', 64);

    if (history.empty()) {
        cout << YELLOW << "  No processed records to export.\n" << RESET;
        cout << "\n  Press Enter to continue...";
        string dummy; getline(cin, dummy);
        return;
    }

    string filename = "toll_history_" + currentTimeStr() + ".csv";
    // Replace colons with underscores for valid filename
    for (char& c : filename) if (c == ':') c = '-';

    ofstream f(filename);
    if (!f.is_open()) {
        cout << RED << "  Error: Could not create file.\n" << RESET;
        cout << "\n  Press Enter to continue...";
        string dummy; getline(cin, dummy);
        return;
    }

    f << "plate,type,lane,toll_paid,process_time,owner\n";
    for (int i=0; i<(int)history.size(); i++) {
        const ProcessedRecord& r = history[i];
        f << r.vehicle.plate     << ","
          << r.vehicle.vtype     << ","
          << r.vehicle.lane      << ","
          << r.toll_paid         << ","
          << r.process_time      << ","
          << r.vehicle.owner_name<< "\n";
    }
    f.close();

    cout << GREEN << "  ✔ Exported " << history.size()
         << " records to " << WHITE << BOLD << filename << RESET << "\n";

    cout << "\n  Press Enter to continue...";
    string dummy; getline(cin, dummy);
}

// ── ACTION 13: Clear all ──────────────────────────────────────────────────────
void TollPlazaSimulator::clearAll() {
    printHeader();
    cout << BOLD << "\n  CLEAR ALL LANES\n" << RESET;
    hr('-', 64);
    cout << RED << "  WARNING: This will empty all 3 lanes and reset all stats.\n" << RESET;
    cout << "  Are you sure? [y/N]: ";
    string ans;
    getline(cin, ans);

    if (ans != "y" && ans != "Y") {
        cout << DIM << "  Cancelled.\n" << RESET;
        cout << "\n  Press Enter to continue...";
        string dummy; getline(cin, dummy);
        return;
    }

    for (const string l : {"fastag", "cash", "emergency"}) {
        Queue& q = getLane(l);
        Vehicle tmp;
        while (!q.isEmpty()) q.dequeue(tmp);
    }

    processedFastag = processedCash = processedEmergency = 0;
    enteredFastag   = enteredCash   = enteredEmergency   = 0;
    totalToll = totalProcessed = totalEntered = 0;
    history.clear();

    UndoEntry tmp;
    while (!undoStack.isEmpty()) undoStack.pop(tmp);

    cout << GREEN << "  ✔ All lanes cleared successfully.\n" << RESET;
    cout << "\n  Press Enter to continue...";
    string dummy; getline(cin, dummy);
}

// ── Main event loop ───────────────────────────────────────────────────────────
void TollPlazaSimulator::run() {
    while (true) {
        printHeader();
        printAllLanes();
        printMenu();

        string choice;
        getline(cin, choice);

        if      (choice == "1")  addVehicle();
        else if (choice == "2")  processLaneInteractive();
        else if (choice == "3")  processAll();
        else if (choice == "4")  processEmergencyFirst();
        else if (choice == "5")  viewLanes();
        else if (choice == "6")  viewStats();
        else if (choice == "7")  viewHistory();
        else if (choice == "8")  filterHistory();
        else if (choice == "9")  searchVehicle();
        else if (choice == "10") viewTollRates();
        else if (choice == "11") undoLast();
        else if (choice == "12") exportHistory();
        else if (choice == "13") clearAll();
        else if (choice == "0") {
            clearScreen();
            cout << CYAN << BOLD << "\n  Thank you for using Toll Plaza Simulator v2.0!\n" << RESET;
            cout << DIM  << "  Final Toll Collected : Rs." << totalToll
                 << "  |  Vehicles Processed : " << totalProcessed << "\n\n" << RESET;
            break;
        } else {
            cout << RED << "  Invalid choice. Press Enter...\n" << RESET;
            string d; getline(cin, d);
        }
    }
}

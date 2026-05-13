#include "utils.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <ctime>
#include <cstdlib>
using namespace std;

// Clear the terminal screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Print a horizontal divider line
void hr(char ch, int width) {
    cout << DIM;
    for (int i = 0; i < width; i++) cout << ch;
    cout << RESET << "\n";
}

// Convert string to all uppercase
string toUpper(string s) {
    transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

// Convert string to all lowercase
string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

// Check if vehicle type is one of the allowed values
bool isValidVtype(const string& v) {
    return v == "car" || v == "bus" || v == "truck" || v == "bike";
}

// Check if lane name is one of the allowed values
bool isValidLane(const string& l) {
    return l == "fastag" || l == "cash" || l == "emergency";
}

// Generate a random Indian-style number plate  e.g. TN09AB1234
string genPlate() {
    string states[] = {"TN", "MH", "DL", "KA", "UP", "GJ", "RJ", "AP"};
    string letters  = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string st = states[rand() % 8];
    string n1 = to_string(10 + rand() % 89);
    char   l1 = letters[rand() % 26];
    char   l2 = letters[rand() % 26];
    int    n2 = 1000 + rand() % 8999;
    ostringstream oss;
    oss << st << n1 << l1 << l2 << n2;
    return oss.str();
}

// Get current wall-clock time as a string
string currentTimeStr() {
    time_t now = time(0);
    char buf[10];
    strftime(buf, sizeof(buf), "%H:%M:%S", localtime(&now));
    return string(buf);
}

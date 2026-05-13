#ifndef UTILS_H
#define UTILS_H

#include <string>
using namespace std;

// ─────────────────────────────────────────────
//  ANSI colour codes (works on Linux/Mac/Win10+)
// ─────────────────────────────────────────────
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define DIM     "\033[2m"
#define CYAN    "\033[96m"
#define GREEN   "\033[92m"
#define YELLOW  "\033[93m"
#define RED     "\033[91m"
#define WHITE   "\033[97m"
#define ORANGE  "\033[38;5;214m"
#define BLUE    "\033[94m"

// ─────────────────────────────────────────────
//  Utility functions used across multiple files
// ─────────────────────────────────────────────
void   clearScreen();
void   hr(char ch = '-', int width = 64);

string toUpper(string s);
string toLower(string s);

bool   isValidVtype(const string& v);
bool   isValidLane(const string& l);

string genPlate();          // random number plate generator
string currentTimeStr();    // returns current time as HH:MM:SS

#endif

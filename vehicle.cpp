#include "vehicle.h"
#include <ctime>
#include <string>
using namespace std;

Vehicle::Vehicle() {
    plate      = "";
    vtype      = "";
    lane       = "";
    entry_time = "";
    owner_name = "";
}

Vehicle::Vehicle(string p, string v, string l, string owner)
    : plate(p), vtype(v), lane(l), owner_name(owner)
{
    time_t now = time(0);
    char buf[10];
    strftime(buf, sizeof(buf), "%H:%M:%S", localtime(&now));
    entry_time = string(buf);
}

// Toll rate table — Emergency lane always free
int Vehicle::toll() const {
    if (lane == "emergency") return 0;
    if (vtype == "car")   { return (lane == "fastag") ? 65  : 90;  }
    if (vtype == "bus")   { return (lane == "fastag") ? 130 : 180; }
    if (vtype == "truck") { return (lane == "fastag") ? 160 : 215; }
    if (vtype == "bike")  { return (lane == "fastag") ? 35  : 50;  }
    return 0;
}

string Vehicle::label() const {
    return "[" + plate + "  " + vtype + "  " + lane + "]";
}

string Vehicle::typeIcon() const {
    if (vtype == "car")   return "[CAR]  ";
    if (vtype == "bus")   return "[BUS]  ";
    if (vtype == "truck") return "[TRUCK]";
    if (vtype == "bike")  return "[BIKE] ";
    return "[???]  ";
}

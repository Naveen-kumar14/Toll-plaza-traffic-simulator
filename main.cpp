#include "simulator.h"
#include <cstdlib>
#include <ctime>

int main()
{

    // Enable ANSI escape codes on Windows 10+
#ifdef _WIN32
    system("chcp 65001 > nul");
    system("");
#endif

    srand((unsigned)time(0));   // seed random number generator for plate generation

    TollPlazaSimulator sim;
    sim.seedDemoData();         // pre-load 5 demo vehicles so menu is not empty on first run
    sim.run();                  // hand control to the simulator event loop

    return 0;
}

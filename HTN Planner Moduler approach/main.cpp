
#include "Simulation.h"
#include <iostream>

int Aircraft::nextID = 1;

int SDL_main(int argc, char* argv[]) {
    try {
        Simulation& simulation = Simulation::get_instance();
        simulation.run();
    }
    catch (const std::exception& e) {
        std::cerr << "C++ Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

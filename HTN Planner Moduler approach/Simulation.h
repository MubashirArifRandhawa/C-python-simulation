#ifndef SIMULATION_H
#define SIMULATION_H

#include <memory>
#include <vector>
#include <string>
#include <SDL.h>
#include "Aircraft.h"
#include <atomic>
#include "CoordinateSystem.h"

#include <pybind11/embed.h>

namespace py = pybind11;

class Simulation {
public:
    Simulation(const Simulation&) = delete;
    Simulation& operator=(const Simulation&) = delete;

    static Simulation& get_instance(); // Returns a reference to the singleton

    void add_aircraft(const std::string& name, const std::string& force, int health, float x, float y, CoordinateSystem& coordSystem);
    const std::vector<Aircraft>& get_aircrafts() const;
    std::vector<Aircraft>& get_aircrafts_mutable();
    void run();
    bool is_quit() const { return quit; }
    bool is_running() const; // New method to check simulation state
    ~Simulation(); // Make sure the destructor is public
    void simulation_update(std::vector<Aircraft>& aircrafts, SDL_Renderer* renderer);
    void initialize();

private:
    Simulation(); // Keep the constructor private

    SDL_Window* window;
    SDL_Renderer* renderer;
    bool quit;
    std::vector<Aircraft> aircrafts;
    std::atomic<bool> running; // Tracks whether the simulation is running
    static std::unique_ptr<Simulation> instance; // Use unique_ptr for automatic memory management

    CoordinateSystem coordSystem;

    //################################ python ################################
    bool is_initialized = false;
    // Initializes Python interpreter
    py::scoped_interpreter guard{}; 
    // Import Python script
    py::module behavior_module = py::module::import("aircraft_behavior");

    //################################ python ################################
};

    

#endif

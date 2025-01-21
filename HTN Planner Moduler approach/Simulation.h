#ifndef SIMULATION_H
#define SIMULATION_H

#include <memory>
#include <vector>
#include <string>
//#include <SDL.h>
//#include <SDL_image.h>
#include "Aircraft.h"
#include "Waypoint.h"
#include <atomic>
#include "CoordinateSystem.h"
#include <iostream>
#include "Button.h"
#include "enums.h"


#include <pybind11/embed.h>

namespace py = pybind11;




class Simulation {
public:
    Simulation(const Simulation&) = delete;
    Simulation& operator=(const Simulation&) = delete;

    static Simulation& get_instance(); // Returns a reference to the singleton

    void render_aircrafts(std::string color);
    void add_aircraft(const std::string& name, const std::string& force, int health, float x, float y, float heading, float speed, CoordinateSystem& coordSystem);
    const std::vector<Aircraft>& get_aircrafts() const;
    const std::vector<Waypoint>& get_waypoints() const;
    std::vector<Aircraft>& get_aircrafts_mutable();
    void run();
    bool is_quit() const { return quit; }
    bool is_running() const; // New method to check simulation state
    ~Simulation(); // Make sure the destructor is public
    void simulation_update(std::vector<Aircraft>& aircrafts, SDL_Renderer* renderer);
    void initialize();
    void handleMouseClick(int x, int y);
    void handleMouseEvent(const SDL_Event& e);
    void handleMouseWheel(SDL_Event& e);

    void render_single_aircraft(std::string color);
    void render_single_aircraft(std::string color, int x, int y);
    void onButtonclick(std::string color);
    void render_aircraft_preview(const std::string& color, int x, int y);
    void add_waypoint(const std::string& name, const std::string& force, float x, float y, CoordinateSystem& coordSystem);
    void render_waypoint(std::string color, int x, int y);
    std::string simulationObjectTypeToString(SimulationObjectType type);

private:
    Simulation(); // Keep the constructor private

    SDL_Window* window;
    SDL_Renderer* renderer;
    bool quit;
    std::vector<Button> buttons;

    std::string selectedAircraft;
    std::string selectedWaypoint;

    std::vector<Aircraft> aircrafts;
    std::vector<Waypoint> waypoints;


    std::atomic<bool> running; // Tracks whether the simulation is running
    static std::unique_ptr<Simulation> instance; // Use unique_ptr for automatic memory management
    SimulationObjectType deployMode;  // Flag to track if we are in deploy mode
    float angle = 0.0f;

    void initialize_deploy() { deployMode = SimulationObjectType::Unknown; selectedAircraft = ""; selectedWaypoint=""; }

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

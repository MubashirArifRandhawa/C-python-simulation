#include "Simulation.h"
#include <iostream>
#include <stdexcept>
#include <atomic> // For thread-safe running state



// Initialize the static instance to nullptr
std::once_flag flag;
std::unique_ptr<Simulation> Simulation::instance = nullptr;

Simulation::Simulation()
    : coordSystem(-90.0f, 90.0f, -180.0f, 180.0f, 1067, 600), window(nullptr), renderer(nullptr), quit(false), running(false) { // Initialize running to false

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
    }

    window = SDL_CreateWindow("Aircraft Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1067, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Quit();
        throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
    }

    // Aircrafts ceated
    render_aircrafts();
    //add_aircraft("Fighter1", "Blue", 100, 70.0f, 60.0f, 270.0f, 0.25f, coordSystem);
    //add_aircraft("Fighter2", "Blue", 100, 60.0f, 60.0f, 270.0f, 0.25f, coordSystem);
    //add_aircraft("Fighter3", "Blue", 100, 50.0f, 60.0f, 270.0f, 0.25f, coordSystem);

    //add_aircraft("Bomber1", "Red", 100, -60.0f, -60.0f, 90.0f, 0.25f, coordSystem);
    //add_aircraft("Bomber2", "Red", 100, -50.0f, -60.0f, 90.0f, 0.25f, coordSystem);
    //add_aircraft("Bomber3", "Red", 100, -40.0f, -60.0f, 90.0f, 0.25f, coordSystem);

    //################################ python ################################

    // Pass the simulation object to Python
    py::object py_sim = py::cast(this);
    behavior_module.attr("set_simulation")(py_sim);

    //################################ python ################################
}

Simulation::~Simulation() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}



Simulation& Simulation::get_instance() {
    std::call_once(flag, []() {
        instance = std::unique_ptr<Simulation>(new Simulation());
        });
    return *instance;
}

void Simulation::render_aircrafts() {
    int grid_size = 5; // 7x7 grid
    float spacing = 15.0f; // Distance between aircraft in the grid

    // Red team: Top-left corner
    float red_start_lat = 90.0f-20;  // Top of the map
    float red_start_lon = -180.0f+30; // Left of the map

    // Blue team: Bottom-right corner
    float blue_start_lat = -90.0f+20; // Bottom of the map
    float blue_start_lon = 180.0f-30; // Right of the map

    // Render Red team
    for (int row = 0; row < grid_size; ++row) {
        for (int col = 0; col < grid_size; ++col) {
            float lat = red_start_lat - row * spacing; // Move downward
            float lon = red_start_lon + col * spacing; // Move rightward
            std::string name = "RedAircraft" + std::to_string(row * grid_size + col + 1);
            add_aircraft(name, "Red", 100, lat, lon, 90.0f, 0.25f, coordSystem);
        }
    }

    // Render Blue team
    for (int row = 0; row < grid_size; ++row) {
        for (int col = 0; col < grid_size; ++col) {
            float lat = blue_start_lat + row * spacing; // Move upward
            float lon = blue_start_lon - col * spacing; // Move leftward
            std::string name = "BlueAircraft" + std::to_string(row * grid_size + col + 1);
            add_aircraft(name, "Blue", 100, lat, lon, 270.0f, 0.25f, coordSystem);
        }
    }
}

// Add an aircraft
void Simulation::add_aircraft(const std::string& name, const std::string& force, int health, float x, float y, float heading, float speed, CoordinateSystem& coordSystem) {
    // Create an aircraft
    aircrafts.emplace_back(name, force, health, x, y, heading, speed, coordSystem);
}

// Get aircrafts (const version)
const std::vector<Aircraft>& Simulation::get_aircrafts() const {
    return aircrafts;
}

// Get aircrafts (modifiable version)
std::vector<Aircraft>& Simulation::get_aircrafts_mutable() {
    return aircrafts;
}

// Main simulation loop
void Simulation::run() {
    SDL_Event e;
    running = true; // Set running to true when the simulation starts

    while (!quit) {
        
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Simulation Update Call
        simulation_update(aircrafts, renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);  // Cap frame rate to ~60 FPS
    }
    running = false; // Set running to false when the simulation stops
}

void Simulation::simulation_update(std::vector<Aircraft>& aircrafts, SDL_Renderer* renderer) {
    for (auto& aircraft : aircrafts) {
        // aircraft.move_to(70.0f, 120.0f);
        aircraft.update(renderer); // Update each aircraft's state
    }
    initialize();
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();

    // Perform Python actions here.
     //behavior_module.attr("sim_update")();
    // behavior_module.attr("call_once")();

    // Release the thread. No Python API allowed beyond this point.
    PyGILState_Release(gstate);
}

void Simulation::initialize() {
    if (!is_initialized) {
        std::cout << "Simulation initialized!" << std::endl;
        
        PyGILState_STATE gstate;
        gstate = PyGILState_Ensure();

        behavior_module.attr("call_once")();

        PyGILState_Release(gstate);

        is_initialized = true;
    }
}

// Check if the simulation is running
bool Simulation::is_running() const {
    return running;
}

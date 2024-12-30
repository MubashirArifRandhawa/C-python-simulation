#include "Simulation.h"
#include <iostream>
#include <stdexcept>
#include <atomic> // For thread-safe running state


// Initialize the static instance to nullptr
std::once_flag flag;
std::unique_ptr<Simulation> Simulation::instance = nullptr;

Simulation::Simulation()
    : window(nullptr), renderer(nullptr), quit(false), running(false) { // Initialize running to false
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
    }

    window = SDL_CreateWindow("Aircraft Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
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

    add_aircraft("Fighter1", "Blue", 100, 100, 100);
    add_aircraft("Bomber1", "Red", 200, 200, 100);

    //################################ python ################################

    // Pass the simulation object to Python
    py::object py_sim = py::cast(this);
    behavior_module.attr("set_simulation")(py_sim);
    // behavior_module.attr("control_aircrafts_once")();

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

//Simulation& Simulation::get_instance() {
//    if (!instance) {
//        instance = std::unique_ptr<Simulation>(new Simulation());
//    }
//    return *instance;
//}

// Add an aircraft
void Simulation::add_aircraft(const std::string& name, const std::string& color, int force, float x, float y) {
    aircrafts.emplace_back(name, color, force, x, y);
}

// Get aircrafts (const version)
const std::vector<Aircraft>& Simulation::get_aircrafts() const {
    return aircrafts;
}

// Get aircrafts (modifiable version)
std::vector<Aircraft>& Simulation::get_aircrafts_mutable() {
    return aircrafts;
}

// Update aircraft positions
void Simulation::update_aircrafts() {
    for (auto& aircraft : aircrafts) {
        aircraft.wrap_around_screen(800, 600);
    }
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

       
        // Draw all aircraft
        for (const auto& aircraft : aircrafts) {
            aircraft.draw(renderer);
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(16);  // Cap frame rate to ~60 FPS

        PyGILState_STATE gstate;
        gstate = PyGILState_Ensure();

        ///* Perform Python actions here. */
        behavior_module.attr("sim_update")();
        ///* evaluate result or handle exception */

        ///* Release the thread. No Python API allowed beyond this point. */
        PyGILState_Release(gstate);
        
    }

    running = false; // Set running to false when the simulation stops
}

// Check if the simulation is running
bool Simulation::is_running() const {
    return running;
}

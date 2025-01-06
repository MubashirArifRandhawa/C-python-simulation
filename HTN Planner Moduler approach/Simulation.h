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
#include <functional>

#include <pybind11/embed.h>

namespace py = pybind11;

struct Button {
// Button structure
    SDL_Rect rect;                     // Button rectangle (position and size)
    std::string force;                   // Button color
    std::function<void()> onClick;     // Callback for button click


    void render(SDL_Renderer* renderer) const {
        //Load the aircraft image
        SDL_Texture* button_1 = IMG_LoadTexture(renderer, "../assets/icons/button/aircraft.png");
        if (button_1 == nullptr) {
            std::cerr << "Error loading aircraft texture: " << SDL_GetError() << "\n";
            return;
        }

        // Get the dimensions of the aircraft texture
        int texture_width = 32, texture_height = 32;
        SDL_QueryTexture(button_1, nullptr, nullptr, &texture_width, &texture_height);

        // Calculate the render rectangle for the image
        SDL_Rect renderQuad = {
            rect.x + (rect.w - texture_width) / 2, // Center the image horizontally within the button
            rect.y + (rect.h - texture_height) / 2, // Center the image vertically within the button
            texture_width,
            texture_height
        };

        // Set aircraft color modulation based on force (optional)
        if (force == "Blue") {
            SDL_SetTextureColorMod(button_1, 30, 144, 255); // Blue
            //SDL_SetRenderDrawColor(renderer, 30, 144, 255, 255); // White border (adjust as needed)
        }
        else if (force == "Red") {
            SDL_SetTextureColorMod(button_1, 220, 20, 60); // Red
            //SDL_SetRenderDrawColor(renderer, 220, 20, 60, 255); // White border (adjust as needed)
        }
        else if (force == "Green") {
            button_1 = IMG_LoadTexture(renderer, "../assets/icons/button/plan.png");
            if (button_1 == nullptr) {
                std::cerr << "Error loading aircraft texture: " << SDL_GetError() << "\n";
                return;
            }

            // Get the dimensions of the aircraft texture
            int texture_width = 32, texture_height = 32;
            SDL_QueryTexture(button_1, nullptr, nullptr, &texture_width, &texture_height);

            // Calculate the render rectangle for the image
            SDL_Rect renderQuad = {
                rect.x + (rect.w - texture_width) / 2, // Center the image horizontally within the button
                rect.y + (rect.h - texture_height) / 2, // Center the image vertically within the button
                texture_width,
                texture_height
            };

            SDL_SetTextureColorMod(button_1, 80, 200, 120); // Red
            //SDL_SetRenderDrawColor(renderer, 80, 200, 120, 255); // White border (adjust as needed)
        }

        // Render the texture
        SDL_RenderCopy(renderer, button_1, nullptr, &renderQuad);

        // Clean up texture after rendering
        SDL_DestroyTexture(button_1);


        // Draw the border around the button
        SDL_RenderDrawRect(renderer, &rect);
    }


    bool isClicked(int x, int y) const {
        return x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h;
    }
};


class Simulation {
public:
    Simulation(const Simulation&) = delete;
    Simulation& operator=(const Simulation&) = delete;

    static Simulation& get_instance(); // Returns a reference to the singleton

    void render_aircrafts(std::string color);
    void add_aircraft(const std::string& name, const std::string& force, int health, float x, float y, float heading, float speed, CoordinateSystem& coordSystem);
    const std::vector<Aircraft>& get_aircrafts() const;
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
    bool deployMode;  // Flag to track if we are in deploy mode
    float angle = 0.0f;

    void initialize_deploy() { deployMode = false; selectedAircraft = ""; selectedWaypoint=""; }

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

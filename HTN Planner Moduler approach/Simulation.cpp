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

    // Add buttons
    // Add buttons at the top-left corner

    //buttons.push_back({ {1030, 10, 25, 25}, "Red", [this]() { render_aircrafts("red"); }});
    //buttons.push_back({ {1030, 40, 25, 25}, "Blue", [this]() { render_aircrafts("blue"); }});

    //buttons.push_back({ {1030, 10, 25, 25}, "Red", [this]() { render_single_aircraft("red"); }});
    //buttons.push_back({ {1030, 40, 25, 25}, "Blue", [this]() { render_single_aircraft("blue"); }});
    initialize_deploy();

    buttons.push_back({ {1030, 10, 25, 25}, "Red", SimulationObjectType::Aircraft, [this]() {
        onButtonclick("red");
    } });
    buttons.push_back({ {1030, 40, 25, 25}, "Blue", SimulationObjectType::Aircraft, [this]() {
        onButtonclick("blue");
    } });

    buttons.push_back({ {1030, 70, 25, 25}, "Red", SimulationObjectType::Waypoint, [this]() {
    onButtonclick("red-waypoint");
    } });
    buttons.push_back({ {1030, 100, 25, 25}, "Blue", SimulationObjectType::Waypoint, [this]() {
        onButtonclick("blue-waypoint");
    } });

    buttons.push_back({ {1030, 550, 25, 25}, "Green", SimulationObjectType::Unknown, [this]() { initialize(); }});


    // Aircrafts ceated
    // render_aircrafts();
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

std::string Simulation::simulationObjectTypeToString(SimulationObjectType type) {
    switch (type) {
    case SimulationObjectType::Aircraft: return "Aircraft";
    case SimulationObjectType::Waypoint: return "Waypoint";
    case SimulationObjectType::Missile: return "Missile";
    default: return "Unknown";
    }
}

void Simulation::onButtonclick(std::string color) {
    if (color == "red") {
        selectedAircraft = "Red";
        deployMode = SimulationObjectType::Aircraft;
    }

    if (color == "blue") {
        selectedAircraft = "Blue";
        deployMode = SimulationObjectType::Aircraft;
    }

    if (color == "red-waypoint") {
        selectedWaypoint = "Red";
        deployMode = SimulationObjectType::Waypoint;
    }

    if (color == "blue-waypoint") {
        selectedWaypoint = "Blue";
        deployMode = SimulationObjectType::Waypoint;
    }
}

Simulation& Simulation::get_instance() {
    std::call_once(flag, []() {
        instance = std::unique_ptr<Simulation>(new Simulation());
        });
    return *instance;
}

void Simulation::render_single_aircraft(std::string color) {
    if (color == "red") {
        add_aircraft("Fighter - Red", "Red", 100, 60.f, -120.0f, 90.0f, 0.25f, coordSystem);
    }

    if (color == "blue") {
        add_aircraft("Fihgter - Blue", "Blue", 100, -60.f, 120.0f, 270.0f, 0.25f, coordSystem);
    }
}

void Simulation::render_waypoint(std::string color, int x, int y) {
    if (color == "Red") {
        add_waypoint("Fighter - Red", "Red", x, y, coordSystem);
    }
    else if (color == "Blue") {
        add_waypoint("Fighter - Blue", "Blue", x, y, coordSystem);
    }
}

void Simulation::render_single_aircraft(std::string color, int x, int y) {
    if (color == "Red") {
        add_aircraft("Fighter - Red", "Red", 100, x, y, angle, 0.25f, coordSystem);
    }
    else if (color == "Blue") {
        add_aircraft("Fighter - Blue", "Blue", 100, x, y, angle, 0.25f, coordSystem);
    }
}

void Simulation::render_aircraft_preview(const std::string& force, int x, int y) {
    
    SDL_Texture* aircraftTexture;
    aircraftTexture = IMG_LoadTexture(renderer, FileLoader::getSimulationObjectTexture(SimulationObjectType::Aircraft).c_str());
    if (aircraftTexture == nullptr) {
        std::cerr << "Error loading aircraft texture: " << SDL_GetError() << "\n";
        return;
    }

    int texture_width = 32, texture_height = 32;
    SDL_QueryTexture(aircraftTexture, nullptr, nullptr, &texture_width, &texture_height);

    SDL_Rect renderQuad = { x - texture_width / 2, y - texture_height / 2, texture_width, texture_height };

    // Set aircraft color (based on the force)
    if (force == "Blue") {
        //SDL_SetTextureColorMod(aircraftTexture, 0, 0, 255); // Blue
        SDL_SetTextureColorMod(aircraftTexture, 30, 144, 255); // Blue
    }
    else if (force == "Red") {
        //SDL_SetTextureColorMod(aircraftTexture, 255, 0, 0); // Red
        SDL_SetTextureColorMod(aircraftTexture, 220, 20, 60); // Red
    }
    else {
        SDL_SetTextureColorMod(aircraftTexture, 255, 255, 255); // Default white
    }

     // Aircraft's heading (in degrees)

    // Rotate the aircraft image based on its heading (rotate around its center)
    SDL_RenderCopyEx(renderer, aircraftTexture, nullptr, &renderQuad, angle, nullptr, SDL_FLIP_NONE);
    SDL_DestroyTexture(aircraftTexture);

}


void Simulation::render_aircrafts(std::string color) {
    int grid_size = 5; // 7x7 grid
    float spacing = 15.0f; // Distance between aircraft in the grid

    if (color == "red") {
        // Red team: Top-left corner
        float red_start_lat = 90.0f - 20;  // Top of the map
        float red_start_lon = -180.0f + 30; // Left of the map

        // Render Red team
        for (int row = 0; row < grid_size; ++row) {
            for (int col = 0; col < grid_size; ++col) {
                float lat = red_start_lat - row * spacing; // Move downward
                float lon = red_start_lon + col * spacing; // Move rightward
                std::string name = "RedAircraft" + std::to_string(row * grid_size + col + 1);
                add_aircraft(name, "Red", 100, lat, lon, 90.0f, 0.25f, coordSystem);
            }
        }
    }
    
    if (color == "blue") {
        // Blue team: Bottom-right corner
        float blue_start_lat = -90.0f + 20; // Bottom of the map
        float blue_start_lon = 180.0f - 30; // Right of the map

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
    
}

// Add an aircraft
void Simulation::add_aircraft(const std::string& name, const std::string& force, int health, float x, float y, float heading, float speed, CoordinateSystem& coordSystem) {
    // Create an aircraft
    aircrafts.emplace_back(name, force, health, x, y, heading, speed, coordSystem);
}

void Simulation::add_waypoint(const std::string& name, const std::string& force, float x, float y, CoordinateSystem& coordSystem) {
    // Create an aircraft
    waypoints.emplace_back(name, force, x, y, coordSystem);
}

// Get aircrafts (const version)
const std::vector<Aircraft>& Simulation::get_aircrafts() const {
    return aircrafts;
}

const std::vector<Waypoint>& Simulation::get_waypoints() const {
    return waypoints;
}

// Get aircrafts (modifiable version)
std::vector<Aircraft>& Simulation::get_aircrafts_mutable() {
    return aircrafts;
}

// Main simulation loop
void Simulation::run() {
    SDL_Event e;
    running = true; // Set running to true when the simulation starts

    int mouseX = 0, mouseY = 0;

    while (!quit) {
        
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                //handleMouseClick(e.button.x, e.button.y);
                handleMouseEvent(e);
            }
            else if (e.type == SDL_MOUSEMOTION) {
                SDL_GetMouseState(&mouseX, &mouseY);
            }
            else if (e.type == SDL_MOUSEWHEEL) {
                handleMouseWheel(e);
            }

        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render buttons
        for (const auto& button : buttons) {
            button.render(renderer);
        }


        // Render aircraft following mouse pointer in deployMode
        if (deployMode == SimulationObjectType::Aircraft && !selectedAircraft.empty()) {
            render_aircraft_preview(selectedAircraft, mouseX, mouseY);
        }
        // Simulation Update Call
        simulation_update(aircrafts, renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);  // Cap frame rate to ~60 FPS
    }
    running = false; // Set running to false when the simulation stops
}

void Simulation::handleMouseEvent(const SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        int x = e.button.x;
        int y = e.button.y;

        std::cout << "Left click detected at (" << x << ", " << y << ")\n";

        bool buttonClicked = false;

        // Check if any button is clicked
        for (const auto& button : buttons) {
            if (button.isClicked(x, y)) {
                button.onClick();
                buttonClicked = true;  // Mark that a button was clicked
                std::cout << "Button clicked at (" << x << ", " << y << ")\n";
                break;  // Exit loop since a button was clicked
            }
        }

        // If no button is clicked, check for deploying aircraft
        if (!buttonClicked) {
            std::cout << "Screen clicked at (" << x << ", " << y << ")\n";
            std::cout << "Deploy mode: " << simulationObjectTypeToString(deployMode)
                << ", Selected aircraft: " << selectedAircraft << "\n";

            if (deployMode == SimulationObjectType::Aircraft && !selectedAircraft.empty()) {
                // Deploy the selected aircraft at the mouse location
                std::pair<int, int> lat_lon = coordSystem.to_lat_lon(x, y);
                int lat = lat_lon.first;
                int lon = lat_lon.second;
                render_single_aircraft(selectedAircraft, lat, lon);
            }

            if (deployMode == SimulationObjectType::Waypoint && !selectedWaypoint.empty()) {
                // Deploy the selected aircraft at the mouse location
                std::pair<int, int> lat_lon = coordSystem.to_lat_lon(x, y);
                int lat = lat_lon.first;
                int lon = lat_lon.second;
                render_waypoint(selectedWaypoint, lat, lon);
            }
        }

    }
    else {
        // Always reset deploy mode after handling the click
        if (deployMode != SimulationObjectType::Unknown) {
            std::cout << "Deploy mode deactivated.\n";
            deployMode = SimulationObjectType::Unknown;
        }
    }
}


void Simulation::handleMouseClick(int x, int y) {
    bool buttonClicked = false;

    // Check if any button is clicked
    for (const auto& button : buttons) {
        if (button.isClicked(x, y)) {
            button.onClick();
            buttonClicked = true;  // Mark that a button was clicked
            std::cout << "Button clicked at (" << x << ", " << y << ")\n";
            break;  // Exit loop since a button was clicked
        }
    }

    // If no button is clicked, check for deploying aircraft
    if (!buttonClicked) {
        std::cout << "Screen clicked at (" << x << ", " << y << ")\n";
        std::cout << "Deploy mode: " << simulationObjectTypeToString(deployMode)
            << ", Selected aircraft: " << selectedAircraft << "\n";

        if (deployMode == SimulationObjectType::Aircraft && !selectedAircraft.empty()) {
            // Deploy the selected aircraft at the mouse location
            std::pair<int, int> lat_lon = coordSystem.to_lat_lon(x, y);
            int lat = lat_lon.first;
            int lon = lat_lon.second;
            render_single_aircraft(selectedAircraft, lat, lon);
            //deployMode = false;  // Turn off deploy mode after deploying
        }
    }
}

void Simulation::handleMouseWheel(SDL_Event& e) {
    if (e.type == SDL_MOUSEWHEEL) {
        angle += e.wheel.y * 5.0f; // Adjust rotation by 5 degrees per scroll step
    }
}



void Simulation::simulation_update(std::vector<Aircraft>& aircrafts, SDL_Renderer* renderer) {
    for (auto& aircraft : aircrafts) {
        aircraft.update(renderer); // Update each aircraft's state
    }

    for (auto& waypoint : waypoints) {
        waypoint.update(renderer); // Update each aircraft's state
    }
    //initialize();
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();

    // Perform Python actions here.
     //behavior_module.attr("sim_update")();
    // behavior_module.attr("call_once")();

    // Release the thread. No Python API allowed beyond this point.
    PyGILState_Release(gstate);
}

void Simulation::initialize() {
    //if (!is_initialized) {
    /*    std::cout << "Simulation initialized!" << std::endl;
        
        PyGILState_STATE gstate;
        gstate = PyGILState_Ensure();

        behavior_module.attr("call_once")();

        PyGILState_Release(gstate);*/

        //is_initialized = true;
    //}

    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();

    try {
        // Call the Python function and capture the return value
        pybind11::object result = behavior_module.attr("call_once")();

        // Process the returned value (example for a dictionary)
        if (pybind11::isinstance<pybind11::dict>(result)) {
            pybind11::dict result_dict = result.cast<pybind11::dict>();
            std::string status = pybind11::str(result_dict["status"]);
            pybind11::list data = result_dict["data"].cast<pybind11::list>();

            std::cout << "Status: " << status << "\n";
            std::cout << "Data: ";
            for (auto item : data) {
                std::cout << item.cast<int>() << " ";
            }
            std::cout << "\n";
        }
        else {
            std::cerr << "Unexpected return type from call_once\n";
        }
    }
    catch (const pybind11::error_already_set& e) {
        std::cerr << "Python error: " << e.what() << "\n";
    }

    PyGILState_Release(gstate);

}

// Check if the simulation is running
bool Simulation::is_running() const {
    return running;
}

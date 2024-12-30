//#include <SDL.h>  // Ensure SDL is included first
#include <pybind11/pybind11.h>  // pybind11 should come after SDL includes
//#include "Aircraft.h"
//#include <iostream>
//
namespace py = pybind11;
//
//PYBIND11_MODULE(aircraft_simulation, m) {
//    py::class_<Aircraft>(m, "Aircraft")
//        .def(py::init<const std::string&, const std::string&, int, float, float>())
//        .def("get_name", &Aircraft::get_name)
//        .def("get_force", &Aircraft::get_force)
//        .def("get_health", &Aircraft::get_health)
//        .def("get_x", &Aircraft::get_x)
//        .def("get_y", &Aircraft::get_y)
//        .def("move_to", &Aircraft::move_to)
//        .def("attack", &Aircraft::attack)
//        .def("defend", &Aircraft::defend)
//        .def("is_alive", &Aircraft::is_alive)
//        //.def("draw", &Aircraft::draw)  // Binding the draw function
//        .def("wrap_around_screen", &Aircraft::wrap_around_screen);
//}
//
//int SDL_main(int argc, char* argv[]) {
//    // Initialize SDL
//    SDL_Window* window = nullptr;
//    SDL_Renderer* renderer = nullptr;
//
//    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
//        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
//        return 1;
//    }
//
//    window = SDL_CreateWindow("Aircraft Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
//    if (window == nullptr) {
//        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
//        SDL_Quit();
//        return 1;
//    }
//
//    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//    if (renderer == nullptr) {
//        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
//        SDL_DestroyWindow(window);
//        SDL_Quit();
//        return 1;
//    }
//
//    // Create Aircraft objects
//    Aircraft aircraft1("Fighter1", "Blue", 100, 100, 100);
//    Aircraft aircraft2("Bomber1", "Red", 100, 300, 300);
//
//    bool quit = false;
//    SDL_Event e;
//
//    // Main game loop
//    while (!quit) {
//        while (SDL_PollEvent(&e)) {
//            if (e.type == SDL_QUIT) {
//                quit = true;
//            }
//        }
//
//        // Update aircraft positions
//        aircraft1.move_to(aircraft1.get_x() + 1, aircraft1.get_y());
//        aircraft2.move_to(aircraft2.get_x() - 1, aircraft2.get_y());
//
//        // Wrap aircraft around the screen
//        aircraft1.wrap_around_screen(800, 600);
//        aircraft2.wrap_around_screen(800, 600);
//
//        // Render the screen
//        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Set white background color
//        SDL_RenderClear(renderer);  // Clear the screen
//
//        // Draw aircraft
//        aircraft1.draw(renderer);  // Draw the first aircraft
//        aircraft2.draw(renderer);  // Draw the second aircraft
//
//        SDL_RenderPresent(renderer);  // Present the renderer
//
//        SDL_Delay(16);  // 60 FPS delay
//    }
//
//    // Clean up
//    SDL_DestroyRenderer(renderer);
//    SDL_DestroyWindow(window);
//    SDL_Quit();
//
//    return 0;
//}

//#include "Simulation.h"
//#include <iostream>
//
//int SDL_main(int argc, char* argv[]) {
//    try {
//        Simulation simulation;
//
//        // Add aircraft to the simulation
//        simulation.add_aircraft("Fighter1", "Blue", 100, 100, 100);
//        simulation.add_aircraft("Bomber1", "Red", 100, 300, 300);
//
//        // Run the simulation
//        simulation.run();
//    } catch (const std::exception& e) {
//        std::cerr << "Error: " << e.what() << std::endl;
//        return 1;
//    }
//
//    return 0;
//}

#include "Simulation.h"
#include <iostream>
#include <thread>
#include <atomic>

std::atomic<bool> simulation_running(true);

void run_simulation(Simulation& simulation) {
    std::thread simulation_thread([&simulation] {
        simulation.run();
        simulation_running = false;  // Signal the main thread when done
        });
    simulation_thread.detach();
}

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

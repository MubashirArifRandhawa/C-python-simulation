#include "Waypoint.h"
#include <iostream>
#include <cmath> // For sin and cos
#include "CoordinateSystem.h"
#include "FileLoader.cpp"

Waypoint::Waypoint(const std::string& name, const std::string& force,
    float startLatitude, float startLongitude, CoordinateSystem& coordinateSystem)
    : name(name), latitude(startLatitude), longitude(startLongitude),
    coordinateSystem(coordinateSystem) { // Default heading is North
    if (force != "Red" && force != "Blue") {
        std::cerr << "Invalid force value: " << force << ". Force must be 'Red' or 'Blue'.\n";
        this->force = "Blue"; // Default to Blue if invalid
    }
    else {
        this->force = force;
    }
    iconPath = FileLoader::getSimulationObjectTexture(SimulationObjectType::Waypoint);
}

std::string Waypoint::get_name() const {
    return name;
}

std::string Waypoint::get_force() const {
    return force;
}

std::pair<float, float> Waypoint::get_position() const {
    return { latitude, longitude };
}

void Waypoint::draw(SDL_Renderer* renderer) const {

    // Get screen coordinates of the aircraft
    std::pair<int, int> screen_coordinates = coordinateSystem.to_screen_coordinates(latitude, longitude);
    int screen_x = screen_coordinates.first;
    int screen_y = screen_coordinates.second;

    SDL_Texture* waypointTexture = loadTexture(renderer, iconPath);
    if (!waypointTexture) return;

    // Load the aircraft image (if not already loaded)

    // Get the dimensions of the aircraft texture
    int texture_width = 512, texture_height = 512;
    SDL_QueryTexture(waypointTexture, nullptr, nullptr, &texture_width, &texture_height);

    // Calculate the position for the aircraft image (centered around the coordinates)
    SDL_Rect renderQuad = { screen_x - texture_width / 2, screen_y - texture_height / 2, texture_width, texture_height };

    // Set aircraft color (based on the force)
    applyColorMod(waypointTexture);

    // Calculate the angle in radians and ensure it's correct
    float angle = 0; // Aircraft's heading (in degrees)

    // Rotate the aircraft image based on its heading (rotate around its center)
    SDL_RenderCopyEx(renderer, waypointTexture, nullptr, &renderQuad, angle, nullptr, SDL_FLIP_NONE);

    // Free the texture after rendering
    SDL_DestroyTexture(waypointTexture);

}

void Waypoint::update(SDL_Renderer* renderer) {
    draw(renderer);
}

SDL_Texture* Waypoint::loadTexture(SDL_Renderer* renderer, const std::string& path) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
    if (!texture) {
        std::cerr << "Error loading texture (" << path << "): " << SDL_GetError() << "\n";
    }
    return texture;
}

void Waypoint::applyColorMod(SDL_Texture* texture) const {
    if (force == "Blue") {
        SDL_SetTextureColorMod(texture, 30, 144, 255); // Blue
    }
    else if (force == "Red") {
        SDL_SetTextureColorMod(texture, 220, 20, 60);  // Red
    }
    else if (force == "Green") {
        SDL_SetTextureColorMod(texture, 80, 200, 120); // Green
    }
    else {
        SDL_SetTextureColorMod(texture, 255, 255, 255);
    }
}
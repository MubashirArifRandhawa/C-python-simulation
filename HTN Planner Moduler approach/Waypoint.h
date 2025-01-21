#pragma once
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "SimulationObject.h"


class CoordinateSystem;

//class Waypoint : public SimulationObject
class Waypoint
{
    std::string name;
    std::string force; // "Red" or "Blue"
    int health;
    float heading; // Heading in degrees (0 = North)
    CoordinateSystem& coordinateSystem; // Reference to a coordinate system
    float latitude, longitude; // Current position in lat/lon
    float target_latitude = 0;
    float target_longitude = 0; // Target position in lat/lon
    float speed; // Speed in lat/lon units per update

    bool is_moving; // Flag to indicate if the aircraft is moving
    std::string iconPath;

public:
    // Constructor
    Waypoint(const std::string& name, const std::string& force, float startLatitude, float startLongitude, CoordinateSystem& coordinateSystem);

    // Getters
    std::string get_name() const;
    std::string get_force() const;
    std::pair<float, float> get_position() const;

    // Movement
    void update(SDL_Renderer* renderer); // Update function for simulation loop
    void draw(SDL_Renderer* renderer) const;

private:
    static SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& path);
    void applyColorMod(SDL_Texture* texture) const;

};


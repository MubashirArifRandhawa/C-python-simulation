#pragma once

#include <string>
#include <SDL.h>
#include <SDL_image.h>


class CoordinateSystem;

class Aircraft {
private:
    std::string name;
    std::string force; // "Red" or "Blue"
    int health;
    float heading; // Heading in degrees (0 = North)
    CoordinateSystem& coordinateSystem; // Reference to a coordinate system
    float latitude, longitude; // Current position in lat/lon
    float target_latitude=0; 
    float target_longitude=0; // Target position in lat/lon
    float speed; // Speed in lat/lon units per update

    bool is_moving; // Flag to indicate if the aircraft is moving

public:
    // Constructor
    Aircraft(const std::string& name, const std::string& force, int health,
        float startLatitude, float startLongitude, float heading, float speed, CoordinateSystem& coordinateSystem);

    // Getters
    std::string get_name() const;
    std::string get_force() const;
    int get_health() const;
    float get_heading() const;
    std::pair<float, float> get_position() const;

    // Movement
    void move_to(float newLatitude, float newLongitude); // Initiate movement
    void move(float distance);
    void update(SDL_Renderer* renderer); // Update function for simulation loop
    void attack(Aircraft& target);
    void defend();
    bool is_alive() const;
    void draw(SDL_Renderer* renderer) const;
    void adjustHeadingToNorth();
    void set_heading(float new_heading);

private:
    void update_position(); // Gradually move towards the target
};

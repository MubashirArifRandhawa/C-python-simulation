#pragma once

#include <string>
#include <SDL.h>

class CoordinateSystem;

class Aircraft {
private:
    std::string name;
    std::string force; // "Red" or "Blue"
    int health;
    float heading; // Heading in degrees (0 = North)
    CoordinateSystem& coordinateSystem; // Reference to a coordinate system
    float latitude, longitude; // Position in lat/lon

public:
    // Constructor
    Aircraft(const std::string& name, const std::string& force, int health,
        float startLatitude, float startLongitude, CoordinateSystem& coordinateSystem);

    // Getters
    std::string get_name() const;
    std::string get_force() const;
    int get_health() const;
    float get_heading() const;
    std::pair<float, float> get_position() const;

    // Methods
    void move_to(float newLatitude, float newLongitude);
    void move(float distance);
    void attack(Aircraft& target);
    void defend();
    bool is_alive() const;
    void draw(SDL_Renderer* renderer) const;
    void adjustHeadingToNorth();
    void set_heading(float new_heading);
};

#include "Aircraft.h"
#include <iostream>
#include <cmath> // For sin and cos
#include "CoordinateSystem.h"

// Constructor
Aircraft::Aircraft(const std::string& name, const std::string& force, int health,
    float startLatitude, float startLongitude, CoordinateSystem& coordinateSystem)
    : name(name), health(health), latitude(startLatitude), longitude(startLongitude),
    coordinateSystem(coordinateSystem), heading(0.0f) { // Default heading is North
    if (force != "Red" && force != "Blue") {
        std::cerr << "Invalid force value: " << force << ". Force must be 'Red' or 'Blue'.\n";
        this->force = "Blue"; // Default to Blue if invalid
    }
    else {
        this->force = force;
    }

    // Adjust the heading so 0 degrees is north
    adjustHeadingToNorth();  // Adjust heading after setting it

}

void Aircraft::adjustHeadingToNorth() {
    heading = 90.0f - heading;  // Adjust so 0 degrees points north
    if (heading < 0.0f) {
        heading += 360.0f;  // Ensure heading stays within 0-360 range
    }
}

// Getter Methods
std::string Aircraft::get_name() const {
    return name;
}

std::string Aircraft::get_force() const {
    return force;
}

int Aircraft::get_health() const {
    return health;
}

float Aircraft::get_heading() const {
    return heading;
}

std::pair<float, float> Aircraft::get_position() const {
    return { latitude, longitude };
}

// Set the heading
void Aircraft::set_heading(float new_heading) {
    heading = std::fmod(new_heading, 360.0f); // Ensure heading is within 0-359 degrees
    if (heading < 0) {
        heading += 360.0f; // Normalize negative headings
    }
    adjustHeadingToNorth();  // Adjust heading after setting it
}

// Move the aircraft to a specific lat/lon position
void Aircraft::move_to(float newLatitude, float newLongitude) {
    latitude = newLatitude;
    longitude = newLongitude;
}

// Move the aircraft in the direction of its current heading
void Aircraft::move(float distance) {
    float heading_radians = heading * M_PI / 180.0f;

    // Calculate new latitude and longitude based on heading
    float delta_latitude = distance * std::cos(heading_radians);
    float delta_longitude = distance * std::sin(heading_radians);

    latitude += delta_latitude;
    longitude += delta_longitude;

    // Wrap around the coordinate system if needed
    coordinateSystem.wrap_coordinates(latitude, longitude);
}

// Attack another aircraft
void Aircraft::attack(Aircraft& target) {
    if (!is_alive()) {
        std::cout << name << " cannot attack because it is destroyed.\n";
        return;
    }
    if (!target.is_alive()) {
        std::cout << target.get_name() << " is already destroyed.\n";
        return;
    }
    target.health -= 10; // Example damage value
    target.health = std::max(0, target.health); // Ensure health does not go below 0
    std::cout << name << " attacked " << target.get_name() << "!\n";
}

// Defend and restore health
void Aircraft::defend() {
    if (!is_alive()) {
        std::cout << name << " cannot defend because it is destroyed.\n";
        return;
    }
    health += 5; // Example defense healing value
    health = std::min(100, health); // Assume 100 is max health
    std::cout << name << " is defending and restored health to " << health << ".\n";
}

// Check if the aircraft is still alive
bool Aircraft::is_alive() const {
    return health > 0;
}

void Aircraft::draw(SDL_Renderer* renderer) const {
    std::cout << "Drawing aircraft: " << get_name() << "\n";
    std::cout << "Current position in lat/lon: (" << latitude << ", " << longitude << ")\n";

    // Get screen coordinates of the aircraft
    std::pair<int, int> screen_coordinates = coordinateSystem.to_screen_coordinates(latitude, longitude);
    int screen_x = screen_coordinates.first;
    int screen_y = screen_coordinates.second;

    std::cout << "Converted screen coordinates: (" << screen_x << ", " << screen_y << ")\n";

    // Draw the aircraft body
    SDL_Rect rect = { static_cast<int>(screen_x - 10), static_cast<int>(screen_y - 10), 20, 20 };
    if (force == "Blue") {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue
    }
    else if (force == "Red") {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
    }
    else {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Default white
    }
    SDL_RenderFillRect(renderer, &rect);

    // Draw the heading line
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White

    // Calculate the endpoint of the heading line
    float line_length = 30.0f; // Length of the heading line in pixels
    float heading_radians = get_heading() * M_PI / 180.0f;  // Convert heading from degrees to radians

    // Adjust for SDL's coordinate system, flip Y-axis
    int end_x = static_cast<int>(screen_x + line_length * std::cos(heading_radians));  // X stays as is
    int end_y = static_cast<int>(screen_y - line_length * std::sin(heading_radians)); // Flip the Y-axis for SDL

    // Draw the heading line
    SDL_RenderDrawLine(renderer, screen_x, screen_y, end_x, end_y);
}




//void Aircraft::draw(SDL_Renderer* renderer) const {
//    std::cout << "Drawing aircraft: " << get_name() << "\n";
//    std::cout << "Current position in lat/lon: (" << latitude << ", " << longitude << ")\n";
//
//    std::pair<int, int> screen_coordinates = coordinateSystem.to_screen_coordinates(latitude, longitude);
//    int screen_x = screen_coordinates.first;
//    int screen_y = screen_coordinates.second;
//
//    std::cout << "Converted screen coordinates: (" << screen_x << ", " << screen_y << ")\n";
//
//    // Draw the aircraft body
//    SDL_Rect rect = { static_cast<int>(screen_x - 10), static_cast<int>(screen_y - 10), 20, 20 };
//
//    std::cout << "Force: "<<force <<" Screen_x: "<< screen_x << " Screen_y: " << screen_y << std::endl;
//    if (force == "Blue") {
//        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue
//    }
//    else if (force == "Red") {
//        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
//    }
//    else {
//        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Default white if invalid force
//    }
//    SDL_RenderFillRect(renderer, &rect);
//
//    // Draw the heading line
//    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
//
//    // Calculate the endpoint of the heading line
//    float line_length = 30.0f;
//    float heading_radians = get_heading() * M_PI / 180.0f;
//
//    float end_latitude = latitude + (line_length / 100.0f) * std::cos(heading_radians);
//    float end_longitude = longitude + (line_length / 100.0f) * std::sin(heading_radians);
//
//    std::pair<int, int> screen_coordinates_end = coordinateSystem.to_screen_coordinates(end_latitude, end_longitude);
//    int end_x = screen_coordinates_end.first;
//    int end_y = screen_coordinates_end.second;
//
//    SDL_RenderDrawLine(renderer, static_cast<int>(screen_x), static_cast<int>(screen_y),
//        static_cast<int>(end_x), static_cast<int>(end_y));
//}
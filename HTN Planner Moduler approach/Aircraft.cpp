#include "Aircraft.h"
#include <iostream>
#include <cmath> // For sin and cos
#include "CoordinateSystem.h"

// Constructor
Aircraft::Aircraft(const std::string& name, const std::string& force, int health,
    float startLatitude, float startLongitude, CoordinateSystem& coordinateSystem)
    : name(name), health(health), latitude(startLatitude), longitude(startLongitude),
    coordinateSystem(coordinateSystem), heading(0.0f), speed(0.25f) { // Default heading is North
    if (force != "Red" && force != "Blue") {
        std::cerr << "Invalid force value: " << force << ". Force must be 'Red' or 'Blue'.\n";
        this->force = "Blue"; // Default to Blue if invalid
    }
    else {
        this->force = force;
    }
    // Adjust the heading so 0 degrees is north
    //adjustHeadingToNorth();  // Adjust heading after setting it

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
    //adjustHeadingToNorth();  // Adjust heading after setting it
}


void Aircraft::move_to(float newLatitude, float newLongitude) {
    target_latitude = newLatitude;
    target_longitude = newLongitude;
    is_moving = true; // Start moving
}

void Aircraft::update_position() {
    if (!is_moving) return;

    // Calculate differences
    float delta_latitude = target_latitude - latitude;
    float delta_longitude = target_longitude - longitude;
    std::cout << "Target Lat Long: " << target_latitude << ", " << target_longitude << std::endl;
    // Calculate the distance to the target
    float distance_to_target = std::sqrt(delta_latitude * delta_latitude +
        delta_longitude * delta_longitude);

    if (distance_to_target < speed) {
        // Close enough to the target, snap to target and stop moving
        latitude = target_latitude;
        longitude = target_longitude;
        is_moving = false;
    }
    else {
        // Move a step towards the target
        float ratio = speed / distance_to_target;
        latitude += delta_latitude * ratio;
        longitude += delta_longitude * ratio;

        // Update heading to point towards the target
        heading = std::atan2(delta_longitude, delta_latitude) * 180.0f / M_PI;
        // adjustHeadingToNorth();
    }
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

//void Aircraft::draw(SDL_Renderer* renderer) const {
//    std::cout << "Drawing aircraft: " << get_name() << "\n";
//    std::cout << "Current position in lat/lon: (" << latitude << ", " << longitude << ")\n";
//
//    // Get screen coordinates of the aircraft
//    std::pair<int, int> screen_coordinates = coordinateSystem.to_screen_coordinates(latitude, longitude);
//    int screen_x = screen_coordinates.first;
//    int screen_y = screen_coordinates.second;
//
//    // Draw the aircraft body
//    SDL_Rect rect = { static_cast<int>(screen_x - 10), static_cast<int>(screen_y - 10), 20, 20 };
//    if (force == "Blue") {
//        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue
//    }
//    else if (force == "Red") {
//        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
//    }
//    else {
//        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Default white
//    }
//    SDL_RenderFillRect(renderer, &rect);
//
//    // Draw the heading line
//    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
//
//    // Calculate the endpoint of the heading line
//    float line_length = 30.0f; // Length of the heading line in pixels
//    float heading_radians = get_heading() * M_PI / 180.0f;  // Convert heading from degrees to radians
//
//    // Adjust for SDL's coordinate system, flip Y-axis
//    int end_x = static_cast<int>(screen_x + line_length * std::cos(heading_radians));  // X stays as is
//    int end_y = static_cast<int>(screen_y - line_length * std::sin(heading_radians)); // Flip the Y-axis for SDL
//
//    // Draw the heading line
//    SDL_RenderDrawLine(renderer, screen_x, screen_y, end_x, end_y);
//}

//void Aircraft::draw(SDL_Renderer* renderer) const {
//    std::cout << "Drawing aircraft: " << get_name() << "\n";
//    std::cout << "Current position in lat/lon: (" << latitude << ", " << longitude << ")\n";
//
//    // Get screen coordinates of the aircraft
//    std::pair<int, int> screen_coordinates = coordinateSystem.to_screen_coordinates(latitude, longitude);
//    int screen_x = screen_coordinates.first;
//    int screen_y = screen_coordinates.second;
//
//    // Load the aircraft image (if not already loaded)
//    SDL_Texture* aircraftTexture = IMG_LoadTexture(renderer, "../assets/icons/aircraft.png");
//    if (aircraftTexture == nullptr) {
//        std::cerr << "Error loading aircraft texture: " << SDL_GetError() << "\n";
//        return;
//    }
//
//    // Get the dimensions of the aircraft texture
//    int texture_width, texture_height;
//    SDL_QueryTexture(aircraftTexture, nullptr, nullptr, &texture_width, &texture_height);
//
//    // Calculate the position for the aircraft image (centered around the coordinates)
//    SDL_Rect renderQuad = { screen_x - texture_width / 2, screen_y - texture_height / 2, texture_width, texture_height };
//
//    // Set aircraft color (based on the force)
//    if (force == "Blue") {
//        SDL_SetTextureColorMod(aircraftTexture, 0, 0, 255); // Blue
//    }
//    else if (force == "Red") {
//        SDL_SetTextureColorMod(aircraftTexture, 255, 0, 0); // Red
//    }
//    else {
//        SDL_SetTextureColorMod(aircraftTexture, 255, 255, 255); // Default white
//    }
//
//    // Render the aircraft image
//    SDL_RenderCopy(renderer, aircraftTexture, nullptr, &renderQuad);
//    SDL_DestroyTexture(aircraftTexture); // Free the texture after rendering
//
//    // Draw the heading line
//    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
//
//    // Calculate the endpoint of the heading line
//    float line_length = 30.0f; // Length of the heading line in pixels
//    float heading_radians = get_heading() * M_PI / 180.0f;  // Convert heading from degrees to radians
//
//    // Adjust for SDL's coordinate system, flip Y-axis
//    int end_x = static_cast<int>(screen_x + line_length * std::cos(heading_radians));  // X stays as is
//    int end_y = static_cast<int>(screen_y - line_length * std::sin(heading_radians)); // Flip the Y-axis for SDL
//
//    // Draw the heading line
//    SDL_RenderDrawLine(renderer, screen_x, screen_y, end_x, end_y);
//}


//void Aircraft::update_position() {
//    if (!is_moving) return;
//
//    // Calculate differences
//    float delta_latitude = target_latitude - latitude;
//    float delta_longitude = target_longitude - longitude;
//
//    // Calculate the distance to the target
//    float distance_to_target = std::sqrt(delta_latitude * delta_latitude +
//        delta_longitude * delta_longitude);
//
//    if (distance_to_target < speed) {
//        // Close enough to the target, snap to target and stop moving
//        latitude = target_latitude;
//        longitude = target_longitude;
//        is_moving = false;
//    }
//    else {
//        // Move a step towards the target
//        float ratio = speed / distance_to_target;
//        latitude += delta_latitude * ratio;
//        longitude += delta_longitude * ratio;
//
//        // Calculate heading towards the target (angle in radians)
//        heading = std::atan2(delta_longitude, delta_latitude) * 180.0f / M_PI;
//        adjustHeadingToNorth(); // Ensure the heading is always in the correct range
//    }
//}

void Aircraft::draw(SDL_Renderer* renderer) const {
    std::cout << "Drawing aircraft: " << get_name() << "\n";
    std::cout << "Current position in lat/lon: (" << latitude << ", " << longitude << ")\n";

    // Get screen coordinates of the aircraft
    std::pair<int, int> screen_coordinates = coordinateSystem.to_screen_coordinates(latitude, longitude);
    int screen_x = screen_coordinates.first;
    int screen_y = screen_coordinates.second;

    // Load the aircraft image (if not already loaded)
    SDL_Texture* aircraftTexture = IMG_LoadTexture(renderer, "../assets/icons/aircraft_32.png");
    if (aircraftTexture == nullptr) {
        std::cerr << "Error loading aircraft texture: " << SDL_GetError() << "\n";
        return;
    }

    // Get the dimensions of the aircraft texture
    int texture_width = 32, texture_height=32;
    SDL_QueryTexture(aircraftTexture, nullptr, nullptr, &texture_width, &texture_height);

    // Calculate the position for the aircraft image (centered around the coordinates)
    SDL_Rect renderQuad = { screen_x - texture_width / 2, screen_y - texture_height / 2, texture_width, texture_height };

    // Set aircraft color (based on the force)
    if (force == "Blue") {
        SDL_SetTextureColorMod(aircraftTexture, 0, 0, 255); // Blue
    }
    else if (force == "Red") {
        SDL_SetTextureColorMod(aircraftTexture, 255, 0, 0); // Red
    }
    else {
        SDL_SetTextureColorMod(aircraftTexture, 255, 255, 255); // Default white
    }

    std::cout << "heading: " << get_heading() << std::endl;
    // Calculate the angle in radians and ensure it's correct
    float angle = get_heading(); // Aircraft's heading (in degrees)
    //angle = 90.0f - angle;  // Adjust so 0 degrees points north
    //if (angle < 0.0f) {
    //    angle += 360.0f;  // Ensure heading stays within 0-360 range
    //}

    // Rotate the aircraft image based on its heading (rotate around its center)
    SDL_RenderCopyEx(renderer, aircraftTexture, nullptr, &renderQuad, angle, nullptr, SDL_FLIP_NONE);


    // Free the texture after rendering
    SDL_DestroyTexture(aircraftTexture);

    // Draw the heading line
    //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White

    //// Calculate the endpoint of the heading line
    //float line_length = 30.0f; // Length of the heading line in pixels
    //float heading_radians = get_heading() * M_PI / 180.0f;  // Convert heading from degrees to radians

    //// Adjust for SDL's coordinate system, flip Y-axis
    //int end_x = static_cast<int>(screen_x + line_length * std::cos(heading_radians));  // X stays as is
    //int end_y = static_cast<int>(screen_y - line_length * std::sin(heading_radians)); // Flip the Y-axis for SDL

    //// Draw the heading line
    //SDL_RenderDrawLine(renderer, screen_x, screen_y, end_x, end_y);

    std::pair<int, int> target_coordinates = coordinateSystem.to_screen_coordinates(target_latitude, target_longitude);
    int target_x = target_coordinates.first;
    int target_y = target_coordinates.second;

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Green for target line
    // SDL_RenderDrawLine(renderer, 1067/2, 600/2, target_x, target_y);
    SDL_RenderDrawLine(renderer, screen_x, screen_y, target_x, target_y);
}



void Aircraft::update(SDL_Renderer* renderer) {
    if (!is_alive()) {
        std::cout << name << " is destroyed and cannot perform updates.\n";
        return;
    }

    // Update the position if the aircraft is moving
    update_position();
    draw(renderer);
}
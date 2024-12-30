#include "Aircraft.h"
#include <iostream>
// #include <SDL.h>
#include <cmath> // For sin and cos

// Constructor
Aircraft::Aircraft(const std::string& name, const std::string& force, int health, float startX, float startY)
    : name(name), health(health), x(startX), y(startY) {
    validate_force(force);
    this->force = force;
    this->heading = 0.0f;
}

void Aircraft::set_heading(float new_heading) {
    heading = new_heading;
}

float Aircraft::get_heading() const {
    return heading;
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

float Aircraft::get_x() const {
    return x;
}

float Aircraft::get_y() const {
    return y;
}

// Methods
void Aircraft::move_to(float newX, float newY) {
    x = newX;
    y = newY;
}

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
    if (target.health < 0) {
        target.health = 0; // Ensure health does not go below 0
    }
    std::cout << name << " attacked " << target.get_name() << "!\n";
}

void Aircraft::defend() {
    if (!is_alive()) {
        std::cout << name << " cannot defend because it is destroyed.\n";
        return;
    }
    health += 5; // Example defense healing value
    if (health > 100) {  // Assume 100 is max health
        health = 100;
    }
    std::cout << name << " is defending and restored health to " << health << ".\n";
}

bool Aircraft::is_alive() const {
    return health > 0;
}


void Aircraft::draw(SDL_Renderer* renderer) const {
    // Draw the aircraft body (as a rectangle for simplicity)
    SDL_Rect rect = { static_cast<int>(x - 10), static_cast<int>(y - 10), 20, 20 };
    // Set color based on force
    if (force == "Blue") {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // Blue
    }
    else if (force == "Red") {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red
    }
    SDL_RenderFillRect(renderer, &rect);

    // Draw the heading line
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white color for heading line

    // Calculate the endpoint of the heading line
    float line_length = 30.0f; // Length of the heading line
    float angle_radians = heading * M_PI / 180.0f; // Convert degrees to radians

    int x_end = static_cast<int>(x + line_length * cos(angle_radians));
    int y_end = static_cast<int>(y - line_length * sin(angle_radians)); // SDL's y-axis is inverted

    // Draw the line
    SDL_RenderDrawLine(renderer, static_cast<int>(x), static_cast<int>(y), x_end, y_end);
}

void Aircraft::wrap_around_screen(int screen_width, int screen_height) {
    // Adjust the screen width/height logic to avoid aircraft disappearing partially.
    if (x < 0) {
        x = screen_width - 15;  // Wrap to the right side of the screen
    }
    else if (x > screen_width) {
        x = 0;  // Wrap to the left side of the screen
    }

    if (y < 0) {
        y = screen_height - 15;  // Wrap to the bottom
    }
    else if (y > screen_height) {
        y = 0;  // Wrap to the top
    }
}

void Aircraft::validate_force(const std::string& force) {
    if (force != "Red" && force != "Blue") {
        std::cerr << "Invalid force value: " << force << ". Force must be 'Red' or 'Blue'.\n";
        this->force = "Blue";  // Default to Blue if invalid
    }
}

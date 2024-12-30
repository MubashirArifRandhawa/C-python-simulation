#pragma once

#include <string>
#include <SDL.h>

class Aircraft {
private:
    std::string name;
    std::string force; // "Red" or "Blue"
    int health;
    float x, y;

public:
    // Constructor
    Aircraft(const std::string& name, const std::string& force, int health, float startX, float startY);

    // Getters
    std::string get_name() const;
    std::string get_force() const;
    int get_health() const;
    float get_x() const;
    float get_y() const;

    // Methods
    void move_to(float x, float y);
    void attack(Aircraft& target);
    void defend();
    bool is_alive() const;
    void draw(SDL_Renderer* renderer) const;
    void wrap_around_screen(int screen_width, int screen_height);

private:
    // Helper function to validate force input
    void validate_force(const std::string& force);
};

//#pragma once
//#include <SDL.h>
//#include <SDL_image.h>
//#include "Button.h"
//#include "INIReader.h"
//#include <functional>
//#include <iostream>
//#include "enums.h"
//#include <string>
//
//class Button {
//
//    // Button structure
//    SDL_Rect rect;                     // Button rectangle (position and size)
//    std::string force;                   // Button color
//    SimulationObjectType buttonType;
//    std::string iconPath;
//public:
//    std::function<void()> onClick;     // Callback for button click
//
//    Button(SDL_Rect rect, std::string force, SimulationObjectType buttonType, std::function<void()> onClick)
//    : rect(rect), force(force), buttonType(buttonType), onClick(onClick)
//    {
//        iconPath = readIni(buttonType);
//
//    }
//
//    void render(SDL_Renderer* renderer) const {
//        //Load the aircraft image
//        SDL_Texture* button = IMG_LoadTexture(renderer, iconPath.c_str());
//
//        if (button == nullptr) {
//            std::cerr << "Error loading aircraft texture: " << SDL_GetError() << "\n";
//            return;
//        }
//
//        // Get the dimensions of the aircraft texture
//        int texture_width = 32, texture_height = 32;
//        SDL_QueryTexture(button, nullptr, nullptr, &texture_width, &texture_height);
//
//        // Calculate the render rectangle for the image
//        SDL_Rect renderQuad = {
//            rect.x + (rect.w - texture_width) / 2, // Center the image horizontally within the button
//            rect.y + (rect.h - texture_height) / 2, // Center the image vertically within the button
//            texture_width,
//            texture_height
//        };
//
//        // Set aircraft color modulation based on force (optional)
//        if (force == "Blue") {
//            SDL_SetTextureColorMod(button, 30, 144, 255); // Blue
//            //SDL_SetRenderDrawColor(renderer, 30, 144, 255, 255); // White border (adjust as needed)
//        }
//        else if (force == "Red") {
//            SDL_SetTextureColorMod(button, 220, 20, 60); // Red
//            //SDL_SetRenderDrawColor(renderer, 220, 20, 60, 255); // White border (adjust as needed)
//        }
//        else if (force == "Green") {
//            SDL_SetTextureColorMod(button, 80, 200, 120); // Red
//            //SDL_SetRenderDrawColor(renderer, 80, 200, 120, 255); // White border (adjust as needed)
//        }
//
//        // Render the texture
//        SDL_RenderCopy(renderer, button, nullptr, &renderQuad);
//
//        // Clean up texture after rendering
//        SDL_DestroyTexture(button);
//
//
//        // Draw the border around the button
//        SDL_RenderDrawRect(renderer, &rect);
//    }
//
//
//    bool isClicked(int x, int y) const {
//        return x >= rect.x && x <= rect.x + rect.w &&
//            y >= rect.y && y <= rect.y + rect.h;
//    }
//
//    std::string readIni(SimulationObjectType buttonType) {
//        INIReader reader("../assets/files/paths.ini");
//
//        if (reader.ParseError() < 0) {
//            std::cout << "Can't load 'paths.ini'\n";
//            return "error";
//        }
//
//        if (buttonType == SimulationObjectType::Aircraft)
//            return reader.Get("ICONS", "aircraft", "default_value");
//        else if (buttonType == SimulationObjectType::Waypoint)
//            return reader.Get("ICONS", "waypoint", "default_value");
//        else if (buttonType == SimulationObjectType::Unknown)
//            return reader.Get("ICONS", "plan", "default_value");
//
//        // Add a fallback to handle unexpected values
//        return "default_value";
//    }
//};

#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Button.h"
#include "INIReader.h"
#include <functional>
#include <iostream>
#include "enums.h"
#include <string>
#include "FileLoader.cpp"

class Button {
    SDL_Rect rect;                      // Button rectangle (position and size)
    std::string force;                  // Button color
    SimulationObjectType buttonType;    // Type of the button (Aircraft, Waypoint, etc.)
    std::string iconPath;               // Path to the button icon

public:
    std::function<void()> onClick;      // Callback for button click
    Button(const SDL_Rect& rect, std::string force, SimulationObjectType buttonType, std::function<void()> onClick)
        : rect(rect), force(std::move(force)), buttonType(buttonType), onClick(std::move(onClick)) {
        iconPath = FileLoader::getButtonTexture(buttonType);
    }

    void render(SDL_Renderer* renderer) const {
        SDL_Texture* buttonTexture = loadTexture(renderer, iconPath);
        if (!buttonTexture) return;

        // Calculate the render rectangle for the image
        SDL_Rect renderQuad = {
            rect.x + (rect.w - 32) / 2,  // Center horizontally
            rect.y + (rect.h - 32) / 2,  // Center vertically
            32, 32                       // Fixed size (adjust if needed)
        };

        // Apply color modulation
        applyColorMod(buttonTexture);

        // Render the texture
        SDL_RenderCopy(renderer, buttonTexture, nullptr, &renderQuad);

        // Draw button border
        //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White border
        SDL_RenderDrawRect(renderer, &rect);

        // Clean up texture
        SDL_DestroyTexture(buttonTexture);
    }

    bool isClicked(int x, int y) const {
        return x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h;
    }

private:
    static SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& path) {
        SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
        if (!texture) {
            std::cerr << "Error loading texture (" << path << "): " << SDL_GetError() << "\n";
        }
        return texture;
    }

    void applyColorMod(SDL_Texture* texture) const {
        if (force == "Blue") {
            SDL_SetTextureColorMod(texture, 30, 144, 255); // Blue
        }
        else if (force == "Red") {
            SDL_SetTextureColorMod(texture, 220, 20, 60);  // Red
        }
        else if (force == "Green") {
            SDL_SetTextureColorMod(texture, 80, 200, 120); // Green
        }
    }

};

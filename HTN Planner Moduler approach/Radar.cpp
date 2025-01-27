#include "Radar.h"
#include "Aircraft.h"


Radar::Radar() : radarRadius(100), radarAngle(45.0f) {

}

Radar::Radar(int radius, float angle) : radarRadius(radius), radarAngle(angle) {

}


std::vector<std::reference_wrapper<Aircraft>> Radar::getEntitiesInRadarCone(std::vector<Aircraft>& entities, int centerX, int centerY, float heading) {
    std::vector<std::reference_wrapper<Aircraft>> entitiesInCone;

    // Convert heading and angles to radians
    float radarHeadingRad = normalizeAngle(heading * M_PI / 180.0f);
    float leftBound = normalizeAngle(radarHeadingRad - (radarAngle * M_PI / 180.0f));
    float rightBound = normalizeAngle(radarHeadingRad + (radarAngle * M_PI / 180.0f));

    for (auto& entity : entities) {
        // Calculate the vector from radar center to the entity
        std::pair<int, int> screen_coordinates = entity.get_position_xy();
        int dx = screen_coordinates.first - centerX;
        int dy = screen_coordinates.second - centerY;

        // Calculate the distance from the center
        float distance = std::sqrt(dx * dx + dy * dy);

        // Ignore entities outside the radar radius
        if (distance > radarRadius) {
            continue;
        }

        // Calculate the angle of the entity relative to the radar center
        float entityAngle = std::atan2(dy, dx);
        entityAngle = normalizeAngle(entityAngle);

        // Check if the entity is within the cone's angle range
        bool isInCone = false;
        if (leftBound <= rightBound) {
            isInCone = (entityAngle >= leftBound && entityAngle <= rightBound);
        }
        else {
            // Handle cone crossing 0/2π boundary
            isInCone = (entityAngle >= leftBound || entityAngle <= rightBound);
        }

        if (isInCone) {
            entitiesInCone.push_back(entity);
        }
    }

    return entitiesInCone;

}

// Helper function to normalize an angle to the range [0, 2π]
float Radar::normalizeAngle(float angle) {
    while (angle < 0) angle += 2 * M_PI;
    while (angle >= 2 * M_PI) angle -= 2 * M_PI;
    return angle;
}

void Radar::drawRadarCone(SDL_Renderer* renderer, int centerX, int centerY, float heading) const {


    // Convert heading and angles to radians
    float headingRad = heading * M_PI / 180.0f;
    float leftEdgeRad = headingRad - (radarAngle * M_PI / 180.0f);
    float rightEdgeRad = headingRad + (radarAngle * M_PI / 180.0f);

    // Calculate the end points of the cone edges
    int leftX = centerX + radarRadius * cos(leftEdgeRad);
    int leftY = centerY + radarRadius * sin(leftEdgeRad);
    int rightX = centerX + radarRadius * cos(rightEdgeRad);
    int rightY = centerY + radarRadius * sin(rightEdgeRad);

    // Set radar boundary color (yellow)
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

    // Draw the cone's boundary edges
    SDL_RenderDrawLine(renderer, centerX, centerY, leftX, leftY); // Left edge
    SDL_RenderDrawLine(renderer, centerX, centerY, rightX, rightY); // Right edge

    // Optionally, draw the arc connecting the edges to complete the cone boundary
    const int segments = 50; // Number of segments for smoothness
    for (int i = 0; i < segments; ++i) {
        float t1 = leftEdgeRad + (i / static_cast<float>(segments)) * (rightEdgeRad - leftEdgeRad);
        float t2 = leftEdgeRad + ((i + 1) / static_cast<float>(segments)) * (rightEdgeRad - leftEdgeRad);
        int arcX1 = centerX + radarRadius * cos(t1);
        int arcY1 = centerY + radarRadius * sin(t1);
        int arcX2 = centerX + radarRadius * cos(t2);
        int arcY2 = centerY + radarRadius * sin(t2);
        SDL_RenderDrawLine(renderer, arcX1, arcY1, arcX2, arcY2);
    }
}


void Radar::drawRadarCone(SDL_Renderer* renderer, int x, int y, float radius, float orientation, float angle, SDL_Color color) {
    // Set the radar cone color
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // Calculate start and end angles of the cone
    float halfAngle = angle / 2.0f;
    float startAngle = orientation - halfAngle;
    float endAngle = orientation + halfAngle;

    // Convert angles to radians
    float startAngleRad = startAngle * M_PI / 180.0f;
    float endAngleRad = endAngle * M_PI / 180.0f;

    // Number of points to approximate the cone
    const int NUM_POINTS = 100;
    float angleStep = (endAngleRad - startAngleRad) / NUM_POINTS;

    // Draw lines to form the cone
    for (int i = 0; i <= NUM_POINTS; ++i) {
        float theta = startAngleRad + i * angleStep;
        int pointX = x + static_cast<int>(radius * cos(theta));
        int pointY = y + static_cast<int>(radius * sin(theta));

        // Draw a line from the aircraft to the calculated point
        SDL_RenderDrawLine(renderer, x, y, pointX, pointY);

        // Fill the area by drawing triangles (optional)
        if (i > 0) {
            int prevPointX = x + static_cast<int>(radius * cos(theta - angleStep));
            int prevPointY = y + static_cast<int>(radius * sin(theta - angleStep));

            SDL_Vertex vertices[3] = {
                { SDL_FPoint{float(x), float(y)}, color, SDL_FPoint{0, 0} },
                { SDL_FPoint{float(prevPointX), float(prevPointY)}, color, SDL_FPoint{0, 0} },
                { SDL_FPoint{float(pointX), float(pointY)}, color, SDL_FPoint{0, 0} }
            };

            SDL_RenderGeometry(renderer, nullptr, vertices, 3, nullptr, 0);
        }
    }
}

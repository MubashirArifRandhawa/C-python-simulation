#include "HTNCore.h"
#include <iostream>

std::vector<std::string> AttackPlanner::plan(Aircraft& aircraft) {
    return { "Identify target", "Move to target", "Attack" };
}

std::vector<std::string> DefendPlanner::plan(Aircraft& aircraft) {
    return { "Identify threats", "Reposition to defensive location", "Defend" };
}

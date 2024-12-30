#pragma once
#ifndef HTNCORE_H
#define HTNCORE_H

#include "Aircraft.h"
#include <vector>
#include <string>

class HTNPlanner {
public:
    virtual ~HTNPlanner() {}

    virtual std::vector<std::string> plan(Aircraft& aircraft) = 0; // Abstract task generation
};

class AttackPlanner : public HTNPlanner {
public:
    std::vector<std::string> plan(Aircraft& aircraft) override;
};

class DefendPlanner : public HTNPlanner {
public:
    std::vector<std::string> plan(Aircraft& aircraft) override;
};

#endif

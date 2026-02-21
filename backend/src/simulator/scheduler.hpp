#pragma once
#include "model.hpp"
#include <vector>

namespace simulator {

    struct Request {
        SimConfigurtation config;
        std::vector<TaskSpecification> tasks;
    };

    SimulationResult run_simulation(const Request& request);

}
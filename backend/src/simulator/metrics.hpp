#pragma once
#include "model.hpp"
#include <vector>

namespace simulator {

    Metrics compute_metrics(const std::vector<Slice>& timeline, 
        const std::vector<TaskSpecification>& tasks, int cpus);

}
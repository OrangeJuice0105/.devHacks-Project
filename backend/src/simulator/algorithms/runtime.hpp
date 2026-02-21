#pragma once
#include "simulator/model.hpp"

namespace simulator {

    struct Runtime {
        int pid{};
        tick_t arrival{};
        int priority{};
        std::vector<tick_t> cpu;
        std::vector<tick_t> io;
        int cpuIndex{0};
        int ioIndex{0};
        tick_t cpuLeft{0};
        tick_t wakeAt{-1};
        bool done{false};
    };

};

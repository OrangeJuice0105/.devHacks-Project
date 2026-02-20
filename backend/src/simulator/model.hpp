#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <optional>

namespace simulator {

    using tick_t = int32_t;

    enum class Algorithm {
        FCFS,
        RR,
        SJF
    };

    enum class StopType {
        DONE,
        QUANTUM,
        IO,
        PREEMPT,
        IDLE
    };

    struct TaskSpecification {
        int pid{};
        tick_t arrival{};
        int priority{0};
        std::vector<tick_t> cpuBursts;
        std::vector<tick_t> ioBursts;
    };

    struct SimConfigurtation {
        int cpus{1};
        tick_t timeQuantum{4};
        Algorithm algo{Algorithm::FCFS};
    };

    struct Slice {
        int cpu{0};
        int pid{-1};
        tick_t start{0};
        tick_t end{0};
        StopType reason{StopType::IDLE};
    };

    struct Metrics {
        double avgWaitTime{0};
        double avgTurnaroundTime{0};
        double cpuUtil{0};
        int contextSwitches{0};
        int completed{0};
        tick_t makespan{0};
    };

    struct SimulationResult {
        std::vector<Slice> timeline;
        Metrics metrics;
    };

}
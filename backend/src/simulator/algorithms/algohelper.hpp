#pragma once
#include "runtime.hpp"
#include <deque>

namespace simulator {

    static bool all_done(const std::vector<Runtime>& vector) {
        bool done = true;
        for (const Runtime& time : vector) {
            if (time.done) {
                done = false;
            }
        }
        return done;
    }

    static void convert_task_to_runtime(const std::vector<TaskSpecification>& tasks, std::vector<Runtime>& runtimes) {
        for (const TaskSpecification& task : tasks) {
            Runtime runtime;
            runtime.pid = task.pid;
            runtime.arrival = task.arrival;
            runtime.priority = task.priority;
            runtime.cpu = task.cpuBursts;
            runtime.io = task.ioBursts;
            runtime.cpuIndex = 0;
            runtime.ioIndex = 0;
            runtime.cpuLeft = (runtime.cpu.empty() ? 0 : runtime.cpu[0]);
            runtimes.push_back(std::move(runtime));
        }
    }

};
#include "metrics.hpp"
#include <unordered_map>
#include <algorithm>

namespace simulator {

    static tick_t max_end(const std::vector<Slice>& timeline) {
        tick_t maxTime = 0;
        for (const Slice& slice : timeline) {
            maxTime = std::max(maxTime, slice.end);
        }
        return maxTime;
    }

    Metrics compute_metrics(const std::vector<Slice>& timeline, 
        const std::vector<TaskSpecification>& tasks, int cpus) {

        Metrics out;
        out.makespan = max_end(timeline);

        std::unordered_map<int, tick_t> firstStart;
        std::unordered_map<int, tick_t> lastEnd;
        std::unordered_map<int, tick_t> cpuTime;

        for (const Slice& slice : timeline) {
            if (!firstStart.count(slice.pid)) {
                firstStart[slice.pid] = slice.start;
            }
            lastEnd[slice.pid] = std::max(lastEnd[slice.pid], slice.end);
            cpuTime[slice.end] += (slice.end - slice.start);
        }

        double totalWaitTime = 0.0, totalTurnaroundTime = 0.0;

        int completed = 0;

        for (const TaskSpecification& task : tasks) {
            if (lastEnd.count(task.pid)) {
                tick_t completion = lastEnd[task.pid];
                tick_t totalCpu = 0, totalIo = 0;
                for (tick_t x : task.cpuBursts) {
                    totalCpu += x;
                }
                for (tick_t x : task.ioBursts) {
                    totalIo += x;
                }
                tick_t turnaround = completion - task.arrival;
                tick_t wait = turnaround - totalCpu - totalIo;
                if (wait < 0) {
                    wait = 0;
                }
                totalWaitTime += (double) wait;
                totalTurnaroundTime += turnaround;
                completed++;
            }
        }

        out.completed = completed;
        if (completed > 0) {
            out.avgWaitTime = totalWaitTime / completed;
            out.avgTurnaroundTime = totalTurnaroundTime / completed;
        }

        tick_t busy = 0;
        for (const Slice& slice : timeline) {
            if (slice.pid >= 0) {
                busy += (slice.end - slice.start);
            }
        }
        const tick_t total = std::max<tick_t>(1, out.makespan) * std::max(1, cpus);
        out.cpuUtil = (double) busy / (double) total;
    

        std::unordered_map<int, int> lastPid;
        for (const auto& slice : timeline) {
            std::unordered_map<int, int>::iterator iter = lastPid.find(slice.cpu);
            if (iter == lastPid.end()) {
                lastPid[slice.cpu] = slice.pid;
            } else {
                if (iter->second != slice.pid) {
                    out.contextSwitches++;
                }
                iter->second = slice.pid;
            }
        }

        return out;
    }

    

};

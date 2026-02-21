#include "algohelper.hpp"
#include <deque>
#include <algorithm>

namespace simulator {

    void push_events_now(std::vector<Runtime>& runtimes, std::deque<int>& ready, tick_t now) {
        for (int i = 0; i < (int)runtimes.size(); i++) {
            Runtime& runtime = runtimes[i];
            if (!runtime.done) {
                if (runtime.arrival == now) {
                    ready.push_back(i);
                }
                if (runtime.wakeAt == now) {
                    runtime.wakeAt = -1;
                    ready.push_back(i);
                }
            }
        }
    }

    std::vector<Slice> run_rr(const std::vector<TaskSpecification>& tasks, int cpus, tick_t quantum) {
        
        (void)cpus;
        std::vector<Runtime> runtimes;
        runtimes.reserve(tasks.size());

        convert_task_to_runtime(tasks, runtimes);

        std::deque<int> ready;
        tick_t now = 0;
        std::vector<Slice> timeline;
        
        tick_t first = INT32_MAX;

        for (Runtime& runtime : runtimes) {
            first = std::min(first, runtime.arrival);
        }

        if (first == INT32_MAX) {
            return timeline;
        }

        now = first;

        while (!all_done(runtimes)) {
            push_events_now(runtimes, ready, now);
            if (ready.empty()) {
                tick_t next = INT32_MAX;
                for (Runtime& runtime : runtimes) {
                    if (!runtime.done) {
                        if (runtime.arrival > now) {
                            next = std::min(next, runtime.arrival);
                        }
                        if (runtime.wakeAt > now) {
                            next = std::min(next, runtime.wakeAt);
                        }
                    }
                }
                if (next != INT32_MAX) {
                    if (next > now) {
                        timeline.push_back(Slice{.cpu = 0, .pid = -1, .start = now, .end = next, .reason = StopType::IDLE});
                        now = next;
                    }
                }
            } else {
                int index = ready.front();
                ready.pop_front();
                Runtime& runtime = runtimes[index];
                if (!runtime.done) {

                    tick_t start = now;
                    tick_t runFor = std::min<tick_t>(runtime.cpuLeft, quantum);
                    tick_t end = start + runFor;

                    runtime.cpuLeft -= runFor;
                    now = end;

                    StopType reason = StopType::QUANTUM;

                    if (runtime.cpuLeft == 0) {
                        runtime.cpuIndex++;
                        if (runtime.cpuIndex >= (int)runtime.cpu.size()) {
                            runtime.done = true;
                            reason = StopType::DONE;
                        } else {
                            tick_t ioLen = (runtime.ioIndex < (int)runtime.io.size()) ? runtime.io[runtime.ioIndex++] : 0;
                            runtime.wakeAt = now + ioLen;
                            runtime.cpuLeft = runtime.cpu[runtime.cpuIndex];
                            reason = StopType::IO;
                        }
                    }
                    timeline.push_back(Slice{.cpu = 0, .pid = runtime.pid,.start = start,.end = end,.reason = reason});
                
                    push_events_now(runtimes, ready, now);
                    if (!runtime.done && runtime.wakeAt < 0 && runtime.cpuLeft > 0 && reason == StopType::QUANTUM) {
                        ready.push_back(index);
                    }

                }
            }
        }

        std::vector<Slice> merged;
        for (const Slice& slice : timeline) {
            if (!merged.empty() && merged.back().cpu == slice.cpu && merged.back().pid == slice.pid && merged.back().end == slice.start) {
                merged.back().end = slice.end;
                merged.back().reason = slice.reason;
            } else {
                merged.push_back(slice);
            }
        }
        return merged;

    }
};
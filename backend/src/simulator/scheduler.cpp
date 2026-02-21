#include "scheduler.hpp"
#include "metrics.hpp"
#include "model.hpp"

namespace simulator {
    std::vector<Slice> run_fcfs(const std::vector<TaskSpecification>& tasks, int cpus);
    std::vector<Slice> run_rr(const std::vector<TaskSpecification>& tasks, int cpus, tick_t quantum);
}

namespace simulator {

    SimulationResult run_simulation(const Request& req) {
        
        std::vector<Slice> tl;

        switch (req.config.algo) {
            case Algorithm::FCFS:
                tl = run_fcfs(req.tasks, req.config.cpus);
                break;
            case Algorithm::RR:
                tl = run_rr(req.tasks, req.config.cpus, req.config.timeQuantum);
                break;
        }

        SimulationResult out;
        out.timeline = std::move(tl);
        out.metrics = compute_metrics(out.timeline, req.tasks, req.config.cpus);
        return out;
    }

} 
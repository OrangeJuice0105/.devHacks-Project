#include "httplib.h"
#include "json.hpp"

#include "simulator/scheduler.hpp"
#include "simulator/model.hpp"

#include <iostream>
#include <fstream>
#include <string>

#ifdef _WIN32
    #include <winsock2.h>
    #include <Windows.h>
    #define HOST_NAME_MAX 40
    // Link with Ws2_32.lib if using winsock functions like gethostname on older Windows
    // For GetComputerName, no extra linking is generally needed.
#else
    #include <unistd.h>
    #include <limits.h> // For HOST_NAME_MAX
#endif

using json = nlohmann::json;

static simulator::Algorithm parse_algo(const std::string& str) {
    if (str == "FCFS") return simulator::Algorithm::FCFS;
    else if (str == "RR") return simulator::Algorithm::RR;
    else throw std::invalid_argument("Wrong scheduling algorithm");
}

static std::string get_reason(simulator::StopType reason) {
    switch (reason) {
        case simulator::StopType::DONE: return "DONE";
        case simulator::StopType::QUANTUM: return "QUANTUM";
        case simulator::StopType::IO: return "IO";
        case simulator::StopType::PREEMPT: return "PREEMPT";
        case simulator::StopType::IDLE: return "IDLE";
        default: throw std::invalid_argument("Wrong reason");
    }
}

int main(int argc, const char *argv[]) {
    httplib::Server application;

    application.set_mount_point("/", "./public");

    application.Options(R"(/api/.*)", [](const httplib::Request&, httplib::Response& result) {
        result.set_header("Access-Control-Allow-Origin", "*");
        result.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        result.set_header("Access-Control-Allow-Headers", "Content-Type");
        result.status = 204;
    });

    application.Post("/api/simulate", [](const httplib::Request& request, httplib::Response& response) {
        response.set_header("Access-Control-Allow-Origin", "*");
        json body;
        try {
            body = json::parse(request.body);
        } catch (...) {
            response.status = 400;
            response.set_content(R"({"error":"Invalid JSON"})", "application/json");
            return;
        }

        simulator::Request req;
        json cfg = body.value("config", json::object());
        req.config.cpus = std::max(1, cfg.value("cpus", 1));
        req.config.timeQuantum = cfg.value("timeQuantum", 4);
        req.config.algo = parse_algo(cfg.value("algo", "FCFS"));

        json tasks = body.value("tasks", json::array());

        for (json& json_task : tasks) {
            simulator::TaskSpecification task;
            task.pid = json_task.value("pid", 0);
            task.arrival = json_task.value("arrival", 0);
            task.priority = json_task.value("priority", 0);

            if (json_task.contains("cpuBursts")) {
                for (auto& x : json_task["cpuBursts"]) {
                    task.cpuBursts.push_back(x.get<int>());
                }
            }
            if (json_task.contains("ioBursts")) {
                for (auto& x : json_task["ioBursts"]) {
                    task.ioBursts.push_back(x.get<int>());
                }
            }
            req.tasks.push_back(std::move(task));
        }

        simulator::SimulationResult output = simulator::run_simulation(req);

        json resp;
        resp["timeline"] = json::array();
        for (simulator::Slice& s : output.timeline) {
            json js;
            js["cpu"] = s.cpu;
            js["pid"] = s.pid;
            js["start"] = s.start;
            js["end"] = s.end;
            js["reason"] = get_reason(s.reason);
            resp["timeline"].push_back(js);
        }

        resp["metrics"] = {
            {"avgWaitTime", output.metrics.avgWaitTime},
            {"avgTurnaroundTime", output.metrics.avgTurnaroundTime},
            {"cpuUtil", output.metrics.cpuUtil},
            {"contextSwitches", output.metrics.contextSwitches},
            {"completed", output.metrics.completed},
            {"makespan", output.metrics.makespan}
        };

        response.set_content(resp.dump(4), "application/json");

        

    });

    application.Get(R"(/(.*))", [](const httplib::Request&, httplib::Response& response) {
        std::ifstream file("./public/index.html", std::ios::binary);
        if (!file) { 
            response.status = 404; 
            response.set_content("index.html not found", "text/plain");
        } else {
            std::string html((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            response.set_content(html, "text/html; charset=utf-8");
        }   
    });

    std::cout << "OS Arena backend listening on http://localhost:8080\n";
    const bool ok = application.listen("127.0.0.1", 8080);
    std::cerr << "listen() returned " << ok << "\n";

    return EXIT_SUCCESS;
}
#include "httplib.h"
#include "json.hpp"

#include "simulator/scheduler.hpp"
#include "simulator/model.hpp"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

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

    const std::string public_directory = (std::filesystem::current_path() / "public").string();
    const std::string assets_directory = (std::filesystem::current_path() / "public" / "assets").string();

    
    if (!application.set_mount_point("/", public_directory)) {
        std::cerr << "Directory does not exist\n";
        return EXIT_FAILURE;
    }

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
                for (json& x : json_task["cpuBursts"]) {
                    task.cpuBursts.push_back(x.get<int>());
                }
            }
            
            if (json_task.contains("ioBursts")) {
                for (json& x : json_task["ioBursts"]) {
                    task.ioBursts.push_back(x.get<int>());
                }
            }
            req.tasks.push_back(std::move(task));
        }

        simulator::SimulationResult output = simulator::run_simulation(req);

        json resp;
        resp["timeline"] = json::array();
        for (simulator::Slice& slice : output.timeline) {
            json js;
            js["cpu"] = slice.cpu;
            js["pid"] = slice.pid;
            js["start"] = slice.start;
            js["end"] = slice.end;
            js["reason"] = get_reason(slice.reason);
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

        response.set_content(resp.dump(2), "application/json");


    });

    application.Get("/", [](const httplib::Request& request, httplib::Response& response) {
        std::cout << "Received request for path: " << request.path << std::endl;
        std::ifstream file("index.html", std::ios::binary);
        if (!file) { 
            response.status = 404; 
            response.set_content("index.html not found", "text/plain");
        } else {
            std::string html((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            response.set_content(html, "text/html; charset=utf-8");
        }   
    });

    std::cout << "OS Arena backend listening on address http://localhost:8080\n";
    const bool ok = application.listen("127.0.0.1", 8080);
    std::cerr << "listen() returned " << ok << "\n";

    return EXIT_SUCCESS;
}
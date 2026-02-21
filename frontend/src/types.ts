export type Algorithm = "FCFS" | "RR";

export type TaskSpecification = {
    pid: number;
    arrival: number;
    priority: number;
    cpuBursts: number[];
    ioBursts: number[];
};

export type SimulatorConfiguration = {
    algo: Algorithm;
    cpus: number;
    timeQuantum: number;
};

export type Slice = {
    cpu: number;
    pid: number; 
    start: number;
    end: number;
    reason: "DONE" | "QUANTUM" | "IO" | "PREEMPT" | "IDLE";
};

export type Metrics = {
    avgWaitTime: number;
    avgTurnaroundTime: number;
    cpuUtil: number;
    contextSwitches: number;
    completed: number;
    makespan: number;
};

export type SimulatorResponse = {
    timeline: Slice[];
    metrics: Metrics;
};
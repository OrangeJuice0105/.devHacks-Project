import React, { useMemo, useState } from "react";
import { simulate } from "./api";
import type { Algorithm, SimulatorConfiguration, TaskSpecification, SimulatorResponse } from "./types";
import ControlPanel from "./components/ControlPanel";
import GanttSvg from "./components/GanttSvg";
import MetricsCards from "./components/MetricsCards";
import QueuePreview from "./components/QueuePreview";

const defaultTasks: TaskSpecification[] = [
    { pid: 1, arrival: 0, priority: 2, cpuBursts: [5, 3], ioBursts: [4] },
    { pid: 2, arrival: 1, priority: 1, cpuBursts: [8], ioBursts: [] },
    { pid: 3, arrival: 2, priority: 3, cpuBursts: [3, 4], ioBursts: [2] }
];

export default function App() {
    const [algo, setAlgo] = useState<Algorithm>("RR");
    const [cpus, setCpus] = useState(1);
    const [quantum, setQuantum] = useState(4);
    const [tasksJson, setTasksJson] = useState(JSON.stringify(defaultTasks, null, 2));
    const [result, setResult] = useState<SimulatorResponse | null>(null);
    const [err, setErr] = useState<string | null>(null);

    const config: SimulatorConfiguration = useMemo(() => ({
        algo,
        cpus,
        timeQuantum: quantum
    }), [algo, cpus, quantum]);

    async function onRun() {
        setErr(null);
        try {
            const tasks = JSON.parse(tasksJson) as TaskSpecification[];
            const r = await simulate(config, tasks);
            setResult(r);
        } catch (e: any) {
            setErr(e?.message ?? "Failed");
        }
    }

    return (
        <div className="min-h-screen bg-zinc-950 text-zinc-100">
        <div className="mx-auto max-w-7xl p-4">
            <header className="flex items-center justify-between gap-3 py-3">
            <div>
                <h1 className="text-2xl font-bold tracking-tight">OS Arena</h1>
                <p className="text-sm text-zinc-400">CPU scheduling simulator with a fancy timeline UI</p>
            </div>
            <button
                onClick={onRun}
                className="rounded-xl bg-zinc-100 px-4 py-2 text-zinc-950 font-semibold hover:opacity-90 active:opacity-80"
            >
                Run Simulation
            </button>
            </header>

            {err && (
            <div className="mt-2 rounded-xl border border-red-500/40 bg-red-500/10 p-3 text-sm">
                {err}
            </div>
            )}

            <div className="mt-4 grid grid-cols-12 gap-4">
            <div className="col-span-12 lg:col-span-4">
                <ControlPanel
                algo={algo}
                setAlgo={setAlgo}
                cpus={cpus}
                setCpus={setCpus}
                quantum={quantum}
                setQuantum={setQuantum}
                tasksJson={tasksJson}
                setTasksJson={setTasksJson}
                />
            </div>

            <div className="col-span-12 lg:col-span-8 space-y-4">
                <MetricsCards metrics={result?.metrics ?? null} />
                <div className="rounded-2xl border border-zinc-800 bg-zinc-900/40 p-4">
                <div className="flex items-center justify-between">
                    <h2 className="text-lg font-semibold">Timeline</h2>
                    <div className="text-xs text-zinc-400">
                    {result ? `makespan: ${result.metrics.makespan}` : "run to generate"}
                    </div>
                </div>
                <div className="mt-3">
                    <GanttSvg slices={result?.timeline ?? []} />
                </div>
                </div>

                <QueuePreview slices={result?.timeline ?? []} />
            </div>
            </div>
        </div>
        </div>
    );
}
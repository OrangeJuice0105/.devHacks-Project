import type { Algorithm } from "../types";

export default function ControlPanel(props: {
  algo: Algorithm; setAlgo: (a: Algorithm) => void;
  cpus: number; setCpus: (n: number) => void;
  quantum: number; setQuantum: (n: number) => void;
  tasksJson: string; setTasksJson: (s: string) => void;
}) {
  const { algo, setAlgo, cpus, setCpus, quantum, setQuantum, tasksJson, setTasksJson } = props;

  return (
    <div className="rounded-2xl border border-zinc-800 bg-zinc-900/40 p-4">
      <h2 className="text-lg font-semibold">Controls</h2>

      <div className="mt-4 space-y-3">
        <label className="block text-sm text-zinc-300">Algorithm</label>
        <select
          value={algo}
          onChange={(e) => setAlgo(e.target.value as Algorithm)}
          className="w-full rounded-xl bg-zinc-950 border border-zinc-800 px-3 py-2"
        >
          <option value="FCFS">FCFS</option>
          <option value="RR">Round Robin</option>
        </select>

        <div className="grid grid-cols-2 gap-3">
          <div>
            <label className="block text-sm text-zinc-300">CPUs</label>
            <input
              type="number"
              min={1}
              max={4}
              value={cpus}
              onChange={(e) => setCpus(Number(e.target.value))}
              className="w-full rounded-xl bg-zinc-950 border border-zinc-800 px-3 py-2"
            />
          </div>
          <div>
            <label className="block text-sm text-zinc-300">Quantum</label>
            <input
              type="number"
              min={1}
              max={50}
              value={quantum}
              onChange={(e) => setQuantum(Number(e.target.value))}
              disabled={algo !== "RR"}
              className="w-full rounded-xl bg-zinc-950 border border-zinc-800 px-3 py-2 disabled:opacity-50"
            />
          </div>
        </div>

        <div>
          <div className="flex items-center justify-between">
            <label className="block text-sm text-zinc-300">Workload (tasks JSON)</label>
            <span className="text-xs text-zinc-500">pid, arrival, priority, cpuBursts, ioBursts</span>
          </div>
          <textarea
            value={tasksJson}
            onChange={(e) => setTasksJson(e.target.value)}
            rows={14}
            className="mt-2 w-full rounded-xl bg-zinc-950 border border-zinc-800 px-3 py-2 font-mono text-xs"
          />
        </div>
      </div>
    </div>
  );
}
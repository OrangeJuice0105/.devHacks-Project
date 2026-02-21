import React from "react";
import type { Metrics } from "../types";

function Card({ title, value }: { title: string; value: string }) {
  return (
    <div className="rounded-2xl border border-zinc-800 bg-zinc-900/40 p-4">
      <div className="text-xs text-zinc-400">{title}</div>
      <div className="mt-1 text-xl font-semibold">{value}</div>
    </div>
  );
}

export default function MetricsCards({ metrics }: { metrics: Metrics | null }) {
  if (!metrics) {
    return (
      <div className="rounded-2xl border border-zinc-800 bg-zinc-900/40 p-4 text-sm text-zinc-400">
        Run a simulation to see metrics.
      </div>
    );
  }

  return (
    <div className="grid grid-cols-2 md:grid-cols-3 gap-3">
      <Card title="Avg Wait" value={metrics.avgWaitTime.toFixed(2)} />
      <Card title="Avg Turnaround" value={metrics.avgTurnaroundTime.toFixed(2)} />
      <Card title="CPU Util" value={(metrics.cpuUtil * 100).toFixed(1) + "%"} />
      <Card title="Context Switches" value={String(metrics.contextSwitches)} />
      <Card title="Completed" value={String(metrics.completed)} />
      <Card title="Makespan" value={String(metrics.makespan)} />
    </div>
  );
}
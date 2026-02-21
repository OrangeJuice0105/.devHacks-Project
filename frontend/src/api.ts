import type { SimulatorConfiguration, TaskSpecification, SimulatorResponse } from "./types";

const BASE = "";

export async function simulate(config: SimulatorConfiguration, tasks: TaskSpecification[]): Promise<SimulatorResponse> {
    const res = await fetch(`${BASE}/api/simulate`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ config, tasks }),
    });
    if (!res.ok) {
        throw new Error(`API error ${res.status}`);
    }
    return res.json();
}
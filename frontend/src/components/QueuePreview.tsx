import React, { useMemo } from "react";
import type { Slice } from "../types";

// Super simple “preview”: show which PIDs ever ran, and idle time share.
export default function QueuePreview({ slices }: { slices: Slice[] }) {
    const info = useMemo(() => {
        const ran = new Set<number>();
        let idle = 0;
        let total = 0;
        for (const s of slices) {
            const len = s.end - s.start;
            total += len;
            if (s.pid < 0) idle += len;
            else ran.add(s.pid);
        }
        return { ran: [...ran].sort((a, b) => a - b), idle, total };
    }, [slices]);

    return (
        <div className="rounded-2xl border border-zinc-800 bg-zinc-900/40 p-4">
        <h2 className="text-lg font-semibold">Quick stats</h2>
        <div className="mt-2 text-sm text-zinc-300">
            Ran: {info.ran.length ? info.ran.map(p => `P${p}`).join(", ") : "—"}
        </div>
        <div className="mt-1 text-sm text-zinc-400">
            Idle ticks: {info.idle} / {info.total || 0}
        </div>
        <p className="mt-3 text-xs text-zinc-500">
            (This panel is a placeholder—replace with live Ready/Blocked queue visualization if you add tick-by-tick events.)
        </p>
        </div>
    );
}
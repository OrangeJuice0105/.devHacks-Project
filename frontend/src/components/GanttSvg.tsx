import type { Slice } from "../types";

function hashColor(pid: number): string {
    if (pid < 0) return "rgba(255,255,255,0.08)";
    // deterministic “pretty enough” color via HSL
    const hue = (pid * 67) % 360;
    return `hsl(${hue} 80% 55%)`;
}

export default function GanttSvg({ slices }: { slices: Slice[] }) {
    const height = 120;
    const laneH = 28;
    const pad = 12;

    const makespan = slices.reduce((m, s) => Math.max(m, s.end), 0);
    const width = Math.max(640, makespan * 20 + pad * 2);

    const maxCpu = slices.reduce((m, s) => Math.max(m, s.cpu), 0);
    const lanes = maxCpu + 1;

    return (
        <div className="w-full overflow-x-auto rounded-xl border border-zinc-800 bg-zinc-950">
        <svg width={width} height={pad * 2 + lanes * laneH + 24}>
            {/* background grid */}
            {Array.from({ length: makespan + 1 }).map((_, t) => {
            const x = pad + t * 20;
            return (
                <g key={t}>
                <line x1={x} y1={pad} x2={x} y2={pad + lanes * laneH} stroke="rgba(255,255,255,0.06)" />
                {t % 5 === 0 && (
                    <text x={x + 2} y={pad + lanes * laneH + 16} fontSize="10" fill="rgba(255,255,255,0.35)">
                    {t}
                    </text>
                )}
                </g>
            );
            })}

            {/* lanes */}
            {Array.from({ length: lanes }).map((_, cpu) => (
            <text
                key={cpu}
                x={6}
                y={pad + cpu * laneH + 18}
                fontSize="11"
                fill="rgba(255,255,255,0.55)"
            >
                CPU {cpu}
            </text>
            ))}

            {/* slices */}
            {slices.map((s, i) => {
            const x = pad + s.start * 20;
            const w = Math.max(2, (s.end - s.start) * 20);
            const y = pad + s.cpu * laneH + 4;
            const color = hashColor(s.pid);

            const label = s.pid < 0 ? "idle" : `P${s.pid}`;

            return (
                <g key={i}>
                <rect x={x} y={y} width={w} height={laneH - 8} rx={8} fill={color} opacity={s.pid < 0 ? 1 : 0.9} />
                <text x={x + 8} y={y + 16} fontSize="11" fill="rgba(0,0,0,0.85)">
                    {label}
                </text>
                <title>{`cpu=${s.cpu} pid=${s.pid} [${s.start},${s.end}) reason=${s.reason}`}</title>
                </g>
            );
            })}
        </svg>
        </div>
    );
}
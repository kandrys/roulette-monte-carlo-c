#!/usr/bin/env python3
"""
Reads the binary ranking file (ranking.bin) from the roulette game
and generates charts: scores over time, score distribution, top players.
"""
import struct
from pathlib import Path
from datetime import datetime

# Try to import matplotlib
try:
    import matplotlib.pyplot as plt
    import matplotlib.dates as mdates
except ImportError:
    print("Install matplotlib: pip install matplotlib")
    raise SystemExit(1)

# Match C struct ScoreRecord: char name[50], int final_score, time_t game_date
# With typical padding: 50 bytes + 2 pad + 4 (int) + 8 (time_t) = 64 bytes
MAX_NAME = 50
RECORD_FMT = "50s 2x i q"  # 50s=name, 2x=padding, i=int, q=time_t (8 bytes)
RECORD_SIZE = struct.calcsize(RECORD_FMT)
RANKING_FILE = Path(__file__).resolve().parent / "ranking.bin"


def read_ranking(path=None):
    """Read ranking.bin and return list of (name, score, timestamp)."""
    path = path or RANKING_FILE
    if not path.exists():
        return []
    records = []
    with open(path, "rb") as f:
        while True:
            chunk = f.read(RECORD_SIZE)
            if len(chunk) < RECORD_SIZE:
                break
            name_bytes, score, ts = struct.unpack(RECORD_FMT, chunk)
            name = name_bytes.split(b"\x00")[0].decode("utf-8", errors="replace").strip()
            records.append((name, score, ts))
    return records


def main():
    records = read_ranking()
    if not records:
        print("No data in ranking.bin. Play the game first to generate records.")
        return

    names = [r[0] or "Anonymous" for r in records]
    scores = [r[1] for r in records]
    dates = [datetime.fromtimestamp(r[2]) for r in records]

    fig, axes = plt.subplots(2, 2, figsize=(12, 10))

    # 1) Scores over time (chronological)
    ax1 = axes[0, 0]
    ax1.plot(dates, scores, "o-", color="steelblue", markersize=6)
    ax1.set_xlabel("Date")
    ax1.set_ylabel("Final score")
    ax1.set_title("Scores over time (game order)")
    ax1.xaxis.set_major_formatter(mdates.DateFormatter("%Y-%m-%d"))
    ax1.xaxis.set_major_locator(mdates.AutoDateLocator())
    plt.setp(ax1.xaxis.get_majorticklabels(), rotation=45, ha="right")
    ax1.grid(True, alpha=0.3)

    # 2) Top scores (bar chart)
    ax2 = axes[0, 1]
    n = len(records)
    colors_bars = plt.cm.viridis([(n - i) / max(n, 1) for i in range(n)])
    bars = ax2.barh(range(n), scores, color=colors_bars)
    ax2.set_yticks(range(n))
    ax2.set_yticklabels(names, fontsize=9)
    ax2.set_xlabel("Final score")
    ax2.set_title("Ranking (top entries)")
    ax2.invert_yaxis()
    ax2.grid(True, alpha=0.3, axis="x")

    # 3) Score distribution (histogram)
    ax3 = axes[1, 0]
    ax3.hist(scores, bins=min(15, max(5, len(set(scores)))), color="teal", edgecolor="white", alpha=0.8)
    ax3.set_xlabel("Final score")
    ax3.set_ylabel("Count")
    ax3.set_title("Score distribution")
    ax3.grid(True, alpha=0.3, axis="y")

    # 4) Scores by date (sorted by time, as timeline)
    ax4 = axes[1, 1]
    ax4.scatter(dates, scores, c=scores, cmap="plasma", s=50, alpha=0.8)
    ax4.set_xlabel("Date")
    ax4.set_ylabel("Final score")
    ax4.set_title("Scores by date (color = score)")
    ax4.xaxis.set_major_formatter(mdates.DateFormatter("%Y-%m-%d"))
    ax4.xaxis.set_major_locator(mdates.AutoDateLocator())
    plt.setp(ax4.xaxis.get_majorticklabels(), rotation=45, ha="right")
    ax4.grid(True, alpha=0.3)

    plt.tight_layout()
    out_path = Path(__file__).resolve().parent / "ranking_analysis.png"
    plt.savefig(out_path, dpi=150, bbox_inches="tight")
    print(f"Saved: {out_path}")
    try:
        plt.show()
    except Exception:
        pass  # headless / no display


if __name__ == "__main__":
    main()

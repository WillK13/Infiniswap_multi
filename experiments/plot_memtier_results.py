#!/usr/bin/env python3
from __future__ import print_function

import sys
import os
import json

import matplotlib
matplotlib.use("Agg")  # safe on headless servers
import matplotlib.pyplot as plt


def find_section_with_key(obj, key_name):
    """
    Recursively search a nested JSON structure for a dict that has `key_name`.
    Returns obj[key_name] when found, otherwise None.
    """
    if isinstance(obj, dict):
        if key_name in obj:
            return obj[key_name]
        for v in obj.values():
            found = find_section_with_key(v, key_name)
            if found is not None:
                return found
    elif isinstance(obj, list):
        for v in obj:
            found = find_section_with_key(v, key_name)
            if found is not None:
                return found
    return None


def main(json_path):
    if not os.path.isfile(json_path):
        print("ERROR: File not found: %s" % json_path)
        sys.exit(1)

    # Load JSON
    with open(json_path, "r") as f:
        data = json.load(f)

    # Find the intervals map (the thing with "501", "502", ... "600")
    intervals = find_section_with_key(data, "Intervals")
    if intervals is None:
        print("ERROR: Could not find 'Intervals' section in JSON.")
        sys.exit(1)

    # Build time series
    # Keys are strings like "501", "502", ...; interpret as seconds since start.
    times = sorted(int(k) for k in intervals.keys())
    times_str = [str(t) for t in times]

    throughput = []  # requests / interval (Count)
    p50 = []
    p90 = []
    p99 = []

    for t_str in times_str:
        entry = intervals.get(t_str, {})
        throughput.append(entry.get("Count", 0))

        # Percentile fields may be missing occasionally; default to None
        p50.append(entry.get("p50.00"))
        p90.append(entry.get("p90.00"))
        p99.append(entry.get("p99.00"))

    print("Read %d intervals from %s" % (len(times), json_path))

    out_dir = os.path.dirname(os.path.abspath(json_path))
    base_name = os.path.splitext(os.path.basename(json_path))[0]

    # ---------- Plot 1: Throughput vs time ----------
    plt.figure()
    plt.plot(times, throughput)
    plt.xlabel("Time (seconds since start)")
    plt.ylabel("Throughput (requests / interval)")
    plt.title("Memtier Throughput Over Time")
    plt.grid(True)
    throughput_png = os.path.join(out_dir, base_name + "_throughput.png")
    plt.savefig(throughput_png, bbox_inches="tight")
    print("Saved throughput plot to %s" % throughput_png)

    # ---------- Plot 2: Latency percentiles vs time ----------
    plt.figure()
    # Filter out None values (if any)
    valid_t = []
    valid_p50 = []
    valid_p90 = []
    valid_p99 = []

    for i, t in enumerate(times):
        if p50[i] is None or p90[i] is None or p99[i] is None:
            continue
        valid_t.append(t)
        valid_p50.append(p50[i])
        valid_p90.append(p90[i])
        valid_p99.append(p99[i])

    if len(valid_t) == 0:
        print("WARNING: No valid percentile data found to plot.")
    else:
        plt.plot(valid_t, valid_p50, label="p50")
        plt.plot(valid_t, valid_p90, label="p90")
        plt.plot(valid_t, valid_p99, label="p99")
        plt.xlabel("Time (seconds since start)")
        plt.ylabel("Latency (milliseconds)")
        plt.title("Memtier Latency Percentiles Over Time")
        plt.legend()
        plt.grid(True)
        latency_png = os.path.join(out_dir, base_name + "_latencies.png")
        plt.savefig(latency_png, bbox_inches="tight")
        print("Saved latency plot to %s" % latency_png)


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 plot_memtier_results.py /path/to/memtier_results.json")
        sys.exit(1)
    main(sys.argv[1])


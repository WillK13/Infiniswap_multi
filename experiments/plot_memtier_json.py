#!/usr/bin/env python3
import sys
import json
import matplotlib.pyplot as plt

def is_interval_bucket_dict(d):
    """
    Heuristic: dict whose keys are all digits ("1","2",...) and whose
    values are dicts with 'Average Latency' and 'Count' fields.
    """
    if not isinstance(d, dict):
        return False
    if not d:
        return False

    keys = [k for k in d.keys() if isinstance(k, str)]
    if not keys:
        return False
    for k in keys:
        if not k.isdigit():
            return False

    first_val = d[keys[0]]
    if not isinstance(first_val, dict):
        return False
    if "Average Latency" not in first_val or "Count" not in first_val:
        return False

    return True


def find_interval_buckets(obj):
    """
    Recursively search for the per-second bucket dict.
    """
    if isinstance(obj, dict):
        if is_interval_bucket_dict(obj):
            return obj
        for v in obj.values():
            res = find_interval_buckets(v)
            if res is not None:
                return res
    elif isinstance(obj, list):
        for v in obj:
            res = find_interval_buckets(v)
            if res is not None:
                return res
    return None


def find_overall_percentiles(obj):
    """
    Recursively search for a dict that looks like the overall
    'Percentile Latencies' section with keys like 'p50.00'.
    """
    if isinstance(obj, dict):
        # Direct hit: a dict that *is* the Percentile Latencies
        if ("p50.00" in obj and
            "p90.00" in obj and
            "p99.00" in obj and
            isinstance(obj["p50.00"], (int, float))):
            return obj

        for k, v in obj.items():
            res = find_overall_percentiles(v)
            if res is not None:
                return res
    elif isinstance(obj, list):
        for v in obj:
            res = find_overall_percentiles(v)
            if res is not None:
                return res
    return None


def main():
    if len(sys.argv) != 2:
        print("Usage: python3 plot_memtier_json.py <memtier_json_file>")
        sys.exit(1)

    json_path = sys.argv[1]

    with open(json_path, "r") as f:
        data = json.load(f)

    # 1) Find per-second buckets
    buckets = find_interval_buckets(data)
    if buckets is None:
        print("Could not find per-second buckets in JSON.")
        sys.exit(1)

    # 2) Extract time series
    #    memtier's per-interval keys appear to be "1", "2", ..., "600"
    times = sorted([int(k) for k in buckets.keys()])
    times_str = [str(t) for t in times]

    p50 = []
    p90 = []
    p99 = []
    throughput_ops = []

    # We assume each bucket is 1 second long
    interval_seconds = 1.0

    for t_str in times_str:
        entry = buckets[t_str]

        # Latencies in milliseconds
        p50.append(entry.get("p50.00", 0.0))
        p90.append(entry.get("p90.00", 0.0))
        p99.append(entry.get("p99.00", 0.0))

        count = entry.get("Count", 0)
        throughput_ops.append(float(count) / interval_seconds)

    # 3) Try to get overall percentiles (for printing)
    overall = find_overall_percentiles(data)
    if overall is not None:
        print("Overall Percentile Latencies (ms):")
        print("  p50 = {0}".format(overall.get("p50.00")))
        print("  p90 = {0}".format(overall.get("p90.00")))
        print("  p99 = {0}".format(overall.get("p99.00")))
    else:
        print("Could not find overall Percentile Latencies in JSON.")

    # 4) Plot throughput vs time
    plt.figure()
    plt.plot(times, throughput_ops)
    plt.xlabel("Time (s)")
    plt.ylabel("Throughput (ops/s)")
    plt.title("Memtier Throughput Over Time")

    # 5) Plot p50/p90/p99 latency vs time
    plt.figure()
    plt.plot(times, p50, label="p50")
    plt.plot(times, p90, label="p90")
    plt.plot(times, p99, label="p99")
    plt.xlabel("Time (s)")
    plt.ylabel("Latency (ms)")
    plt.title("Memtier Latency Percentiles Over Time")
    plt.legend()

    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    main()


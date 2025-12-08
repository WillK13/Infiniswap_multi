#!/usr/bin/env bash
set -euo pipefail

RESULTS_ROOT="../results"
TS=$(date +%Y%m%d-%H%M%S)
OUTDIR="$RESULTS_ROOT/mixed_memc_redis_$TS"

echo "Creating results dir: $OUTDIR"
mkdir -p "$OUTDIR"

echo "Starting vmstat logging..."
vmstat 1 > "$OUTDIR/vmstat.txt" &
VMSTAT_PID=$!

# ---- MEMCACHED: memory-heavy workload ----
# Big values (32 KB), many keys, same style as your hog experiment
echo "Starting memtier for MEMCACHED..."
memtier_benchmark \
  -s 127.0.0.1 \
  -p 11211 \
  --protocol=memcache_text \
  --threads=2 \
  --clients=8 \
  --ratio=1:10 \
  --test-time=600 \
  --key-maximum=200000 \
  --data-size=32768 \
  --distinct-client-seed \
  --key-pattern=G:G \
  --hide-histogram \
  --print-percentiles=50,90,99 \
  --json-out-file="$OUTDIR/memtier_memcached.json" \
  > "$OUTDIR/memtier_memcached.log" 2>&1 &
PID_MEMC=$!

# ---- REDIS: more CPU-ish workload ----
# Smaller values, same duration, lots of ops
echo "Starting memtier for REDIS..."
memtier_benchmark \
  -s 127.0.0.1 \
  -p 6379 \
  --protocol=redis \
  --threads=2 \
  --clients=8 \
  --ratio=1:10 \
  --test-time=600 \
  --data-size=1024 \
  --distinct-client-seed \
  --key-pattern=G:G \
  --hide-histogram \
  --print-percentiles=50,90,99 \
  --json-out-file="$OUTDIR/memtier_redis.json" \
  > "$OUTDIR/memtier_redis.log" 2>&1 &
PID_REDIS=$!

echo "Waiting for both memtier workloads to finish..."
wait "$PID_MEMC"
wait "$PID_REDIS"

echo "Stopping vmstat..."
kill "$VMSTAT_PID" || true

echo "Done. Results in: $OUTDIR"


#!/usr/bin/env bash
set -euo pipefail

# Configurable knobs
SERVER_HOST="127.0.0.1"
SERVER_PORT="11211"
TEST_TIME=600          # seconds (10 minutes) so we have time to see swapping
CLIENTS=10
THREADS=4
RATIO="1:10"           # read:write ratio (example: mostly reads)
OUTDIR="${HOME}/infiniswap_exps/results"

TIMESTAMP="$(date +%Y%m%d-%H%M%S)"
RUN_DIR="${OUTDIR}/memtier_swap_${TIMESTAMP}"

mkdir -p "${RUN_DIR}"

echo "=== Memtier swap test starting ==="
echo "Run directory: ${RUN_DIR}"

# 1) Log current free/memory info at start
free -m > "${RUN_DIR}/start_free_${TIMESTAMP}.txt"
vmstat 1 > "${RUN_DIR}/vmstat_${TIMESTAMP}.txt" &
VMSTAT_PID=$!

# 2) Run memtier with CSV output so we can graph later
#   --show-intervals: per-second (or per period) stats
#   --csv: machine-readable
#   --test-time: duration
#   --key-pattern: sets/gets distribution
#   --ratio: read/write mix
#
# You can tune these as needed.
memtier_benchmark \
  -s "${SERVER_HOST}" \
  -p "${SERVER_PORT}" \
  --protocol=memcache_text \
  --test-time="${TEST_TIME}" \
  --clients="${CLIENTS}" \
  --threads="${THREADS}" \
  --ratio="${RATIO}" \
  --data-size=1024 \
  --key-minimum=1 \
  --key-maximum=200000 \
  --key-pattern=G:G \
  --distinct-client-seed \
  --show-intervals \
  --csv \
  > "${RUN_DIR}/memtier_results_${TIMESTAMP}.csv"

MEMTIER_STATUS=$?

# 3) Stop vmstat
kill "${VMSTAT_PID}" 2>/dev/null || true

# 4) Capture end memory stats
free -m > "${RUN_DIR}/end_free_${TIMESTAMP}.txt"

echo "=== Memtier swap test finished with status ${MEMTIER_STATUS} ==="
echo "Results:"
echo "  ${RUN_DIR}/memtier_results_${TIMESTAMP}.csv"
echo "  ${RUN_DIR}/vmstat_${TIMESTAMP}.txt"
echo "  ${RUN_DIR}/start_free_${TIMESTAMP}.txt"
echo "  ${RUN_DIR}/end_free_${TIMESTAMP}.txt"

exit "${MEMTIER_STATUS}"


// memcached_bench.c
// Simple Memcached throughput + latency benchmark using libmemcached.
// Usage:
//   ./memcached_bench <server:port> <num_ops> <value_size_bytes> <key_space> <get_ratio> <csv_path>
// Example:
//   ./memcached_bench 127.0.0.1:11211 200000 1024 50000 0.9 experiments/memcached/bench_latencies.csv

#include <libmemcached/memcached.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>

static uint64_t now_us(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000000ULL + (uint64_t)tv.tv_usec;
}

static int cmp_u64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 7) {
        fprintf(stderr,
            "Usage: %s <server:port> <num_ops> <value_size_bytes> <key_space> <get_ratio_0_to_1> <csv_path_or_->\n"
            "Example: %s 127.0.0.1:11211 200000 1024 50000 0.9 bench_latencies.csv\n",
            argv[0], argv[0]);
        return 1;
    }

    const char *server_str = argv[1];
    long num_ops = atol(argv[2]);
    long val_size = atol(argv[3]);
    long key_space = atol(argv[4]);
    double get_ratio = atof(argv[5]);
    const char *csv_path = argv[6];

    if (num_ops <= 0 || val_size <= 0 || key_space <= 0 ||
        get_ratio < 0.0 || get_ratio > 1.0) {
        fprintf(stderr, "Invalid arguments.\n");
        return 1;
    }

    memcached_return_t rc;
    memcached_st *memc = memcached_create(NULL);
    if (!memc) {
        fprintf(stderr, "Failed to create memcached_st\n");
        return 1;
    }

    memcached_server_st *servers = memcached_servers_parse(server_str);
    if (!servers) {
        fprintf(stderr, "Failed to parse server string '%s'\n", server_str);
        memcached_free(memc);
        return 1;
    }

    rc = memcached_server_push(memc, servers);
    if (rc != MEMCACHED_SUCCESS) {
        fprintf(stderr, "Failed to add server: %s\n", memcached_strerror(memc, rc));
        memcached_server_list_free(servers);
        memcached_free(memc);
        return 1;
    }

    char *value = (char *)malloc((size_t)val_size);
    if (!value) {
        perror("malloc");
        memcached_server_list_free(servers);
        memcached_free(memc);
        return 1;
    }
    memset(value, 'X', (size_t)val_size);

    uint64_t *latencies_us = (uint64_t *)malloc((size_t)num_ops * sizeof(uint64_t));
    char *op_types = (char *)malloc((size_t)num_ops * sizeof(char));
    if (!latencies_us || !op_types) {
        fprintf(stderr, "Failed to allocate latency arrays\n");
        free(value);
        memcached_server_list_free(servers);
        memcached_free(memc);
        free(latencies_us);
        free(op_types);
        return 1;
    }

    srand((unsigned int)time(NULL));
    int get_threshold = (int)(get_ratio * (double)RAND_MAX);

    char key[64];
    uint64_t start_total = now_us();

    for (long i = 0; i < num_ops; i++) {
        int do_get = (rand() <= get_threshold);
        long key_id = i % key_space;
        int key_len = snprintf(key, sizeof(key), "bench_key_%ld", key_id);

        uint64_t t0 = now_us();

        if (do_get) {
            size_t val_len = 0;
            uint32_t flags = 0;
            memcached_return_t rc_get;
            char *ret = memcached_get(memc, key, (size_t)key_len,
                                      &val_len, &flags, &rc_get);
            if (ret) {
                free(ret);
            }
            // We count latency even for misses or errors.
        } else {
            memcached_return_t rc_set = memcached_set(memc, key, (size_t)key_len,
                                                      value, (size_t)val_size,
                                                      (time_t)0, (uint32_t)0);
            (void)rc_set; // ignore for now; still count latency
        }

        uint64_t t1 = now_us();
        latencies_us[i] = t1 - t0;
        op_types[i] = do_get ? 'G' : 'S';
    }

    uint64_t end_total = now_us();
    double total_sec = (double)(end_total - start_total) / 1e6;
    double throughput = (total_sec > 0.0) ? ((double)num_ops / total_sec) : 0.0;

    // Compute percentiles
    uint64_t *sorted = (uint64_t *)malloc((size_t)num_ops * sizeof(uint64_t));
    if (!sorted) {
        fprintf(stderr, "Failed to allocate sorted array\n");
        free(value);
        free(latencies_us);
        free(op_types);
        memcached_server_list_free(servers);
        memcached_free(memc);
        return 1;
    }
    memcpy(sorted, latencies_us, (size_t)num_ops * sizeof(uint64_t));
    qsort(sorted, (size_t)num_ops, sizeof(uint64_t), cmp_u64);

    #define PIDX(pct) ((size_t)((pct) * (num_ops - 1) / 100.0))
    uint64_t p50 = sorted[PIDX(50.0)];
    uint64_t p95 = sorted[PIDX(95.0)];
    uint64_t p99 = sorted[PIDX(99.0)];

    // Average latency
    long double sum = 0.0;
    for (long i = 0; i < num_ops; i++) {
        sum += (long double)latencies_us[i];
    }
    double avg = (double)(sum / (long double)num_ops);

    printf("Memcached benchmark summary:\n");
    printf("  Server:      %s\n", server_str);
    printf("  Operations:  %ld\n", num_ops);
    printf("  Value size:  %ld bytes\n", val_size);
    printf("  Key space:   %ld keys\n", key_space);
    printf("  GET ratio:   %.3f\n", get_ratio);
    printf("  Total time:  %.6f s\n", total_sec);
    printf("  Throughput:  %.2f ops/s\n", throughput);
    printf("  Avg latency: %.2f us\n", avg);
    printf("  p50 latency: %llu us\n", (unsigned long long)p50);
    printf("  p95 latency: %llu us\n", (unsigned long long)p95);
    printf("  p99 latency: %llu us\n", (unsigned long long)p99);

    // Optional CSV output
    if (strcmp(csv_path, "-") != 0) {
        FILE *fp = fopen(csv_path, "w");
        if (!fp) {
            perror("fopen(csv_path)");
        } else {
            fprintf(fp, "op_index,op_type,latency_us\n");
            for (long i = 0; i < num_ops; i++) {
                fprintf(fp, "%ld,%c,%llu\n",
                        i, op_types[i], (unsigned long long)latencies_us[i]);
            }
            fclose(fp);
        }
    }

    free(sorted);
    free(value);
    free(latencies_us);
    free(op_types);
    memcached_server_list_free(servers);
    memcached_free(memc);

    return 0;
}


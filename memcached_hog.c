#include <libmemcached/memcached.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc < 5) {
        fprintf(stderr,
            "Usage: %s <server:port> <num_keys> <value_size_bytes> <loops>\n"
            "Example: %s 127.0.0.1:11211 2000000 4096 1\n",
            argv[0], argv[0]);
        return 1;
    }

    const char *server_str = argv[1];
    long num_keys = atol(argv[2]);
    long val_size = atol(argv[3]);
    int loops = atoi(argv[4]);

    memcached_return_t rc;
    memcached_st *memc = memcached_create(NULL);
    memcached_server_st *servers = memcached_servers_parse(server_str);
    if (!servers) {
        fprintf(stderr, "Failed to parse server string '%s'\n", server_str);
        return 1;
    }

    rc = memcached_server_push(memc, servers);
    if (rc != MEMCACHED_SUCCESS) {
        fprintf(stderr, "Failed to add server: %s\n", memcached_strerror(memc, rc));
        return 1;
    }

    char *value = malloc(val_size);
    if (!value) {
        perror("malloc");
        return 1;
    }
    memset(value, 'A', val_size);

    char key[64];

    for (int l = 0; l < loops; l++) {
        printf("Loop %d/%d\n", l + 1, loops);
        for (long i = 0; i < num_keys; i++) {
            int len = snprintf(key, sizeof(key), "key_%ld_%d", i, l);
            rc = memcached_set(memc, key, (size_t)len,
                               value, (size_t)val_size,
                               (time_t)0, (uint32_t)0);
            if (rc != MEMCACHED_SUCCESS) {
                fprintf(stderr, "set failed at i=%ld: %s\n",
                        i, memcached_strerror(memc, rc));
                // you can break here if it's failing consistently
            }

            if (i % 100000 == 0) {
                printf("  wrote %ld keys in this loop\n", i);
                fflush(stdout);
            }
        }
    }

    printf("Finished writing keys.\n");
    fflush(stdout);

    // Keep process alive so memory stays allocated and hot
    printf("Sleeping to keep data resident...\n");
    fflush(stdout);
    for (;;) {
        sleep(60);
    }

    // Not reached, but for completeness:
    free(value);
    memcached_free(memc);
    memcached_server_list_free(servers);
    return 0;
}


#include <stdio.h>
#include <string.h>
#include "bfs.h"

int bfs_distance(Graph *g, char *start, char *target) {
    int src = hmap_get(g->hm, start);
    int dst = hmap_get(g->hm, target);

    if (src == -1 || dst == -1) {
        printf("User not found\n");
        return -1;
    }
    if (src == dst) return 0;

    int q[MAX_USERS];
    int depth[MAX_USERS];
    int seen[MAX_USERS];

    memset(seen, 0, sizeof(seen));
    memset(depth, 0, sizeof(depth));

    int head = 0, tail = 0;
    seen[src] = 1;
    q[tail++] = src;

    while (head < tail) {
        int node = q[head++];
        AdjNode *nb = g->users[node].head;
        while (nb != NULL) {
            int next = nb->dest;
            if (!seen[next]) {
                seen[next] = 1;
                depth[next] = depth[node] + 1;
                if (next == dst)
                    return depth[next];
                q[tail++] = next;
            }
            nb = nb->next;
        }
    }
    return -1;
}

void bfs_print_reachable(Graph *g, char *start) {
    int src = hmap_get(g->hm, start);
    if (src == -1) {
        printf("User not found\n");
        return;
    }

    int q[MAX_USERS];
    int seen[MAX_USERS];
    memset(seen, 0, sizeof(seen));

    int head = 0, tail = 0;
    seen[src] = 1;
    q[tail++] = src;

    printf("Reachable from %s: ", start);
    while (head < tail) {
        int node = q[head++];
        if (node != src)
            printf("%s ", g->users[node].name);
        AdjNode *nb = g->users[node].head;
        while (nb != NULL) {
            if (!seen[nb->dest]) {
                seen[nb->dest] = 1;
                q[tail++] = nb->dest;
            }
            nb = nb->next;
        }
    }
    printf("\n");
}
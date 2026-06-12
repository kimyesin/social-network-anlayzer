#include <stdio.h>
#include <string.h>
#include "bfs.h"

// BFS: search nearest users first (1st degree -> 2nd degree -> ...)
// queue implemented with array directly

int bfs_distance(Graph *g, char *start, char *target) {
    int start_id = hash_search(g->ht, start);
    int target_id = hash_search(g->ht, target);

    if (start_id == -1 || target_id == -1) {
        printf("User not found.\n");
        return -1;
    }
    if (start_id == target_id) return 0;

    int visited[MAX_USERS] = {0};
    int distance[MAX_USERS];
    int queue[MAX_USERS];
    int front = 0, rear = 0;

    visited[start_id] = 1;
    distance[start_id] = 0;
    queue[rear++] = start_id;

    while (front < rear) {
        int cur = queue[front++];

        AdjNode *adj = g->users[cur].head;
        while (adj != NULL) {
            int next = adj->dest;
            if (!visited[next]) {
                visited[next] = 1;
                distance[next] = distance[cur] + 1;
                if (next == target_id)
                    return distance[next];
                queue[rear++] = next;
            }
            adj = adj->next;
        }
    }
    return -1;
}

void bfs_print_reachable(Graph *g, char *start) {
    int start_id = hash_search(g->ht, start);
    if (start_id == -1) {
        printf("User not found.\n");
        return;
    }

    int visited[MAX_USERS] = {0};
    int queue[MAX_USERS];
    int front = 0, rear = 0;

    visited[start_id] = 1;
    queue[rear++] = start_id;

    printf("Reachable from %s: ", start);
    while (front < rear) {
        int cur = queue[front++];
        if (cur != start_id)
            printf("%s ", g->users[cur].name);

        AdjNode *adj = g->users[cur].head;
        while (adj != NULL) {
            if (!visited[adj->dest]) {
                visited[adj->dest] = 1;
                queue[rear++] = adj->dest;
            }
            adj = adj->next;
        }
    }
    printf("\n");
}
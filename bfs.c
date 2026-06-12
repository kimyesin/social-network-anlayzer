#include <stdio.h>
#include <string.h>
#include "bfs.h"

int bfs_distance(Graph *g, char *start, char *target) {
    int start_id = hmap_get(g->hm, start);
    int target_id = hmap_get(g->hm, target);

    if (start_id == -1 || target_id == -1) {
        printf("User not found.\n");
        return -1;
    }
    if (start_id == target_id) return 0;

    int visited[MAX_USERS] = {0};
    int distance[MAX_USERS] = {0};
    int queue[MAX_USERS];
    int front = 0, rear = 0;

    visited[start_id] = 1;
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
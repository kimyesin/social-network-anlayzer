#include <stdio.h>
#include "dfs.h"

static void dfs_visit(Graph *g, int id, int visited[]) {
    visited[id] = 1;
    printf("%s ", g->users[id].name);
    AdjNode *cur = g->users[id].head;
    while (cur != NULL) {
        if (!visited[cur->dest])
            dfs_visit(g, cur->dest, visited);
        cur = cur->next;
    }
}

void dfs_find_groups(Graph *g) {
    int visited[MAX_USERS] = {0};
    int group_count = 0;
    for (int i = 0; i < MAX_USERS; i++) {
        if (g->users[i].is_active && !visited[i]) {
            printf("Group %d: ", ++group_count);
            dfs_visit(g, i, visited);
            printf("\n");
        }
    }
    printf("%d group(s) found\n", group_count);
}
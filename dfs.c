#include <stdio.h>
#include "dfs.h"

void dfs_visit(Graph *g, int id, int visited[]) {
    visited[id] = 1;
    printf("%s ", g->users[id].name);

    AdjNode *cur = g->users[id].head;
    while (cur != NULL) {
        if (!visited[cur->dest])
            dfs_visit(g, cur->dest, visited);  // 재귀로 깊이 탐색
        cur = cur->next;
    }
}

void dfs_find_groups(Graph *g) {
    int visited[MAX_USERS] = {0};
    int group_count = 0;

    for (int i = 0; i < MAX_USERS; i++) {
        if (g->users[i].is_active && !visited[i]) {
            group_count++;
            printf("그룹 %d: ", group_count);
            dfs_visit(g, i, visited);
            printf("\n");
        }
    }
    printf("총 %d개 그룹\n", group_count);
}
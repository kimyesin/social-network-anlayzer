#include <stdio.h>
#include <stdlib.h>
#include "dfs.h"

// 명시적 스택을 이용한 DFS로 연결 그룹(컴포넌트) 탐지
void dfs_find_groups(Graph *g) {
    int visited[MAX_USERS] = {0};
    int stack[MAX_USERS];
    int group_num = 0;

    for (int i = 0; i < MAX_USERS; i++) {
        if (!g->users[i].is_active || visited[i])
            continue;

        group_num++;
        printf("Group %d: ", group_num);

        int top = 0;
        stack[top++] = i;
        visited[i] = 1;

        while (top > 0) {
            int cur = stack[--top];
            printf("%s ", g->users[cur].name);

            AdjNode *nb = g->users[cur].head;
            while (nb != NULL) {
                if (!visited[nb->dest]) {
                    visited[nb->dest] = 1;
                    stack[top++] = nb->dest;
                }
                nb = nb->next;
            }
        }
        printf("\n");
    }
    printf("%d group(s) found\n", group_num);
}
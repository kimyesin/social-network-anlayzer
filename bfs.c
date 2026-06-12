#include <stdio.h>
#include <string.h>
#include "bfs.h"

// BFS 핵심 원리:
// 가까운 사람부터 차례로 탐색 (1단계 친구 → 2단계 친구 → ...)
// 큐를 직접 배열로 구현

int bfs_distance(Graph *g, char *start, char *target) {
    int start_id = hash_search(g->ht, start);
    int target_id = hash_search(g->ht, target);

    if (start_id == -1 || target_id == -1) {
        printf("존재하지 않는 사용자입니다.\n");
        return -1;
    }
    if (start_id == target_id) return 0;

    int visited[MAX_USERS] = {0};  // 방문 여부
    int distance[MAX_USERS];       // 각 사용자까지의 거리
    int queue[MAX_USERS];          // 큐 (배열로 직접 구현)
    int front = 0, rear = 0;

    // 시작점 초기화
    visited[start_id] = 1;
    distance[start_id] = 0;
    queue[rear++] = start_id;     // 큐에 시작점 넣기

    while (front < rear) {        // 큐가 빌 때까지
        int cur = queue[front++]; // 큐에서 꺼내기

        AdjNode *adj = g->users[cur].head;
        while (adj != NULL) {
            int next = adj->dest;
            if (!visited[next]) {
                visited[next] = 1;
                distance[next] = distance[cur] + 1; // 거리 1 증가
                if (next == target_id)
                    return distance[next];
                queue[rear++] = next; // 큐에 넣기
            }
            adj = adj->next;
        }
    }
    return -1; // 연결 안 됨
}

void bfs_print_reachable(Graph *g, char *start) {
    int start_id = hash_search(g->ht, start);
    if (start_id == -1) {
        printf("존재하지 않는 사용자입니다.\n");
        return;
    }

    int visited[MAX_USERS] = {0};
    int queue[MAX_USERS];
    int front = 0, rear = 0;

    visited[start_id] = 1;
    queue[rear++] = start_id;

    printf("%s로부터 연결된 사용자: ", start);
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
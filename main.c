#include <stdio.h>
#include <string.h>
#include "graph.h"
#include "bfs.h"
#include "dfs.h"
#include "sort.h"

void print_menu() {
    printf("\n=== 소셜 네트워크 분석기 ===\n");
    printf("1. 사용자 추가\n");
    printf("2. 사용자 삭제\n");
    printf("3. 친구 추가\n");
    printf("4. 친구 삭제\n");
    printf("5. 친구 목록 보기\n");
    printf("6. 친구 추천\n");
    printf("7. 관계 거리 탐색 (BFS)\n");
    printf("8. 연결 그룹 보기 (DFS)\n");
    printf("9. 종료\n");
    printf("선택: ");
}

int main() {
    Graph *g = graph_create();
    int choice;
    char name1[MAX_USERNAME], name2[MAX_USERNAME];

    while (1) {
        print_menu();
        scanf("%d", &choice);
        getchar();  // 버퍼 비우기

        switch (choice) {
            case 1:
                printf("이름 입력: ");
                fgets(name1, MAX_USERNAME, stdin);
                name1[strcspn(name1, "\n")] = 0;  // 개행 제거
                graph_add_user(g, name1);
                break;
            case 2:
                printf("삭제할 이름: ");
                fgets(name1, MAX_USERNAME, stdin);
                name1[strcspn(name1, "\n")] = 0;
                graph_remove_user(g, name1);
                break;
            case 3:
                printf("첫 번째 이름: ");
                fgets(name1, MAX_USERNAME, stdin);
                name1[strcspn(name1, "\n")] = 0;
                printf("두 번째 이름: ");
                fgets(name2, MAX_USERNAME, stdin);
                name2[strcspn(name2, "\n")] = 0;
                graph_add_friend(g, name1, name2);
                break;
            case 4:
                printf("첫 번째 이름: ");
                fgets(name1, MAX_USERNAME, stdin);
                name1[strcspn(name1, "\n")] = 0;
                printf("두 번째 이름: ");
                fgets(name2, MAX_USERNAME, stdin);
                name2[strcspn(name2, "\n")] = 0;
                graph_remove_friend(g, name1, name2);
                break;
            case 5:
                printf("이름 입력: ");
                fgets(name1, MAX_USERNAME, stdin);
                name1[strcspn(name1, "\n")] = 0;
                graph_print_friends(g, name1);
                break;
            case 6:
                printf("이름 입력: ");
                fgets(name1, MAX_USERNAME, stdin);
                name1[strcspn(name1, "\n")] = 0;
                recommend_friends(g, name1);
                break;
            case 7:
                printf("시작 이름: ");
                fgets(name1, MAX_USERNAME, stdin);
                name1[strcspn(name1, "\n")] = 0;
                printf("목표 이름: ");
                fgets(name2, MAX_USERNAME, stdin);
                name2[strcspn(name2, "\n")] = 0;
                int dist = bfs_distance(g, name1, name2);
                if (dist == -1)
                    printf("연결되지 않음\n");
                else
                    printf("관계 거리: %d\n", dist);
                break;
            case 8:
                dfs_find_groups(g);
                break;
            case 9:
                graph_free(g);
                printf("종료합니다.\n");
                return 0;
            default:
                printf("잘못된 입력입니다.\n");
        }
    }
}
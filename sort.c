#include <stdio.h>
#include <string.h>
#include "sort.h"

// 공통 친구 수 기준 내림차순 정렬 (버블 정렬)
static void bubble_sort(Candidate *arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j].common_count < arr[j+1].common_count) {
                Candidate temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

void recommend_friends(Graph *g, char *name) {
    int id = hash_search(g->ht, name);
    if (id == -1) {
        printf("존재하지 않는 사용자: %s\n", name);
        return;
    }

    Candidate candidates[MAX_USERS];
    int count = 0;

    for (int i = 0; i < MAX_USERS; i++) {
        if (!g->users[i].is_active) continue;
        if (i == id) continue;

        // 이미 친구면 추천 제외
        AdjNode *check = g->users[id].head;
        int already_friend = 0;
        while (check != NULL) {
            if (check->dest == i) { already_friend = 1; break; }
            check = check->next;
        }
        if (already_friend) continue;

        // 공통 친구 수 계산
        int common = 0;
        AdjNode *a = g->users[id].head;
        while (a != NULL) {
            AdjNode *b = g->users[i].head;
            while (b != NULL) {
                if (a->dest == b->dest) common++;
                b = b->next;
            }
            a = a->next;
        }

        if (common > 0) {
            candidates[count].user_id = i;
            strncpy(candidates[count].name, g->users[i].name, MAX_USERNAME - 1);
            candidates[count].common_count = common;
            count++;
        }
    }

    bubble_sort(candidates, count);

    printf("%s의 친구 추천:\n", name);
    if (count == 0) {
        printf("추천할 친구가 없습니다.\n");
        return;
    }
    for (int i = 0; i < count; i++)
        printf("  %s (공통 친구 %d명)\n", candidates[i].name, candidates[i].common_count);
}
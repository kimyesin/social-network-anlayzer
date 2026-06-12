#include <stdio.h>
#include <string.h>
#include "centrality.h"

static int count_degree(Graph *g, int id) {
    int count = 0;
    AdjNode *cur = g->users[id].head;
    while (cur != NULL) {
        count++;
        cur = cur->next;
    }
    return count;
}

static void sort_by_degree(DegreeInfo *arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j].degree < arr[j+1].degree) {
                DegreeInfo temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

void centrality_top_k(Graph *g, int k) {
    DegreeInfo info[MAX_USERS];
    int count = 0;

    for (int i = 0; i < MAX_USERS; i++) {
        if (!g->users[i].is_active) continue;
        info[count].user_id = i;
        strncpy(info[count].name, g->users[i].name, MAX_USERNAME - 1);
        info[count].degree = count_degree(g, i);
        count++;
    }

    sort_by_degree(info, count);

    if (k > count) k = count;
    printf("Top %d influential users:\n", k);
    for (int i = 0; i < k; i++)
        printf("  %d. %s (friends: %d)\n", i+1, info[i].name, info[i].degree);
}
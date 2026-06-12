#include <stdio.h>
#include <string.h>
#include "sort.h"

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
    int id = hmap_get(g->hm, name);
    if (id == -1) {
        printf("User not found: %s\n", name);
        return;
    }

    Candidate candidates[MAX_USERS];
    int count = 0;

    for (int i = 0; i < MAX_USERS; i++) {
        if (!g->users[i].is_active) continue;
        if (i == id) continue;

        AdjNode *check = g->users[id].head;
        int already_friend = 0;
        while (check != NULL) {
            if (check->dest == i) { already_friend = 1; break; }
            check = check->next;
        }
        if (already_friend) continue;

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

    printf("Friend recommendations for %s:\n", name);
    if (count == 0) {
        printf("  No recommendations.\n");
        return;
    }
    for (int i = 0; i < count; i++)
        printf("  %s (common friends: %d)\n", candidates[i].name, candidates[i].common_count);
}
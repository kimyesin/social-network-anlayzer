#include <stdio.h>
#include <string.h>
#include "graph.h"
#include "bfs.h"
#include "dfs.h"
#include "sort.h"
#include "centrality.h"
#include "log.h"

void print_menu() {
    printf("\n=== Social Network Analyzer ===\n");
    printf("1. Add user\n");
    printf("2. Remove user\n");
    printf("3. Add friend\n");
    printf("4. Remove friend\n");
    printf("5. Print friend list\n");
    printf("6. Recommend friends\n");
    printf("7. Find relationship distance (BFS)\n");
    printf("8. Find connected groups (DFS)\n");
    printf("9. Influence analysis (Top-K)\n");
    printf("10. View activity log\n");
    printf("11. Exit\n");
    printf("Select: ");
}

int main() {
    Graph *g = graph_create();
    ActivityLog *log = log_create();
    int choice;
    char name1[MAX_USERNAME], name2[MAX_USERNAME];
    char log_msg[100];

    while (1) {
        print_menu();
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("Enter name: ");
                fgets(name1, MAX_USERNAME, stdin);
                name1[strcspn(name1, "\n")] = 0;
                graph_add_user(g, name1);
                snprintf(log_msg, 100, "Added user: %s", name1);
                log_add(log, log_msg);
                break;
            case 2:
                printf("Enter name to remove: ");
                fgets(name1, MAX_USERNAME, stdin);
                name1[strcspn(name1, "\n")] = 0;
                graph_remove_user(g, name1);
                snprintf(log_msg, 100, "Removed user: %s", name1);
                log_add(log, log_msg);
                break;
            case 3:
                printf("First user: ");
                fgets(name1, MAX_USERNAME, stdin);
                name1[strcspn(name1, "\n")] = 0;
                printf("Second user: ");
                fgets(name2, MAX_USERNAME, stdin);
                name2[strcspn(name2, "\n")] = 0;
                graph_add_friend(g, name1, name2);
                snprintf(log_msg, 100, "Added friend: %s <-> %s", name1, name2);
                log_add(log, log_msg);
                break;
            case 4:
                printf("First user: ");
                fgets(name1, MAX_USERNAME, stdin);
                name1[strcspn(name1, "\n")] = 0;
                printf("Second user: ");
                fgets(name2, MAX_USERNAME, stdin);
                name2[strcspn(name2, "\n")] = 0;
                graph_remove_friend(g, name1, name2);
                snprintf(log_msg, 100, "Removed friend: %s <-> %s", name1, name2);
                log_add(log, log_msg);
                break;
            case 5:
                printf("Enter name: ");
                fgets(name1, MAX_USERNAME, stdin);
                name1[strcspn(name1, "\n")] = 0;
                graph_print_friends(g, name1);
                break;
            case 6:
                printf("Enter name: ");
                fgets(name1, MAX_USERNAME, stdin);
                name1[strcspn(name1, "\n")] = 0;
                recommend_friends(g, name1);
                break;
            case 7:
                printf("Start user: ");
                fgets(name1, MAX_USERNAME, stdin);
                name1[strcspn(name1, "\n")] = 0;
                printf("Target user: ");
                fgets(name2, MAX_USERNAME, stdin);
                name2[strcspn(name2, "\n")] = 0;
                int dist = bfs_distance(g, name1, name2);
                if (dist == -1)
                    printf("Not connected.\n");
                else
                    printf("Relationship distance: %d\n", dist);
                break;
            case 8:
                dfs_find_groups(g);
                break;
            case 9:
                printf("Enter K: ");
                int k;
                scanf("%d", &k);
                getchar();
                centrality_top_k(g, k);
                break;
            case 10:
                log_print(log);
                break;
            case 11:
                graph_free(g);
                log_free(log);
                printf("Goodbye!\n");
                return 0;
            default:
                printf("Invalid input.\n");
        }
    }
}
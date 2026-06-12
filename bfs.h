#ifndef BFS_H
#define BFS_H

#include "graph.h"

// 두 사용자 사이의 거리 탐색 (몇 단계 친구인지)
// 못 찾으면 -1 반환
int bfs_distance(Graph *g, char *start, char *target);

// 연결된 모든 사용자 출력 (start로부터 갈 수 있는 사람들)
void bfs_print_reachable(Graph *g, char *start);

#endif
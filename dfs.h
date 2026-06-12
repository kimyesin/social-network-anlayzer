#ifndef DFS_H
#define DFS_H

#include "graph.h"

// 연결 그룹 탐지 (몇 개의 그룹으로 나뉘는지)
void dfs_find_groups(Graph *g);

// 특정 사용자로부터 DFS 탐색 (내부용)
void dfs_visit(Graph *g, int id, int visited[]);

#endif
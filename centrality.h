#ifndef CENTRALITY_H
#define CENTRALITY_H

#include "graph.h"

// 영향력 분석 결과 저장
typedef struct {
    int user_id;
    char name[MAX_USERNAME];
    int degree;  // 친구 수 (degree centrality)
} DegreeInfo;

// 상위 k명 출력
void centrality_top_k(Graph *g, int k);

#endif
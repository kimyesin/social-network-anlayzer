#ifndef HASH_H
#define HASH_H

#define TABLE_SIZE 101  // 해시 테이블 크기 (소수로 하면 충돌이 줄어듦)
#define MAX_USERNAME 20     // 이름 최대 길이

// 해시 테이블 한 칸 (충돌 시 체이닝으로 연결)
typedef struct HashEntry {
    char name[MAX_USERNAME];      // 사용자 이름 (key)
    int user_id;              // 사용자 ID (value)
    struct HashEntry *next;   // 충돌 시 다음 항목 연결
} HashEntry;

// 해시 테이블 전체
typedef struct {
    HashEntry *table[TABLE_SIZE];  // 각 칸마다 연결리스트 시작점
} HashTable;

// 함수 선언
HashTable *hash_create();                            // 테이블 생성
void hash_insert(HashTable *ht, char *name, int id); // 이름 → ID 저장
int  hash_search(HashTable *ht, char *name);         // 이름으로 ID 찾기
void hash_delete(HashTable *ht, char *name);         // 항목 삭제
void hash_free(HashTable *ht);                       // 메모리 해제

#endif
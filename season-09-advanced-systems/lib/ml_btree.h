/*
 * OPERATION MOONLIGHT — библиотека Season 9: индекс
 * Извлечено из s09e04 (B-дерево, T=3).
 */
#ifndef ML_BTREE_H
#define ML_BTREE_H

#define ML_BT_T     3
#define ML_BT_MAXK  (2 * ML_BT_T - 1)
#define ML_BT_NODES 512

typedef struct {
    int  nkeys, leaf;
    int  key[ML_BT_MAXK];
    long val[ML_BT_MAXK];
    int  child[2 * ML_BT_T];
} MlBtNode;

typedef struct {
    MlBtNode pool[ML_BT_NODES];
    int used, root;
    long page_reads;              /* метрика дисковых структур */
} MlBtree;

void ml_bt_init(MlBtree *t);
void ml_bt_insert(MlBtree *t, int key, long val);
int  ml_bt_search(MlBtree *t, int key, long *out);   /* 1 — найден */
int  ml_bt_height(const MlBtree *t);

#endif /* ML_BTREE_H */

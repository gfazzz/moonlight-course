#include "ml_btree.h"

static int node_alloc(MlBtree *t, int leaf) {
    int id = t->used++;
    t->pool[id].nkeys = 0;
    t->pool[id].leaf = leaf;
    return id;
}

void ml_bt_init(MlBtree *t) { t->used = 0; t->root = -1; t->page_reads = 0; }

static void split_child(MlBtree *t, int parent, int i) {
    MlBtNode *p = &t->pool[parent];
    int full_id = p->child[i];
    MlBtNode *full = &t->pool[full_id];
    int new_id = node_alloc(t, full->leaf);
    MlBtNode *nn = &t->pool[new_id];
    nn->nkeys = ML_BT_T - 1;
    for (int j = 0; j < ML_BT_T - 1; j++) {
        nn->key[j] = full->key[j + ML_BT_T];
        nn->val[j] = full->val[j + ML_BT_T];
    }
    if (!full->leaf)
        for (int j = 0; j < ML_BT_T; j++) nn->child[j] = full->child[j + ML_BT_T];
    full->nkeys = ML_BT_T - 1;
    for (int j = p->nkeys; j > i; j--) p->child[j + 1] = p->child[j];
    p->child[i + 1] = new_id;
    for (int j = p->nkeys - 1; j >= i; j--) { p->key[j+1] = p->key[j]; p->val[j+1] = p->val[j]; }
    p->key[i] = full->key[ML_BT_T - 1];
    p->val[i] = full->val[ML_BT_T - 1];
    p->nkeys++;
}

static void insert_nonfull(MlBtree *t, int id, int k, long v) {
    MlBtNode *n = &t->pool[id];
    int i = n->nkeys - 1;
    if (n->leaf) {
        while (i >= 0 && k < n->key[i]) { n->key[i+1] = n->key[i]; n->val[i+1] = n->val[i]; i--; }
        n->key[i+1] = k; n->val[i+1] = v; n->nkeys++;
        return;
    }
    while (i >= 0 && k < n->key[i]) i--;
    i++;
    if (t->pool[n->child[i]].nkeys == ML_BT_MAXK) {
        split_child(t, id, i);
        if (k > n->key[i]) i++;
    }
    insert_nonfull(t, n->child[i], k, v);
}

void ml_bt_insert(MlBtree *t, int key, long val) {
    if (t->used >= ML_BT_NODES - 2) return;
    if (t->root < 0) {
        t->root = node_alloc(t, 1);
        t->pool[t->root].key[0] = key;
        t->pool[t->root].val[0] = val;
        t->pool[t->root].nkeys = 1;
        return;
    }
    if (t->pool[t->root].nkeys == ML_BT_MAXK) {
        int nr = node_alloc(t, 0);
        t->pool[nr].child[0] = t->root;
        t->root = nr;
        split_child(t, nr, 0);
    }
    insert_nonfull(t, t->root, key, val);
}

static int search_at(MlBtree *t, int id, int k, long *out) {
    if (id < 0) return 0;
    t->page_reads++;
    MlBtNode *n = &t->pool[id];
    int i = 0;
    while (i < n->nkeys && k > n->key[i]) i++;
    if (i < n->nkeys && k == n->key[i]) { if (out) *out = n->val[i]; return 1; }
    if (n->leaf) return 0;
    return search_at(t, n->child[i], k, out);
}

int ml_bt_search(MlBtree *t, int key, long *out) { return search_at(t, t->root, key, out); }

int ml_bt_height(const MlBtree *t) {
    int id = t->root, h = 0;
    while (id >= 0) { h++; if (t->pool[id].leaf) break; id = t->pool[id].child[0]; }
    return h;
}

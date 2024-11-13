#ifndef RBTREE_H
#define RBTREE_H

#include <stdlib.h>

struct rbtree;

enum rbtree_node_color
{
    RED,
    BLACK
};

enum state
{
    INSERT_SUCCESS,
    INSERT_REPEATED_KEY,

    DELETE_SUCCESS,
    DELETE_KET_NOT_FOUND,

    SEARCH_SUCCESS,
    SEARCH_KET_NOT_FOUND
};

struct rbtree_node
{
    int value;
    enum rbtree_node_color color;
    struct rbtree_node *left, *right, *parent;
};

void rbtree_node_init(struct rbtree_node *node, struct rbtree_node *parent, int value);

enum state rbtree_node_insert(struct rbtree *tree, struct rbtree_node *node, struct rbtree_node *new_node);

struct rbtree_node *rbtree_node_minimum(struct rbtree_node *node);

/*
    Red-Black Tree properties:

    1. Node Color: Each node is either red or black.
    2. Root Property: The root of the tree is always black.
    3. Red Property: Red nodes cannot have red children (no two consecutive red nodes on any path).
    4. Black Property: Every path from a node to its descendant null nodes (leaves) has the same number of black nodes.
    5. Leaf Property: All leaves (NIL nodes) are black.

    Properties extracted from: https://www.geeksforgeeks.org/introduction-to-red-black-tree/#what-is-a-redblack-tree
*/
struct rbtree
{
    struct rbtree_node *root;
};

void rbtree_transplant(struct rbtree *tree, struct rbtree_node *u, struct rbtree_node *v);

void rbtree_insert_fixup(struct rbtree *tree, struct rbtree_node *node);

void rbtree_delete_fixup(struct rbtree *tree, struct rbtree_node *node);

void rbtree_left_rotate(struct rbtree *tree, struct rbtree_node *x);

void rbtree_right_rotate(struct rbtree *tree, struct rbtree_node *y);

void rbtree_init(struct rbtree *tree);

enum state rbtree_insert(struct rbtree *tree, int key);

enum state rbtree_search(struct rbtree *tree, int key);

enum state rbtree_delete(struct rbtree *tree, int key);

struct rbtree_node *rbtree_find(struct rbtree *tree, int key);

struct rbtree_node *rbtree_minimum(struct rbtree *tree);


#endif // RBTREE_H

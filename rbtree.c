#ifndef RBTREE_C
#define RBTREE_C

#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

void rbtree_node_init(struct rbtree_node *node, struct rbtree_node *parent, int value)
{
    node->value = value;
    node->color = RED;
    node->left = node->right = NULL;
    node->parent = parent;
}

enum state rbtree_node_insert(struct rbtree *tree, struct rbtree_node *node, struct rbtree_node *new_node)
{
    if (new_node->value < node->value)
    {
        if (node->left == NULL)
        {
            node->left = new_node;
            new_node->parent = node;
            return INSERT_SUCCESS;
        }
        else
        {
            return rbtree_node_insert(tree, node->left, new_node);
        }
    }
    else if (new_node->value > node->value)
    {
        if (node->right == NULL)
        {
            node->right = new_node;
            new_node->parent = node;
            return INSERT_SUCCESS;
        }
        else
        {
            return rbtree_node_insert(tree, node->right, new_node);
        }
    }
    else 
    {
        return INSERT_REPEATED_KEY;
    }
}

struct rbtree_node *rbtree_node_minimum(struct rbtree_node *node) 
{
    while (node != NULL && node->left != NULL)
        node = node->left;

    return node;
}

void rbtree_transplant(struct rbtree *tree, struct rbtree_node *u, struct rbtree_node *v)
{
    if (u->parent == NULL)
        tree->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    if (v != NULL)
        v->parent = u->parent;
}

void rbtree_insert_fixup(struct rbtree *tree, struct rbtree_node *node)
{
    while (node != tree->root && node->parent->color == RED)
    {
        struct rbtree_node *grandparent = node->parent->parent;
        struct rbtree_node *uncle = (node->parent == grandparent->left) ? grandparent->right : grandparent->left;

        if (uncle != NULL && uncle->color == RED)
        {
            node->parent->color = BLACK;
            uncle->color = BLACK;
            grandparent->color = RED;
            node = grandparent;
        }
        else
        {
            if (node->parent == grandparent->left)
            {
                if (node == node->parent->right)
                {
                    rbtree_left_rotate(tree, node->parent);
                    node = node->left;
                }
                node->parent->color = BLACK;
                grandparent->color = RED;
                rbtree_right_rotate(tree, grandparent);
            }
            else
            {
                if (node == node->parent->left)
                {
                    rbtree_right_rotate(tree, node->parent);
                    node = node->right;
                }
                node->parent->color = BLACK;
                grandparent->color = RED;
                rbtree_left_rotate(tree, grandparent);
            }
        }
    }
    tree->root->color = BLACK;
}

void rbtree_delete_fixup(struct rbtree *tree, struct rbtree_node *node) 
{
    while (node != tree->root && node->color == BLACK) 
    {
        if (node == node->parent->left) 
        {
            struct rbtree_node *sibling = node->parent->right;
            if (sibling->color == RED) 
            {
                sibling->color = BLACK;
                node->parent->color = RED;
                rbtree_left_rotate(tree, node->parent);
                sibling = node->parent->right;
            }
            if (sibling->left->color == BLACK && sibling->right->color == BLACK) 
            {
                sibling->color = RED;
                node = node->parent;
            } 
            else 
            {
                if (sibling->right->color == BLACK) 
                {
                    sibling->left->color = BLACK;
                    sibling->color = RED;
                    rbtree_right_rotate(tree, sibling);
                    sibling = node->parent->right;
                }
                sibling->color = node->parent->color;
                node->parent->color = BLACK;
                sibling->right->color = BLACK;
                rbtree_left_rotate(tree, node->parent);
                node = tree->root;
            }
        } 
        else 
        {
            struct rbtree_node *sibling = node->parent->left;
            if (sibling->color == RED) 
            {
                sibling->color = BLACK;
                node->parent->color = RED;
                rbtree_right_rotate(tree, node->parent);
                sibling = node->parent->left;
            }
            if (sibling->left->color == BLACK && sibling->right->color == BLACK) 
            {
                sibling->color = RED;
                node = node->parent;
            } 
            else 
            {
                if (sibling->left->color == BLACK) 
                {
                    sibling->right->color = BLACK;
                    sibling->color = RED;
                    rbtree_left_rotate(tree, sibling);
                    sibling = node->parent->left;
                }
                sibling->color = node->parent->color;
                node->parent->color = BLACK;
                sibling->left->color = BLACK;
                rbtree_right_rotate(tree, node->parent);
                node = tree->root;
            }
        }
    }
    node->color = BLACK;
}


void rbtree_left_rotate(struct rbtree *tree, struct rbtree_node *x)
{
    struct rbtree_node *y = x->right;

    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;

    y->parent = x->parent;
    if (x->parent == NULL)
        tree->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

void rbtree_right_rotate(struct rbtree *tree, struct rbtree_node *y)
{
    struct rbtree_node *x = y->left;

    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;

    x->parent = y->parent;

    if (y->parent == NULL)
        tree->root = x;
    else if (y == y->parent->right)
        y->parent->right = x;
    else
        y->parent->left = x;

    x->right = y;
    y->parent = x;
}

void rbtree_init(struct rbtree *tree)
{
    tree->root = NULL;
}

enum state rbtree_insert(struct rbtree *tree, int key)
{
    struct rbtree_node *new_node = malloc(sizeof(struct rbtree_node));
    rbtree_node_init(new_node, NULL, key);

    enum state insert_state;
    
    if (tree->root == NULL)
    {
        tree->root = new_node;
        tree->root->color = BLACK;
        insert_state = INSERT_SUCCESS;
    }
    else
    {
        insert_state = rbtree_node_insert(tree, tree->root, new_node);
    }
    
    rbtree_insert_fixup(tree, new_node);

    return insert_state;
}

enum state rbtree_search(struct rbtree *tree, int key)
{
    struct rbtree_node *current = tree->root;
    while (current != NULL)
    {
        if (key < current->value)
            current = current->left;
        else if (key > current->value)
            current = current->right;
        else
            return SEARCH_SUCCESS;
    }
    return SEARCH_KET_NOT_FOUND;
}


enum state rbtree_delete(struct rbtree *tree, int key)
{
    struct rbtree_node *node = tree->root;
    struct rbtree_node *y, *x;

    while (node != NULL && node->value != key) 
    {
        if (key < node->value)
            node = node->left;
        else
            node = node->right;
    }

    if (node == NULL || node->value != key)
        return DELETE_KET_NOT_FOUND;

    y = node;
    int y_original_color = y->color;

    if (node->left == NULL) 
    {
        x = node->right;
        rbtree_transplant(tree, node, node->right);
    } 
    else if (node->right == NULL) 
    {
        x = node->left;
        rbtree_transplant(tree, node, node->left);
    } 
    else 
    {
        y = rbtree_node_minimum(node->right);
        y_original_color = y->color;
        x = y->right;

        if (y->parent == node) 
        {
            if (x != NULL)
                x->parent = y;
        } 
        else 
        {
            rbtree_transplant(tree, y, y->right);
            y->right = node->right;
            y->right->parent = y;
        }

        rbtree_transplant(tree, node, y);
        y->left = node->left;
        y->left->parent = y;
        y->color = node->color;
    }

    free(node);

    if (y_original_color == BLACK && x != NULL)
        rbtree_delete_fixup(tree, x);
}

struct rbtree_node *rbtree_find(struct rbtree *tree, int key)
{
    struct rbtree_node *current = tree->root;
    while (current != NULL)
    {
        if (key < current->value)
            current = current->left;
        else if (key > current->value)
            current = current->right;
        else
            return current;
    }
    return NULL;
}

struct rbtree_node *rbtree_minimum(struct rbtree *tree)
{
    return rbtree_node_minimum(tree->root);
}

#endif // RBTREE_C

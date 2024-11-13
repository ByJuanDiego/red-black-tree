#include "rbtree.c"

int main()
{
    struct rbtree tree;

    rbtree_init(&tree);
    
    rbtree_insert(&tree, 15);
    rbtree_insert(&tree, 55);
    rbtree_insert(&tree, 95);
    rbtree_insert(&tree, 5);
    rbtree_insert(&tree, 3);
    rbtree_insert(&tree, 10);
    rbtree_insert(&tree, 1);
    rbtree_insert(&tree, 25);

    if (rbtree_delete(&tree, 10) == DELETE_KET_NOT_FOUND)
    {
        printf("%d is not in the tree\n", 10);
    }

    for (int i = 1; i < 6; ++i)
    {     
        if (rbtree_search(&tree, i) == SEARCH_KET_NOT_FOUND)
        {
            printf("%d is not in the tree\n", i); 
        }
        else
        {
            printf("%d is in the tree\n", i); 
        }
    }

    struct rbtree_node *min = rbtree_minimum(&tree);

    printf("minimum: %d\n", min != NULL? min->value : -1);
}

#ifndef BINARY_TRIANGLE_TREE_H
#define BINARY_TRIANGLE_TREE_H

#include <stddef.h>

typedef struct BTTNodeT
{
	struct BTTNodeT *left_child;
	struct BTTNodeT *right_child;

	struct BTTNodeT *base_neighbor;
	struct BTTNodeT *left_neighbor;
	struct BTTNodeT *right_neighbor;

} BTTNode;


size_t BTTNode_number_of_leaves(BTTNode *tree);

#endif // BINARY_TRIANGLE_TREE_H

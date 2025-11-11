#include "Tree.h"

static tree_err_t _AddNode(node_t *node, tree_elem_t elem)
{
	if(node == NULL)
		return NODE_NULLPTR;
	
	static size_t call_count = 0;
    
    if(call_count++ > MAX_REC_DEPTH)
        return TREE_LOOP;
    
    if(node->parent == NULL)
        return TREE_PARENT_NULLPTR;

    tree_err_t err = TREE_NO_ERR;
    
    if(elem < node->data)
	{
		if(node->left == NULL)
		{
			node->left = (node_t *)calloc(1, sizeof(node_t));
			assert(node->left);

			node->left->parent = node;
			node->left->data = elem;
			
		}
		else
			err = _AddNode(node->left, elem);
	}
	else
	{
		if(node->right == NULL)
		{
			node->right = (node_t *)calloc(1, sizeof(node_t));
			assert(node->right);

			node->right->parent = node;
			node->right->data = elem;
		}
		else
			err = _AddNode(node->right, elem);
	}

	return err;
}

tree_err_t AddNode(tree_t *tree, tree_elem_t elem)
{
	if(tree == NULL)
		return TREE_NULLPTR;

	return _AddNode(tree->node, elem);
}

static tree_err_t NodeDestroy(node_t *node)
{
	if(node == NULL)
		return NODE_NULLPTR;
	
	static size_t call_count = 0;
    
    if(call_count++ > MAX_REC_DEPTH)
        return TREE_LOOP;
    
    if(node->parent == NULL)
        return TREE_PARENT_NULLPTR;

    tree_err_t err = TREE_NO_ERR;

	node->data = TRASH;
	
	if(node->left)
		err = NodeDestroy(node->left);
	
	if(node->right)
		err = NodeDestroy(node->right);

	node->parent = NULL;
	free(node);
	node = NULL;

	return err;
}

tree_err_t TreeDestroy(tree_t *tree)
{
	if(tree == NULL)
		return TREE_NULLPTR;

	return NodeDestroy(tree->node);
}


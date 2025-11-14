#include "Tree.h"



tree_err_t AddNode(node_t *node, tree_elem_t elem)
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
			err = AddNode(node->left, elem);
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
			err = AddNode(node->right, elem);
	}

	return err;
}

tree_err_t NodeDestroy(node_t *node)
{
	if(node == NULL)
		return NODE_NULLPTR;
	
	static size_t call_count = 0;
    
    if(call_count++ > MAX_REC_DEPTH)
        return TREE_LOOP;
    
    if(node->parent == NULL)
        return TREE_PARENT_NULLPTR;

    tree_err_t err = TREE_NO_ERR;

		free(node->data);
		node->data = TRASH;

	//node->data = TRASH;
	
	if(node->left)
		err = NodeDestroy(node->left);
	
	if(node->right)
		err = NodeDestroy(node->right);

	node->parent = NULL;
	free(node);
	node = NULL;

	return err;
}


tree_err_t _TreeDestroyExt(node_t *tree, const char *buf, const size_t buf_size)
{
	if(tree == NULL)
		return NODE_NULLPTR;

	//tree->data = TRASH;
	tree->parent = NULL;
	
	tree_err_t err = TREE_NO_ERR;
	
	if(buf == NULL || tree->data < buf || buf + buf_size < tree->data)
	{
		free(tree->data);
		tree->data = TRASH;
	}
	else
		tree->data = TRASH;

	err = _TreeDestroyExt(tree->left, buf, buf_size);

	err = _TreeDestroyExt(tree->right, buf, buf_size);

	free(tree);

	return err;
}

tree_err_t TreeDestroyExt(node_t *tree, const char *buf, const size_t buf_size)
{
	if(tree == NULL)
		return NODE_NULLPTR;

	//tree->data = TRASH;
	tree->parent = NULL;
	
	tree_err_t err = TREE_NO_ERR;
	
	if(buf == NULL || tree->data < buf || buf + buf_size < tree->data)
	{
		free(tree->data);
		tree->data = TRASH;
	}
	else
		tree->data = TRASH;

	err = _TreeDestroyExt(tree->left, buf, buf_size);

	err = _TreeDestroyExt(tree->right, buf, buf_size);

	return err;
}



tree_err_t TreeDestroy(node_t *tree)
{
	return TreeDestroyExt(tree, NULL, 0);
}
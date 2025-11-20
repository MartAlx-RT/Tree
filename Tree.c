#include "Tree.h"

void AddLeaves(node_t *node)
{
	assert(node);
	
	node->left = (node_t *)calloc(1, sizeof(node_t));
	assert(node->left);
	
	node->right = (node_t *)calloc(1, sizeof(node_t));
	assert(node->right);

	node->left->data = TRASH;
	node->right->data = TRASH;

	node->left->parent = node->right->parent = node;
}

tree_err_t SortAddNode(node_t *node, tree_elem_t elem)
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
			err = SortAddNode(node->left, elem);
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
			err = SortAddNode(node->right, elem);
	}

	return err;
}

static tree_err_t _TreeDestroy(node_t *tree, const char *buf, const size_t buf_size, size_t *call_count)
{
	assert(call_count);
	
	if (tree == NULL)
		return NODE_NULLPTR;

    if((*call_count)++ > MAX_REC_DEPTH)
        return TREE_LOOP;
	/*-----------------------------*/
	
	tree_err_t err = TREE_NO_ERR;
    
	if(tree->parent == NULL)
		err = TREE_PARENT_NULLPTR;
	
	if(tree->data < buf || buf + buf_size <= tree->data) /* if data is out of range buf */
		free(tree->data);								/* it's necessary to use free */
	
	tree->data = TRASH;


	err = _TreeDestroy(tree->left, buf, buf_size, call_count);
	if(err)
		_TreeDestroy(tree->right, buf, buf_size, call_count);
	else
		err = _TreeDestroy(tree->right, buf, buf_size, call_count);

	
	free(tree);
	return err;
}

tree_err_t TreeDestroy(node_t *tree, const char *buf, const size_t buf_size)
{
	size_t call_counts = 0;
	return _TreeDestroy(tree, buf, buf_size, &call_counts);
}

static node_t *_FindNode(node_t *tree, tree_elem_t elem, int (*comp)(const tree_elem_t, const tree_elem_t), size_t *call_count)
{
	assert(call_count);
	
	if (tree == NULL || (*call_count)++ > MAX_REC_DEPTH || comp == NULL)
		return NULL;

	if(comp(tree->data, elem) == 0)
		return tree;

	node_t *l_search = _FindNode(tree->left, elem, comp, call_count);
	if(l_search)
		return l_search;

	return _FindNode(tree->right, elem, comp, call_count);
}

node_t *FindNode(node_t *tree, tree_elem_t elem, int (*comp)(const tree_elem_t, const tree_elem_t))
{
	size_t call_count = 0;
	return _FindNode(tree, elem, comp, &call_count);
}

static int _NodeIsExist(const node_t *node, const node_t *tree, size_t *call_count)
{
	assert(call_count);
	
	if(node == NULL || tree == NULL)
		return 0;
	
	if((*call_count)++ > MAX_REC_DEPTH)
		return 0;

	/*------------------------*/

	if(tree == node)
		return 1;

	if(tree->left)
		if(_NodeIsExist(node, tree->left, call_count))
			return 1;

	return _NodeIsExist(node, tree->right, call_count);
}

int NodeIsExist(const node_t *node, const node_t *tree)
{
	size_t call_count = 0;
	return _NodeIsExist(node, tree, &call_count);
}

static long _NodeDepth(const node_t *node, long *depth)
{
	assert(depth);
	
	if (node == NULL || node->parent == NULL)
		return -1;
	
	if(*depth > (long)MAX_REC_DEPTH)
		return -1;

	if(node->parent == node)
		return *depth;

	(*depth)++;
	return _NodeDepth(node->parent, depth);
}

long NodeDepth(const node_t *node)
{
	long depth = 0;
	return _NodeDepth(node, &depth);
}

node_t *FindCommonParent(node_t *node1, node_t *node2, node_t *tree)
{
	if(tree == NULL || node1 == NULL || node2 == NULL)
		return NULL;
	
	if(node1->parent == NULL || node2->parent == NULL)
        return NULL;
	
	/*------------------------*/

	long depth1 = NodeDepth(node1), depth2 = NodeDepth(node2);
	if(depth1 < 0 || depth2 < 0)
		return NULL;
		
	while(depth1 != depth2)
	{
		if(depth1 > depth2)
		{
			if(node1->parent)
				node1 = node1->parent;
			else
				return NULL;

			depth1--;
		}
		else
		{
			if(node2->parent)
				node2 = node2->parent;
			else
				return NULL;

			depth2--;
		}
	}
	
	while(node1 != node2)
	{
		if(node1)
			node1 = node1->parent;
		else
			return NULL;
		
		if(node2)
			node2 = node2->parent;
		else
			return NULL;
	}

	return node1;
}


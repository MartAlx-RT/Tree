#include "Tree.h"

void ClearBuffer(void)
{
	while(getchar() != '\n');
}

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

tree_err_t NewTree(node_t *tree)
{
	if(tree == NULL)
		return NODE_NULLPTR;

	tree->left = tree->right = NULL;
	tree->data = TRASH;
	tree->parent = tree;

	printf("Введите первый объект =): ");
	
	if(scanf(" %m[^'\n']", &(tree->data)) == 1)
	{
		ClearBuffer();
		printf("Добавлено!\n");
		return TREE_NO_ERR;
	}

	printf("Считывание прошло неуспешно =(\n");

	TreeDestroy(tree);
	return GAME_ADD_NODE_ERR;
}

tree_err_t Play(node_t *node)
{
	static size_t call_count = 0;

	if(node == NULL)
		return NODE_NULLPTR;
	
	if(call_count++ > MAX_REC_DEPTH)
		return TREE_LOOP;
	
	if(node->parent == NULL)
        return TREE_PARENT_NULLPTR;

	char ans[100] = "";
	if(node->left == NULL && node->right == NULL)
	{
		printf("Это " MODE_PRINT "?[да или нет]: ", node->data);
		scanf(" %10s", ans);
		ClearBuffer();

		if(strcmp(ans, "нет") == 0)
		{
			// node->left = (node_t *)calloc(1, sizeof(node_t));
			// assert(node->left);
			// node->left->parent = node;
			// node->left->data = TRASH;
			// node->left->left = node->left->right = NULL;

			AddLeaves(node);

			printf("А кто это был?: ");
			if(scanf("%m[^'\n']", &(node->left->data)) == 1)
			{
				assert(node->left->data);
				
				ClearBuffer();

				// char *old_data = strdup(node->data);
				// assert(old_data);
				node->right->data = node->data;
				assert(node->right->data);

				// if(node->data != TRASH)
				// {
				// 	free(node->data);
				// 	node->data = TRASH;
				// }

				printf("old_data = {%s}\n", node->right->data);

				printf("Добавлено!\nА чем " MODE_PRINT " отличается от " MODE_PRINT "?\n(продолжите фразу) %s...",
					   node->left->data, node->right->data, node->left->data);

				if (scanf("%m[^'\n']", &(node->data)) == 1)
				{
					assert(node->data);
					ClearBuffer();
					// node->right = (node_t *)calloc(1, sizeof(node_t));
					// assert(node->right);
					// node->right->parent = node;
					// node->right->data = old_data;
					// node->right->left = node->right->right = NULL;

					printf("Понял\n");
					return TREE_NO_ERR;
				}
				else
				{
					printf("Считывание прошло неуспешно =(\n");
					// node->right->data = TRASH;
					// node->right->parent = NULL;
					// free(node->right);

					TreeDestroy(node);
					return GAME_ADD_NODE_ERR;
				}
			}
			else
			{
				printf("Считывание прошло неуспешно =(\n");
				// node->right->data = TRASH;
				// node->right->parent = NULL;
				// free(node->right);
				// return GAME_ADD_NODE_ERR;
				
				TreeDestroy(node);
				return GAME_ADD_NODE_ERR;
			}
		}
		else
		{
			printf("Я угадал =)\n");
			return TREE_NO_ERR;
		}
	}
	else if(node->left && node->right)
	{
		printf(MODE_PRINT "?[да или нет]: ", node->data);
		scanf(" %10s", ans);
		ClearBuffer();
		
		if(strcmp(ans, "нет") == 0)
			return Play(node->right);

		return Play(node->left);
	}

	return GAME_WRONG_TREE;
}

/*
tree_err_t LaunchGame(tree_t *tree)
{
	if(tree == NULL)
	{
		printf("Tree nullptr, aborted\n");
		return TREE_NULLPTR;
	}

	return Play(tree->node->right);
}
*/
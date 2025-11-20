#include "Tree.h"



static tree_err_t NewTree(node_t *tree)
{
	if(tree == NULL)
		return NODE_NULLPTR;

	tree->left = tree->right = NULL;
	tree->data = TRASH;
	tree->parent = tree;

	printf(colorize("Введите первый объект =): ", _BOLD_ _GREEN_));
	if(scanf(" %m[^'\n']", &(tree->data)) == 1)
	{
		ClearBuf();
		printf(colorize("Добавлено!\n", _BOLD_ _GREEN_));
		return TREE_NO_ERR;
	}

	printf(colorize("Считывание прошло неуспешно =(\n", _BOLD_ _RED_));
	return GAME_ADD_NODE_ERR;
}

static tree_err_t _Play(node_t *node, size_t *call_count)
{
	assert(call_count);

	if(node == NULL)
		return NODE_NULLPTR;
	
	if((*call_count)++ > MAX_REC_DEPTH)
		return TREE_LOOP;
	
	if(node->parent == NULL)
        return TREE_PARENT_NULLPTR;
	/*--------------------------------*/

	char ans[11] = "";
	
	if(node->left == NULL && node->right == NULL) /* If the node is a leave */
	{
		printf(colorize("Это " MODE_PRINT "?[да или нет]: ", _BOLD_ _YELLOW_), node->data);
		scanf(" %10s", ans);
		ClearBuf();

		if(strcmp(ans, "нет") == 0)
		{
			AddLeaves(node);

			printf(colorize("А кто это был?: ", _BOLD_ _MAGENTA_));
			if(scanf("%m[^'\n']", &(node->left->data)) == 1)
			{
				ClearBuf();
				assert(node->left->data);
				
				node->right->data = node->data;
				//assert(node->right->data);
				//free(node->data);

				//printf("old_data = {%s}\n", node->right->data); /* debugging printf */

				printf(colorize("Добавлено!\n", _BOLD_ _GREEN_)
						   colorize("А чем " MODE_PRINT " отличается от " MODE_PRINT "?\n", _BOLD_ _MAGENTA_) "(продолжите фразу) " MODE_PRINT "...",
					   node->left->data, node->right->data, node->left->data);

				if (scanf("%m[^'\n']", &(node->data)) == 1)
				{
					ClearBuf();
					assert(node->data);
					
					printf("Понял\n");
					return TREE_NO_ERR;
				}
				else
				{
					printf(colorize("Считывание прошло неуспешно =(\n", _BOLD_ _RED_));
					TreeDestroy(node, NULL, 0);
					return GAME_ADD_NODE_ERR;
				}
			}
			else
			{
				printf(colorize("Считывание прошло неуспешно =(\n", _BOLD_ _RED_));
				TreeDestroy(node, NULL, 0);
				return GAME_ADD_NODE_ERR;
			}
		}
		else
		{
			printf(colorize("Я угадал =)\n", _BOLD_ _GREEN_));
			return TREE_NO_ERR;
		}
	}
	else if(node->left && node->right) /* If the node is a node */
	{
		printf(colorize(MODE_PRINT "?[да или нет]: ", _BOLD_ _YELLOW_), node->data);
		scanf(" %10s", ans);
		ClearBuf();
		
		if(strcmp(ans, "нет") == 0)
			return _Play(node->right, call_count);

		return _Play(node->left, call_count);
	}

	return GAME_WRONG_TREE;
}

static tree_err_t Play(node_t *tree)
{
	size_t call_count = 0;
	return _Play(tree, &call_count);
}

static tree_err_t _GetDefinition(node_t *obj, size_t *call_count)
{
	assert(call_count);
	
	if (obj == NULL)
		return NODE_NULLPTR;
	
	if((*call_count)++ > MAX_REC_DEPTH)
		return TREE_LOOP;
	
	if(obj->parent == NULL)
        return TREE_PARENT_NULLPTR;
	
	/*------------------------*/
	tree_err_t err = TREE_NO_ERR;

	if(obj->parent != obj)
	{
		err = _GetDefinition(obj->parent, call_count);
		if (err)
			return err;
		
		
	}
	else
		return TREE_NO_ERR;

	if(obj->parent->parent != obj->parent)
		printf(", ");
	
	if(obj->parent->left == obj)
		printf("%s", obj->parent->data);
	else if(obj->parent->right == obj)
		printf("не %s", obj->parent->data);
	else
		return GAME_WRONG_TREE;

	//printf(", ");

	return TREE_NO_ERR;
}

static tree_err_t GetDefinition(node_t *obj)
{
	size_t call_count = 0;
	return _GetDefinition(obj, &call_count);
}

static tree_err_t GetDifferent(node_t *obj1, node_t *obj2, node_t *tree)
{
	if(obj1 == NULL || obj2 == NULL || tree == NULL)
		return NODE_NULLPTR;

	if(obj1 == obj2)
	{
		printf(colorize("Так это одно и то же", _BOLD_ _CYAN_));
		return TREE_NO_ERR;
	}

	node_t *common = FindCommonParent(obj1, obj2, tree);
	if(common == NULL)
		return GAME_WRONG_TREE;

	node_t *common_parent = common->parent;

	common->parent = common;
	printf(colorize("%s и %s различны тем, что :\n%s - ", _BOLD_ _CYAN_), obj1->data, obj2->data, obj1->data);
	GetDefinition(obj1);
	
	printf(colorize("\nа %s - ", _BOLD_ _CYAN_), obj2->data);
	GetDefinition(obj2);
	
	
	common->parent = common_parent;
	if(common->parent != common)
	{
		printf(colorize("\nно что %s, что %s - ", _BOLD_ _CYAN_), obj1->data, obj2->data);
		GetDefinition(common);
	}

	putchar('\n');

	return TREE_NO_ERR;
}

static game_mode_t SelectGameMode(void)
{
	char ans[101] = "";

	printf(colorize("\nвведите режим игры [выйти / играть / определение <объект> / сравнить <объект> <объект>]: ", _BOLD_ _BLUE_));
	scanf(" %100[^'\n']", ans);	
	ClearBuf();

	if(strcmp(ans, "играть") == 0)
		return MODE_PLAY;
	
	if(strcmp(ans, "выйти") == 0)
		return MODE_QUIT;
	
	if(strcmp(ans, "определение") == 0)
		return MODE_DEF;
	
	if(strcmp(ans, "сравнить") == 0)
		return MODE_CMP;

	return MODE_UNDEF;
}

int LaunchGame(const char *db_path)
{
	if(db_path == NULL)
	{
		printf("data base cannot be open\n");
		return 1;
	}

	node_t *tree = (node_t *)calloc(1, sizeof(node_t));
	assert(tree);

	char *buf = NULL;
	long buf_size = ReadFileToBuf(db_path, &buf);
	if(buf == NULL || buf_size < 0)
	{
		printf("data base cannot be open\n");
		return 1;
	}
	
	if(buf_size == 0)
		NewTree(tree);
	else
	{
		printf(colorize("file is reading, buf = %lu ...", _GRAY_), (size_t)buf);
		tree->parent = tree;
		if(ReadNode(buf, tree))
			printf(colorize("success\n\n", _GREEN_));
		else
		{
			TreeDestroy(tree, buf, (size_t)buf_size);
			free(buf);
			return 1;
		}
	}

	TreeDumpHTML(tree, "DumpOutput.dot", "./Img", "DumpOutput.html", "After initialization");

	char ans1[101] = "", ans2[101] = "";

	tree_err_t err = TREE_NO_ERR;
	node_t *obj1 = NULL, *obj2 = NULL;

	while(1)
	{
		switch(SelectGameMode())
		{
		case MODE_PLAY:
			err = Play(tree);
			
			break;
			
		case MODE_DEF:
			scanf(" %100[^'\n']", ans1);
			ClearBuf();
			obj1 = FindNode(tree, ans1, StrComp);
			if(obj1)
			{
				printf(colorize("%s - ", _BOLD_ _CYAN_), obj1->data);
				err = GetDefinition(obj1);
				putchar('\n');
			}
			else
				printf(colorize("я не нашел объекта '%s' =(\n", _BOLD_ _RED_), ans1);
			
			break;
		
		case MODE_CMP:
			scanf(" %100[^'\n'] %100[^'\n']", ans1, ans2);
			ClearBuf();
			obj1 = FindNode(tree, ans1, StrComp);
			obj2 = FindNode(tree, ans2, StrComp);
			
			if(obj1 == NULL)
			{
				printf(colorize("я не нашел объекта '%s' =(\n", _BOLD_ _RED_), ans1);
				break;
			}
			if(obj2 == NULL)
			{
				printf(colorize("я не нашел объекта '%s' =(\n", _BOLD_ _RED_), ans2);
				break;
			}

			err = GetDifferent(obj1, obj2, tree);
			break;
		
		case MODE_QUIT:
			err = GAME_QUIT;
			break;
		
		case MODE_UNDEF:
		default:
			printf(colorize("что?\n", _RED_));
			break;
		}

		if(err)
		{
			if(err == GAME_QUIT)
			{
				printf(colorize("сохранить изменения? [да/нет]: ", _YELLOW_));
				scanf(" %100[^'\n']", ans1);
				ClearBuf();
				
				if(strcmp(ans1, "да") == 0)
				{
					FILE *db = fopen(db_path, "w");
					if(PrintTree(tree, db, PREORDER))
					{
						printf(colorize("не удалось записать в файл =(\n", _BOLD_ _RED_));
						return 1;
					}

					printf(colorize("Записано!\n", _GREEN_));
					fclose(db);
					TreeDestroy(tree, buf, (size_t)buf_size);
					free(buf);
					
					return 0;
				}
				
				TreeDestroy(tree, buf, (size_t)buf_size);
				free(buf);
				
				return 0;
			}
			else
			{
				printf(colorize("возникли ошибки (try 'echo $?' to get error code)\n", _BOLD_ _RED_));
				
				TreeDestroy(tree, buf, (size_t)buf_size);
				free(buf);
				
				return (int)err;
			}
		}
	}
}


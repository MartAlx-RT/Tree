#include "Tree.h"

long ReadFileToBuf(const char *file_path, char **buf)
{
    if(file_path == NULL || buf == NULL)
		return -1;

	FILE *file = fopen(file_path, "r");
    if(file == NULL)
    {
		printf("input file cannot be open\n");
		return -1;
	}

    struct stat file_info = {};
    if(stat(file_path, &file_info))
        return -1;
    
    size_t buf_size = (size_t)file_info.st_size;

    *buf = (char *)calloc(buf_size + 2, sizeof(char));
	if(*buf == NULL)
	{
		printf("buffer overflow\n");
		return -1;
	}

	if(fread(*buf, sizeof(char), buf_size, file) != buf_size)
        return -1;

    fclose(file);

    (*buf)[buf_size] = '\0';
    (*buf)[buf_size + 1] = EOF;

    return (long)buf_size;
}

char *ReadNode(char *curs, node_t *node)
{
	//assert(node);
	
	if(node == NULL || curs == NULL)
		return NULL;
	
	if(node->parent == NULL)
	{
		printf("parent nullptr!\n");
		return NULL;
	}

	if(*curs == '\0' || *curs == EOF)
	{
		printf("EOF!\n");
		return NULL;
	}

	if(strncmp(curs, "(nil", 4) == 0)
	{
		curs += 4;
		if(*curs != ')')
		{
			printf("missing ')' after nil\n");
			return NULL;
		}

		if(node->parent->left == node)
		{
			node->parent->left = NULL;
			free(node);
		}
		else
		{
			node->parent->right = NULL;
			free(node);
		}

		return curs + 1;
	}
	
	if(*curs == '(')
	{
		AddLeaves(node);
		curs++;
		if(*curs != '\"')
		{
			printf("Missing opening ' \" '\n");
			return NULL;
		}
		node->data = ++curs;
		curs = strchr(curs, '\"');
		if(curs == NULL)
		{
			printf("Missing closing ' \" '\n");
			return NULL;
		}
		*curs = '\0';
		curs++;
		curs = ReadNode(curs, node->left);
		curs = ReadNode(curs, node->right);
		if(curs == NULL)
		{
			return NULL;
		}
		if(*curs != ')')
		{
			printf("missing ')'\n");
			return NULL;
		}
		return curs + 1;
	}

	printf("Wrong syntaxis\n");
	return NULL;
}


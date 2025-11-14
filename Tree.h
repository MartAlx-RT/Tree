#pragma once

#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/stat.h>

#define TREE_OK_OR_LEAVE(err)              \
    if (err && err != TREE_PARENT_NULLPTR) \
        return err

typedef char * tree_elem_t;
#define MODE_DUMP "%s"
#define MODE_PRINT "\"%s\""
#define TRASH "none"

static const size_t MAX_REC_DEPTH = 100;

typedef enum traversal_type_t
{
    PREORDER,
    POSTORDER,
    INORDER
} traversal_type_t;

typedef struct node_t
{
    tree_elem_t data;
    node_t *left;
    node_t *right;
    node_t *parent;
} node_t;

typedef enum tree_err_t
{
    NODE_NULLPTR = -3,
    TREE_FILE_NULLPTR = -2,
    TREE_NULLPTR = -1,
    TREE_NO_ERR,
    TREE_LOOP,
    TREE_PARENT_NULLPTR,
	GAME_ADD_NODE_ERR,
	GAME_WRONG_TREE,
} tree_err_t;

tree_err_t PrintTree(node_t *node, FILE *dump_file, const traversal_type_t traversal_type);
tree_err_t PrintDiagraphNode(node_t *node, FILE *dot_file);
tree_err_t CreateDigraph(node_t *node, const char *dot_file_path);
tree_err_t TreeDumpHTML(node_t *node, const char *dot_file_path, const char *img_dir_path, const char *html_file_path, const char *caption);

tree_err_t Play(node_t *node);

tree_err_t AddNode(node_t *node, tree_elem_t elem);
tree_err_t TreeDestroyExt(node_t *tree, const char *buf, const size_t buf_size);
tree_err_t TreeDestroy(node_t *tree);
tree_err_t NewTree(node_t *tree);
void ClearBuffer(void);
void AddLeaves(node_t *node);
char *ReadNode(char *curs, node_t *node);
long ReadFileToBuf(const char *file_path, char **buf);


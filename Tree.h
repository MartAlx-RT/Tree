#pragma once

#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <stdlib.h>
#include <math.h>

#define TREE_OK_OR_LEAVE(err)              \
    if (err && err != TREE_PARENT_NULLPTR) \
        return err

typedef double tree_elem_t;
#define MODE "%lg"
#define TRASH NAN
//static const char *DIGRAPH_FMT = 
//"label%lu[shape=record, style=\"rounded, filled\", fillcolor=\"#a8daf0ff\", label=\"{ prnt[%p] | %lg | node[%p] | { l[%p] | r[%p] }}\"];\n"
//"label%lu->label%lu\n";
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

typedef struct tree_t
{
    node_t *node;
} tree_t;

typedef enum tree_err_t
{
    NODE_NULLPTR = -3,
    TREE_FILE_NULLPTR = -2,
    TREE_NULLPTR = -1,
    TREE_NO_ERR,
    TREE_LOOP,
    TREE_PARENT_NULLPTR
} tree_err_t;

tree_err_t PrintNode(node_t *node, FILE *dump_file, const traversal_type_t traversal_type);
tree_err_t PrintDiagraphNode(node_t *node, FILE *dot_file);
tree_err_t CreateDigraph(tree_t *tree, const char *dot_file_path);
tree_err_t TreeDumpHTML(tree_t *tree, const char *dot_file_path, const char *img_dir_path, const char *html_file_path, const char *caption);
tree_err_t AddNode(tree_t *tree, tree_elem_t elem);
//tree_err_t NodeDestroy(node_t *node);
tree_err_t TreeDestroy(tree_t *tree);

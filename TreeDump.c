#include "Tree.h"

tree_err_t PrintNode(node_t *node, FILE *dump_file, const traversal_type_t traversal_type)
{
    static size_t call_count = 0;
    
    if(call_count++ > MAX_REC_DEPTH)
        return TREE_LOOP;

    if (node == NULL)
        return TREE_NULLPTR;
    
    if(dump_file == NULL)
        return TREE_FILE_NULLPTR;
    
    if(node->parent == NULL)
        return TREE_PARENT_NULLPTR;

    tree_err_t err = TREE_NO_ERR;

    putc('(', dump_file);
    
    if(traversal_type == PREORDER)
        fprintf(dump_file, MODE, node->data);
    
    if(node->left)
        err = PrintNode(node->left, dump_file, traversal_type);

    if(traversal_type == INORDER)
        fprintf(dump_file, MODE, node->data);
    
    TREE_OK_OR_LEAVE(err);
    
    if(node->right)
        err = PrintNode(node->right, dump_file, traversal_type);

    if(traversal_type == POSTORDER)
        fprintf(dump_file, MODE, node->data);

    putc(')', dump_file);

    return err;
}

tree_err_t PrintDiagraphNode(node_t *node, FILE *dot_file)
{
	static size_t call_count = 0;

	//printf("count = %lu\n", call_count);
	if (call_count++ > MAX_REC_DEPTH)
		return TREE_LOOP;

    if (node == NULL)
	{
		fprintf(dot_file, "NULL[shape=record, style=\"rounded, filled\", fillcolor=\"#e00808ff\"");
        return TREE_NULLPTR;
	}
    if(dot_file == NULL)
        return TREE_FILE_NULLPTR;
    
    if(node->parent == NULL)
	{
		fprintf(dot_file, "label%lu->NULL!!!(No_parent)\n", (size_t)node);
		return TREE_PARENT_NULLPTR;
	}

    tree_err_t err = TREE_NO_ERR;
	
	//if(node->parent != node)
	fprintf(dot_file, "label%lu[shape=record, style=\"rounded, filled\", fillcolor=\"#a8daf0ff\", "
	"label=\"{ node[%p] | " MODE " | prnt[%p] | { l[%p] | r[%p] }}\"];\n",
	(size_t)node, node, node->data, node->parent, node->left, node->right);
	
	if(node->left)
	{
		if(node->left->parent == node)
			fprintf(dot_file, "label%lu->label%lu [color=purple, dir=both]\n", (size_t)node, (size_t)node->left);
		else
			fprintf(dot_file, "label%lu->label%lu [color=red]\n", (size_t)node, (size_t)node->left);
		
		err = PrintDiagraphNode(node->left, dot_file);
	}

	TREE_OK_OR_LEAVE(err);
	
	if(node->right)
	{
		if(node->right->parent == node)
			fprintf(dot_file, "label%lu->label%lu [color=purple, dir=both]\n", (size_t)node, (size_t)node->right);
		else
			fprintf(dot_file, "label%lu->label%lu [color=red]\n", (size_t)node, (size_t)node->right);
		
		err = PrintDiagraphNode(node->right, dot_file);
	}

	return err;
}

tree_err_t CreateDigraph(tree_t *tree, const char *dot_file_path)
{
    if(tree == NULL)
        return TREE_NULLPTR;

    assert(dot_file_path);

    FILE *dot_file = fopen(dot_file_path, "w");
    if(dot_file == NULL)
    {
        printf("dot file cannot be open\n");
        return TREE_FILE_NULLPTR;
    }

    fprintf(dot_file, "digraph Tree\n{\n");
	tree_err_t err = PrintDiagraphNode(tree->node, dot_file);
	
	

	putc('}', dot_file);
	fclose(dot_file);
	return err;
}

tree_err_t TreeDumpHTML(tree_t *tree, const char *dot_file_path, const char *img_dir_path, const char *html_file_path, const char *caption)
{
	if(tree == NULL)
		return TREE_NULLPTR;
	
	if(dot_file_path == NULL || html_file_path == NULL || img_dir_path == NULL || caption == NULL)
		return TREE_FILE_NULLPTR;

	static size_t dump_call_counter = 0;

    FILE *html_file = NULL;

    if(dump_call_counter == 0)
    {
        html_file = fopen(html_file_path, "w");
        fclose(html_file);
    }

    html_file = fopen(html_file_path, "a");
    if(html_file == NULL)
	{
		printf("file(s) cannot be open\n");
	}
	
    tree_err_t err = CreateDigraph(tree, dot_file_path);

    char system_msg[100] = "";
    snprintf(system_msg, 100, "dot %s -Tsvg -o %s/%lu.svg\n", dot_file_path, img_dir_path, dump_call_counter);
    //printf("sys_msg = {%s}\n", system_msg);
    if(system(system_msg))
	{
		printf("'system' error\n");
		return TREE_FILE_NULLPTR;
	}

	fprintf(html_file, "<pre>\n\t<h2>%s</h2>\n", caption);

    fprintf(html_file, "\t<img  src=\"%s/%lu.svg\" alt=\"%s\" width=\"1200px\"/>\n", img_dir_path,dump_call_counter, caption);

    fprintf(html_file, "\t<hr>\n</pre>\n\n");
    fclose(html_file);

    dump_call_counter++;

    return err;
}


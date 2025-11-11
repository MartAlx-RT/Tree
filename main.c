#include "Tree.h"


int main(void)
{
    tree_t tree = {};

	tree_err_t err = TREE_NO_ERR;

	tree.node = (node_t *)calloc(1, sizeof(node_t));
	tree.node->parent = tree.node;
	
	assert(tree.node);

	AddNode(&tree, 10);
	TreeDumpHTML(&tree, "output.dot", "./Img", "DumpOutput.html", "inserted 10");
	
	AddNode(&tree, 5);
	TreeDumpHTML(&tree, "output.dot", "./Img", "DumpOutput.html", "inserted 5");
	
	AddNode(&tree, 20);
	TreeDumpHTML(&tree, "output.dot", "./Img", "DumpOutput.html", "inserted 20");
	
	AddNode(&tree, 3);
	TreeDumpHTML(&tree, "output.dot", "./Img", "DumpOutput.html", "inserted 3");
	
	AddNode(&tree, 7);
	TreeDumpHTML(&tree, "output.dot", "./Img", "DumpOutput.html", "inserted 7");
	
	AddNode(&tree, 15);
	TreeDumpHTML(&tree, "output.dot", "./Img", "DumpOutput.html", "inserted 15");
	
	AddNode(&tree, 13);
	TreeDumpHTML(&tree, "output.dot", "./Img", "DumpOutput.html", "inserted 13");
	
	AddNode(&tree, 16);
	TreeDumpHTML(&tree, "output.dot", "./Img", "DumpOutput.html", "inserted 16");

	err = AddNode(&tree, 6);
	printf("err = %d\n", err);

	err = TreeDumpHTML(&tree, "output.dot", "./Img", "DumpOutput.html", "after adding 6");
	printf("err = %d\n", err);

	TreeDestroy(&tree);

	//free(fre);

	return 0;
}
#include "Tree.h"


int main(void)
{
    node_t tree = {};

	tree_err_t err = TREE_NO_ERR;

	char *curs = NULL;

	long buf_size = ReadFileToBuf("dump.txt", &curs);
	assert(curs);
	assert(buf_size >= 0);

	//printf("read it: {%s}\n", curs);

	tree.parent = &tree;

	ReadNode(curs, &tree);
	TreeDumpHTML(&tree, "output.dot", "./Img", "DumpOutput.html", "");
	
	//NewTree(&tree);
	//TreeDumpHTML(&tree, "output.dot", "./Img", "DumpOutput.html", "hz");

	//tree_err_t err = TREE_NO_ERR;

	char ans[11] = "";

	while(1)
	{
		Play(&tree);
		printf("Заново? [да или нет]: ");
		scanf(" %10s", ans);
		ClearBuffer();

		if(strcmp(ans, "нет") == 0)
		{
			FILE *dump = fopen("dump.txt", "w");
			assert(dump);

			PrintTree(&tree, dump, PREORDER);
			TreeDestroyExt(&tree, curs, (size_t)buf_size);
			free(curs);
			fclose(dump);
			return 0;
		}
	}


	//fclose(dump);
	TreeDestroyExt(&tree, curs, (size_t)buf_size);
	free(curs);

	//fclose(dump);
	return 0;
}
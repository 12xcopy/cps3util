
#include <stdio.h>
#include "common.h"
#include "system.h"
#include "vfs.h"
#include "simm.h"

#define NAME "cpsutil"

int help()
{
	return puts("usage: " NAME " < -split, -combine > -i jojobanr1.zip -o output.zip"), 1;
}

// combined = 10, 20
// split = -simm1.0


int main(int argc,char** argv)
{
	if (argc <= 0) help();

	char* input = NULL;
	char* output = NULL;

	int num_tasks = 0;
	
	bool split = false;
	bool combine = false;

	for (int i =0;i<argc;i++)
	{
		char* c = argv[i];
		char* next = argv[i+1];
		if (i == argc-1) next = 0;
		if (!strcmp(c,"-i")) input = next;
		if (!strcmp(c,"-o")) output = next;
		if (!strcmp(c,"--help")) return help();

		if (!strcmp(c,"-split")) split= true;
		if (!strcmp(c,"-combine")) combine= true;
	}
	if (!input) return puts("No input"),1;
	struct vfs_s* v = vfs_openauto(input);
	if (!v) puts("no vfs");
	
	struct vfs_s* combo = simm_combine(v);
	if (combo)
	{
		struct vfs_s* n = combo;
		while (n)
		{
			printf("%s : %i : %x\n",n->name,n->size,n->data);

			char bnn[512];
			snprintf(bnn,512,"./out/%s",n->name);
			FILE* f = fopen(bnn,"wb");
			rtassert(f);
			fwrite(n->data,n->size,1,f);
			fclose(f);

			n=n->next;
		}
	}
	

	vfs_free(v);


	return 0;
}
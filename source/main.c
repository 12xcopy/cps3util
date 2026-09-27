
#include <stdio.h>
#include "common.h"
#include "system.h"
#include "vfs.h"
#include "simm.h"

#define NAME "cpsutil"

int help()
{
	const char* helptxt = 
	"good help text will be here\n"
	;
	return puts(helptxt), 1;
}

// combined = 10, 20
// split = -simm1.0
enum
{
	TASK_NONE,
	TASK_SPLIT,
	TASK_COMBINE,
	TASK_ENCRYPT,
	TASK_DECRYPT,
};

int main(int argc,char** argv)
{
	if (argc <= 0) help();

	char* input = NULL;
	char* output = NULL;
	
	bool split = false;
	bool combine = false;
	int cps_game = CPS_UNKNOWN;

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
		if (!strcmp(c,"-g"))
		{
			int C = 1;
			const char* n;
			while ((n = CPS_NAMES[C]))
			{
				if (!strcmp(next,n)) cps_game = C;
				C++;
			}
		}
	}
	if (!input) return puts("No input"),1;
	if (!output)
	{
		char dummy[512];
		snprintf(dummy,512,"./output");
		if (split)
			snprintf(dummy,512,"./split");
		else if (combine)
			snprintf(dummy,512,"./combine");

		output = strdup(dummy);

		printf("no output specified, defaulting to %s\n",output);
	}

	if (!cps_game)
	{
		// try to autodetect
		int C = 1;
		const char* n;
		while ((n = CPS_NAMES[C]))
		{
			if (strstr(input,n)) cps_game = C;
			C++;
		}

		if (cps_game) printf("detected game as %s\n",CPS_NAMES[cps_game]);
		else printf("could not auto detect game\n");
	}

	struct vfs_s* v = vfs_openauto(input);
	if (!v) puts("no vfs");
	
	struct vfs_s* vfsout = 0;
	if (combine)
	{
		vfsout = simm_combine(v);
	}
	else if (split)
	{
		vfsout = simm_split(v,CPS_NAMES[cps_game]);
	}
	if (vfsout)
	{
		vfs_export(vfsout,output);
	}
	else
	{
		puts("(no output)");
	}
	

	vfs_free(v);


	return 0;
}
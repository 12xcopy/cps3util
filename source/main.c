
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
#define MAX_TASKS 32

void task_add(int8_t* a,int new)
{
	for (int i =0;i<MAX_TASKS;i++)
	{
		if (!a[i]) {
			a[i] = new;
			return;
		}
	}
	puts("too many tasks!!");
	abort();
}

int main(int argc,char** argv)
{
	if (argc <= 0) help();

	char* input = NULL;
	char* output = NULL;
	
	
	int8_t tasks[MAX_TASKS];
	memset(tasks,0,sizeof(tasks));
	bool nocompress = false;

	int cps_game = CPS_UNKNOWN;

	for (int i =0;i<argc;i++)
	{
		char* c = argv[i];
		char* next = argv[i+1];
		if (i == argc-1) next = 0;
		if (!strcmp(c,"-i")) input = next;
		if (!strcmp(c,"-o")) output = next;
		if (!strcmp(c,"--help")) return help();

		if (!strcmp(c,"-nocompress")) nocompress=1;
		if (!strcmp(c,"-split")) task_add(tasks,TASK_SPLIT);
		if (!strcmp(c,"-combine")) task_add(tasks,TASK_COMBINE);
		if (!strcmp(c,"-encrypt")) task_add(tasks,TASK_ENCRYPT);
		if (!strcmp(c,"-decrypt")) task_add(tasks,TASK_DECRYPT);
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
	
	struct vfs_s* vfsin = v;
	struct vfs_s* vfsout = 0;

	for (int i =0;i<MAX_TASKS;i++)
	{
		int task = tasks[i];
		if (!task) continue;

		switch(task)
		{
			case TASK_COMBINE: vfsout = simm_combine(vfsin); break;
			case TASK_SPLIT: vfsout = simm_split(vfsin,CPS_NAMES[cps_game]); break;
			case TASK_ENCRYPT: vfsout = simm_encrypt(vfsin,cps_game); break;
			case TASK_DECRYPT: vfsout = simm_decrypt(vfsin,cps_game); break;
		}
		vfs_free(vfsin);
		vfsin = vfsout;
	}

	if (output && vfsout)
	{
		vfs_export(vfsout,output,!nocompress);
		vfs_free(vfsout);
	}
	else
	{
		puts("(no output)");
	}


	return 0;
}
#include "common.h"
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <alloca.h>

static bool chk_dir(const char *path)
{
	struct stat sb;
	if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		return true;
	}
	return false;
}
static bool chk_file(const char *file)
{
	struct stat sb;
	if (stat(file, &sb) == 0 && S_ISREG(sb.st_mode))
	{
		return true;
	}
	return false;
}

static void copy(char *a1, char *a2)
{
	pid_t pid = fork();
	while (pid > 0)
		wait(NULL);
	char *args[] = {"/bin/cp", "-r", a1, a2, 0};
	execv(args[0], args);
}

static void os_mkdir(const char *path, long permissions)
{
	mkdir(path, permissions);
}
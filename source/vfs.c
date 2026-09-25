#include "common.h"
#include "system.h"
#include <zip.h>
#include "vfs.h"
#include <dirent.h>

struct vfs_s *vfs_openzip(const char *zipfile)
{
	rtassert_msgf(chk_file(zipfile), "cant find or access %s", zipfile);
	int err	      = 0;
	printf("opening zip %s\n",zipfile);
	struct zip *z = zip_open(zipfile, ZIP_RDONLY, &err);
	if (z == NULL) rtassert_msgf(0, "%s not a zip file (error code %i)", zipfile, err);

	long_t entries		= zip_get_num_entries(z, 0);
	
	struct vfs_s *outputvfs = 0;
	for (long_t i = 0; i < entries; i++)
	{
		struct zip_stat stat;
		zip_stat_init(&stat);
		if (zip_stat_index(z, i, 0, &stat)) continue;
		long_t namelen = strlen(stat.name);
		rtassert_msg(namelen, "malformed zip");
		char *nuname = strdupa(stat.name);

		switch (nuname[namelen - 1])
		{
		case '/':
		case '\\':
			continue;
		}
		struct zip_file *zf = zip_fopen_index(z, i, 0);
		rtassert_msg(zf, "malformed zip");

		long_t filesize = stat.size;
		
		{
			char *buffer = calloc(filesize, 1);
			long_t bs    = zip_fread(zf, buffer, filesize);
			rtassert_msg(bs == filesize, "malformed zip");
			char fname[512];
			snprintf(fname, 512, "%s", nuname);

			if (!outputvfs) outputvfs = vfs_new(0);
			vfs_append(outputvfs, fname, buffer)->size = filesize;
			
		}
	}
	zip_close(z);
	return outputvfs;
}

struct vfs_s *vfs_opendir(const char *directory)
{
	struct vfs_s * output = 0;
	DIR *dr = opendir(directory);
	struct dirent *de;

	rtassert(dr);

	while ((de = readdir(dr)) != NULL)
	{
		// normal file
		if (de->d_type == DT_REG)
		{
			char path[1024];
			snprintf(path,sizeof(path),"%s/%s",directory,de->d_name);
			FILE* f = fopen(path,"rb");
			fseek(f,0,SEEK_END);
			size_t len = ftell(f);
			fseek(f,0,SEEK_SET);
			char* b = calloc(len,1);
			fwrite(b,len,1,f);
			fclose(f);
			if (!output) output = vfs_new(0);
			vfs_append(output,de->d_name,b)->size = len;
		}
	}

	closedir(dr);
	return output;
}


struct vfs_s* vfs_openauto(const char* thing)
{
	bool is_archive = strstr(thing,".zip");
	if (chk_file(thing) == true) is_archive = true; 
	if (is_archive) return vfs_openzip(thing);
	return vfs_opendir(thing);
}
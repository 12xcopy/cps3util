#include "common.h"
#include "vfs.h"
#include <stdio.h>

int get_num(char c)
{
	return (c = (c - '0')) <= 9 ? c : -1;
}

// for a file called 10, 1 is major 0 is minor
// the file 10 is made up of simm-1.0 simm-1.1 simm-1.2 simm-1.3
bool simm_try_combine(struct vfs_s *x, struct vfs_s *output, long major, long minor)
{
	struct vfs_s *a[4];
	memset(a, 0, sizeof(a));

	struct vfs_s *n = x;
	while (n)
	{
		char *w;
		if ((w = strstr(n->name, "-simm")))
		{
			w += 4;
			int my_major = get_num(w[1]);
			int my_minor = get_num(strstr(w, ".")[1]);

			if (my_major == major)
			{
				// yay
				my_minor -= (minor * 4);
				if (my_minor < 0 || my_minor >= 4) goto label_continue;
				a[my_minor] = n;
			}
		}
	label_continue:
		n = n->next;
	}

	if (!(a[0] && a[1] && a[2] && a[3]))
	{
		return false;
	}
	size_t total_size = a[0]->size + a[1]->size + a[2]->size + a[3]->size;
	char *buffer	  = malloc(total_size);

	// program rom
	if (major <= 2)
	{
		// four way
		// A B C D A B C D
		for (size_t i = 0; i < total_size; i += 4)
		{
			size_t p      = i >> 2;
			buffer[i]     = a[0]->data[p];
			buffer[i + 1] = a[1]->data[p];
			buffer[i + 2] = a[2]->data[p];
			buffer[i + 3] = a[3]->data[p];
		}
	}
	// character rom
	else
	{
		// four way (split)
		// A B A B ...
		// C D C D
		size_t rsize = total_size / 2;
		for (size_t i = 0; i < rsize; i += 2)
		{
			size_t p      = i >> 1;
			buffer[i]     = a[0]->data[p];
			buffer[i + 1] = a[1]->data[p];
			buffer[i + rsize] = a[2]->data[p];
			buffer[i + rsize + 1] = a[3]->data[p];
		}
	}


	char filename[16];
	snprintf(filename, 16, "%li%li", major, minor);
	vfs_append(output, filename, buffer)->size = total_size;
	return true;
}

struct vfs_s *simm_combine(struct vfs_s *source)
{
	struct vfs_s *out = vfs_new(0);
	long_t x, y;
	for (x = 1; true; x++)
	{
		y = 0;
		// test major versions first
		if (simm_try_combine(source,out,x,y))
		{
			// worked, test the minor versions until they dont work
			for (y = 1; true; y++)
			{
				if (!simm_try_combine(source,out,x,y)) break;
			}
		}
		else
		{
			// done
			break;
		}
	}
	if (!out->data)
	{
		// failed
		vfs_free(out);
		return 0;
	}
	return out;
}



struct splitinf_s
{
	char* a[4];
	char* out[4];
	size_t out_size;
};

struct vfs_s *simm_split(struct vfs_s *source,const char* gamename)
{
	struct vfs_s *out = vfs_new(0);
	char outname[64];
	if (gamename)
	{
		snprintf(outname,64,"%s-simm%%i.%%i",gamename);
	}
	else
	{
		snprintf(outname,64,"split-simm%%i.%%i");
	}
	
	while (source)
	{
		const int nm = atoi(source->name);
		if (!nm) goto label_failed;
		// make sure its divisible by four
		if (fract(source->size / 4.0)) goto label_failed;

		const int major = nm / 10;
		int minor = nm % 10;
		const size_t split_size = source->size / 4ul;

		struct splitinf_s inf;
		inf.out_size = split_size;
		inf.out[0] = calloc(split_size,1);
		inf.out[1] = calloc(split_size,1);
		inf.out[2] = calloc(split_size,1);
		inf.out[3] = calloc(split_size,1);
		inf.a[0] = source->data;
		inf.a[1] = source->data + 1;
		inf.a[2] = source->data + 2;
		inf.a[3] = source->data + 3;
		size_t loop_size = source->size;

		if (major >= 3)
		{
			inf.a[2] = source->data + (source->size / 2ul);
			inf.a[3] = inf.a[2] + 1;
			loop_size /= 2;
		}

		for (size_t i =0;i<loop_size;i+=4)
		{
			for (size_t m = 0;m<4;m++)
			{
				char* dest = &inf.out[m][i / 4];
				char* trg = &inf.a[m][i];
				*dest = *trg;

			}
		}

		minor *= 4;
		for (int i =0;i<4;i++)
		{
			char name[128];
			snprintf(name,128,outname,major,minor + i);
			vfs_append(out,name,inf.out[i])->size = split_size;
		}
		printf("split %s\n",source->name);

		source=source->next;
		continue;
		label_failed:
		printf("not splitting %s\n",source->name);
		source=source->next;
		continue;
	}
	
	if (!out->data)
	{
		// failed
		vfs_free(out);
		return 0;
	}
	return out;
}
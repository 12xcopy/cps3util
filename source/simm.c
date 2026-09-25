#include "common.h"
#include "vfs.h"
#include <stdio.h>

int get_num(char c)
{
	return (c = (c - '0')) <= 9 ? c : -1;
}

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

	for (size_t i = 0; i < total_size; i += 4)
	{
		size_t p      = i >> 2;
		buffer[i]     = a[0]->data[p];
		buffer[i + 1] = a[1]->data[p];
		buffer[i + 2] = a[2]->data[p];
		buffer[i + 3] = a[3]->data[p];
	}
	char filename[16];
	snprintf(filename, 16, "%i%i", major, minor);
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
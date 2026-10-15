#include "common.h"

// virtual file system to avoid temporary directories (Not cross platform)

// linked list
struct vfs_s
{
	struct vfs_s* next;
	char name[128];
	size_t size;
	char* data;
};


static struct vfs_s* vfs_new(struct vfs_s* parent)
{
	if (parent && parent->name[0] == '\0') return parent;

	struct vfs_s* n = malloc(sizeof(struct vfs_s));
	if (parent)
	{
		while (parent->next) parent = parent->next;
		parent->next = n;
	}
	n->next=0;
	n->data = 0;
	n->name[0] = '\0';
	return n;
}

static void* vfs_free(struct vfs_s* n)
{	
	while (n)
	{
		struct vfs_s* next = n->next;
		if (n->data) free(n->data);
		free(n);
		n=next;
	}
	return NULL;
}

static struct vfs_s* vfs_find(struct vfs_s* n,const char* file)
{
	while (n)
	{
		struct vfs_s* next = n->next;
		if (!strcmp(file,n->name)) return n;
		n=next;
	}
	return 0;
}

static struct vfs_s* vfs_append(struct vfs_s* n,const char* filename,void* data)
{
	struct vfs_s* m = vfs_new(n);
	strncpy(m->name,filename,sizeof(m->name) - 1);
	m->data = data;
	return m;
}

static struct vfs_s* vfs_clone(struct vfs_s* a)
{
	struct vfs_s* b = vfs_new(0);
	while (a)
	{
		struct vfs_s* c = vfs_append(b,"",0);
		memcpy(c,a,sizeof(struct vfs_s));
		c->next = 0;
		c->data = memdup(c->data,c->size);
		a=a->next;
	}
	return b;
}


struct vfs_s* vfs_openzip(const char* zipfile);
struct vfs_s* vfs_opendir(const char* directory);
struct vfs_s* vfs_openauto(const char* thing);

// output can be a zip file or a directory
void vfs_export(struct vfs_s* vfs,const char* output,bool compress);
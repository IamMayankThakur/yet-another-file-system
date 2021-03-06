//gcc -Wall -o opfile_name file_name.c `pkg-config fuse --cflags --libs`

#define FUSE_USE_VERSION 31
#include <fuse.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/time.h>
#include <libgen.h>
#include <limits.h>
#include <math.h>
#define BLOCKSIZE 4096
#define N 100
#define PATH_MAX_1 100
#define MARKER ")"
// A node of N-ary tree

typedef struct inode
{
	int offset_no;
	int permissions;
	int no_of_links;
	char modify_date[100];
	char *creation_date[100];
	int size;
	int blocks;
} inode;

struct Node
{
	char name[PATH_MAX_1];
	struct Node *child[N]; // An array of pointers for N children
	inode inode;
	char *data;
};
typedef struct Node Node;

Node *root;
Node *temp_node_cxt;
char path_name[100][100];

int name_count = 0, j = 0;
int comparison_count = 0;

// A utility function to create a new N-ary tree node
Node *newNode(char *name)
{
	Node *temp = malloc(sizeof(Node));
	strcpy(temp->name, name);
	for (int i = 0; i < N; i++)
		temp->child[i] = NULL;
	temp->inode.no_of_links = 0;
	temp->inode.size = 0;
	/* struct stat *stbuf = &temp->statit;
    memset(stbuf, 0, sizeof(stsruct stat));*/
	temp->data = NULL;
	return temp;
}
void name_fetch(char *s)
{
	for (int i = 0; i < 100; i++)
		path_name[i][0] = '\0';
	char *temp_direc = malloc(sizeof(s));
	int k = 1;

	strcpy(path_name[0], basename(s));
	strcpy(temp_direc, dirname(s));

	while (strcmp(basename(temp_direc), "/") != 0)
	{
		strcpy(path_name[k++], basename(temp_direc));
		strcpy(temp_direc, dirname(temp_direc));
	}
	int i = 0;
	while (path_name[i][0] != '\0')
	{
		i++;
		name_count++;
	}
}

void path_to_node(Node *root_node, char *path)
{

	if (root_node)
	{
		if (strcmp(root_node->name, path_name[j]) == 0)
		{
			comparison_count += 1;
			temp_node_cxt = root_node;
			printf("\nCurrent Context = %s\n", temp_node_cxt->name);
			if (j > 0)
			{
				path_to_node(temp_node_cxt, path_name[--j]);
			}
			return;
		}
		else
		{
			for (int i = 0; i < 100; i++)
			{
				if (root_node->child[i] != NULL)
					path_to_node(root_node->child[i], path);
			}
		}
	}
}

int search(const char *s)
{
	temp_node_cxt = NULL;
	name_count = 0;
	comparison_count = 0;
	char path_prevent[strlen(s)], path_prevent1[strlen(s)];
	strcpy(path_prevent, s);
	strcpy(path_prevent1, s);
	name_fetch(path_prevent);
	strcpy(path_name[name_count], "root");
	j = name_count;
	path_to_node(root, s);
	if (strcmp(path_prevent1, "/") == 0)
		strcpy(path_prevent1, "root");
	if (strcmp(temp_node_cxt->name, basename(path_prevent1)) != 0)
		return 0;
	if (strcmp(temp_node_cxt->name, basename(path_prevent1)) == 0 && comparison_count == (j))
		return 0;
	return 1;
}
static int getattr_f(const char *path, struct stat *st)
{
	printf("\tAttributes of %s requested\n", path);
	st->st_uid = getuid();
	st->st_gid = getgid();
	st->st_atime = time(NULL);
	st->st_mtime = time(NULL);
	if (strcmp(path, "/") == 0)
	{
		st->st_mode = S_IFDIR | 0777;
		st->st_nlink = 2;
	}
	else if (path[0] == '/' && (path[1] == '.' || (path[1] == 'a' && path[2] == 'u')))
	{
		st->st_mode = S_IFDIR | 0777;
		st->st_nlink = 1;
		st->st_size = 1024;
	}
	else
	{
		if (path[0] == '/')
		{
			if (!search(path))
			{
				return -ENOENT;
			}
			else
			{
				Node *cur = temp_node_cxt;
				st->st_mode = cur->inode.permissions;
				st->st_nlink = cur->inode.no_of_links;
				st->st_size = cur->inode.size;
				st->st_blocks = cur->inode.blocks;
			}
		}
	}
	return 0;
}

static int chmod_f(const char *path, mode_t mode)
{
	if (!search(path))
	{
		return -ENOENT;
	}
	Node *cur = temp_node_cxt;
	cur->inode.permissions = mode;
	return 0;
}

static int mkdir_f(const char *path, mode_t mode)
{
	printf("\n Mkdir called \n");
	int found = 1;
	char path_prevent[strlen(path)];
	strcpy(path_prevent, path);
	if (!search(dirname(path_prevent)))
	{
		found = 0;
		return -ENOENT;
	}
	Node *cur = temp_node_cxt;
	if (found == 1)
	{
		printf("\nCurrent path is %s\n", cur->name);
		for (int i = 0; i < 100; i++)
		{
			if (cur->child[i] == NULL)
			{
				cur->child[i] = newNode(basename(path));
				cur->child[i]->inode.permissions = S_IFDIR | 0777;
				cur->child[i]->inode.no_of_links = 2;
				cur->child[i]->inode.size = 0;
				break;
			}
		}
	}
	else
		printf("\n Invalid path \b");
	for (int i = 0; i < 100; i++)
	{
		if (cur->child[i] != NULL)
			printf("\t Child of cur : %s\t", cur->child[i]->name);
	}

	return 0;
}
void traverse(Node *root, void *buf, fuse_fill_dir_t filler)
{
	if (root)
	{
		//filler(buf,root->name,NULL,0);
		for (int i = 0; i < N; i++)
		{
			if (root->child[i] != NULL)
				filler(buf, root->child[i]->name, NULL, 0);
			else
				break;
		}
	}
}

static int rmdir_f(const char *path)
{
	if (!search(path))
	{
		printf("Invalid path");
		return -ENOENT;
	}
	else
	{
		Node *cur = temp_node_cxt;
		cur = NULL;
		printf("Tryna delete this : %s ", path);
		printf(" Current cxt at : %s", temp_node_cxt->name);
		remove_node(path);
	}
	return 0;
}

static int mv_f(const char *path, const char *new)
{
	if (!search(path))
	{
		printf("\n Invalid Path \n");
		return -ENOENT;
	}
	else
	{
		Node *cur = temp_node_cxt;
		strcpy(cur->name, basename(new));
	}
	return 0;
}

static int mknod_f(const char *path, mode_t mode, dev_t dev)
{
	//exit(0);
	char *path_prevent = malloc(sizeof(char) * strlen(path));
	strcpy(path_prevent, path);
	if (!search(dirname(path_prevent)))
	{
		return -ENOENT;
	}
	Node *cur = temp_node_cxt;
	for (int i = 0; i < N; i++)
	{
		if (cur->child[i] == NULL)
		{
			cur->child[i] = newNode(basename(path));
			cur->child[i]->inode.permissions = mode;
			cur->child[i]->inode.size = 1;
			cur->child[i]->inode.no_of_links = 1;
			break;
		}
	}
	return 0;
}

static int readdir_f(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
	printf("\n Readdir called for %s", path);
	if (strcmp(path, "/") == 0)
		search(path);
	else
	{
		int found = 1;
		if (!search(path))
		{
			found = 0;
			return -ENOENT;
		}
		if (temp_node_cxt == NULL)
			printf("Null temp node");
	}

	Node *cur = temp_node_cxt;
	printf("\n Child = %s--\n", cur->name);
	int i = 0;
	if (cur == NULL)
	{
		return 0;
	}
	else
	{

		traverse(cur, buf, filler);
	}
}

static int write_f(const char *path, const char *buf, size_t size, off_t offset,
				   struct fuse_file_info *f)
{
	printf("\n Write called for %s", path);
	if (!search(path))
		return -ENOENT;
	Node *cur = temp_node_cxt;
	if (offset < 0)
		return -1;
	int l;
	if (cur->data != NULL)
		l = strlen(cur->data);
	else
		l = 0;
	if ((offset + size) > l)
	{
		cur->data = realloc(cur->data, sizeof(char) * (offset + size));
		cur->inode.size = offset + size;
		cur->inode.blocks += (offset + size) / 4096;
		if ((offset + size) % 4096 != 0)
			cur->inode.blocks += 1;
		printf("\n Size is:%d", cur->inode.size);
		//exit(0);
	}
	int k = 0;
	for (int i = offset; i < (offset + size); i++)
	{
		cur->data[i] = buf[k++];
	}
	printf("\n Write complete data = %s----", cur->data);
	return size;
}

static int open_f(const char *path, struct fuse_file_info *f)
{
	printf("\n Open called \n");
	if (!search(path))
		return -ENOENT;
	printf("\n Flags = %d", f->flags);

	return 0;
}

static int read_f(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	printf("\n Read called \n");
	if (!search(path))
	{
		return -ENOENT;
	}
	Node *cur = temp_node_cxt;
	if (cur->data == NULL)
		return -1;
	//buf=malloc(sizeof(char)*strlen(cur->data));
	int l = strlen(cur->data);
	if (offset < 0 || offset > l)
		return -1;
	else
	{
		//int l=strlen(cur->data);
		int flag = 1;
		char s[l];
		strcpy(s, cur->data);
		char t[l];
		int k = 0;
		if (l < (offset + size))
			flag = 0;
		if (flag == 0)
		{
			for (int i = offset; i <= l; i++)
				t[k++] = s[i];
			printf("t is = %s", t);
			strcpy(buf, t);
			printf("\nl-offset=%d\n", l - offset);
			//fflush(buf);
			return (l - offset);
		}
		else
		{
			for (int i = offset; i <= (offset + size); i++)
				t[k++] = s[i];
			strcpy(buf, t);
			return size;
		}
	}
}

static int unlink_f(const char *path)
{
	if (!search(path))
	{
		return -ENOENT;
	}
	Node *cur = temp_node_cxt;
	cur = NULL;
	return 0;
}

void serialize(Node *root, FILE *fp)
{
	// Base case
	if (root == NULL)
		return;

	// Else, store current node and recur for its children
	fprintf(fp, "%s", root->name);
	fprintf(fp, "%s", "#");
	if (root->data != NULL)
		fprintf(fp, "%s", root->data);
	fprintf(fp, "%s", ">");
	fwrite(&root->inode, sizeof(inode), 1, fp);
	for (int i = 0; i < 100 && root->child[i]; i++)
		serialize(root->child[i], fp);

	// Store marker at the end of children
	fprintf(fp, "%s", MARKER);
	//fprintf(fp,"%s","/");
}

int deSerialize(Node **root, FILE *fp)
{
	// Read next item from file. If theere are no more items or next
	// item is marker, then return 1 to indicate same
	char *val = calloc(1, sizeof(char));
	char *data = calloc(1, sizeof(char));
	int i = 0;
	val[i] = fgetc(fp);
	printf("\n Val = %c ", val[i]);
	if (val[i] == EOF)
		return 1;
	if (val[i] == ')')
		return 1;
	while (val[i])
	{

		if (val[i] == '#')
		{
			val[i] = '\0';
			printf("%s\n", val);
			*root = newNode(val);
			break;
		}
		i += 1;
		val = realloc(val, strlen(val) + 1);
		val[i] = fgetc(fp);
		//printf("%c\n",val[i]);
	}
	i = 0;
	data[i] = fgetc(fp);
	while (data[i])
	{
		if (data[i] == '>')
		{
			data[i] = '\0';
			printf("%s\n", data);
			if (root != NULL)
				(*root)->data = data;
			break;
		}
		i += 1;
		data = realloc(data, strlen(data) + 1);
		data[i] = fgetc(fp);
	}
	fread(&((*root)->inode), sizeof(inode), 1, fp);
	// Else create node with this item and recur for children
	if (*root != NULL)
	{
		for (int i = 0; i < N; i++)
			if (deSerialize(&((*root)->child[i]), fp))
				break;
	}
	// Finally return 0 for successful finish
	return 0;
}

int remove_node(char *path)
{
	path_to_node(root, dirname(path));
	Node *temp = temp_node_cxt;
	printf(" parent path name : %s", temp->name);
	int i = 0;
	for (i = 0; i < N; i++)
	{
		if (strcmp(basename(path), temp->child[i]->name) == 0)
		{
			temp->child[i] = NULL;
			return 1;
		}
	}
	free(temp_node_cxt);
	temp_node_cxt = NULL;
	printf("DELETED");
	printf("Now cxt at : %s", temp_node_cxt->name);
}

static struct fuse_operations operations = {
	.mkdir = mkdir_f,
	.readdir = readdir_f,
	.getattr = getattr_f,
	.rmdir = rmdir_f,
	.rename = mv_f,
	.open = open_f,
	.read = read_f,
	.write = write_f,
	.mknod = mknod_f,
	.chmod = chmod_f,
	.unlink = unlink_f,
};

// Driver program to test above functions
int main(int argc, char *argv[])
{
	FILE *fd = fopen("M", "r+");
	if (fd == NULL)
		perror("open");
	root = NULL;
	deSerialize(&root, fd);
	fd = freopen("M", "r+", fd);
	if (root == NULL)
		root = newNode("root");
	umask(0);
	fuse_main(argc, argv, &operations, NULL);
	serialize(root, fd);
	return 0;
}

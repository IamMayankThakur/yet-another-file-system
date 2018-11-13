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
#include <time.h>

#define MAXPATH 128
#define N 100
#define BLOCK_SIZE 4096
#define INODE_SIZE sizeof(inode)
#define NODE_SIZE sizeof(node)
#define MARKER ")"

typedef struct inode
{
	int offset_no;
	int permissions;
	int no_of_links;
	char *modify_date;
	char *creation_date;
} inode;

typedef struct node
{
	int no;
	int is_directory;
	int no_of_children;	// No of used children
	char name[MAXPATH];	// with path
	struct node *child[N]; // An array of pointers for N children
	inode inode;
	char data[BLOCK_SIZE];
} node;

/*
static int mkdir_f(const char *path,mode_t mode)
static int readdir_f(const char *path,void *buf, fuse_fill_dir_t filler, off_t offset,struct fuse_file_info *fi);
void traverse(node *root,void *buf,fuse_fill_dir_t filler);
int search(const char *s);
*/
typedef struct name_inode_map
{
	char *name;
	int offset_no;
} name_inode_map;

// For usage of serialize functions, check the `serialize.c` file in reference
int de_serialize(node **root, FILE *fp);
void serialize(node *root, FILE *fp);
node *newNode(char *name);
void traverse(node *root, void *buf, fuse_fill_dir_t filler);
static int mkdir_f(const char *path, mode_t mode);
static int getattr_f(const char *path, struct stat *stbuf);
int search(const char *s);
void get_node_cxt(node *root1, char *path);
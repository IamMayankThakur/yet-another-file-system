#define FUSE_USE_VERSION 26
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
#include<math.h>
#define BLOCKSIZE 4096
#define PATH_MAX 128
#define N 100

struct inode 
{
    char * name;
    int size;
    int blocks;
    int permissions;
    int mode;
    int no_of_links;
    char * modify_date;
};

typedef struct node
{
    char name[PATH_MAX];
    struct node *child[N];  // An array of pointers for N children
    struct inode inode;
}node;

static int readdir_f(const char *path,void *buf, fuse_fill_dir_t filler, off_t offset,struct fuse_file_info *fi);
void traverse(node *root,void *buf,fuse_fill_dir_t filler);
int search(const char *s);
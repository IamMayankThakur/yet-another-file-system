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
#include <math.h>
#define BLOCKSIZE 4096
#define PATH_MAX 128
#define N 100

struct inode 
{
    char * name; //without path 
    // int size; // in bytes
    // int blocks;
    int offset_no;
    int permissions;
    int is_directory;
    int no_of_links;
    char * modify_date;
    char * creation_date;
};

typedef struct node
{
    int offset_no;
    int data_offset;
    int no_of_children; // No of used children
    char name[PATH_MAX]; // with path
    name_inode_map *child[N];  // An array of pointers for N children
    struct inode inode;
}node;

typedef struct name_inode_map
{
   char * name;
   int offset_no; 
}name_inode_map;

static int readdir_f(const char *path,void *buf, fuse_fill_dir_t filler, off_t offset,struct fuse_file_info *fi);
void traverse(node *root,void *buf,fuse_fill_dir_t filler);
int search(const char *s);


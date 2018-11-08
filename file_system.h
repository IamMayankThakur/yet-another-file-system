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

#define PATH_MAX 128
#define N 100
#define NODE_FILE node_file 
#define INODE_FILE inode_file 
#define DATA_FILE data_file 
#define INODE_SIZE sizeof(inode)
#define NODE_SIZE sizeof(node)
#define OFFSET_SIZE to_be_decided

typedef struct inode 
{
    char name[PATH_MAX]; //without path 
    // int size; // in bytes
    // int blocks;
    int offset_no;
    int permissions;
    int is_directory;
    int no_of_links;
    char * modify_date;
    char * creation_date;
}inode;


typedef struct name_inode_map
{
   char * name;
   int offset_no; 
}name_inode_map;

typedef struct node
{
    int offset_no;
    int data_offset;
    int no_of_children; // No of used children
    char name[PATH_MAX]; // with path
    name_inode_map *child[N];  // An array of pointers for N children
    inode inode;
}node;

/*
static int mkdir_f(const char *path,mode_t mode)
static int readdir_f(const char *path,void *buf, fuse_fill_dir_t filler, off_t offset,struct fuse_file_info *fi);
void traverse(node *root,void *buf,fuse_fill_dir_t filler);
int search(const char *s);
*/
typedef struct name_inode_map
{
   char * name;
   int offset_no; 
}name_inode_map;

// For usage of serialize functions, check the `serialize.c` file in reference
int serialize(void * struct_to_be_serialized,int size, char * file_name); // Returns 1 on success, pass the address of the structure
int deserialize(void * op_struct, int size, char * file_name, int offset); // Returns 1 on success, allocate memory for op_struct in the calling function

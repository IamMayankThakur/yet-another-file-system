//#include "file_system.h"
//#include <fuse.h>
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
int offset_no_next = 1;
int data_offset_next = 1;

#define N 100
#define MAXPATH 100
/*static struct fuse_operations operations = {
    // .mkdir= mkdir_f,
    // .readdir = readdir_f,
    // .getattr = getattr_f,
    // .rmdir = rmdir_f,
    // .rename = mv_f,
    // .open = open_f,
    // .read = read_f,
    // .write = write_f,
    // .truncate = truncate_f,
    // .mknod = mknod_f,
    // .chmod = chmod_f,
    // .unlink = unlink_f,
};*/

typedef struct inode 
{
    char name[MAXPATH]; //without path 
    // int size; // in bytes
    // int blocks;
    int offset_no;
    int permissions;
    int is_directory;
    int no_of_links;
    char modify_date[100];
    char creation_date[100];
}inode;


typedef struct node
{
    int offset_no;
    int data_offset;
    int no_of_children; // No of used children
    char name[MAXPATH]; // with path
    //---------------------CHANGED TYPE---------------------
    struct node* child[N];//name_inode_map *child[N];  // An array of pointers for N children
    inode inode;
}node;

node * current_node;//use for traverse.
node * root;
char names[100][100];
int count_names=0,j=0;
int count_compare=0;
//node path_nodes[]


char* get_time()
{
    time_t current;
    struct tm *local;
    current = time(NULL);
    local = (localtime(&current));
    return asctime(local);
}

node* new_node(char* name)
{
    //printf("%d",strlen(name));
    //permission(?),is_directory, no_of_links to be updated in respective functions.
    //Should go to parent's node and update 'child'
    time_t current;
    struct tm *local;
    node* new = malloc(sizeof(node));
    new->offset_no = offset_no_next;
    //new->data_offset = data_offset_next++; //for directory ??
    new->no_of_children = 0;
    strcpy(new->name,name);//full path or only name?
    for(int i=0;i<N;i++)
    {
        (new->child)[i]=NULL;
    }
    (new->inode).offset_no = offset_no_next++;
    current = time(NULL);
    local = (localtime(&current));
    // strcpy((new->inode).last_access_date,asctime(local));
    strcpy((new->inode).modify_date,asctime(local));
    strcpy((new->inode).creation_date,asctime(local));
    strcpy((new->inode).name,name);
    return new;
}

void trav_help(char *s)
{

    char path[MAXPATH],dir_name[MAXPATH];
    int i;
    for(i=0;i<100;i++)
        names[i][0]='\0';
    int k=0;
    i=1;
    printf("hello");
    return;
    strcpy(names[0],basename(s));
    strcpy(dir_name,dirname(s));
    count_names = 1;
    while(strcmp(basename(dir_name),"/")!=0)
    {
        strcpy(names[k++],basename(dir_name));
        strcpy(dir_name,dirname(dir_name));
        count_names++;
    }
}

void traverse_path(node *root,char *path,node* parent,node*cur)
{
    if(root)
    {
        trav_help(path);
        int i = count_names;
        node* temp = root;
        int t;
        parent = temp;
        for(int i=count_names-1;i>=0;i--)
        {
            if(strcmp(temp->name,names[i])==0)
            {
                
                if(i>0) 
                {
                    t=i-1;
                    for(int j=0;j<N;j++)
                    {
                        if(temp->child[i]!=NULL && strcmp(temp->name,names[t])==0)
                        {   
                            parent = temp;
                            temp = temp->child[t];
                            cur = temp;
                        }
                    }
                }
                else
                {

                }
            }
        }
    }
}

int main()
{
    printf("hello1");
    node* root = new_node("/");
    (root->child)[0] = new_node("Folder1");
    (root->child)[1] = new_node("Folder2");
    node parent;
    node child;
    char pathaa[MAXPATH];
    strcpy(pathaa,"/Folder1");
    traverse_path(root,pathaa,&parent,&child);
    printf("%s %s",child.name,parent.name);
}

/*
static int mknod_f(const char *path,mode_t mode,dev_t dev)
{
    //traverse(); to the parent
    if(not exists){error}
    else
    {
        int i;

        if(i==N)
        {
            printf("NO space.");
        }
        else
        {
            for(i=0;i<N;i++)
            {
                if((parent->child)[i]==NULL)
                {
                    node* new = new_node(path);
                    new->data_offset = data_offset_next++;
                    new->inode.is_directory = 0;
                    new->inode.no_of_links = 1;
                    //new->node.permission = ?;  
                    strcpy(parent->inode.modify_date,new->inode.modify_date);
                    break;
                }
            }
        }
    }
    return 0;
}

static int open_f(const char *path,struct fuse_file_info *f)
{
    //traverse() through the Fs to node.
    if(not found)
    {
        return error;
    }
    else return 0;
}



static int rmdir_f(const char *path)
{
    //get to parent directory. parent = parentnode
    //find the child
    //if(!found){error}
    else
    {
        if(parent->no_of_children>0)
        {   
            node* deleted = (parent->child)[i];
            parent[i] = NULL;
            free(deleted);
            parent->no_of_children--;
            strcpy(parent->modify_date,get_time()); 
            printf("%s deleted. time:%s",path,parent->modify_date);           
        }      
        else
        {
            printf("Make sure the directory is empty");
        }
    }
    return 0;
}

static int mv_f(const char *path,const char *new)
{
    //p=traverse to find parent
    //o=traverse to find the old file
    //n=traverse to new path.

}

*/
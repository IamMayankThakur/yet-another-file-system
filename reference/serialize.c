#include <stdio.h>
#include<fcntl.h> 
#include<errno.h> 
#include <stdlib.h>

typedef struct inode 
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
}inode;

int main()
{
    FILE * fp;
    fp = fopen("foo","wb");
    inode i = {"asdas",1,2,3,4,"inode1","inode1"};
    inode i2 = {"node2",4,5,6,7,"inode2","inode2"};

    fwrite(&i,1,sizeof(inode),fp);
    fwrite(&i2,1,sizeof(inode),fp);
    fclose(fp);
    printf("Read");
    fp = fopen("foo","rb");
    fseek(fp,sizeof(inode),SEEK_CUR);
    inode * j = malloc(sizeof(inode));
    int x = fread(j,1,sizeof(inode),fp);
    printf("%s\n%d\n%d\n%d",j->name,j->is_directory,j->offset_no,j->permissions);
    // printf("%s,%d,%p",j,j,j);
}
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

int serialize(void * struct_to_be_serialized,int size, char * file_name)
{
	FILE * fp = fopen(file_name,"wb+");
	fwrite(struct_to_be_serialized,1,size,fp);
	fclose(fp);
}

void * deserialize(void * op,int size, char * file_name, int offset)
{
    FILE * fp = fopen("foo","rb");
    fread(&op,size,1,fp);
    fclose(fp);
}

int main()
{
    inode i = {"asdas",1,2,3,4,"inode1","inode1"};
    inode i2 = {"node2",4,5,6,7,"inode2","inode2"};
    serialize(&i,sizeof(i),"foo");
    inode j;// = malloc(sizeof(inode));
    // j = deserialize(&j,sizeof(inode),"foo",0);

    // This works but the deserialize function doesnt work.
    FILE * fp = fopen("foo","rb");
    fread(&j,sizeof(inode),1,fp);
    fclose(fp);
    printf("%s\n%d\n%d\n%d",j.name,j.is_directory,j.offset_no,j.permissions);
}



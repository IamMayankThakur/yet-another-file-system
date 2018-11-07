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

void * deserialize(inode op,size_t size, char * file_name, int offset);

int main()
{
    inode i = {"asdas",1,2,3,4,"inode1","inode1"};
    inode i2 = {"node2",4,5,6,7,"inode2","inode2"};
    serialize(&i,sizeof(i),"foo");
    inode j;//* j = malloc(sizeof(inode));
    deserialize(j,sizeof(inode),"foo",0);
    printf("%s\n%d\n%d\n%d",j.name,j.is_directory,j.offset_no,j.permissions);
}

int serialize(void * struct_to_be_serialized,int size, char * file_name)
{
	FILE * fp = fopen(file_name,"wb+");
	fwrite(struct_to_be_serialized,1,size,fp);
	fclose(fp);
}

void * deserialize(inode op,size_t size, char * file_name, int offset)
{
	FILE * fp = fopen(file_name,"rb");
	// fseek(fp,size * offset,SEEK_CUR);
	fread(&op,1,size,fp);
	fclose(fp);
    // return &op;
}
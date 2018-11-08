#include "file_system.h"

int offset_no_next = 1;
int data_offset_next = 1;
static struct fuse_operations operations = {
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
};

node * temp_node_cxt = NULL;
node * root;
char names[100][100];
int count_names=0,j=0;
int count_compare=0;
/*
initialiseFS()
{
	root = new_node();
}
*/
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

int main(int argc,char *argv[])
{    
	//node* hi = new_node("hi");
	//printf("%s-%s-",hi->inod.modify_date,hi->inod.name);
    fuse_main( argc, argv, &operations, NULL );
    return 0;
}
void traverse(node *root, void *buf, fuse_fill_dir_t filler)
{
    if (root)
    {
        for (int i = 0; i < root->no_of_children; i++)
	    {
            if(root->child[i]!=NULL)
            {
                filler(buf,root->child[i]->name,NULL,0);
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        return ;
    }
}


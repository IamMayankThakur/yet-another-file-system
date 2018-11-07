#include "file_system.h"
#include <time.h>
#include <string.h>

int BLOCK_NUMBER = 0;
int offset_no_next = 1;
int data_offset_next = 1;
static struct fuse_operations operations = {
    // .mkdir= mkdir_f,
    //.readdir = readdir_f,
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
	strcpy((new->inode).last_access_date,asctime(local));
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



/*
static int getattr_f( const char *path, struct stat *st )
{
	printf( "\tAttributes of %s requested\n", path );
	//st->st_uid = getuid(); 
	st->st_gid = getgid(); 
	st->st_atime = time( NULL ); 
	st->st_mtime = time( NULL ); 
	//st->is_directory = 1;
	if ( strcmp( path, "/" ) == 0 )
	{
		st->st_mode = S_IFDIR | 0755;
		st->st_nlink = 2; 
	}
	else if(path[0]=='/' && (path[1]=='.' || (path[1]=='a' && path[2]=='u')))
	{
		st->st_mode = S_IFDIR | 0777;
		st->st_nlink = 1;	
	 	st->st_size = 1024;
	}
	else
	{
		if(path[0]=='/')
		{
			printf("----attributes---");
			if(!search(path))
			{
				printf("------WHAT!!!!-----");
				return -ENOENT;
			}
			else
			{
				node *cur=temp_node_cxt;
				st->st_mode = cur->statit.st_mode;
				st->st_nlink = cur->statit.st_nlink;
				st->st_size = cur->statit.st_size;
				st->st_blocks = cur->statit.st_blocks;
			}
		}

		
	}
	
	return 0;
}*/

/*
void traverse(char* path,char* node)
{

}





static int open_f(const char *path,struct fuse_file_info *f)
{

}

static int mkdir_f(const char *path,mode_t mode)
{

}



void traverse(node *root,void *buf,fuse_fill_dir_t filler)
{
    if (root)
    {
        //filler(buf,root->name,NULL,0);
        for (int i = 0; i < N; i++)
	{
	    if(root->child[i]!=NULL)
            	 filler(buf,root->child[i]->name,NULL,0);
	    else
		break;
    	}
    }
}

static int readdir_f(const char *path,void *buf, fuse_fill_dir_t filler, off_t offset,struct fuse_file_info *fi)
{

	printf("----%s--readdir--",path);
	if(strcmp(path,"/")==0)
		search(path);
	else
	{
		int found=1;
		if(!search(path))
		{
			found=0;
			return -ENOENT;
		}
		if(temp_node_cxt==NULL)
			printf("--temp_node is null--");
		
	}
	
	node *cur=temp_node_cxt;
	printf("--child=%s----",cur->name);
	int i=0;
	if(cur==NULL)
	{
		printf("how can it be null-----");
		return 0;
	}
	//else if(!S_ISDIR(cur->statit.st_mode))
	//{
	//	printf("----what is happening-----");
	//	return -ENOTDIR;
	//}
	else
	{

		traverse(cur,buf,filler);
	}
}

int search(const char *s)
{
	temp_node_cxt=NULL;
	count_names=0;
	count_compare=0;
	char path_prevent[strlen(s)],path_prevent1[strlen(s)];
	strcpy(path_prevent,s);
	strcpy(path_prevent1,s);
	get_names(path_prevent);
        strcpy(names[count_names],"root");
	j=count_names;
	getNodecxt(root,s);
	if(strcmp(path_prevent1,"/")==0)
		strcpy(path_prevent1,"root");
	if(strcmp(temp_node_cxt->name,basename(path_prevent1))!=0)  
		return 0;
	if(strcmp(temp_node_cxt->name,basename(path_prevent1))==0 && count_compare==(j))
		return 0;
	return 1;
}
void get_names(char *s)
{
	for(int i=0;i<100;i++)
		names[i][0]='\0';
	char *dir=malloc(sizeof(s));
	int k=1;
	strcpy(names[0],basename(s));
	strcpy(dir,dirname(s));
	while(strcmp(basename(dir),"/")!=0)
	{
		strcpy(names[k++],basename(dir));
		strcpy(dir,dirname(dir));
	}
	int i=0;
	while(names[i][0]!='\0')
	{
		i++;
		count_names++;
	}

}
void getNodecxt(node *root1,char *path)
{
	
	if(root1)
	{
		if(strcmp(root1->name,names[j])==0)
		{
			count_compare+=1;
			temp_node_cxt=root1;
			printf("\ntemp=%s\n",temp_node_cxt->name);
			if(j>0)
			{
				getNodecxt(temp_node_cxt,names[--j]);
			}
			return;
		}
		else
		{
			for(int i=0;i<100;i++)
			{
				if(root1->child[i]!=NULL)
					getNodecxt(root1->child[i],path);
			}
		}
	}
}
*/

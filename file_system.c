#include "file_system.h"

int BLOCK_NUMBER = 0;

static struct fuse_operations operations = {
    // .mkdir= mkdir_f,
    .readdir = readdir_f,
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
node * root = "/";
char names[100][100];
int count_names=0,j=0;
int count_compare=0;

int main(int argc,char *argv[])
{    
    fuse_main( argc, argv, &operations, NULL );
    return 0;
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
	/*else if(!S_ISDIR(cur->statit.st_mode))
	{
		printf("----what is happening-----");
		return -ENOTDIR;
	}*/
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
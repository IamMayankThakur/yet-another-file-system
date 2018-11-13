#include "file_system.h"

int offset_no_next = 1;
int data_offset_next = 1;
static struct fuse_operations operations = {
	// .mkdir = mkdir_f,
	// .readdir = readdir_f,
	.getattr = getattr_f,
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

node *temp_node_cxt = NULL;
node *root;
char names[100][100];
int count_names = 0, j = 0;
int count_compare = 0;
/*
initialiseFS()
{
	root = new_node();
}
*/
node *new_node(char *name)
{
	//printf("%d",strlen(name));
	//permission(?),is_directory, no_of_links to be updated in respective functions.
	//Should go to parent's node and update 'child'
	time_t current;
	struct tm *local;
	node *new = malloc(sizeof(node));
	// new->offset_no = offset_no_next;
	//new->data_offset = data_offset_next++; //for directory ??
	new->no_of_children = 0;
	strcpy(new->name, name); //full path or only name?
	for (int i = 0; i < N; i++)
	{
		(new->child)[i] = NULL;
	}
	(new->inode).offset_no = offset_no_next++;
	current = time(NULL);
	local = (localtime(&current));
	// strcpy((new->inode).last_access_date,asctime(local));
	strcpy((new->inode).modify_date, asctime(local));
	strcpy((new->inode).creation_date, asctime(local));
	// strcpy((new->inode).name, name);
	return new;
}

int main(int argc, char *argv[])
{
	//node* hi = new_node("hi");
	//printf("%s-%s-",hi->inod.modify_date,hi->inod.name);
	printf("Starting . . . \n");
	fuse_main(argc, argv, &operations, NULL);
	return 0;
}
void traverse__(node *root, void *buf, fuse_fill_dir_t filler)
{
	if (root)
	{
		for (int i = 0; i < root->no_of_children; i++)
		{
			if (root->child[i] != NULL)
			{
				filler(buf, root->child[i]->name, NULL, 0);
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		return;
	}
}

void serialize(node *root, FILE *fp)
{
	// Base case
	if (root == NULL)
		return;

	// Else, store current node and recur for its children
	fprintf(fp, "%s", root->name);
	fprintf(fp, "%s", "$");
	if (root->data != NULL)
		fprintf(fp, "%s", root->data);
	fprintf(fp, "%s", "%");
	fwrite(&root->inode, sizeof(inode), 1, fp);
	for (int i = 0; i < 100 && root->child[i]; i++)
		serialize(root->child[i], fp);

	// Store marker at the end of children
	fprintf(fp, "%s", MARKER);
	//fprintf(fp,"%s","/");
}

int de_serialize(node **root, FILE *fp)
{
	// Read next item from file. If theere are no more items or next
	// OK COOL
	// item is marker, then return 1 to indicate same
	char *val = calloc(1, sizeof(char));
	char *data = calloc(1, sizeof(char));
	int i = 0;
	val[i] = fgetc(fp);
	if (val[i] == EOF)
		return 1;
	if (val[i] == ')')
		return 1;
	while (val[i])
	{
		if (val[i] == '$')
		{
			val[i] = '\0';
			printf("%s\n", val);
			*root = newNode(val);
			break;
		}
		i += 1;
		val = realloc(val, strlen(val) + 1);
		val[i] = fgetc(fp);
		//printf("%c\n",val[i]);
	}
	i = 0;
	data[i] = fgetc(fp);
	while (data[i])
	{
		if (data[i] == '%')
		{
			data[i] = '\0';
			printf("%s\n", data);
			if (root != NULL)
				strcpy((*root)->data, data);
			break;
		}
		i += 1;
		data = realloc(data, strlen(data) + 1);
		data[i] = fgetc(fp);
	}
	fread(&((*root)->inode), sizeof(inode), 1, fp);
	// Else create node with this item and recur for children
	if (*root != NULL)
	{
		for (int i = 0; i < 100; i++)
			if (de_serialize(&((*root)->child[i]), fp))
			{
				break;
			}
	}
	// Finally return 0 for successful finish
	return 0;
}

static int getattr_f(const char *path, struct stat *stbuf)
{
	node *n;
	// int valid = check_path(path, &n);
	int valid = 1;
	if (!valid)
	{
		return -ENOENT; //No such file or directory
	}
	else
	{
		// *stbuf = n->data.st;
		return 0;
	}
}

node *newNode(char *name)
{
	node *temp = malloc(sizeof(node));
	strcpy(temp->name, name);
	for (int i = 0; i < N; i++)
		temp->child[i] = NULL;
	temp->inode.no_of_links = 0;
	/* struct stat *stbuf = &temp->statit;
    memset(stbuf, 0, sizeof(struct stat));*/
	strcpy(temp->data, NULL);
	return temp;
}

void traverse(node *root, void *buf, fuse_fill_dir_t filler)
{
	if (root)
	{
		//filler(buf,root->name,NULL,0);
		for (int i = 0; i < N; i++)
		{
			if (root->child[i] != NULL)
				filler(buf, root->child[i]->name, NULL, 0);
			else
				break;
		}
	}
}

static int mkdir_f(const char *path, mode_t mode)
{
	printf("--------------------mkdir called-------------------");
	int found = 1;
	char path_prevent[strlen(path)];
	strcpy(path_prevent, path);
	if (!search(dirname(path_prevent)))
	{
		printf("---sahi shak tha--");
		found = 0;
		return -ENOENT;
	}
	node *cur = temp_node_cxt;
	if (found == 1)
	{
		printf("----Valid path---");
		printf("--current is--%s", cur->name);
		for (int i = 0; i < 100; i++)
		{
			if (cur->child[i] == NULL)
			{
				cur->child[i] = newNode(basename(path));
				cur->child[i]->inode.permissions = S_IFDIR | 0777;
				cur->child[i]->inode.no_of_links;
				break;
			}
		}
	}
	else
		printf("---invalid path");
	for (int i = 0; i < 100; i++)
	{
		if (cur->child[i] != NULL)
			printf("--extreme=%s---", cur->child[i]->name);
	}

	return 0;
}

int search(const char *s)
{
	temp_node_cxt = NULL;
	count_names = 0;
	count_compare = 0;
	char path_prevent[strlen(s)], path_prevent1[strlen(s)];
	strcpy(path_prevent, s);
	strcpy(path_prevent1, s);
	get_names(path_prevent);
	strcpy(names[count_names], "root");
	j = count_names;
	get_node_cxt(root, s);
	if (strcmp(path_prevent1, "/") == 0)
		strcpy(path_prevent1, "root");
	if (strcmp(temp_node_cxt->name, basename(path_prevent1)) != 0)
		return 0;
	if (strcmp(temp_node_cxt->name, basename(path_prevent1)) == 0 && count_compare == (j))
		return 0;
	return 1;
}

void get_names(char *s)
{
	for (int i = 0; i < 100; i++)
		names[i][0] = '\0';
	char *dir = malloc(sizeof(s));
	int k = 1;
	strcpy(names[0], basename(s));
	strcpy(dir, dirname(s));
	while (strcmp(basename(dir), "/") != 0)
	{
		strcpy(names[k++], basename(dir));
		strcpy(dir, dirname(dir));
	}
	int i = 0;
	while (names[i][0] != '\0')
	{
		i++;
		count_names++;
	}
}

void get_node_cxt(node *root1, char *path)
{

	if (root1)
	{
		if (strcmp(root1->name, names[j]) == 0)
		{
			count_compare += 1;
			temp_node_cxt = root1;
			printf("\ntemp=%s\n", temp_node_cxt->name);
			if (j > 0)
			{
				get_node_cxt(temp_node_cxt, names[--j]);
			}
			return;
		}
		else
		{
			for (int i = 0; i < 100; i++)
			{
				if (root1->child[i] != NULL)
					get_node_cxt(root1->child[i], path);
			}
		}
	}
}
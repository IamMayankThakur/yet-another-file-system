#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#define MARKER ")"
typedef struct inode
{
	char *name; //without path
	// int size; // in bytes
	// int blocks;
	int offset_no;
	int permissions;
	int is_directory;
	int no_of_links;
	char *modify_date;
	char *creation_date;
} inode;

typedef struct node
{
	int offset_no;
	int data_offset;
	int no_of_children;		 // No of used children
	char name[100];			 // with path
	struct node *child[100]; // An array of pointers for N children
	inode inode;
	char *data;
} node;

node *newNode(char *name)
{
	node *temp = malloc(sizeof(node));
	strcpy(temp->name, name);
	for (int i = 0; i < 100; i++)
		temp->child[i] = NULL;
	temp->inode.no_of_links = 0;
	// temp->inode.st_size = 0;
	/* struct stat *stbuf = &temp->statit;
    memset(stbuf, 0, sizeof(struct stat));*/
	temp->data = NULL;
	return temp;
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
				(*root)->data = data;
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
				break;
	}
	// Finally return 0 for successful finish
	return 0;
}

int main()
{
	inode i = {"asdas", 1, 2, 3, 4, "inode1", "inode1"};
	inode i2 = {"node2", 4, 5, 6, 7, "inode2", "inode2"};
	FILE *fp = fopen("foo", "r+");
	node *root = newNode("root");
	root->child[0] = newNode("A");
	root->child[1] = newNode("B");
	// serialize(root, fp);
	// root = NULL;
	de_serialize(&root, fp);
	// printf("%s\t", root->name);
	fclose(fp);
}

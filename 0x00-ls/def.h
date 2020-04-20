#include <stdlib.h>
#include <dirent.h>

typedef struct File
{
	char path[4096];
	char name[256];
	char type;
	char perm[9];
	char *time;
	long size;
	char *user;
	char *group;
} File;

File **get_list(char *path);
File **create_list(size_t size);
DIR *open_directory(char **path);
int file_count(char *path);
void cleanup(File **file_list);


/* definitions for string utilities */
int _strlen(char *s);
char *_strncat(char *dest, char *src, int n);
char *_strncpy(char *dest, char *src, int n);

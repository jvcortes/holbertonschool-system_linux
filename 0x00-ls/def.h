#ifndef LS
#define LS

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

File **get_list(char *path, int hidden);
File **create_list(size_t size);
DIR *open_directory(char **path);
int file_count(char *path, int hidden);
void cleanup(File **file_list);

char **get_shortlist(char *path, int hidden);
void print_shortlist(char **list);
void cleanup_shortlist(char **list);

/* definitions for string utilities */
int _strlen(char *s);
char *_strncat(char *dest, char *src, int n);
char *_strncpy(char *dest, char *src, int n);

#endif /* ifndef LS */

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

typedef struct Option
{
	int vertical_listing;
} Opt;

File **get_long_list(char *path, int hidden);
File **create_long_list(size_t size);
DIR *open_directory(char *path);
int file_count(char *path, int hidden);
void cleanup(File **file_list);

char **create_array(ssize_t size);
char **get_list(char *path, int hidden);
void print_list(char **list);
void cleanup_list(char **list);

char *get_file(char *path);
void print_file(char *file);
int is_file(char *path);
int path_exists(char *path);
int can_read_dir(char *path);

void print_single(char *path);
void print_many(char **arr, size_t size);
void print_files(char **arr, size_t size, size_t count);
void print_directories(char **arr, size_t size, size_t count);

/* definitions for string utilities */
int _strlen(const char *s);
char *_strncat(char *dest, char *src, int n);
char *_strncpy(char *dest, const char *src, int n);
char *_strdup(const char *s);
int _strcmp(char *s1, char *s2);

/* definitions for runtime and error handling utilities */
int status(int num);
int set_opt(char *opt, int num);

#define DEFAULT_LISTING 0
#define VERTICAL_LISTING 1

#endif /* ifndef LS */

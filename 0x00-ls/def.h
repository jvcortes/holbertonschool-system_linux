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

typedef struct Settings
{
	int vertical_listing;
	int visibility;
} Settings;

typedef struct Flag
{
	char *long_name;
	char name;
	char *option;
	int value;
} Flag;

#define DEFAULT_LISTING 0
#define VERTICAL_LISTING 1

#define LIST_VISIBLE 0
#define LIST_HIDDEN 1
#define LIST_ALMOST_ALL 2

#define ARGS { \
		{"", '1', "listing", VERTICAL_LISTING},\
		{"", 'a', "visibility", LIST_HIDDEN},\
		{NULL, '\0', NULL, 0}\
	}


File **get_long_list(char *path, int hidden);
File **create_long_list(size_t size);
void cleanup(File **file_list);

DIR *open_directory(char *path);
int file_count(char *path);
void print_directory(char *path);

char **create_array(ssize_t size);
char **get_list(char *path);
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
void print_directories(
		char **arr,
		size_t size,
		size_t count,
		size_t dir_count
);

/* definitions for string utilities */
int _strlen(const char *s);
char *_strncat(char *dest, char *src, int n);
char *_strncpy(char *dest, const char *src, int n);
char *_strdup(const char *s);
int _strcmp(char *s1, char *s2);
char *strstrp(char *s, char *c);
int stridx(char *s, char c);

/* sorting utilities */
void quicksort_str(char **arr, size_t size);
int lomuto_partition(char **arr, size_t size);

/* array utilities */
char **copy(char **arr, size_t size);
char **filter_null(char **arr, size_t size);
size_t str_array_size(char **arr);


/* definitions for runtime and error handling utilities */
int status(int num);
#define RETRIEVE_STATUS -1

int set_opt(char *opt, int num);
void check_opts(char *args[], int argc);
#define	RETRIEVE_OPT -1

int check_opt_arg(char *arg);


#endif /* ifndef LS */

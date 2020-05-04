#ifndef LS
#define LS

#include <stdlib.h>
#include <dirent.h>

typedef struct file_t
{
	char path[4096];
	char name[256];
	char type;
	int  nlink;
	char perm[9];
	char time[32];
	long size;
	char *user;
	char *group;
} File;

typedef struct settings_t
{
	int vertical_listing;
	int visibility;
} Settings;

typedef struct flag_t
{
	char *long_name;
	char name;
	char *option;
	int value;
} Flag;

typedef struct long_list_formatting_t
{
	int user_spaces;
	int group_spaces;
	int nlink_spaces;
	int size_spaces;
} LongListFormatting;

#define DEFAULT_LISTING 0
#define VERTICAL_LISTING 1
#define LONG_LISTING 2

#define LIST_VISIBLE 0
#define LIST_HIDDEN 1
#define LIST_ALMOST_ALL 2

#define ARGS { \
		{"", '1', "listing", VERTICAL_LISTING},\
		{"", 'l', "listing", LONG_LISTING},\
		{"", 'a', "visibility", LIST_HIDDEN},\
		{"", 'A', "visibility", LIST_ALMOST_ALL},\
		{NULL, '\0', NULL, 0}\
	}


File **get_long_list(char *path, int hidden);
File **create_long_list(size_t size);
void print_files_long_format(char **arr);
void print_file_long_format(File *file, LongListFormatting *formatting);
void set_file_details(File *file, char *path);
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

/* functions for formatting */
LongListFormatting *get_formatting(File **arr);
void format_time(char *s, size_t size, const time_t *t);

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

/* math utilities */
int digits(int n);

/* definitions for runtime and error handling utilities */
int status(int num);
#define RETRIEVE_STATUS -1

int set_opt(char *opt, int num);
void check_opts(char *args[], int argc);
#define	RETRIEVE_OPT -1

int check_opt_arg(char *arg);


#endif /* ifndef LS */

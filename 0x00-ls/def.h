#ifndef LS
#define LS

#include <stdlib.h>
#include <dirent.h>

/**
 * struct file_t - Contains the information of a file entry.
 *
 * @path: abstolute or relative path to the file.
 * @linkname: symlink name of the file, if the file is a symlink.
 * @name: name of the file.
 * @type: character indicating the type of the entry.
 *  - '-': file
 *  - 'd': directory
 *  - 'c': character special file
 *  - 'b': block special file
 *  - 'p': FIFO (named file)
 *  - 'l': symlink
 *  - 's': socket
 *  @nlink: number of the symlinks associated with the entry.
 *  @perm: string which contains the info about the permissions for
 *  the entry. The string is divided in three sections of three characters,
 *  from left to right:
 *  - Permissions for the owner user.
 *  - Permissions for the owner group.
 *  - Permissions for other users.
 *  Each section indicates:
 *  - Read permissions, character 'r'.
 *  - Write permissions, character 'w'.
 *  - Execution permissions, character 'x'.
 *  If the user/group lacks a permission, a dash character ('-') will be
 *  put instead.
 *  @time: date and time of the entry creation.
 *  @size: size, in bytes of the enty. 
 *  @user: owner user.
 *  @group: owner group.
 */
typedef struct file_t
{
	char path[4096];
	char linkname[4096];
	char name[256];
	char type;
	int  nlink;
	char perm[9];
	char time[32];
	long size;
	char *user;
	char *group;
} File;

/**
 * struct settings_t - defines the runtime settings for this application.
 * @vertical_listing: defines the way the files and directories should be
 * printed.
 * @visibility: defines which files should be visible.
 */
typedef struct settings_t
{
	int vertical_listing;
	int visibility;
} Settings;

/**
 * struct flag_t - defines a command line flag.
 * settings_t struct.
 *
 * @full_name: option full name, this can be used in the command line args.
 * @name: option short name, this can be used in the command line args.
 * @option: settings variable name, has to match with the a setting inside a
 * @value: zero or positive value for the setting.
 */
typedef struct flag_t
{
	char *full_name;
	char name;
	char *option;
	int value;
} Flag;

/**
 * struct long_list_formatting_t - defines the formatting for a long list formatted list
 * of files and directories.
 *
 * @user_spaces: right padded spaces for the user column.
 * @group_spaces: right padded spaces for the group column.
 * @nlink_spaces: left padded spaces for the column containing the number of links.
 * @size_spaces: left padded spaces for the size column.
 */
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
int print_files_long_format(char **arr, char *basepath);
void print_file_long_format(File *file, LongListFormatting *formatting);
void set_file_details(File *file, char *name, char *basepath);
void cleanup_long_list(File **file_list);

DIR *open_directory(char *path);
int file_count(char *path);
void print_directory(char *path);

char **create_array(ssize_t size);
char **get_list(char *path);
void print_list(char **list, char *basepath);
void cleanup(char **list);

void print_file(char *file);
int is_dir(char *path);
int is_file(char *path);
int path_exists(char *path);
int can_read_dir(char *path);

void print(char **arr);
void print_files(char **arr);
void print_directories(char **arr, int many);

/* functions used for filtering */
char **get_files_from_args(char **args);
char **get_directories_from_args(char **args);

/* functions used for formatting */
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
char *_memset(char *s, char b, unsigned int n);


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

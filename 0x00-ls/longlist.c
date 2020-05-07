#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include "def.h"


int
print_files_long_format(char **arr, char *basepath)
{
	struct long_list_formatting_t *formatting;
	struct file_t **files;
	int i, j;
	size_t size;

	if (arr == NULL || arr[0] == NULL)
		return (0);

	for (i = 0, size = 0; arr[i] != NULL; i++)
		size++;

	files = create_long_list(size);

	for (i = 0, j = 0; arr[i] != NULL; i++)
		set_file_details(files[j++], arr[i], basepath);

	formatting = get_formatting(files);

	for (j = 0; files[j] != NULL; j++)
	{
		print_file_long_format(files[j], formatting);
		if (files[j + 1] != NULL)
			printf("\n");
	}

	free(formatting);
	cleanup_long_list(files);

	return (1);
}

void
print_file_long_format(File *file, LongListFormatting *formatting)
{
	int i;

	printf("%c", file->type);
	for (i = 0; i <= 8; i++)
		printf("%c", file->perm[i]);

	printf(" %*d", formatting->nlink_spaces, file->nlink);
	printf(" %*s", formatting->user_spaces, file->user);
	printf(" %-*s", formatting->group_spaces, file->group);
	printf(" %*ld", formatting->size_spaces, file->size);
	printf(" %s", file->time);
	printf(" %s", file->name);
	if (file->linkname[0] != '\0')
		printf(" -> %s", file->linkname);
}


void
set_file_details(File *file, char *name, char *basepath)
{
	struct stat filestat;
	int buf_size;
	char *user;
	char *group;
	char *t;

	if (name == NULL || file == NULL || basepath == NULL)
		return;

	_strncpy(file->path, basepath, sizeof(file->path) - 1);
	if (basepath[0] != '\0' && basepath[_strlen(basepath) - 1] != '/')
		_strncat(file->path, "/", sizeof(file->path) - _strlen(file->path) - 1);
	_strncat(file->path, name, sizeof(file->path) - _strlen(file->path) - 1);

	_strncpy(file->name, name, sizeof(file->name) - 1);
	if ((lstat(file->path, &filestat)) == -1)
	{
		perror("");
		exit(2);
	}

	switch (filestat.st_mode & S_IFMT)
	{
		case S_IFREG:
			file->type = '-';
			break;
		case S_IFDIR:
			file->type = 'd';
			break;
		case S_IFCHR:
			file->type = 'c';
			break;
		case S_IFBLK:
			file->type = 'b';
			break;
		case S_IFIFO:
			file->type = 'p';
			break;
		case S_IFLNK:
			readlink(file->path, file->linkname, sizeof(file->linkname));
			file->type = 'l';
			break;
		case S_IFSOCK:
			file->type = 's';
			break;
	}

	file->perm[0] = filestat.st_mode & S_IRUSR ? 'r' : '-';
	file->perm[1] = filestat.st_mode & S_IWUSR ? 'w' : '-';
	file->perm[2] = filestat.st_mode & S_IXUSR ? 'x' : '-';
	file->perm[3] = filestat.st_mode & S_IRGRP ? 'r' : '-';
	file->perm[4] = filestat.st_mode & S_IWGRP ? 'w' : '-';
	file->perm[5] = filestat.st_mode & S_IXGRP ? 'x' : '-';
	file->perm[6] = filestat.st_mode & S_IROTH ? 'r' : '-';
	file->perm[7] = filestat.st_mode & S_IWOTH ? 'w' : '-';
	file->perm[8] = filestat.st_mode & S_IXOTH ? 'x' : '-';

	file->nlink = filestat.st_nlink;
	t = ctime(&(filestat.st_mtime));
	_strncpy(file->time, t + 4, 12);
	/* format_time(file->time, sizeof(file->time), &filestat.st_ctime); */
	file->size = filestat.st_size;

	user = getpwuid(filestat.st_uid)->pw_name;
	if (user != NULL)
	{
		buf_size = (_strlen(user) * sizeof(char)) + sizeof(char);
		file->user = malloc(buf_size);
		_strncpy(file->user, user, buf_size);
	}

	group = getgrgid(filestat.st_gid)->gr_name;
	if (group != NULL)
	{
		buf_size = (_strlen(group) * sizeof(char)) + sizeof(char);
		file->group = malloc(buf_size);
		_strncpy(file->group, group, buf_size);
	}

}


/**
 * cleanup - frees an dynamically allocated array of File instances.
 * @arr: pointer to the array.
 *
 * Return: nothing.
 */
void
cleanup_long_list(File **arr)
{
	int i = 0;

	while (arr[i] != NULL)
	{
		free(arr[i]->user);
		free(arr[i]->group);
		free(arr[i++]);
	}

	free(arr);
}

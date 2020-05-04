#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include "def.h"


/**
 * get_long_list - populates an array of File instances using a
 * directory stream.
 *
 * @path: path to the directory.
 * @hidden: list hidden files and directories.
 *
 * Return: pointer to the array. If memory allocation fails for any of the File
 * struct members, the function will return a null pointer.
 */
File
**get_long_list(char *path, int hidden)
{
	DIR *dir;
	struct dirent *read;
	struct stat filestat;
	ssize_t size = file_count(path);
	File **files;
	unsigned int i = 0;
	char *c;

	files = create_long_list(size + 1);
	dir = open_directory(path);

	while ((read = readdir(dir)) != NULL)
	{
		if (!hidden)
		{
			if (read->d_name[0] == '.')
				continue;
		}
		_strncpy(files[i]->name, read->d_name, sizeof(files[i]->name) - 1);
		_strncpy(files[i]->path, path, sizeof(files[i]->path) - 1);
		_strncat(
			files[i]->path,
			files[i]->name,
			sizeof(files[i]->path) - _strlen(files[i]->path) - 1
		);

		if ((lstat(files[i]->path, &filestat)) == -1)
		{
			exit(2);
		}

		switch (filestat.st_mode & S_IFMT)
		{
			case S_IFREG:
				files[i]->type = '-';
				break;
			case S_IFDIR:
				files[i]->type = 'd';
				break;
			case S_IFCHR:
				files[i]->type = 'c';
				break;
			case S_IFBLK:
				files[i]->type = 'b';
				break;
			case S_IFIFO:
				files[i]->type = 'p';
				break;
			case S_IFLNK:
				files[i]->type = 'l';
				break;
			case S_IFSOCK:
				files[i]->type = 's';
				break;
		}

		files[i]->perm[0] = filestat.st_mode & S_IRUSR ? 'r' : '-';
		files[i]->perm[1] = filestat.st_mode & S_IWUSR ? 'w' : '-';
		files[i]->perm[2] = filestat.st_mode & S_IXUSR ? 'x' : '-';
		files[i]->perm[3] = filestat.st_mode & S_IRGRP ? 'r' : '-';
		files[i]->perm[4] = filestat.st_mode & S_IWGRP ? 'w' : '-';
		files[i]->perm[5] = filestat.st_mode & S_IXGRP ? 'x' : '-';
		files[i]->perm[6] = filestat.st_mode & S_IROTH ? 'r' : '-';
		files[i]->perm[7] = filestat.st_mode & S_IROTH ? 'w' : '-';
		files[i]->perm[8] = filestat.st_mode & S_IROTH ? 'x' : '-';

		c = getpwuid(filestat.st_uid)->pw_name;
		if (_strlen(c))
		{
			files[i]->user = malloc(_strlen(c) * sizeof(char));
			files[i]->user = _strncpy(files[i]->user, c, _strlen(c));
		}

		c = getgrgid(filestat.st_uid)->gr_name;
		if (_strlen(c))
		{
			files[i]->group = malloc(_strlen(c) * sizeof(char));
			files[i]->group = _strncpy(files[i]->group, c, _strlen(c));
		}

		i++;
	}

	free(dir);
	return (files);
}

void
print_files_long_format(char **arr)
{
	int i, j;
	size_t size;
	File **files;
	LongListFormatting *formatting;

	for (i = 0, size = 0; arr[i] != NULL; i++)
		if (is_file(arr[i]))
			size++;

	files = create_long_list(size);

	for (i = 0, j = 0; arr[i] != NULL; i++)
		if (is_file(arr[i]))
			set_file_details(files[j++], arr[i]);

	formatting = get_formatting(files);

	for (j = 0; files[j] != NULL; j++)
	{
		print_file_long_format(files[j], formatting);
		if (files[j + 1] != NULL)
			printf("\n");
	}

	free(formatting);
	cleanup(files);
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
}


void
set_file_details(File *file, char *path)
{
	struct stat filestat;
	int buf_size;
	char *user;
	char *group;

	if (path == NULL || file == NULL)
		return;

	_strncpy(file->name, path, sizeof(file->name) - 1);
	if ((lstat(file->name, &filestat)) == -1)
		exit(2);

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
	file->perm[7] = filestat.st_mode & S_IROTH ? 'w' : '-';
	file->perm[8] = filestat.st_mode & S_IROTH ? 'x' : '-';

	file->nlink = filestat.st_nlink;
	format_time(file->time, sizeof(file->time), &filestat.st_ctime);
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
cleanup(File **arr)
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

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include "def.h"


/**
 * open_directory - wrapper function, opens a directory stream using
 * opendir(), copies the path to a new allocated memory adress and
 * handles the missing trailing slash condition.
 * @path: path to the directory (memory address).
 *
 * Return: directory stream pointing to the directory, if memory allocation
 * fails, the function will return a null pointer, if opendir() call fails,
 * the function will exit with status 1.
 */
DIR
*open_directory(char **path)
{
	int trailing_slash = 0;
	int path_len = 0;
	char *tmp;
	DIR *dir;

	if (path == NULL || *path == NULL)
		return (NULL);

	path_len = strlen(*path);

	if (path[0][path_len - 1] != '/')
		trailing_slash = 1;
	tmp = malloc(path_len + trailing_slash + 1);
	if (tmp == NULL)
	{
		free(tmp);
		return (NULL);
	}

	_strncpy(tmp, *path, path_len + trailing_slash + 1);
	if (trailing_slash)
		tmp[path_len] = '/';
	*path = tmp;

	dir = opendir(*path);
	if (dir == NULL)
	{
		fprintf(stderr,
			"hls: cannot access '%s': No such file or directory\n",
			*path);
		free(*path);
		exit(1);
	}

	return (dir);
}

/**
 * create_list - creates a dynamically allocated array of pointers to File
 * instances. Contains a null pointer at its end.
 * @size: size of the list, has to include the size for the null pointer.
 *
 * Return: pointer to the newly created array. If memory allocation fails
 * the function will return a null pointer.
 */
File
**create_list(size_t size)
{
	int i;
	File **files;

	if (size == 0)
		return (NULL);

	files = malloc(size * sizeof(File *));
	if (files == NULL)
	{
		free(files);
		return (NULL);
	}

	for (i = 0; (size_t) i < size - 1; i++)
	{
		files[i] = malloc(sizeof(File));
		if (files[i] == NULL)
		{
			while (i >= 0)
				free(files[i--]);
			free(files);
			return (NULL);
		}
	}

	files[size - 1] = NULL;

	return (files);
}

/**
 * get_list - populates an array of File instances using a directory stream.
 * @path: path to the directory.
 *
 * Return: pointer to the array. If memory allocation fails for any of the File
 * struct members, the function will return a null pointer.
 */
File
**get_list(char *path)
{
	DIR *dir;
	struct dirent *read;
	struct stat filestat;
	ssize_t size = file_count(path);
	File **files;
	unsigned int i = 0;
	char *c;

	files = create_list(size + 1);
	dir = open_directory(&path);

	while ((read = readdir(dir)) != NULL)
	{
		_strncpy(files[i]->name, read->d_name, sizeof(files[i]->name) - 1);
		_strncpy(files[i]->path, path, sizeof(files[i]->path) - 1);
		_strncat(files[i]->path, files[i]->name, sizeof(files[i]->path) - _strlen(files[i]->path) - 1);

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

		files[i]->time = ctime(&(filestat.st_mtime));
		files[i]->time[strlen(files[i]->time) - 9] = '\0';
		files[i]->size = filestat.st_size;

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
	free(path);
	return (files);
}

/**
 * file_count - counts the files and directories inside a directory.
 * @path: path to the directory.
 *
 * Return: file/directory count, if the open_directory() call fails,
 * the function will return -1.
 */
int
file_count(char *path)
{
	DIR *dir;
	struct dirent *read;
	int count = 0;

	dir = open_directory(&path);
	if (dir == NULL)
		return (-1);

	while ((read = readdir(dir)) != NULL)
		count++;

	closedir(dir);

	free(path);
	return (count);
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


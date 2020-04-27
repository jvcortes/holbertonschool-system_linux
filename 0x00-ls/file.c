#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "def.h"


/**
 * get_file - gets the name of a file
 * @path: path to the file.
 *
 * Return: string duplicate of the file, if no file was found, the function
 * will return a null pointer.
 */
char
*get_file(char *path)
{
	struct stat filestat;

	if ((lstat(path, &filestat)) == -1)
		return (NULL);

	if ((filestat.st_mode & S_IFMT) == S_IFDIR)
		return (NULL);

	return (_strdup(path));
}

/**
 * print_file - prints a file name
 * @file: file name
 *
 * Return: nothing.
 */
void
print_file(char *file)
{
	printf("%s", file);
}

/**
 * is_file - determines if a given path corresponds to a file.
 * @path: path to the resource.
 *
 * Return: non-zero value if the given path corresponds to a file. Otherwise
 * the function will return zero.
 */
int
is_file(char *path)
{
	struct stat filestat;

	if (path == NULL)
		return (0);

	if ((lstat(path, &filestat)) == -1)
		return (0);

	if ((filestat.st_mode & S_IFMT) == S_IFDIR)
		return (0);

	return (1);
}

/**
 * path_exists - checks if a path exists in the filesystem
 * @path: path.
 *
 * Return: non-zero value, if the given path doesn't exist, the function will
 * return zero and will set this program's return code to two.
 */
int
path_exists(char *path)
{
	int errno;
	char *errmsg;
	struct stat filestat;

	if (path == NULL)
		return (0);

	if ((lstat(path, &filestat)) == -1)
	{
		if (errno == ENOENT)
		{
			errmsg = malloc(_strlen("hls: cannot access ") + _strlen(path) + 1);
			sprintf(errmsg,
				"hls: cannot access %s",
				path
			);
			perror(errmsg);
			free(errmsg);
		}
		status(2);
		return (0);
	}

	return (1);
}

/**
 * can_read_dir - determines if the current user can read a directory.
 * @path: path to the directory.
 *
 * Return: non-zero value, if the given directory path cannot be read, the
 * function will return zero and will set this program's exit status to two.
 */
int
can_read_dir(char *path)
{
	int errno;
	char *errmsg;
	DIR *dir;

	if (path == NULL)
		return (0);

	dir = opendir(path);
	if (dir == NULL)
	{
		if (errno == EACCES)
		{
			errmsg = malloc(_strlen("hls: cannot open directory ") + _strlen(path) + 1);
			sprintf(errmsg,
				"hls: cannot open directory %s",
				path
			);
			perror(errmsg);
			free(errmsg);
		}
		status(2);
		return (0);
	}
	free(dir);
	return (1);
}

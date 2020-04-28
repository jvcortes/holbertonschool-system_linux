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
*open_directory(char *path)
{
	DIR *dir;

	if (path == NULL)
		return (NULL);

	dir = opendir(path);
	if (dir == NULL)
	{
		free(dir);
		exit(1);
	}

	return (dir);
}

/**
 * file_count - counts the files and directories inside a directory.
 * @path: path to the directory.
 * @hidden: count hidden files.
 *
 * Return: file/directory count, if the open_directory() call fails,
 * the function will return -1.
 */
int
file_count(char *path, int hidden)
{
	DIR *dir;
	struct dirent *read;
	int count = 0;

	dir = open_directory(path);
	if (dir == NULL)
		return (-1);

	while ((read = readdir(dir)) != NULL)
		if (!hidden)
		{
			if (read->d_name[0] != '.')
				count++;
		}
		else
		{
			count++;
		}

	closedir(dir);

	return (count);
}

/**
 * print_directory - prints the contents of a directory
 * @path: path to the directory
 * @hidden: check for hidden files and directories.
 *
 * Return: nothing.
 */
void
print_directory(char *path, int hidden)
{
	char **arr;

	arr = get_list(path, hidden);
	print_list(arr);
	cleanup_list(arr);
}

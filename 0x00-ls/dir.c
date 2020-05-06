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
	int visibility = set_opt("visibility", RETRIEVE_OPT);

	dir = open_directory(path);
	if (dir == NULL)
		return (-1);

	while ((read = readdir(dir)) != NULL)
		switch (visibility)
		{
		case LIST_VISIBLE:
			if (read->d_name[0] != '.')
				count++;
			break;
		case LIST_ALMOST_ALL:
			if (_strcmp(read->d_name, "..") ||
					_strcmp(read->d_name, "."))
				count++;
			break;
		case LIST_HIDDEN:
			count++;
			break;
		}

	closedir(dir);

	return (count);
}

/**
 * print_directory - prints the contents of a directory
 * @path: path to the directory
 *
 * Return: nothing.
 */
void
print_directory(char *path)
{
	char **arr;

	arr = get_list(path);
	quicksort_str(arr, str_array_size(arr));

	print_list(arr, path);
	cleanup(arr);
}

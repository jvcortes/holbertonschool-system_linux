#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
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

	if ((lstat(path, &filestat)) == -1)
		exit(1);

	if ((filestat.st_mode & S_IFMT) == S_IFDIR)
		return (0);

	return (1);
}

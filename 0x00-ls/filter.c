#include <stdlib.h>
#include "def.h"


/**
 * get_files_from_args - gets the entries in the arguments that correspond to a
 * non-directory.
 *
 * @args: arguments
 *
 * Return: array containing the non-directory entries. If memory allocation fails
 * or no entries were found, the function will return NULL.
 */
char **get_files_from_args(char **args)
{
	int i, j;
	char **files;

	if (args == NULL)
		return (NULL);

	for (i = 0, j = 0; args[i] != NULL; i++)
		if (is_file(args[i]))
			j++;

	if (j)
	{
		files = malloc(j * sizeof(char *) + sizeof(char *));
		if (files == NULL)
		{
			free(files);
			return (NULL);
		}
	}
	else
	{
		return (NULL);
	}

	for (i = 0, j = 0; args[i] != NULL; i++)
		if (is_file(args[i]))
			files[j++] = args[i];
	files[j] = NULL;

	return (files);
}


/**
 * get_directories_from_args - gets the entries in the arguments that correspond
 * to a directory.
 *
 * @args: arguments
 *
 * Return: array containing the directory entries. If memory allocation fails
 * or no entries were found, the function will return NULL.
 */
char **get_directories_from_args(char **args)
{
	int i, j;
	char **files;

	if (args == NULL)
		return (NULL);

	for (i = 0, j = 0; args[i] != NULL; i++)
		if (is_dir(args[i]))
			j++;

	if (j)
	{
		files = malloc(j * sizeof(char *) + sizeof(char *));
		if (files == NULL)
		{
			free(files);
			return (NULL);
		}
	}
	else
	{
		return (NULL);
	}

	for (i = 0, j = 0; args[i] != NULL; i++)
		if (is_dir(args[i]))
			files[j++] = args[i];
	files[j] = NULL;

	return (files);
}

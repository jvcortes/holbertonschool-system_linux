#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "def.h"

#define SIZE 256

/**
 * get_list - creates and fills an array containing the file
 * names inside a directory.
 * @path: path to the directory.
 * @hidden: retrieve hidden files.
 *
 * Return: pointer to the created list, if memory allocation fails, the
 * function will return a null pointer.
 */
char
**get_list(char *path)
{
	unsigned int i = 0;
	int visibility = set_opt("visibility", RETRIEVE_OPT);
	char **files;
	ssize_t size = file_count(path);
	DIR *dir;
	struct dirent *read;

	dir = open_directory(path);
	files = malloc(size * sizeof(char *) + sizeof(char *));
	if (files == NULL)
	{
		free(files);
		return (NULL);
	}

	while ((read = readdir(dir)) != NULL)
	{
		switch (visibility) {
			case LIST_VISIBLE:
				if (read->d_name[0] == '.')
					continue;
				break;
			case LIST_HIDDEN:
				break;
		}

		files[i] = malloc(sizeof(char) * SIZE);
		if (files[i] == NULL)
		{
			while (i > 0)
				free(files[i--]);
			free(files);
			return (NULL);
		}
		_strncpy(files[i], read->d_name, SIZE - 1);
		i++;
	}
	files[i] = NULL;

	free(dir);
	return (files);
}

/**
 * create_array - creates a null terminated array of char pointers.
 * @size: size of the array
 *
 * Return: pointer to the array, if memory allocation falis, the function will
 * return a null pointer.
 */
char
**create_array(ssize_t size)
{
	char **list = malloc(size * sizeof(char *) + sizeof(char *));

	if (list == NULL)
	{
		free(list);
		return (NULL);
	}

	list[size] = NULL;

	return (list);
}

/**
 * print_list - prints a array of file names.
 * @list: pointer to the array.
 *
 * Return: nothing.
 */
void
print_list(char **list)
{
	int i = 0;

	switch (set_opt("listing", -1))
	{
		case DEFAULT_LISTING:
			while (list[i])
			{
				printf("%s", list[i]);
				if (list[i + 1] == NULL)
					printf("\n");
				else
					printf("  ");
				i++;
			}
			break;
		case VERTICAL_LISTING:
			while (list[i])
			{
				printf("%s", list[i]);
				if (list[i] != NULL)
					printf("\n");
				i++;
			}
			break;
	}
}

/**
 * cleanup_list - frees a array of file names.
 * @arr: pointer to the array.
 *
 * Return: nothing.
 */
void
cleanup_list(char **arr)
{
	int i = 0;

	while (arr[i] != NULL)
		free(arr[i++]);

	free(arr);
}

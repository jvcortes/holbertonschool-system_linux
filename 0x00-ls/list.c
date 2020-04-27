#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "def.h"

#define SIZE 256

/**
 * get_shortlist - creates and fills an array containing the file
 * names inside a directory.
 * @path: path to the directory.
 * @hidden: retrieve hidden files.
 *
 * Return: pointer to the created list, if memory allocation fails, the
 * function will return a null pointer.
 */
char
**get_shortlist(char *path, int hidden)
{
	DIR *dir;
	struct dirent *read;
	char **files;
	ssize_t size = file_count(path, hidden);
	unsigned int i = 0;

	dir = open_directory(path);
	files = malloc(size * sizeof(char *) + sizeof(char *));
	if (files == NULL)
	{
		free(files);
		return (NULL);
	}

	while ((read = readdir(dir)) != NULL)
	{
		if (!hidden)
			if (read->d_name[0] == '.')
				continue;

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
**create_list(ssize_t size)
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
 * print_shortlist - prints a array of file names.
 * @list: pointer to the array.
 *
 * Return: nothing.
 */
void
print_shortlist(char **list)
{
	int i = 0;

	while (list[i])
	{
		printf("%s", list[i]);
		if (list[i + 1] == NULL)
			printf("\n");
		else
			printf("  ");
		i++;
	}
}

/**
 * print_shortlist - frees a array of file names.
 * @arr: pointer to the array.
 *
 * Return: nothing.
 */
void
cleanup_shortlist(char **arr)
{
	int i = 0;

	while (arr[i] != NULL)
		free(arr[i++]);

	free(arr);
}

#include <stdlib.h>
#include <stdio.h>
#include "def.h"

/**
 * filter_null - removes null pointers from a string array.
 * @arr: pointer to the array.
 * @size: size of the array.
 *
 * Return: pointer to the null terminated resulting array,
 * if memory allocation fails, the function will return a null pointer.
 */
char
**filter_null(char **arr, size_t size)
{
	ssize_t i = 0, j = 0, new_size = 0;
	char **new;

	if (arr == NULL || size == 0)
		return (NULL);

	while (i < (int) size)
	{
		if (arr[i++] != NULL)
			new_size++;
	}
	i = 0;

	if (new_size == 0)
		return (NULL);

	new = malloc(new_size * sizeof(char *) + sizeof(char *));
	if (new == NULL)
	{
		free(new);
		return (NULL);
	}
	while (i < (int) size)
	{
		if (arr[i] != NULL)
			new[j++] = arr[i];
		i++;
	}
	new[j] = NULL;

	return (new);
}

/**
 * copy - copies an array of strings.
 * @arr: pointer to the array.
 * @size: size of the array.
 *
 * Return: pointer to the copy of the array, if memory allocation fails, the
 * function will return a null pointer.
 */
char
**copy(char **arr, size_t size)
{
	ssize_t i = 0;
	char **new;

	if (arr == NULL || size == 0)
		return (NULL);

	new = malloc(size * sizeof(char *) + sizeof(char *));
	if (new == NULL)
	{
		free(new);
		return (NULL);
	}

	for (i = 0; i < (int) size; i++)
		new[i] = _strdup(arr[i]);

	new[i] = NULL;

	return (new);
}

/**
 * str_array_size - returns the size a null terminated array of strings.
 * @arr: pointer to the array.
 *
 * Return: size of the array.
 */
size_t
str_array_size(char **arr)
{
	int i = 0;

	if (arr == NULL)
		return (-1);

	while (arr[i] != NULL)
		++i;

	return (i);
}


/**
 * create_long_list - creates a dynamically allocated array of pointers to File
 * instances. Contains a null pointer at its end.
 * @size: size of the list, has to include the size for the null pointer.
 *
 * Return: pointer to the newly created array. If memory allocation fails
 * the function will return a null pointer.
 */
File
**create_long_list(size_t size)
{
	int i;
	File **files;

	if (size == 0)
		return (NULL);

	files = malloc(size * sizeof(File *) + sizeof(File *));
	if (files == NULL)
	{
		free(files);
		return (NULL);
	}

	for (i = 0; i < (int) size; i++)
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

	files[i] = NULL;

	return (files);
}

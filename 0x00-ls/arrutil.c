#include <stdlib.h>
#include <stdio.h>
#include "def.h"


char
**filter_null(char **arr, ssize_t len)
{
	ssize_t i = 0, j = 0, new_size = 0;
	char **new;

	if (arr == NULL || len == 0)
		return (NULL);

	while (i < len)
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
	while (i < len)
	{
		if (arr[i] != NULL)
			new[j++] = arr[i];
		i++;
	}
	new[j] = NULL;

	return (new);
}

ssize_t
str_array_length(char **arr)
{
	int i = 0;

	while (arr[i] != NULL)
		++i;

	return (i);
}

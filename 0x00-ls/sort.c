#include <stdio.h>
#include <string.h>
#include "def.h"


/**
 * quicksort_str - Sorts an array of strings using the quicksort method.
 * @arr: pointer to the array.
 * @size: size of the array.
 *
 * Return: nothing.
 */
void quicksort_str(char **arr, size_t size)
{
	int pivot;

	if (!arr || size <= 1)
		return;

	pivot = lomuto_partition(arr, size);
	quicksort_str(arr, pivot);
	quicksort_str(arr + (pivot), size - (pivot));
}


/**
 * lomuto_partition - partitionates an array of strings, ignores the '/'
 * character.
 *
 * @arr: pointer to the array.
 * @size: size of the array.
 *
 * Return: position of the resulting pivot.
 */
int lomuto_partition(char **arr, size_t size)
{
	int i, j;
	char *swap, *stripped_j, *stripped_pivot;
	static char **initial;

	if (!initial)
		initial = arr;

	for (i = -1, j = 0; j <= (int) size - 1;)
	{
		stripped_j = strstrp(arr[j], "/");
		stripped_pivot = strstrp(arr[size - 1], "/");

		if (_strcmp(stripped_j, stripped_pivot) <= 0)
		{
			i++;
			if (i != j)
			{
				swap = arr[j];
				arr[j] = arr[i];
				arr[i] = swap;
			}
		}
		free(stripped_j);
		free(stripped_pivot);
		j++;
	}

	return (i);
}

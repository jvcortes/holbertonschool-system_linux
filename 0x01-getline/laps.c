#include <stdio.h>
#include <stdlib.h>
#include "laps.h"

int search_or_add(Car **cars, int size, int *ids, int ids_size);
void add(Car **cars, int size, int *ids, int ids_size);
void quick_sort(Car *arr, size_t size);
int lomuto_partition(Car *arr, size_t size);

/**
 * race_state - prints the race state.
 *
 * race_state will receive an array of ids, each id will be assigned to a car.
 * Every time this function is called with an existing id, the amount of laps
 * for that car will be increased by 1.
 *
 * If the function is called with size == 0. All the cars will be deleted.
 *
 * @id: array of ids.
 * @size: size of the array.
 *
 * Return: nothing
 */
void race_state(int *id, size_t size)
{
	static Car *cars;
	static int sz;
	int i;

	if (id == NULL && size > 0)
		return;

	if (cars == NULL && size > 0)
	{
		cars = malloc(size * sizeof(Car));
		if (cars == NULL)
		{
			free(cars);
			return;
		}
		for (i = 0; i < (int) size; i++)
		{
			cars[i].id = id[i];
			cars[i].laps = 0;
			printf("Car %d joined the race\n", cars[i].id);
		}
		sz = size;
	}
	else if (cars != NULL && size > 0)
	{
		sz = search_or_add(&cars, sz, id, size);
	}
	else if (size == 0)
	{
		free(cars);
		return;
	}
	quick_sort(cars, sz);
	printf("Race state:\n");
	for (i = 0; i < (int) sz; i++)
		printf("Car %d [%d laps]\n", cars[i].id, cars[i].laps);
}


/**
 * search_or_add - looks for one or many Car structs.
 *
 * search_or_add will look for the Car structs in parameter cars if they are
 * associated with one of the ids in ids. If no Car is found, the respective
 * Car will be created with 0 laps.
 *
 * @cars: array of cars.
 * @size: size of the cars array.
 * @ids: array of ids.
 * @ids_size: size of the array of ids.
 *
 * Return: size of the resulting array. If memory allocation fails, the
 * function will return -1.
 */
int search_or_add(Car **cars, int size, int *ids, int ids_size)
{
	int i, j, k, *not_found;

	not_found = malloc(ids_size * sizeof(int));
	if (not_found == NULL)
	{
		free(not_found);
		return (-1);
	}

	for (i = 0, k = 0; i < ids_size; i++)
	{
		for (j = 0; j < size; j++)
		{
			if ((*cars)[j].id == ids[i])
			{
				(*cars)[j].laps++;
				break;
			}
		}
		if (j == size)
			not_found[k++] = ids[i];
	}

	if (k > 0)
		add(cars, size, not_found, k);

	free(not_found);
	return (size + k);
}

/**
 * add - adds one of many Cars in an array of Car structs.
 *
 * For every id in ids, a Car struct will be created with that
 * id and will be put inside the cars array.
 *
 * @cars: array of cars.
 * @size: size of the cars array.
 * @ids: array of ids.
 * @ids_size: size of the array of ids.
 *
 * Return: nothing.
 */
void add(Car **cars, int size, int *ids, int ids_size)
{
	Car *oc = *cars;
	int i, j;

	*cars = malloc((size + ids_size) * sizeof(Car));
	if (*cars == NULL)
	{
		free(cars);
		return;
	}
	for (i = 0; i < size; i++)
		(*cars)[i] = oc[i];
	for (j = 0; i < (size + ids_size); i++, j++)
	{
		(*cars)[i].id = ids[j];
		(*cars)[i].laps = 0;
		printf("Car %d joined the race\n", (*cars)[i].id);
	}
	free(oc);
}


/**
 * quick_sort - sorts an array of Car structs
 * using the Quicksort algorithm
 *
 * @arr: pointer of the list
 * @size: size of the list
 *
 * Return: nothing
 */
void quick_sort(Car *arr, size_t size)
{
	int pivot;

	if (!arr || size <= 1)
		return;

	pivot = lomuto_partition(arr, size);
	quick_sort(arr, pivot);
	quick_sort(arr + (pivot), size - (pivot));
}

/**
 * lomuto_partition - partitionates an array using the Lomuto scheme
 * @arr: pointero to the array
 * @size: size of the array
 *
 * Return: pivot of the partition.
 */
int lomuto_partition(Car *arr, size_t size)
{
	Car swap;
	int i, j;

	for (i = -1, j = 0; j <= (int)size - 1;)
	{
		if (arr[j].id <= arr[size - 1].id)
		{
			i++;
			if (i != j)
			{
				swap = arr[j];
				arr[j] = arr[i];
				arr[i] = swap;
			}
		}
		j++;
	}

	return (i);
}

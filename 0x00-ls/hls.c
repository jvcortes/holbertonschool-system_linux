#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include "def.h"


/**
 * print_files - prints the associated files with every path in a provided
 * array. Paths containing folders will not be taken into account.
 * @arr: pointer to the array.
 * @size: size of the array.
 * @count: number of files to print from the array.
 *
 * Return: nothing.
 */
void
print_files(char **arr, size_t size, size_t count)
{
	int i, j;

	if (!count)
		return;

	switch (set_opt("listing", -1))
	{
		case DEFAULT_LISTING:
			for (i = 0, j = 0; i < (int) size; i++)
			{
				if (is_file(arr[i]))
				{
					print_file(arr[i]);
					if (++j < (int) count)
						printf("  ");
				}
			}
			break;
		case VERTICAL_LISTING:
			for (i = 0, j = 0; i < (int) size; i++)
			{
				if (is_file(arr[i]))
				{
					print_file(arr[i]);
					if (++j < (int) count)
						printf("\n");
				}
			}
			break;
		case LONG_LISTING:
			print_files_long_format(arr);
			break;
	}
	printf("\n");

}

/**
 * print_directories - prints the associated directories with every path in a
 * provided array. Paths containing single files will not be taken into
 * account.
 * @arr: pointer to the array.
 * @size: size of the array.
 * @count: number of valid entries inside the array
 * @dir_count: number of directories to print from the array.
 *
 * Return: nothing.
 */
void
print_directories(char **arr, size_t size, size_t count, size_t dir_count)
{
	int i, j;
	char **list = NULL;

	for (i = 0, j = 0; i < (int) size; i++)
	{
		if (!is_file(arr[i]) && can_read_dir(arr[i]))
		{
			list = get_list(arr[i]);
			if (list)
			{
				quicksort_str(list, str_array_size(list));
				if (j > 0 && j < (int) dir_count)
					printf("\n");
				if (count > 1)
					printf("%s:\n", arr[i]);
				print_list(list);
				cleanup_list(list);
				j++;
			}
		}
	}
}

/**
 * print_many - prints more than one entry if its present in the filesystem.
 * @arr: pointer to the array.
 * @size: size of the array.
 *
 * Return: nothing.
 */
void
print_many(char **arr, size_t size)
{
	int i = 0, count = 0, file_count = 0, dir_count = 0;

	if (size)
		quicksort_str(arr, size);

	while (i < (int) size)
	{
		if (arr[i])
			count++;
		if (path_exists(arr[i]))
		{
			if (is_file(arr[i]))
			{
				file_count++;
			}
			else
			{
				dir_count++;
			}
		}
		i++;
	}

	if (file_count)
		print_files(arr, size, file_count);

	if (dir_count)
	{
		if (count && file_count)
			printf("\n");
		print_directories(arr, size, count, dir_count);
	}

	if (!file_count && !dir_count && !status(RETRIEVE_STATUS))
		print_directory("./");
}

/**
 * main - entry point.
 * @argc: argument count.
 * @argv: argument list.
 *
 * Return: zero.
 */
int main(int argc, char *argv[])
{
	char **arr;
	
	setlocale(LC_ALL, "");
	check_opts(argv, argc);
	arr = filter_null(argv + 1, argc - 1);
	print_many(arr, str_array_size(arr));
	free(arr);
	return (status(RETRIEVE_STATUS));
}

#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include "def.h"


/**
 * print_files - prints the associated non-folders with every path in a
 * provided array.
 * @arr: pointer to the array.
 *
 * Return: nothing.
 */
void
print_files(char **arr)
{
	int i;

	if (arr == NULL)
		return;

	switch (set_opt("listing", -1))
	{
		case DEFAULT_LISTING:
			for (i = 0; arr[i] != NULL; i++)
			{
				print_file(arr[i]);
				if (arr[i + 1] != NULL)
					printf("  ");
			}
			break;
		case VERTICAL_LISTING:
			for (i = 0; arr[i] != NULL; i++)
			{
				print_file(arr[i]);
				if (arr[i + 1] != NULL)
					printf("\n");
			}
			break;
		case LONG_LISTING:
			print_files_long_format(arr, "");
			break;
	}
	printf("\n");
}

/**
 * print_directories - prints the associated directories with every path in a
 * provided array. Paths containing single files will not be taken into
 * account.
 *
 * @arr: pointer to the array.
 * @many: print more than one directory, separated by a newline character.
 *
 * Return: nothing.
 */
void
print_directories(char **arr, int many)
{
	int i;
	char **file_list = NULL;

	for (i = 0; arr[i] != NULL; i++)
	{
		if (can_read_dir(arr[i]))
		{
			file_list = get_list(arr[i]);
			if (file_list)
			{
				quicksort_str(
					file_list,
					str_array_size(file_list)
				);
				if (i != 0)
					printf("\n");
				if (many != 0)
					printf("%s:\n", arr[i]);
				print_list(file_list, arr[i]);
				cleanup(file_list);
			}
		}
	}
}


/**
 * print_many - prints more than one entry if its present in the filesystem.
 * @arr: pointer to the array.
 *
 * Return: nothing.
 */
void
print(char **arr)
{
	int size = str_array_size(arr);
	char **files, **directories;

	if (arr == NULL)
		print_directory("./");

	quicksort_str(arr, str_array_size(arr));
	files = get_files_from_args(arr);
	directories = get_directories_from_args(arr);

	if (files != NULL)
	{
		print_files(files);
		free(files);
		if (directories != NULL)
			printf("\n");
	}
	if (directories != NULL)
	{
		if (size > 1)
		{
			print_directories(directories, 1);
		}
		else
		{
			print_directories(directories, 0);
		}
		free(directories);
	}
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

	check_opts(argv, argc);
	arr = filter_null(argv + 1, argc - 1);
	print(arr);
	free(arr);
	return (status(RETRIEVE_STATUS));
}

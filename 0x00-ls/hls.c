#include <stdio.h>
#include <stdlib.h>
#include "def.h"


/**
 * print_single - prints a single entry if it is present in the filesystem
 * @path: entry path.
 *
 * Return: nothing.
 */
void
print_single(char *path)
{
	char *file;
	char **short_filelist;

	file = get_file(path);
	if (file)
	{
		print_file(file);
		printf("\n");
		free(file);
	}
	else
	{
		short_filelist = get_shortlist(path, 0);
		print_shortlist(short_filelist);
		cleanup_shortlist(short_filelist);
	}
}

/**
 * print_files - prints the associated files with every path in a provided
 * array. Paths containing folders will not be taken into account.
 * @arr: pointer to the array.
 * @size: size of the array.
 *
 * Return: nothing.
 */
void
print_files(char **arr, size_t size)
{
	int i, j, file_count = 0;

	for (i = 0; i < (int) size; i++)
		if (is_file(arr[i]))
			file_count++;

	if (file_count)
	{
		for (i = 0, j = 0; i < (int) size; i++)
		{
			if (is_file(arr[i]))
			{
				print_file(arr[i]);
				if (++j < file_count)
					printf("  ");
			}
		}
		printf("\n\n");
	}
}

/**
 * print_directories - prints the associated directories with every path in a
 * provided array. Paths containing single files will not be taken into
 * account.
 * @arr: pointer to the array.
 * @size: size of the array.
 *
 * Return: nothing.
 */
void
print_directories(char **arr, size_t size)
{
	int i, j, dir_count = 0;
	char **list = NULL;

	for (i = 0; i < (int) size; i++)
		if (!is_file(arr[i]))
			dir_count++;

	for (i = 0, j = 0; i < (int) size; i++)
	{
		if (!is_file(arr[i]))
		{
			list = get_shortlist(arr[i], 0);
			if (list)
			{
				printf("%s:\n", arr[i]);
				print_shortlist(list);
				cleanup_shortlist(list);
				if (++j < dir_count)
					printf("\n");
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
	print_files(arr, size);
	print_directories(arr, size);
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
	if (argc == 1)
		print_single("./");
	else if (argc == 2)
		print_single(argv[1]);
	else
		print_many((argv + 1), argc - 1);
	return (0);
}

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

	if (path_exists(path))
	{
		file = get_file(path);
		if (file)
		{
			print_file(file);
			printf("\n");
			free(file);
		}
		else if (can_read_dir(path))
		{
			short_filelist = get_shortlist(path, 0);
			print_shortlist(short_filelist);
			cleanup_shortlist(short_filelist);
		}
	}
}

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

	if (count)
	{
		for (i = 0, j = 0; i < (int) size; i++)
		{
			if (is_file(arr[i]))
			{
				print_file(arr[i]);
				if (++j < (int) count)
					printf("  ");
			}
		}
		printf("\n");
	}
}

/**
 * print_directories - prints the associated directories with every path in a
 * provided array. Paths containing single files will not be taken into
 * account.
 * @arr: pointer to the array.
 * @size: size of the array.
 * @count: number of directories to print from the array.
 *
 * Return: nothing.
 */
void
print_directories(char **arr, size_t size, size_t count)
{
	int i, j;
	char **list = NULL;

	for (i = 0, j = 0; i < (int) size; i++)
	{
		if (!is_file(arr[i]) && can_read_dir(arr[i]))
		{
			list = get_shortlist(arr[i], 0);
			if (list)
			{
				if (j > 0 && j < (int) count)
					printf("\n");
				printf("%s:\n", arr[i]);
				print_shortlist(list);
				cleanup_shortlist(list);
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
	int i = 0, file_count = 0, dir_count = 0;

	while (i < (int) size)
	{
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
		if (file_count)
			printf("\n");
		print_directories(arr, size, dir_count);
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
	if (argc == 1)
		print_single("./");
	else if (argc == 2)
		print_single(argv[1]);
	else
		print_many((argv + 1), argc - 1);
	return (status(-1));
}

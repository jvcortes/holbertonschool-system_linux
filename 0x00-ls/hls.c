#include <stdio.h>
#include <stdlib.h>
#include "def.h"


/**
 * main - entry point.
 * @argc: argument count.
 * @argv: argument list.
 *
 * Return: zero.
 */
int main(int argc, char *argv[])
{
	File **files;
	int i = 0;

	if (argc == 1)
	{
		files = get_list("./");
	} 
	else
	{
		files = get_list(argv[1]);
	}
	while (files[i] != NULL)
	{
		printf("%s", files[i]->name);
		if (files[i + 1])
			printf("  ");
		else
			printf("\n");
		i++;
	}
	cleanup(files);

	return (0);
}

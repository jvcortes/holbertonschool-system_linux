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
	char **short_filelist;

	if (argc == 1)
	{
		short_filelist = get_shortlist("./", 0);
		print_shortlist(short_filelist);
		cleanup_shortlist(short_filelist);
	}
	else
	{
		short_filelist = get_shortlist(argv[1], 0);
		print_shortlist(short_filelist);
		cleanup_shortlist(short_filelist);
	}

	return (0);
}

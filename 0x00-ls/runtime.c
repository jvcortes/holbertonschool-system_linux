#include <stdio.h>
#include "def.h"


/**
 * status - wrapper function for the return code, since the use of extern
 * variables is not allowed by `betty`.
 * @num: number used to set the return code. If set to -1, the function will
 * only return the current code.
 *
 * Return: current return code.
 */
int status(int num)
{
	static int status;

	if (num != -1)
		status = num;

	return (status);
}

/**
 * set_opt - sets or retrieves a global setting through a static struct, since
 * global variables are not allowed by `betty`.
 * @opt: option name.
 * @num: option value.
 *
 * Return: zero, if `num` its set to -1, the function will return the current
 * value for the passed option.
 */
int set_opt(char *opt, int num)
{
	static Settings settings = {
		DEFAULT_LISTING
	};

	if (!_strcmp(opt, "listing"))
	{
		if (num != -1)
			settings.vertical_listing = num;
		else
			return (settings.vertical_listing);
	}

	return (0);
}

/**
 * check_opts - checks for options flags set in the arguments.
 * @args: array of arguments.
 * @argc: argument count
 */
void check_opts(char *args[], int argc)
{
	int i;

	for (i = 0; i < argc; i++)
	{
		if (args[i][0] == '-')
		{
			if (check_opt_arg(args[i]))
				args[i] = NULL;
		}
	}
}

/**
 * check_opt_arg - checks an arg string character by character if one of
 * it matches with one of the available flags. If so, the correspondent option
 * will be set.
 *
 * @arg: argument string.
 *
 * Return: if a match is found, the function will return a non-zero value.
 */
int check_opt_arg(char *arg)
{
	static Flag flags[] = ARGS;
	int i, j, ch = 0;

	for (i = 0; flags[i].long_name; i++)
	{
		for (j = 1; arg[j] != '\0'; j++)
		{
			if (arg[j] == flags[i].name)
			{
				ch = 1;
				set_opt(flags[i].option, flags[i].value);
			}
			else
			{
				fprintf(stderr, "hls: invalid option -- '%c'\n", arg[j]);
				fprintf(stderr, "Try hls --help for more information.\n");
				exit(2);
			}
		}
	}

	return (ch);
}

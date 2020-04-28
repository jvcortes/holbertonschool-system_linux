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
	int i, j;
	static Flag flags[] = ARGS;


	for (i = 0; i < argc; i++)
	{
		for (j = 0; flags[j].name; j++)
		{
			if (!_strcmp(args[i], flags[j].name) ||
				!_strcmp(args[i], flags[j].long_name))
			{
				set_opt(flags[j].option, flags[j].value);
				args[i] = NULL;
			}
		}
	}
}

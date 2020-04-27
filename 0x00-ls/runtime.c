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
	static Opt opts = {
		DEFAULT_LISTING
	};

	if (!_strcmp(opt, "listing"))
	{
		if (num != -1)
			opts.vertical_listing = num;
		else 
			return (opts.vertical_listing);
	}

	return (0);
}

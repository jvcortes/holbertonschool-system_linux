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

int set_opt(char *opt, int num)
{
	static Opt opts = {
		.vertical_listing = DEFAULT_LISTING
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

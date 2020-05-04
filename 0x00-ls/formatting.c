#include <time.h>
#include <stdio.h>
#include "def.h"


LongListFormatting *get_formatting(File **arr)
{
	int i, d = 0;
	LongListFormatting *f;

	if (arr == NULL)
		return (NULL);
	f = malloc(sizeof(LongListFormatting));
	if (f == NULL)
	{
		free(f);
		return (NULL);
	}

	f->user_spaces = 0;
	f->group_spaces = 0;
	f->nlink_spaces = 0;
	f->size_spaces = 0;

	for (i = 0 ; arr[i] != NULL; i++)
	{
		d = digits(arr[i]->nlink);
		if (d > f->nlink_spaces)
			f->nlink_spaces = d;
		d = digits(arr[i]->size);
		if (d > f->size_spaces)
			f->size_spaces = d;
		d = _strlen(arr[i]->user);
		if (d > f->user_spaces)
			f->user_spaces = d;
		d = _strlen(arr[i]->group);
		if (d > f->group_spaces)
			f->group_spaces = d;
	}

	return (f);
}

void format_time(char *s, size_t size, const time_t *t)
{
	time_t now_t;
	struct tm *cmp_t, *now;
	long year_now, year;

	now_t = time(0);
	now = localtime(&now_t);
	year_now = now->tm_year + 1900;

	cmp_t = gmtime(t);
	year = cmp_t->tm_year + 1900;

	if (year_now == year)
	{
		strftime(s, size - 1, "%b %e %H:%M", cmp_t);
	}
	else
	{
		strftime(s, size - 1, "%b %e  %Y", cmp_t);
	}
}
